//===--- UapiCheck.cpp - clang-tidy ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "UapiCheck.h"
#include "CheriUtil.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Comment.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"

using namespace clang::ast_matchers;

namespace clang::tidy::cheri {

void UapiCheck::registerMatchers(MatchFinder *Finder) {
  // clang-format off
  Finder->addMatcher(
      recordDecl().bind("struct"),
      this
  );
  Finder->addMatcher(
      typedefDecl(
          hasType(
              hasUnqualifiedDesugaredType(
                  recordType().bind("struct")
              )
          )
      ).bind("typedef"),
      this
  );
  // clang-format on
}

bool UapiCheck::isUapi(const SourceManager *SM, SourceLocation Loc) {
  const auto FE = SM->getFileEntryRefForID(SM->getFileID(Loc));
  if (!FE)
    return false;
  /* Don't treat existing compat headers as UAPI. */
  if (FE->getName().find("/compat32_") != StringRef::npos)
    return false;
  if (FE->getName().find("/compat64_") != StringRef::npos)
    return false;

  /* Uapi.cpp is for the llvm unittest. */
  return (FE->getName().find("/uapi/") != StringRef::npos) ||
         (FE->getName().find("Uapi.cpp") != StringRef::npos);
}

bool UapiCheck::isAsm(const SourceManager *SM, SourceLocation Loc) {
  const auto FE = SM->getFileEntryRefForID(SM->getFileID(Loc));
  if (!FE)
    return false;
  if (FE->getName().find("/asm/") != StringRef::npos)
    return true;
  if (FE->getName().find("/asm-generic/") != StringRef::npos)
    return true;

  return false;
}

SourceLocation UapiCheck::getLoc(RecordProperties &P, SourceLocation OrigLoc) {
  auto Loc = OrigLoc;
  if (!P.canFixup())
    return Loc;
  SourceLocation Best;
  while (1) {
    SourceLocation Tmp = Loc;
    if (P.sLoc_ <= Loc && Loc <= P.eLoc_)
      Best = Loc;
    Loc = P.SM_->getImmediateMacroCallerLoc(Loc);
    if (Loc == Tmp)
      break;
  }
  if (!Best.isValid()) {
    diag(OrigLoc, "OUT OF RANGE");
    diag(P.sLoc_, "BEGIN", DiagnosticIDs::Note);
    diag(P.eLoc_, "END", DiagnosticIDs::Note);
    P.clearFixup(OrigLoc, "Location out of Range");
  }

  return Loc;
}

UapiCheck::UapiMode UapiCheck::getUapiMode(ASTContext *Ctx, const FieldDecl *F)
{
  const comments::FullComment *FC = Ctx->getLocalCommentForDeclUncached(F);
  if (!FC)
    return UapiMode::None;

  for (const auto *B : FC->getBlocks()) {
    const auto *P = dyn_cast<comments::ParagraphComment>(B);
    if (!P)
      continue;
    for (auto it = P->child_begin(); it != P->child_end(); ++it) {
      const auto *TC = dyn_cast<comments::TextComment>(*it);
      if (!TC)
        continue;
      const auto T = TC->getText();
      if (T.starts_with(StringRef(" UAPI: Skip:")))
        return UapiMode::Skip;
      if (T.starts_with(StringRef(" UAPI: NoConvert:")))
        return UapiMode::NoConvert;
      if (T.starts_with(StringRef(" UAPI")))
        diag(F->getLocation(), "Unsupported UAPI annotation");
    }
  }

  return UapiMode::None;
}

void UapiCheck::annotate(SourceLocation Loc, bool DoFixup, const char *Text) {
  if (DoFixup)
    diag(Loc, Text);
}

void UapiCheck::annotate(SourceLocation Loc, bool DoFixup, const char *Text,
                         const std::string &Arg) {
  if (DoFixup)
    diag(Loc, "%0 >>%1<<") << Text << Arg;
}

void UapiCheck::annotateField(const char *Prefix, const std::string &Name,
                              const FieldInfo &FI)
{
  diag(FI.Loc, "%0:%1 >>%2<< >>%3<< >>%4<<")
    << Prefix << (FI.Array ? "ARRAY" : (FI.Bitfield ? "BITFIELD" : "FIELD"))
    << Name << FI.Name << FI.getConversion();
}

void UapiCheck::annotateDep(RecordProperties &P, const RecordProperties &PC,
                            SourceLocation Pos, SourceLocation Dep,
                            bool DoFixup) {
  if (isAsm(P.SM_, Dep) && !isAsm(P.SM_, Pos))
    P.clearFixup(Pos, "ARCH header dependency");
  if (!DoFixup)
    return;
  const auto From =
      P.SM_->getFileEntryRefForID(P.SM_->getFileID(P.sLoc_))->getName();
  const auto To = P.SM_->getFileEntryRefForID(P.SM_->getFileID(Dep))->getName();
  if (From == To)
    return;
  if (PC.needCompat32)
    diag(Pos, "__C32:DEP >>%0<< >>%1<<") << From << To;
  if (PC.needCompat64)
    diag(Pos, "__C64:DEP >>%0<< >>%1<<") << From << To;
}

SourceRange UapiCheck::getTypeRange(RecordProperties &P, const FieldDecl *F) {
  const auto sLoc = getLoc(P, F->getBeginLoc());
  auto eLoc = getLoc(P, F->getLocation());
  while (1) {
    eLoc = eLoc.getLocWithOffset(-1);
    if (!eLoc.isValid()) {
      P.clearFixup(F->getLocation(), "No previous token location");
      break;
    }
    auto E = Lexer::getLocForEndOfToken(eLoc, 0, *P.SM_, P.Ctx_->getLangOpts());
    if (!E.isValid()) {
      P.clearFixup(F->getLocation(), "No previous token");
      break;
    }
    if (E != eLoc)
      break;
  }

  return SourceRange(sLoc, eLoc);
}

SourceRange UapiCheck::getTypeTraitsTypeRange(RecordProperties &P,
                                              const UnaryExprOrTypeTraitExpr *U) {
  const auto sLoc = U->getArgumentTypeInfo()->getTypeLoc().getBeginLoc();
  const auto eLoc = U->getRParenLoc().getLocWithOffset(-1);

  if (P.checkNonMacroLoc(sLoc) && P.checkNonMacroLoc(eLoc) && sLoc < eLoc)
    return SourceRange(sLoc, eLoc);
 
  return SourceRange(sLoc, sLoc);
}

void UapiCheck::annotatePtr(RecordProperties &P, const SourceRange R,
                            bool DoFixup) {
  P.needCompat32 = true;
  P.needCompat64 = true;
  if (R.getBegin() >= R.getEnd())
    P.clearFixup(R.getBegin(), "Type range is invalid. Macro expansion?");
  annotate(R.getBegin(), DoFixup, "__C32:TYPE >>__c32_uptr<<");
  annotate(R.getBegin(), DoFixup, "__C64:TYPE >>__c64_uptr<<");
  annotate(R.getEnd(), DoFixup, "__C32:ENDTYPE");
  annotate(R.getEnd(), DoFixup, "__C64:ENDTYPE");
}

void UapiCheck::annotatePtr64(RecordProperties &P, const SourceRange R,
                              bool DoFixup) {
  P.needCompat64 = true;
  if (R.getBegin() >= R.getEnd())
    P.clearFixup(R.getBegin(), "Type range is invalid. Macro expansion?");
  annotate(R.getBegin(), DoFixup, "__C64:TYPE >>__c64_ptr64<<");
  annotate(R.getEnd(), DoFixup, "__C64:ENDTYPE");
}

// FIXME: Does not handle signed long correctly for conversion
void UapiCheck::annotateULong(RecordProperties &P, const SourceRange R,
                              bool DoFixup) {
  P.needCompat32 = true;
  if (R.getBegin() >= R.getEnd())
    P.clearFixup(R.getBegin(), "Type range is invalid. Macro expansion?");
  annotate(R.getBegin(), DoFixup, "__C32:TYPE >>__c32_ulong<<");
  annotate(R.getEnd(), DoFixup, "__C32:ENDTYPE");
}

void UapiCheck::annotateNestedRecord(RecordProperties &P,
                                     const RecordProperties &PC,
                                     const TypeSourceInfo *Info,
                                     SourceLocation DiagLoc, bool DoFixup) {
  if (!PC.needCompat32 && !PC.needCompat64)
    return;
  auto TLoc = Info->getTypeLoc();
  SourceLocation Loc;

  while (1) {
    if (auto E = TLoc.getAs<ElaboratedTypeLoc>()) {
      TLoc = E.getNamedTypeLoc();
      if (auto R = TLoc.getAs<RecordTypeLoc>()) {
        const auto *RD = R.getDecl()->getDefinition();
        if (!RD || !isUapi(P.SM_, RD->getLocation())) {
          P.clearFixup(RD->getLocation(), "Dependant type is not UAPI");
          return;
        }
        annotateDep(P, PC, DiagLoc, RD->getLocation(), DoFixup);
        Loc = R.getNameLoc();
        /* Hack to detect an unnamed struct. */
        if (Loc == E.getBeginLoc())
          return;
        break;
      }
      continue;
    }
    if (auto Q = TLoc.getAs<QualifiedTypeLoc>()) {
      TLoc = Q.getUnqualifiedLoc();
      continue;
    }
    if (auto A = TLoc.getAs<ArrayTypeLoc>()) {
      TLoc = A.getElementLoc();
      continue;
    }
    if (auto T = TLoc.getAs<TypedefTypeLoc>()) {
      const auto *TT = T.getType()->getAs<TypedefType>();
      const auto *TD = TT ? TT->getDecl() : nullptr;
      if (!TD) {
        P.clearFixup(DiagLoc, "Dependant type has no definition");
        return;
      }
      if (!isUapi(P.SM_, TD->getLocation())) {
        P.clearFixup(TD->getLocation(), "Dependant type is not UAPI");
        return;
      }
      annotateDep(P, PC, DiagLoc, TD->getLocation(), DoFixup);
      Loc = T.getNameLoc();
      break;
    }
    P.clearFixup(DiagLoc, "Cannot find struct name of nested record");
    return;
  }
  if (PC.needCompat32)
    annotate(getLoc(P, Loc), DoFixup, "__C32:INSERT >>__c32_<<");
  if (PC.needCompat64)
    annotate(getLoc(P, Loc), DoFixup, "__C64:INSERT >>__c64_<<");
}

/*
 * To some extend this is a shortcut as the underlying type
 * might depend on the architecture and this code is applied to
 * arch independent code, too.
 */
bool UapiCheck::isUapiSafe(const Type *T) {
  const auto *BT = T->getAs<BuiltinType>();

  if (!BT)
    return false;

  switch (BT->getKind()) {
  case BuiltinType::Bool:
  case BuiltinType::SChar:
  case BuiltinType::UChar:
  case BuiltinType::Char_S:
  case BuiltinType::Char_U:
  case BuiltinType::Short:
  case BuiltinType::UShort:
  case BuiltinType::Int:
  case BuiltinType::UInt:
    return true;
  default:
    break;
  }

  if (const auto *TD = T->getAs<TypedefType>()) {
    const auto &Name = TD->getDecl()->getDeclName().getAsString();
    if (Name == StringRef("__u64"))
      return true;
  }

  return false;
}

bool UapiCheck::isFixed64BitTypedef(const Type *T)
{
  const auto *TD = T->getAs<TypedefType>();
  if (!TD)
    return false;
  const auto &Name = TD->getDecl()->getDeclName().getAsString();

  // clang-format off
  if (Name == StringRef("__u64") ||
      Name == StringRef("__s64") ||
      Name == StringRef("__u128") ||
      Name == StringRef("__s128") ||
      Name == StringRef("__le64") ||
      Name == StringRef("__be64") ||
      Name == StringRef("uint64_t") ||
      Name == StringRef("int64_t") ||
      Name == StringRef("__kernel_time64_t") ||
      Name == StringRef("Elf64_Xword") ||
      Name == StringRef("Elf64_Sxword") ||
      Name == StringRef("Elf64_Off") ||
      Name == StringRef("Elf64_Addr") ||
      Name == StringRef("__virtio64") ||
      Name == StringRef("v4l2_std_id") ||
      Name == StringRef("u_quad_t") ||
      Name == StringRef("name_t"))	// uapi/linux/can/j1939.h
    return true;

  return false;
  // clang-format on
}

bool UapiCheck::isPtr64Typedef(const Type *T)
{
  const auto *TD = T->getAs<TypedefType>();
  if (!TD)
    return false;
  const auto &Name = TD->getDecl()->getDeclName().getAsString();

  // clang-format off
  if (Name == StringRef("__u64ptr") ||
      Name == StringRef("__s64ptr"))
    return true;

  return false;
  // clang-format on
}

bool UapiCheck::isPointerTypedef(const Type *T)
{
  const auto *TD = T->getAs<TypedefType>();
  if (!TD)
    return false;
  const auto &Name = TD->getDecl()->getDeclName().getAsString();

  // clang-format off
  if (Name == StringRef("__uptr") ||
      Name == StringRef("__sptr") ||
      Name == StringRef("__kernel_register_t"))
    return true;

  return false;
  // clang-format on
}

bool UapiCheck::isLongType(const Type *T)
{
  /* Only explicitly spelled out long types! Do not use ->getAs<> */
  if (const auto *BT = dyn_cast<BuiltinType>(T)) {
    switch (BT->getKind()) {
    case BuiltinType::ULong:
    case BuiltinType::Long:
      return true;
    default:
      break;
    }
  }

  const auto *TD = T->getAs<TypedefType>();
  if (!TD)
    return false;
  const auto &Name = TD->getDecl()->getDeclName().getAsString();

  // clang-format off
  if (Name == StringRef("__kernel_ulong_t") ||
      Name == StringRef("__kernel_size_t") ||
      Name == StringRef("packed_ulong") ||
      Name == StringRef("u_long") ||
      Name == StringRef("size_t") ||
      Name == StringRef("__kernel_long_t") ||
      Name == StringRef("__kernel_off_t") ||
      Name == StringRef("__kernel_loff_t") ||
      Name == StringRef("__kernel_old_time_t") ||
      Name == StringRef("__kernel_clock_t") ||
      Name == StringRef("ino_t") ||
      Name == StringRef("snd_pcm_sframes_t") ||
      Name == StringRef("snd_pcm_uframes_t"))
    return true;

  return false;
  // clang-format on
}

bool UapiCheck::isKnownSafeTypedef(const StringRef &Name) {
  // clang-format off
  if (Name == StringRef("dvd_challenge") ||
      Name == StringRef("dvd_key") ||
      Name == StringRef("sbi_instr_data") ||
      Name == StringRef("__kernel_fd_set") ||
      Name == StringRef("__kernel_sigset_t") ||
      Name == StringRef("sigset_t"))
    return true;
  // clang-format on

  return false;
}

bool UapiCheck::isKnownSafeStruct(const StringRef &Name) {
  // clang-format off
  if (Name == StringRef("__kernel_sockaddr_storage") ||
      Name == StringRef("sockaddr"))
    return true;
  // clang-format on

  return false;
}

void UapiCheck::checkTypeSourceInfo(RecordProperties &P,
                                    ConstantExprProperties &Const,
                                    const TypeSourceInfo *Info,
                                    SourceRange Src,
                                    bool DoFixup)
{
  const auto *T = Info->getType().getTypePtr();

  if (!T) {
    Const.isConstant = false;
    P.clearFixup(Src.getBegin(), "Expression is not constant");
    return;
  }
  if (const  auto *RT = T->getAs<RecordType>()) {
    const auto *RD = RT->getDecl()->getDefinition();
    if (!RD) {
    P.clearFixup(Src.getBegin(), "Expression is not constant");
      Const.isConstant = false;
      return;
    }

    auto PC = checkRecord(P, RD, DoFixup);
    annotateNestedRecord(P, PC, Info, Src.getBegin(), DoFixup);
    Const.needCompat32 |= PC.needCompat32;
    Const.needCompat64 |= PC.needCompat64;
    P.needCompat32 = true;
    P.needCompat64 = true;
    return;
  }
  if (isFixed64BitTypedef(T))
    return;
  if (isPointerTypedef(T)) {
    Const.needCompat32 = true;
    Const.needCompat64 = true;
    P.needCompat32 = true;
    P.needCompat64 = true;
    annotatePtr(P, Src, DoFixup);
    return;
  }
  if (isPtr64Typedef(T)) {
    annotatePtr64(P, Src, DoFixup);
    Const.needCompat64 = true;
    P.needCompat64 = true;
    return;
  }
  if (isLongType(T)) {
    Const.needCompat32 = true;
    P.needCompat32 = true;
    annotateULong(P, Src, DoFixup);
    return;
  }
  if (isUapiSafe(T))
    return;

  P.clearFixup(Src.getBegin(), "Don't know how to handle this type");
}

void UapiCheck::checkConstantExpr(RecordProperties &P,
                                  ConstantExprProperties &Const,
                                  const Expr *E, bool DoFixup) {
  const TypeSourceInfo *Info = nullptr;
  SourceRange Src;

  if (const auto *UT = dyn_cast<UnaryExprOrTypeTraitExpr>(E)) {
    switch (UT->getKind()) {
    case UETT_PreferredAlignOf:
    case UETT_AlignOf:
    case UETT_SizeOf:
      if (!UT->isArgumentType()) {
        P.clearFixup(E->getExprLoc(), "Expression is not constant");
        Const.isConstant = false;
        return;
      }
      Info = UT->getArgumentTypeInfo();
      Src = getTypeTraitsTypeRange(P, UT);
      break;
    default:
      break;
    }
  }

  if (const auto *O = dyn_cast<OffsetOfExpr>(E)) {
    Info = O->getTypeSourceInfo();
    /*
     * The source range is invalid for replacement. This is ok
     * because an offsetof expression will be annotate by
     * prefixing the typedef/struct name and not be replacing.
     */
    Src = SourceRange(O->getBeginLoc(), O->getBeginLoc());
  }

  if (Info) {
    checkTypeSourceInfo(P, Const, Info, Src, DoFixup);
    return;
  }

  if (const auto *Paren = dyn_cast<ParenExpr>(E)) {
    checkConstantExpr(P, Const, Paren->getSubExpr(), DoFixup);
    return;
  }
  if (const auto *Cast = dyn_cast<CastExpr>(E)) {
    checkConstantExpr(P, Const, Cast->getSubExpr(), DoFixup);
    return;
  }
  if (const auto *C = dyn_cast<ConstantExpr>(E)) {
    checkConstantExpr(P, Const, C->getSubExpr(), DoFixup);
    return;
  }

  /* Literal constants */
  if (dyn_cast<IntegerLiteral>(E))
    return;
  /* Constant member of an enum */
  if (const auto *DR = dyn_cast<DeclRefExpr>(E)) {
    if (dyn_cast<EnumConstantDecl>(DR->getDecl()))
      return;
  }

  if (const auto *Bin = dyn_cast<BinaryOperator>(E)) {
    switch (Bin->getOpcode()) {
    case BO_Add:
    case BO_Sub:
    case BO_Mul:
    case BO_Div:
    case BO_Rem:
      checkConstantExpr(P, Const, Bin->getLHS(), DoFixup);
      checkConstantExpr(P, Const, Bin->getRHS(), DoFixup);
      return;
    default:
      break;
    }
  }

  Const.isConstant = false;
  P.clearFixup(E->getExprLoc(), "Bad constant expression");
}

UapiCheck::RecordProperties UapiCheck::checkRecord(RecordProperties &PP,
                                                   const RecordDecl *R,
                                                   bool DoFixup) {
  RecordProperties P(PP, R);
  if (!(PP.sLoc_ <= P.sLoc_ && P.eLoc_ <= PP.eLoc_))
    DoFixup = false;

  SourceLocation CurLoc, PrevLoc;
  /* Check an alignment attribute. */
  if (const auto *A = R->getAttr<AlignedAttr>()) {
    const auto *E = A->getAlignmentExpr();
    ConstantExprProperties Const;
    checkConstantExpr(P, Const, E, DoFixup);
  }
  bool FlexibleArray = false;
  unsigned int nconvert = 0;	// Number of non-ignored fields
  for (const auto F : R->fields()) {
    PrevLoc = CurLoc;
    CurLoc = F->getBeginLoc();
    P.UapiMode_ = getUapiMode(P.Ctx_, F);
    if (P.UapiMode_ == UapiMode::Skip)
      continue;
    if (P.UapiMode_ != UapiMode::NoConvert)
      nconvert++;
    const auto *FT = F->getType().getTypePtr();
    bool isArray = false;
    if (!R->isUnion())
      FlexibleArray = false;
    auto TLoc = F->getTypeSourceInfo()->getTypeLoc();
    /* Check for an alignment attribute. */
    if (const auto * A = F->getAttr<AlignedAttr>()) {
      ConstantExprProperties Const;
      checkConstantExpr(P, Const, A->getAlignmentExpr(), DoFixup);
    }
    /* First handle arrays and get the element type. */
    while (1) {
      if (!TLoc) {
        P.clearFixup(F->getLocation(), "Field type has no type location");
        break;
      }
      const auto *AT = dyn_cast<ArrayType>(TLoc.getType().getTypePtr());
      if (!AT)
        break;

      if (isArray)
        P.clearConvert(F->getLocation(), "Multi dimensional array");
      isArray = true;
      if (P.UapiMode_ != UapiMode::NoConvert)
        FlexibleArray = true;
      FT = AT->getElementType().getTypePtr();
      const auto ATLoc = TLoc.getAs<ArrayTypeLoc>();
      if (!ATLoc) {
        P.clearFixup(F->getLocation(), "Not an array type location?");
        break;
      }
      TLoc = ATLoc.getElementLoc();

      if (dyn_cast<IncompleteArrayType>(AT)) {
        P.clearConvert(F->getLocation(), "Incomplete Array");
        continue;
      }
      if (!dyn_cast<ConstantArrayType>(AT)) {
        P.clearFixup(F->getLocation(), "Non-constant array");
        break;
      }
      const auto *Size = ATLoc.getSizeExpr();
      if (!Size) {
        P.clearFixup(F->getLocation(), "Array has no size");
        break;
      }
      const auto Val = Size->getIntegerConstantExpr(*P.Ctx_);
      if (Val && Val > 1)
        FlexibleArray = false;
      ConstantExprProperties Const;
      checkConstantExpr(P, Const, Size, DoFixup);
      if ((Const.needCompat32 || Const.needCompat64))
        P.clearConvert(F->getLocation(), "Array size changes");
    }
    if (FT->isPointerType()) {
      const auto *PT = FT->getAs<PointerType>()->getPointeeType().getTypePtr();
      if (PT->getAs<FunctionType>())
        P.clearFixup(F->getLocation(), "Function pointer type");
      if (F->getBeginLoc() != PrevLoc)
        annotatePtr(P, getTypeRange(P, F), DoFixup);
      P.addField(F, FieldInfo::Ptr);
      continue;
    }
    if (FT->isEnumeralType()) {
      const auto *ED =
          dyn_cast<EnumDecl>(FT->getAs<EnumType>()->getDecl()->getDefinition());
      if (ED) {
        const auto IT = ED->getIntegerType().getTypePtr();
        if (IT && isUapiSafe(IT)) {
          P.addField(F, FieldInfo::Copy);
          continue;
        }
      }
    }
    if (F->isBitField()) {
      if (isUapiSafe(FT)) {
        if (!F->isUnnamedBitField())
          P.addField(F, FieldInfo::Copy, true);
        continue;
      }
    }
    /*
     * Fixed 64-bit types.
     * FIXME:
     * We do not generate compat32 code for these fields even
     * though the alignment of a 64-bit integer or x86_32 is
     * only 32-bit. We trust that fixed 64-bit fields in UAPI
     * structures are either naturally aligned to 64-bit or that
     * the alignment is speicfied manually, e.g. via the use of
     * __aligned_u64.
     */
    if (isFixed64BitTypedef(FT)) {
      P.addField(F, FieldInfo::Copy);
      continue;
    }
    /* Pointer related typedefs. */
    if (isPointerTypedef(FT)) {
      if (F->getBeginLoc() != PrevLoc)
        annotatePtr(P, getTypeRange(P, F), DoFixup);
      P.addField(F, FieldInfo::UPtr);
      continue;
    }

    /* Plain long/unsigned long */
    if (isLongType(FT)) {
      if (F->getBeginLoc() != PrevLoc)
        annotateULong(P, getTypeRange(P, F), DoFixup);
      P.addField(F, FieldInfo::ULong);
      continue;
    }

    /* Pointers with at least 64-bit. */
    if (isPtr64Typedef(FT)) {
      if (F->getBeginLoc() != PrevLoc)
        annotatePtr64(P, getTypeRange(P, F), DoFixup);
      P.addField(F, FieldInfo::UPtr64);
      continue;
    }

    /* Other typedefs. */
    if (const auto *TD = FT->getAs<TypedefType>()) {
      /* Check if we know the name and how to deal with it. */
      const auto &Name = TD->getDecl()->getDeclName().getAsString();

      /* Misc typedefs that are fixed size. */
      if (isKnownSafeTypedef(Name)) {
        P.addField(F, FieldInfo::Memcopy);
        continue;
      }

      /* Strange way of storing an aligned 64-bit integer. */
      if (Name == StringRef("__pad_before_uframe")) {
        P.needCompat32 = true;
        P.clearFixup(F->getLocation(), "Cannot handle __pad_before_uframe");
        P.clearConvert(F->getLocation(), "Cannot handle __pad_before_uframe");
        continue;
      }
      if (Name == StringRef("__pad_after_uframe")) {
        P.needCompat32 = true;
        P.clearFixup(F->getLocation(), "Cannot handle __pad_after_uframe");
        P.clearConvert(F->getLocation(), "Cannot handle __pad_after_uframe");
        continue;
      }
    }

    if (const auto *RD = FT->getAsRecordDecl()) {
      if ((RD->isStruct() || RD->isUnion())) {
        if (RD->isCompleteDefinition()) {
          const auto *I = RD->getIdentifier();
          if (I) {
            const auto &Name = I->getName();
            if (isKnownSafeStruct(Name)) {
              P.addField(F, FieldInfo::Memcopy);
              continue;
            }
          }
          RecordProperties PC = checkRecord(P, RD, DoFixup);
          annotateNestedRecord(P, PC, F->getTypeSourceInfo(), F->getLocation(),
                               DoFixup);
          /* Calculate common prefix. */
          std::string Prefix;
          if (!F->isAnonymousStructOrUnion()) {
            Prefix = F->getName().str();
            if (isArray)
              Prefix += std::string("[0]");
            Prefix += std::string(".");
          } else if (isArray) {
            P.clearConvert(F->getLocation(), "Array of anon structs???");
          }
          /*
           * The struct does not need a conversion. Add the member
           * fields as Ignore and this field as a whole to the
           * conversion field list.
           * FIXME: Need to handle 32 and 64 bit differently.
           */
          if ((!PC.needCompat32 && !PC.needCompat64)
              && !F->isAnonymousStructOrUnion())
          {
            if (!I) {
              if (isArray)
                P.clearConvert(F->getLocation(), "Unnamed struct array");
              P.merge_fields(PC, Prefix, false);
              P.addField(F, FieldInfo::Ignore);
            } else {
              P.merge_fields(PC, Prefix, true);
              P.addField(F, FieldInfo::Memcopy);
            }
            continue;
          }
          /*
           * Convert a convertible struct by converting each field.
           * A union will have PC.canConvert() == false.
           * Merge the fields in the non-convertible case anyway but
           * set them to "Ignore". We cannot do this for Arrays.
           */
          if (!PC.canConvert())
            P.clearConvert(PC);
          if (isArray)
            P.clearConvert(F->getLocation(), "Cannot convert array");
          P.merge_fields(PC, Prefix, !PC.canConvert() || isArray);
          if (!F->isAnonymousStructOrUnion())
            P.addField(F, FieldInfo::Ignore);
          continue;
        }
      }
    }

    /* If the type is small we are presumably ok. */
    if (isUapiSafe(FT)) {
      P.addField(F, FieldInfo::Copy);
      continue;
    }

    /*
     * Handle explicitly spelled out basic types. Do not use
     * ->getAs<BuiltinType>() here because it would strip typedefs etc.
     */
    if (const auto *BT = dyn_cast<BuiltinType>(FT)) {
      switch (BT->getKind()) {
      case BuiltinType::ULongLong:
      case BuiltinType::LongLong:
      case BuiltinType::Float:
      case BuiltinType::Double:
        P.addField(F, FieldInfo::Copy);
        continue;
      default:
        break;
      }
    }

    diag(F->getLocation(), "Don't know how to handle UAPI field with type: %0")
        << FT->getTypeClassName();
    P.needCompat32 = true;
    P.needCompat64 = true;
    P.clearFixup(F->getLocation(), "Unknown type");
    P.clearConvert(F->getLocation(), "Unknown type");
    P.addField(F, FieldInfo::Ignore);
  }
  /*
   * We can convert a union if we haven't already found out that it
   * cannot be converted and all fields that do not have "Ignore"
   * type have the same type conversion type. For a union with only
   * one non-ignored field the conversion is inherited from that field.
   */
  if (R->isUnion() && (P.needCompat32 || P.needCompat64)
      && P.canConvert() && nconvert > 1)
  {
    FieldInfo::Kind K = FieldInfo::None;
    for (auto FI : P.fields) {
      auto conv = FI.Conversion;
      if (conv == FieldInfo::Copy)
          conv = FieldInfo::Memcopy;
      if (conv == FieldInfo::Ignore)
        continue;
      if (conv == FieldInfo::None) {
        K = FieldInfo::None;
        break;
      }
      if (FI.Array) {
        K = FieldInfo::None;
        break;
      }
      if (K == FieldInfo::None) {
        K = conv;
      } else if (K != conv) {
        K = FieldInfo::None;
        break;
      }
    }
    switch (K) {
    case FieldInfo::Ptr:
    case FieldInfo::UPtr:
    case FieldInfo::UPtr64:
    case FieldInfo::ULong:
      /*
       * All fields have the same scalar conversion type and thus the
       * same size. Mark all but the first field as Ignore.
       */
      for (auto &FI : P.fields) {
        if (FI.Conversion == FieldInfo::Ignore)
          continue;
        FI.Conversion = K;
        K = FieldInfo::Ignore;
      }
      break;
    case FieldInfo::Copy:
    case FieldInfo::Memcopy:
      /*
       * If this is nested in another record the surrounding record
       * will catch this and use a memcpy.
       */
      P.clearConvert(R->getLocation(), "Union COPY not implemented");
      break;
    case FieldInfo::None:
    case FieldInfo::Ignore:
      P.clearConvert(R->getLocation(), "Union");
    }
  }
  if (FlexibleArray)
    P.clearConvert(R->getLocation(), "Flexible array member at end");

  PP.merge(P);

  return P;
}

void UapiCheck::checkToplevelRecord(ASTContext *Ctx, const SourceManager *SM,
                                    const RecordDecl *R) {
  if (!R->isStruct() && !R->isUnion())
    return;
  R = R->getDefinition();
  if (!R)
    return;
  if (!isUapi(SM, R->getBeginLoc()))
    return;
  const auto *DC = R->getDeclContext();
  if (!dyn_cast<TranslationUnitDecl>(DC) && !dyn_cast<RecordDecl>(DC)) {
    diag(R->getLocation(), "Cannot annotate nested record");
    return;
  }

  /* Do the actual check but no annotations yet. */
  RecordProperties P(this, Ctx, SM, R);
  checkRecord(P, R, false);

  /* No compat needed => We are done. */
  if (!P.needCompat32 && !P.needCompat64)
    return;

  if (R->getIdentifier()) {
    if (!P.checkNonMacroLoc(R->getLocation()))
      P.clearFixup(R->getLocation(), "Identifier is result of macro expansion");
  } else {
    /*
     * Do not annotate unnamed structs here. Trust that the surrounding
     * record declaration or typedef will be found and do it for us.
     */
    return;
  }

  if (!P.canFixup()) {
    diag(R->getLocation(), "Will not be able to annotate this record");
    if (P.FixupReason_) {
      diag(P.FixupReasonLoc_, "Cannot fixup: %0", DiagnosticIDs::Note)
          << P.FixupReason_;
    }
    return;
  }

  /* Do the fixup for the record fields. */
  auto PC = checkRecord(P, R, true);
  if (!PC.canConvert()) {
    diag(R->getLocation(), "Will not be able to convert this record");
    if (PC.ConvertReason_)
      diag(PC.ConvertReasonLoc_, "Cannot convert: %0", DiagnosticIDs::Note)
        << PC.ConvertReason_;
  }

  const auto &Name = R->getIdentifier()->getName();
  const auto eLoc =
      Lexer::getLocForEndOfToken(PC.eLoc_, 1, *SM, Ctx->getLangOpts());
  std::string N;
  if (R->isStruct())
    N = std::string("struct ") + Name.str();
  else
    N = std::string("union ") + Name.str();
  if (PC.needCompat32) {
    annotate(PC.sLoc_, true, "__C32:BEGIN", N);
    if (PC.canConvert())
      diag(PC.sLoc_, "__C32:CONVERT >>%0<< >>%1<<") << N << PC.getFieldList();
    annotate(R->getLocation(), true, "__C32:INSERT >>__c32_<<");
    annotate(eLoc, true, "__C32:END", N);
    for (auto it: PC.fields)
      annotateField("__C32", N, it);
  }
  if (PC.needCompat64) {
    annotate(PC.sLoc_, true, "__C64:BEGIN", N);
    if (PC.canConvert())
      diag(PC.sLoc_, "__C64:CONVERT >>%0<< >>%1<<") << N << PC.getFieldList();
    annotate(R->getLocation(), true, "__C64:INSERT >>__c64_<<");
    annotate(eLoc, true, "__C64:END", N);
    for (auto it: PC.fields)
      annotateField("__C64", N, it);
  }
}

void UapiCheck::checkUapiTypedef(ASTContext *Ctx, const SourceManager *SM,
                                 const TypedefDecl *T, const RecordDecl *R) {
  if (!R->isStruct() && !R->isUnion())
    return;
  R = R->getDefinition();
  if (!R)
    return;
  if (!isUapi(SM, R->getBeginLoc()))
    return;
  if (!isUapi(SM, T->getBeginLoc()))
    return;
  const auto *DC = R->getDeclContext();
  if (!dyn_cast<TranslationUnitDecl>(DC)) {
    diag(T->getLocation(), "Cannot annotate nested typedef");
    return;
  }

  RecordProperties P(this, Ctx, SM, T->getBeginLoc(), T->getEndLoc());
  auto PC = checkRecord(P, R, false);
  annotateNestedRecord(P, PC, T->getTypeSourceInfo(), T->getLocation(), false);

  if (!P.needCompat64 && !P.needCompat32)
    return;

  if (!T->getIdentifier()) {
    diag("ERROR: Typedef has no identifier");
    return;
  }
  if (!P.checkNonMacroLoc(T->getLocation()))
    P.clearFixup(R->getLocation(), "Identifier is result of macro expansion");
  /*
   * These have fixed size but are defined in terms of unsigned longs
   * for efficiency, so don't warn about the definition. Note that
   * the alignment for these is different between 32-bit and 64-bit,
   * which means that we must warn if they are used inside a structure.
   */
  const auto Name = T->getDeclName().getAsString();
  if (isKnownSafeTypedef(Name))
    return;

  if (!P.canFixup()) {
    diag(T->getLocation(), "Typedef of unfixable record will be ignored");
    if (P.FixupReason_) {
      diag(P.FixupReasonLoc_, "Cannot Fixup: %0", DiagnosticIDs::Note)
          << P.FixupReason_;
    }
    return;
  }

  PC = checkRecord(P, R, true);
  annotateNestedRecord(P, PC, T->getTypeSourceInfo(), T->getLocation(), true);
  if (!PC.canConvert()) {
    diag(R->getLocation(), "Will not be able to convert this record");
    if (PC.ConvertReason_)
      diag(PC.ConvertReasonLoc_, "Cannot convert: %0", DiagnosticIDs::Note)
        << PC.ConvertReason_;
  }

  if (P.needCompat32) {
    annotate(P.sLoc_, true, "__C32:BEGIN", Name);
    if (PC.canConvert())
      diag(P.sLoc_, "__C32:CONVERT >>%0<< >>%1<<")
          << Name << PC.getFieldList();
    annotate(T->getLocation(), true, "__C32:INSERT >>__c32_<<");
    annotate(P.eLoc_, true, "__C32:END", Name);
    for (auto it: PC.fields)
      annotateField("__C32", Name, it);
  }
  if (P.needCompat64) {
    annotate(P.sLoc_, true, "__C64:BEGIN", Name);
    if (PC.canConvert())
      diag(P.sLoc_, "__C64:CONVERT >>%0<< >>%1<<")
          << Name << PC.getFieldList();
    annotate(T->getLocation(), true, "__C64:INSERT >>__c64_<<");
    annotate(P.eLoc_, true, "__C64:END", Name);
    for (auto it: PC.fields)
      annotateField("__C64", Name, it);
  }
}

void UapiCheck::check(const MatchFinder::MatchResult &Result) {
  ASTContext *Ctx = Result.Context;
  const auto *SM = Result.SourceManager;
  const auto *T = Result.Nodes.getNodeAs<TypedefDecl>("typedef");

  if (T) {
    const auto *R = Result.Nodes.getNodeAs<RecordType>("struct");
    checkUapiTypedef(Ctx, SM, T, R->getDecl());
  } else {
    const auto *R = Result.Nodes.getNodeAs<RecordDecl>("struct");
    checkToplevelRecord(Ctx, SM, R);
  }
}

} // namespace clang::tidy::cheri
