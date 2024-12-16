//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// This file defines the Mips subclass for TargetSelectionDAGInfo.
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_MIPS_MIPSSELECTIONDAGINFO_H
#define LLVM_LIB_TARGET_MIPS_MIPSSELECTIONDAGINFO_H

#include "llvm/CodeGen/SelectionDAGTargetInfo.h"

namespace llvm {

class MipsTargetMachine;

class MipsSelectionDAGInfo : public SelectionDAGTargetInfo {
public:
  virtual ~MipsSelectionDAGInfo();
  SDValue EmitTargetCodeForMemcpy(SelectionDAG &DAG, const SDLoc &dl,
                                  SDValue Chain, SDValue Op1, SDValue Op2,
                                  SDValue Op3, Align Alignment, bool isVolatile,
                                  bool AlwaysInline,
                                  PreserveCheriTags PreserveTags,
                                  MachinePointerInfo DstPtrInfo,
                                  MachinePointerInfo SrcPtrInfo) const override;

  SDValue EmitTargetCodeForMemmove(
      SelectionDAG &DAG, const SDLoc &dl, SDValue Chain, SDValue Op1,
      SDValue Op2, SDValue Op3, Align Alignment, bool isVolatile,
      PreserveCheriTags PreserveTags, MachinePointerInfo DstPtrInfo,
      MachinePointerInfo SrcPtrInfo) const override;

  SDValue EmitTargetCodeForMemset(SelectionDAG &DAG, const SDLoc &dl,
                                  SDValue Chain, SDValue Op1, SDValue Op2,
                                  SDValue Op3, Align Alignment, bool isVolatile,
                                  bool AlwaysInline,
                                  MachinePointerInfo DstPtrInfo) const override;
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_MIPS_MIPSSELECTIONDAGINFO_H
