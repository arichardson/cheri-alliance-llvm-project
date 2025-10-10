//===--- UapiCheck.h - clang-tidy -------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_CHERI_UAPICHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_CHERI_UAPICHECK_H

#include <set>
#include <vector>

#include "../ClangTidyCheck.h"

namespace clang::tidy::cheri {

/// Generate annotations for linux kernel UAPI headers that allow
/// automati generation of compat64 and compat32 headers.
class UapiCheck : public ClangTidyCheck {
private:
  struct FieldInfo {
    SourceLocation Loc;
    std::string Name;
    bool Array;
    bool Record;
    bool Bitfield;
    enum Kind {
      /* Invalid type. No field should have this type. */
      None,

      /* Copy the field verbatim. */
      Copy,

      /*
       * Copy the field verbatim but do not assume that a plain
       * assignment does the right thing. Used for aggregate types.
       * Use memmove if when copying in-place.
       */
      Memcopy,

      /*
       * Ignore the field during conversion. The field can still
       * be used for size and offset checking. This happens if
       * the field is embedded in a nested record that need no
       * conversion and is copied as a whole.
       */
      Ignore,

      /*
       * Field is a naturally sized real pointer in the native
       * structure. Use "compat_ptr()" to convert.
       */
      Ptr,

      /*
       * Field is a naturally sized pointer stored in an integer.
       * Use "(__uptr)compat_ptr()" to convert.
       */
      UPtr,

      /*
       * Field is an at least 64-bit sized pointer.
       * Use "(__uptr64)compat_ptr()" to convert.
       */
      UPtr64,

      /*
       * Field is a naturally sized unsigned long.
       * Just zero extend the field during conversion.
       */
      ULong,
    } Conversion;
    std::string getConversion() const {
      switch (Conversion) {
      case Copy: return std::string("Copy");
      case Memcopy: return std::string("Memcopy");
      case Ignore: return std::string("Ignore");
      case Ptr: return std::string("Ptr");
      case UPtr: return std::string("UPtr");
      case UPtr64: return std::string("UPtr64");
      case ULong: return std::string("ULong");
      default:
        return std::string("Bad field type");
      }
    }
  };
  /*
   * Information how to handle a field that is conveyed via
   * field annotations.
   */
  enum class UapiMode {
    /* Handle the field normally. */
    None,

    /*
     * Ignore the field in any considerations. The field is
     * still part of the compat record definition but its type
     * and name is unchanged and it is not considered when
     * generating the conversion function.
     * An example might be a kernel only field that happend to
     * make its way into an UABI structure for historical reasons.
     */
    Skip,

    /*
     * The field is checked normally. However, it is ignored when
     * generating a conversion function.
     * An example might be padding at the end of structure.
     */
    NoConvert,
  };
  struct RecordProperties {
    RecordProperties() = delete;
    RecordProperties(UapiCheck *Obj, ASTContext *Ctx, const SourceManager *SM,
                     SourceLocation sLoc, SourceLocation eLoc)
        : Obj_(Obj), Ctx_(Ctx), SM_(SM), sLoc_(sLoc), eLoc_(eLoc) {
      checkLocations();
    }
    RecordProperties(const RecordProperties &O, const RecordDecl *R)
        : Obj_(O.Obj_), Ctx_(O.Ctx_), SM_(O.SM_) {
      initRecordLocations(R);
    }
    RecordProperties(UapiCheck *Obj, ASTContext *Ctx, const SourceManager *SM,
                     const RecordDecl *R)
        : Obj_(Obj), Ctx_(Ctx), SM_(SM) {
      initRecordLocations(R);
    }
    RecordProperties(const RecordProperties &O, SourceLocation sLoc,
                     SourceLocation eLoc)
        : Obj_(O.Obj_), Ctx_(O.Ctx_), SM_(O.SM_), sLoc_(sLoc), eLoc_(eLoc) {
      checkLocations();
    }
    bool checkNonMacroLoc(SourceLocation Loc) const {
      return SM_->getImmediateMacroCallerLoc(Loc) == Loc && sLoc_ <= Loc &&
             Loc <= eLoc_;
    }
    void clearFixup(SourceLocation Loc, const char *Text) {
      canFixup_ = false;
      if (FixupReason_)
        return;
      FixupReason_ = Text;
      FixupReasonLoc_ = Loc;
    }
    void clearConvert(SourceLocation Loc, const char *Text) {
      if (UapiMode_ == UapiMode::NoConvert)
        return;
      canConvert_ = false;
      if (ConvertReason_)
        return;
      ConvertReason_ = Text;
      ConvertReasonLoc_ = Loc;
    }
    void clearConvert(const RecordProperties &Other) {
      canConvert_ = false;
      if (ConvertReason_)
        return;
      ConvertReason_ = Other.ConvertReason_;
      ConvertReasonLoc_ = Other.ConvertReasonLoc_;
    }
    bool canFixup(void) const { return canFixup_; }
    bool canConvert(void) const { return canConvert_; }

    void merge(const RecordProperties &other) {
      needCompat32 |= other.needCompat32;
      needCompat64 |= other.needCompat64;
      if (other.needCompat32 || other.needCompat64) {
        canFixup_ &= other.canFixup_;
        if (!other.canFixup_ && !FixupReason_) {
          FixupReason_ = other.FixupReason_;
          FixupReasonLoc_ = other.FixupReasonLoc_;
        }
      }
    }

    void addField(const FieldDecl *F, FieldInfo::Kind Kind, bool Bitfield = false) {
      const auto *I = F->getIdentifier();
      if (UapiMode_ == UapiMode::NoConvert)
        return;
      if (!I) {
        Obj_->diag(F->getLocation(), "Field has no name");
        return;
      }
      FieldInfo FI;
      FI.Loc = F->getLocation();
      FI.Name = I->getName();
      FI.Conversion = Kind;
      FI.Record = false;
      FI.Bitfield = Bitfield;
      FI.Array = !!dyn_cast<ArrayType>(F->getType().getTypePtr());
      if (FI.Array && FI.Bitfield) {
        Obj_->diag(F->getLocation(), "Array of bitfields???");
        return;
      }
      fields.push_back(FI);
    }

    void merge_fields(const RecordProperties &Other, std::string Prefix,
                      bool ignore) {
      for (auto it : Other.fields) {
        FieldInfo FI = it;
        FI.Name = Prefix + FI.Name;
        if (ignore)
          FI.Conversion = FieldInfo::Ignore;
        fields.push_back(FI);
      }
    }

    std::string getFieldList(void) const {
      std::string L;
      if (!canConvert())
        return L;
      for (auto f : fields) {
        if (f.Conversion == FieldInfo::Ignore)
          continue;
        if (!L.empty())
          L += std::string(" ");
        L += f.Name;
      }

      return L;
    }

  public:
    UapiCheck *Obj_;
    ASTContext *Ctx_;
    const SourceManager *SM_;
    SourceLocation sLoc_, eLoc_;
    const char *FixupReason_ = nullptr;
    SourceLocation FixupReasonLoc_;
    const char *ConvertReason_ = nullptr;
    SourceLocation ConvertReasonLoc_;
    bool needCompat32 = false;
    bool needCompat64 = false;
    UapiMode UapiMode_ = UapiMode::None;
    std::vector<FieldInfo> fields;

  private:
    bool canFixup_ = true;
    bool canConvert_ = true;
    void checkLocations(void) {
      if (SM_->getImmediateMacroCallerLoc(sLoc_) != sLoc_)
        clearFixup(sLoc_, "Start is the result of macro expansion");
      if (SM_->getImmediateMacroCallerLoc(eLoc_) != eLoc_)
        clearFixup(sLoc_, "End is the result of macro expansion");
    }
    void initRecordLocations(const RecordDecl *R) {
      sLoc_ = R->getBeginLoc();
      eLoc_ = R->getEndLoc();
      checkLocations();
      for (const auto *A: R->attrs()) {
        const auto Loc = A->getRange().getEnd();
        if (SM_->getImmediateMacroCallerLoc(Loc) == Loc && Loc > eLoc_)
          eLoc_ = Loc;
      }
    }
  };

  struct ConstantExprProperties {
    bool isConstant = true;
    bool needCompat32 = false;
    bool needCompat64 = false;
  };

public:
  UapiCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  bool isPointerTypedef(const Type *T);
  bool isFixed64BitTypedef(const Type *T);
  bool isLongType(const Type *T);
  bool isPtr64Typedef(const Type *T);
  bool isUapi(const SourceManager *SM, const SourceLocation loc);
  bool isAsm(const SourceManager *SM, const SourceLocation loc);
  SourceLocation getLoc(RecordProperties &P, SourceLocation Loc);
  UapiMode getUapiMode(ASTContext *Ctx, const FieldDecl *F);
  void annotate(SourceLocation Loc, bool DoFixup, const char *Text);
  void annotate(SourceLocation Loc, bool DoFixup, const char *Text,
                const std::string &Arg);
  void annotateField(const char *Prefix, const std::string &Name,
                     const FieldInfo &FI);
  void annotateDep(RecordProperties &P, const RecordProperties &PC,
                   SourceLocation Pos, SourceLocation Dep,
                   bool DoFixup);
  SourceRange getTypeRange(RecordProperties &P, const FieldDecl *F);
  SourceRange getTypeTraitsTypeRange(RecordProperties &P,
                                     const UnaryExprOrTypeTraitExpr *U);
  SourceRange getOffsetOfTypeRange(RecordProperties &P,
                                   const UnaryExprOrTypeTraitExpr *U);
  void annotatePtr(RecordProperties &P, const SourceRange R, bool DoFixup);
  void annotatePtr64(RecordProperties &P, const SourceRange R, bool DoFixup);
  void annotateULong(RecordProperties &P, const SourceRange R, bool DoFixup);
  void annotateNestedRecord(RecordProperties &P, const RecordProperties &PC,
                            const TypeSourceInfo *Info, SourceLocation DiagLoc,
                            bool DoFixup);
  bool isUapiSafe(const Type *T);
  bool isKnownSafeTypedef(const StringRef &Name);
  bool isKnownSafeStruct(const StringRef &Name);
  void checkTypeSourceInfo(RecordProperties &P, ConstantExprProperties &Const,
                           const TypeSourceInfo *Info, SourceRange sLoc,
                           bool DoFixup);
  void  checkConstantExpr(RecordProperties &P, ConstantExprProperties &Const,
                          const Expr *E, bool DoFixup);
  RecordProperties checkRecord(RecordProperties &P, const RecordDecl *R,
                               bool DoFixup);
  void checkUapiTypedef(ASTContext *Ctx, const SourceManager *SM,
                        const TypedefDecl *T, const RecordDecl *R);
  void checkToplevelRecord(ASTContext *Ctx, const SourceManager *SM,
                           const RecordDecl *R);
};

} // namespace clang::tidy::cheri

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_CHERI_UAPICHECK_H
