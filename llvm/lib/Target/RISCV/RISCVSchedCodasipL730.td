//==- RISCVSchedCodasipL730.td - L730 Scheduling Definitions -*- tablegen -*-=//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//

// TODO: Go over these.
class CodasipL730EarlyToLateBypass<SchedRead read, int cycles = 2>
    : ReadAdvance<read, cycles, [WriteIALU, WriteIALU32, WriteSingleBit,
                                 WriteSingleBitImm, WriteBEXT, WriteBEXTI,
                                 WriteREV8, WriteBREV8, WriteORCB,
                                 WriteRotateReg, WriteRotateImm,
                                 WriteRotateReg32, WriteRotateImm32,
                                 WritePACK, WritePACK32, WriteXPERM,
                                 WriteIMinMax, WriteCTZ, WriteCLZ,
                                 WriteCTZ32, WriteCLZ32, WriteShiftReg,
                                 WriteShiftImm, WriteShiftReg32,
                                 WriteShiftImm32, WriteCPOP, WriteCPOP32]>;

class CodasipL730LoadDelay<SchedRead read, int cycles = -2>
    : ReadAdvance<read, cycles, [WriteLDD, WriteLDW, WriteLDH, WriteLDB]>;

class CodasipL730MulDivDelay<SchedRead read, int cycles = -3>
    : ReadAdvance<read, cycles, [WriteIMul, WriteIMul32, WriteIDiv, WriteIDiv32,
                                 WriteIRem, WriteIRem32]>;

def CodasipL730Model : SchedMachineModel {
  let MicroOpBufferSize = 0; // Explicitly set to zero since CodasipL730 is in-order.
  let IssueWidth = 2;
  let LoadLatency = 3;
  let MispredictPenalty = 3;
  let CompleteModel = 0;
  let EnableIntervals = true;
}

let SchedModel = CodasipL730Model in {

let BufferSize = 0 in {
  def L730PipeI0     : ProcResource<1>;
  def L730PipeI1     : ProcResource<1>;
  def L730Mul        : ProcResource<1>;
  def L730LSU        : ProcResource<1>;
  def L730EarlyALU   : ProcResource<2>;
  def L730LateALU    : ProcResource<2>;
  def L730FPU        : ProcResource<1>;
  def L730CSR        : ProcResource<1>;
  def L730Div        : ProcResource<1>;
}

def L730PipeI0I1 : ProcResGroup<[L730PipeI0, L730PipeI1]>;

def L730EarlyLateALU : ProcResGroup<[L730EarlyALU, L730LateALU]>;


// Grouping based on issue classes

// ALU issue class - executed by the dual ALU

//  xnor EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  orn EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  andn EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  and EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  or EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  xor EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  c.and EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  c.or EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  c.not EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  c.xor EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  andi EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  ori EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  xori EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  c.andi EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  bseti EARLY_LATE_ALU_DUAL Latency 1 WriteSingleBitImm
//  bset EARLY_LATE_ALU_DUAL Latency 1 WriteSingleBit
//  binvi EARLY_LATE_ALU_DUAL Latency 1 WriteSingleBitImm
//  binv EARLY_LATE_ALU_DUAL Latency 1 WriteSingleBit
//  bexti EARLY_LATE_ALU_DUAL Latency 1 WriteBEXTI
//  bext EARLY_LATE_ALU_DUAL Latency 1 WriteBEXT
//  bclri EARLY_LATE_ALU_DUAL Latency 1 WriteSingleBitImm
//  bclr EARLY_LATE_ALU_DUAL Latency 1 WriteSingleBit
//  rev8 EARLY_LATE_ALU_DUAL Latency 1 WriteREV8
//  orc.b EARLY_LATE_ALU_DUAL Latency 1 WriteORCB
//  rorw EARLY_LATE_ALU_DUAL Latency 1 WriteRotateReg32
//  roriw EARLY_LATE_ALU_DUAL Latency 1 WriteRotateImm32
//  rol EARLY_LATE_ALU_DUAL Latency 1 WriteRotateReg
//  rori EARLY_LATE_ALU_DUAL Latency 1 WriteRotateImm
//  ror EARLY_LATE_ALU_DUAL Latency 1 WriteRotateReg
//  rolw EARLY_LATE_ALU_DUAL Latency 1 WriteRotateReg32
//  xperm4 EARLY_LATE_ALU_DUAL Latency 1 WriteXPERM
//  xperm8 EARLY_LATE_ALU_DUAL Latency 1 WriteXPERM
//  brev8 EARLY_LATE_ALU_DUAL Latency 1 WriteBREV8
//  packw EARLY_LATE_ALU_DUAL Latency 1 WritePACK32
//  packh EARLY_LATE_ALU_DUAL Latency 1 WritePACK
//  pack EARLY_LATE_ALU_DUAL Latency 1 WritePACK
//  c.zext.w EARLY_LATE_ALU_DUAL Latency 1 WriteIALU32
//  c.zext.b EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  c.zext.h EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  zext.h EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  c.sext.h EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  sext.h EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  c.sext.b EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  sext.b EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  minu EARLY_LATE_ALU_DUAL Latency 1 WriteIMinMax
//  min EARLY_LATE_ALU_DUAL Latency 1 WriteIMinMax
//  maxu EARLY_LATE_ALU_DUAL Latency 1 WriteIMinMax
//  max EARLY_LATE_ALU_DUAL Latency 1 WriteIMinMax
//  cpopw EARLY_LATE_ALU_DUAL Latency 1 WriteCPOP32
//  cpop EARLY_LATE_ALU_DUAL Latency 1 WriteCPOP
//  ctzw EARLY_LATE_ALU_DUAL Latency 1 WriteCTZ32
//  ctz EARLY_LATE_ALU_DUAL Latency 1 WriteCTZ
//  clzw EARLY_LATE_ALU_DUAL Latency 1 WriteCLZ32
//  clz EARLY_LATE_ALU_DUAL Latency 1 WriteCLZ
//  slli.uw EARLY_LATE_ALU_DUAL Latency 1 WriteShiftImm32
//  sh4add.uw EARLY_LATE_ALU_DUAL Latency 1 WriteSHXADD32
//  sh4add EARLY_LATE_ALU_DUAL Latency 1 WriteSHXADD
//  sh3add.uw EARLY_LATE_ALU_DUAL Latency 1 WriteSHXADD32
//  sh3add EARLY_LATE_ALU_DUAL Latency 1 WriteSHXADD
//  sh2add.uw EARLY_LATE_ALU_DUAL Latency 1 WriteSHXADD32
//  sh2add EARLY_LATE_ALU_DUAL Latency 1 WriteSHXADD
//  sh1add.uw EARLY_LATE_ALU_DUAL Latency 1 WriteSHXADD32
//  sh1add EARLY_LATE_ALU_DUAL Latency 1 WriteSHXADD
//  add.uw EARLY_LATE_ALU_DUAL Latency 1 WriteIALU32
//  sraw EARLY_LATE_ALU_DUAL Latency 1 WriteShiftReg32
//  srlw EARLY_LATE_ALU_DUAL Latency 1 WriteShiftReg32
//  sllw EARLY_LATE_ALU_DUAL Latency 1 WriteShiftReg32
//  subw EARLY_LATE_ALU_DUAL Latency 1 WriteIALU32
//  addw EARLY_LATE_ALU_DUAL Latency 1 WriteIALU32
//  sraiw EARLY_LATE_ALU_DUAL Latency 1 WriteShiftImm32
//  srliw EARLY_LATE_ALU_DUAL Latency 1 WriteShiftImm32
//  slliw EARLY_LATE_ALU_DUAL Latency 1 WriteShiftImm32
//  sra EARLY_LATE_ALU_DUAL Latency 1 WriteShiftReg
//  srl EARLY_LATE_ALU_DUAL Latency 1 WriteShiftReg
//  sltu EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  slt EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  sll EARLY_LATE_ALU_DUAL Latency 1 WriteShiftReg
//  sub EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  add EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  srai EARLY_LATE_ALU_DUAL Latency 1 WriteShiftImm
//  srli EARLY_LATE_ALU_DUAL Latency 1 WriteShiftImm
//  slli EARLY_LATE_ALU_DUAL Latency 1 WriteShiftImm
//  sltiu EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  slti EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  c.add EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  c.slli64 EARLY_LATE_ALU_DUAL Latency 1 WriteShiftImm
//  c.addw EARLY_LATE_ALU_DUAL Latency 1 WriteIALU32
//  c.subw EARLY_LATE_ALU_DUAL Latency 1 WriteIALU32
//  c.sub EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  c.srai64 EARLY_LATE_ALU_DUAL Latency 1 WriteShiftImm
//  c.srli64 EARLY_LATE_ALU_DUAL Latency 1 WriteShiftImm
//  bgeu EARLY_LATE_ALU_DUAL Latency N/A WriteJmp
//  bltu EARLY_LATE_ALU_DUAL Latency N/A WriteJmp
//  bge EARLY_LATE_ALU_DUAL Latency N/A WriteJmp
//  blt EARLY_LATE_ALU_DUAL Latency N/A WriteJmp
//  bne EARLY_LATE_ALU_DUAL Latency N/A WriteJmp
//  beq EARLY_LATE_ALU_DUAL Latency N/A WriteJmp
//  c.bnez EARLY_LATE_ALU_DUAL Latency N/A WriteJmp
//  c.beqz EARLY_LATE_ALU_DUAL Latency N/A WriteJmp
//
//  c.jalr EARLY_ALU_DUAL_SINGLE Latency 1 WriteJalr
//  c.jr EARLY_ALU_DUAL_SINGLE Latency N/A WriteJalr
//  jalr EARLY_ALU_DUAL_SINGLE Latency 1 WriteJalr
//  jal EARLY_ALU_DUAL_SINGLE Latency 1 WriteJal
//  c.j EARLY_ALU_DUAL_SINGLE Latency N/A WriteJmp !! Ambiguous SchedWrite !!
//
//  c.mv EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  addiw EARLY_LATE_ALU_DUAL Latency 1 WriteIALU32
//  addi EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  c.addi16sp EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  c.addiw EARLY_LATE_ALU_DUAL Latency 1 WriteIALU32
//  c.addi EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  c.addi4spn EARLY_LATE_ALU_DUAL Latency 1 WriteIALU
//  c.nop EARLY_LATE_ALU_DUAL Latency N/A WriteNop
//
//  auipc EARLY_ALU_DUAL Latency 0 WriteIALU !!  Ambiguous SchedWrite !!
//
//  lui EARLY_ALU_DUAL Latency 0 WriteIALU !!  Ambiguous SchedWrite !!
//  c.lui EARLY_ALU_DUAL Latency 0 WriteIALU !!  Ambiguous SchedWrite !!
//  c.li EARLY_ALU_DUAL Latency 0 WriteIALU !!  Ambiguous SchedWrite !!
//

let Latency = 0 in
def L730SchedWriteIMat : SchedWriteRes<[L730PipeI0I1, L730EarlyALU]>;

def L730WriteIMat : SchedWriteVariant<[
  SchedVar<NoSchedPred, [L730SchedWriteIMat]>
]>;

def : InstRW<[L730WriteIMat], (instrs AUIPC, LUI, C_LUI, C_LI)>;

let Latency = 1 in {
  def : WriteRes<WriteIALU, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WriteIALU32, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WriteNop, [L730PipeI0I1, L730EarlyLateALU]>;

  // TODO: Jumps cannot be dual-issued with other jumps.
  def : WriteRes<WriteJmp, [L730PipeI0I1, L730EarlyALU]>;
  def : WriteRes<WriteJal, [L730PipeI0I1, L730EarlyALU]>;
  def : WriteRes<WriteJalr, [L730PipeI0I1, L730EarlyALU]>;

  def : WriteRes<WritePACK, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WritePACK32, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WriteZIP, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WriteCPOP, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WriteCPOP32, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WriteCLZ, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WriteCLZ32, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WriteCTZ, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WriteCTZ32, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WriteSHXADD, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WriteSHXADD32, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WriteShiftReg, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WriteShiftReg32, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WriteShiftImm, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WriteShiftImm32, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WriteRotateReg, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WriteRotateReg32, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WriteRotateImm, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WriteRotateImm32, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WriteORCB, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WriteIMinMax, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WriteREV8, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WriteBREV8, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WriteXPERM, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WriteSingleBit, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WriteSingleBitImm, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WriteBEXT, [L730PipeI0I1, L730EarlyLateALU]>;
  def : WriteRes<WriteBEXTI, [L730PipeI0I1, L730EarlyLateALU]>;
}


// ATOMIC issue class - executed by the LSU

//  amomaxu.d SINGLE Latency 3 WriteAtomicD
//  amominu.d SINGLE Latency 3 WriteAtomicD
//  amomax.d SINGLE Latency 3 WriteAtomicD
//  amomin.d SINGLE Latency 3 WriteAtomicD
//  amoor.d SINGLE Latency 3 WriteAtomicD
//  amoand.d SINGLE Latency 3 WriteAtomicD
//  amoxor.d SINGLE Latency 3WriteAtomicD
//  amoadd.d SINGLE Latency 3 WriteAtomicD
//  amoswap.d SINGLE Latency 3 WriteAtomicD
//  amomaxu.w SINGLE Latency 3 WriteAtomicW
//  amominu.w SINGLE Latency 3 WriteAtomicW
//  amomax.w SINGLE Latency 3 WriteAtomicW
//  amomin.w SINGLE Latency 3 WriteAtomicW
//  amoor.w SINGLE Latency 3 WriteAtomicW
//  amoand.w SINGLE Latency 3 WriteAtomicW
//  amoxor.w SINGLE Latency 3 WriteAtomicW
//  amoadd.w SINGLE Latency 3 WriteAtomicW
//  amoswap.w SINGLE Latency 3 WriteAtomicW
//  sc.w SINGLE Latency 3 WriteAtomicSTW
//  sc.d SINGLE Latency 3 WriteAtomicSTD
//  lr.w SINGLE Latency 3 WriteAtomicLDW
//  lr.d SINGLE Latency 3 WriteAtomicLDD

let Latency = 3, SingleIssue = true in {
  def : WriteRes<WriteAtomicD, [L730PipeI0I1, L730LSU]>;
  def : WriteRes<WriteAtomicW, [L730PipeI0I1, L730LSU]>;
  def : WriteRes<WriteAtomicSTD, [L730PipeI0I1, L730LSU]>;
  def : WriteRes<WriteAtomicSTW, [L730PipeI0I1, L730LSU]>;
  def : WriteRes<WriteAtomicLDD, [L730PipeI0I1, L730LSU]>;
  def : WriteRes<WriteAtomicLDW, [L730PipeI0I1, L730LSU]>;
}

// CSR issue class - executed by the CSR unit

//  csrrc (X0 rs1) CSR_I0_ALU_I1 Latency 3 WriteCSR
//  csrrs (X0 rs1) CSR_I0_ALU_I1 Latency 3 WriteCSR
//  csrrci (uimm 0) CSR_I0_ALU_I1 Latency 3 WriteCSR
//  csrrsi (uimm 0) CSR_I0_ALU_I1 Latency 3 WriteCSR
//  csrrc (non-X0 rs1) SINGLE Latency 3 WriteCSR
//  csrrs (non-X0 rs1) SINGLE Latency 3 WriteCSR
//  csrrci (uimm non-0) SINGLE Latency 3 WriteCSR
//  csrrsi (uimm non-0) SINGLE Latency 3 WriteCSR
//  csrrw SINGLE Latency 3 WriteCSR
//  csrrwi SINGLE Latency 3 WriteCSR

let Latency = 3 in
def L730SchedWriteCSRSimple : SchedWriteRes<[L730PipeI0, L730CSR]>;

let Latency = 3, SingleIssue = true in
def L730SchedWriteCSR : SchedWriteRes<[L730PipeI0I1, L730CSR]>;

def L730CSRIsSimplePred : MCSchedPredicate<
  CheckAny<[
    CheckAll<[CheckIsRegOperand<2>, CheckRegOperand<2, X0>]>,
    CheckAll<[CheckIsImmOperand<2>, CheckImmOperand<2, 0>]>
  ]>
>;

def L730WriteCSRSimple : SchedWriteVariant<[
  SchedVar<L730CSRIsSimplePred, [L730SchedWriteCSRSimple]>,
  SchedVar<NoSchedPred, [L730SchedWriteCSR]>
]>;

def : InstRW<[L730WriteCSRSimple], (instrs CSRRCI, CSRRSI, CSRRC, CSRRS)>;

let Latency = 3, SingleIssue = true in
def : WriteRes<WriteCSR, [L730PipeI0I1, L730CSR]>;

// DIV issue class - executed by the Div unit

//  remuw SINGLE Latency 32 WriteIRem32
//  remw SINGLE Latency 32 WriteIRem32
//  divuw SINGLE Latency 32 WriteIDiv32
//  divw SINGLE Latency 32 WriteIDiv32
//  remu SINGLE Latency 32 WriteIRem
//  rem SINGLE Latency 32 WriteIRem
//  divu SINGLE Latency 32 WriteIDiv
//  div SINGLE Latency 32 WriteIDiv

let Latency = 32, ReleaseAtCycles = [32, 32], SingleIssue = true in {
  def : WriteRes<WriteIDiv, [L730PipeI0I1, L730Div]>;
  def : WriteRes<WriteIDiv32, [L730PipeI0I1, L730Div]>;
  def : WriteRes<WriteIRem, [L730PipeI0I1, L730Div]>;
  def : WriteRes<WriteIRem32, [L730PipeI0I1, L730Div]>;
}


// FPU issue class - executed by the FPU

//  fsqrt.d SINGLE Latency 32 WriteFSqrt64
//  fdiv.d SINGLE Latency 32 WriteFDiv64
//  fsqrt.s SINGLE Latency 32 WriteFSqrt32
//  fdiv.s SINGLE Latency 32 WriteFDiv32
//
//  fcvt.d.s FPU_I0_ALU_I1 Latency 4 WriteFCvtF32ToF64
//  fcvt.s.d FPU_I0_ALU_I1 Latency 4 WriteFCvtF64ToF32
//  fcvt.h.d FPU_I0_ALU_I1 Latency 4 WriteFCvtF64ToF16
//  fcvt.d.h FPU_I0_ALU_I1 Latency 4 WriteFCvtF16ToF64
//  fcvt.h.s FPU_I0_ALU_I1 Latency 4 WriteFCvtF32ToF16
//  fcvt.s.h FPU_I0_ALU_I1 Latency 4 WriteFCvtF16ToF32
//
//  fle.d FPU_I0_ALU_I1 Latency 2 WriteFCmp64
//  flt.d FPU_I0_ALU_I1 Latency 2 WriteFCmp64
//  feq.d FPU_I0_ALU_I1 Latency 2 WriteFCmp64
//
//  fmax.d FPU_I0_ALU_I1 Latency 4 WriteFMinMax64
//  fmin.d FPU_I0_ALU_I1 Latency 4 WriteFMinMax64
//  fsgnjx.d FPU_I0_ALU_I1 Latency 4 WriteFSGNJ64
//  fsgnjn.d FPU_I0_ALU_I1 Latency 4 WriteFSGNJ64
//  fsgnj.d FPU_I0_ALU_I1 Latency 4 WriteFSGNJ64
//
//  fmul.d FPU_I0_ALU_I1 Latency 6 WriteFMul64
//
//  fsub.d FPU_I0_ALU_I1 Latency 4 WriteFAdd64
//  fadd.d FPU_I0_ALU_I1 Latency 4 WriteFAdd64
//
//  fnmadd.d FPU_I0_ALU_I1 Latency 6 WriteFMA64
//  fnmsub.d FPU_I0_ALU_I1 Latency 6 WriteFMA64
//  fmsub.d FPU_I0_ALU_I1 Latency 6 WriteFMA64
//  fmadd.d FPU_I0_ALU_I1 Latency 6 WriteFMA64
//
//  fle.s FPU_I0_ALU_I1 Latency 2 WriteFCmp32
//  flt.s FPU_I0_ALU_I1 Latency 2 WriteFCmp32
//  feq.s FPU_I0_ALU_I1 Latency 2 WriteFCmp32
//
//  fmax.s FPU_I0_ALU_I1 Latency 4 WriteFMinMax32
//  fmin.s FPU_I0_ALU_I1 Latency 4 WriteFMinMax32
//  fsgnjx.s FPU_I0_ALU_I1 Latency 4 WriteFSGNJ32
//  fsgnjn.s FPU_I0_ALU_I1 Latency 4 WriteFSGNJ32
//  fsgnj.s FPU_I0_ALU_I1 Latency 4 WriteFSGNJ32
//
//  fmul.s FPU_I0_ALU_I1 Latency 6 WriteFMul64
//
//  fsub.s FPU_I0_ALU_I1 Latency 4 WriteFAdd32
//  fadd.s FPU_I0_ALU_I1 Latency 4 WriteFAdd32
//
//  fnmadd.s FPU_I0_ALU_I1 Latency 6 WriteFMA32
//  fnmsub.s FPU_I0_ALU_I1 Latency 6 WriteFMA32
//  fmsub.s FPU_I0_ALU_I1 Latency 6 WriteFMA32
//  fmadd.s FPU_I0_ALU_I1 Latency 6 WriteFMA32

let Latency = 2 in {
  // TODO: Can only dual issue in I0 alongside an ALU instruction in I1 - not sure yet how we enforce this.
  def : WriteRes<WriteFCmp32, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFCmp64, [L730PipeI0, L730FPU]>;
}

let Latency = 4 in {
  // TODO: Can only dual issue in I0 alongside an ALU instruction in I1 - not sure yet how we enforce this.
  def : WriteRes<WriteFMinMax64, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFMinMax32, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFSGNJ64, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFSGNJ32, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFCvtF32ToF64, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFCvtF64ToF32, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFCvtF16ToF64, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFCvtF64ToF16, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFCvtF32ToF16, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFCvtF16ToF32, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFAdd64, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFAdd32, [L730PipeI0, L730FPU]>;
}

let Latency = 6 in {
  // TODO: Can only dual issue in I0 alongside an ALU instruction in I1 - not sure yet how we enforce this.
  def : WriteRes<WriteFMul64, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFMA64, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFMul32, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFMA32, [L730PipeI0, L730FPU]>;
}

let Latency = 32, ReleaseAtCycles = [32, 32], SingleIssue = true in {
  def : WriteRes<WriteFDiv64, [L730PipeI0I1, L730FPU]>;
  def : WriteRes<WriteFSqrt64, [L730PipeI0I1, L730FPU]>;
  def : WriteRes<WriteFDiv32, [L730PipeI0I1, L730FPU]>;
  def : WriteRes<WriteFSqrt32, [L730PipeI0I1, L730FPU]>;
}

// FTOI issue class - executed by the FPU

//  fmv.x.h FPU_I0_ALU_I1 Latency 3 WriteFMovF16ToI16
//  fmv.x.w FPU_I0_ALU_I1 Latency 3 WriteFMovF32ToI32
//  fmv.x.d FPU_I0_ALU_I1 Latency 3 WriteFMovF64ToI64
//  fcvt.l.d FPU_I0_ALU_I1 Latency 3 WriteFCvtF64ToI64
//  fcvt.lu.s FPU_I0_ALU_I1 Latency 3 WriteFCvtF32ToI64
//  fcvt.l.s FPU_I0_ALU_I1 Latency 3 WriteFCvtF32ToI64
//  fcvt.wu.s FPU_I0_ALU_I1 Latency 3 WriteFCvtF32ToI32
//  fcvt.w.s FPU_I0_ALU_I1 Latency 3 WriteFCvtF32ToI32
//  fcvt.lu.d FPU_I0_ALU_I1 Latency 3 WriteFCvtF64ToI64
//  fcvt.wu.d FPU_I0_ALU_I1 Latency 3 WriteFCvtF64ToI32
//  fcvt.w.d FPU_I0_ALU_I1 Latency 3 WriteFCvtF64ToI32
//  fclass.d FPU_I0_ALU_I1 Latency 3 WriteFClass64
//  fclass.s FPU_I0_ALU_I1 Latency 3 WriteFClass64

let Latency = 3 in {
  def : WriteRes<WriteFClass32, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFClass64, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFMovF16ToI16, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFMovF32ToI32, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFMovF64ToI64, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFCvtF32ToI32, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFCvtF32ToI64, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFCvtF64ToI32, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFCvtF64ToI64, [L730PipeI0, L730FPU]>;
}

// ITOF issue class - executed by the FPU

//  fmv.h.x FPU_I0_ALU_I1 Latency 4 WriteFMovI16ToF16
//  fmv.w.x FPU_I0_ALU_I1 Latency 4 WriteFMovI32ToF32
//  fmv.d.x FPU_I0_ALU_I1 Latency 4 WriteFMovI64ToF64
//  fcvt.d.lu FPU_I0_ALU_I1 Latency 4 WriteFCvtI64ToF64
//  fcvt.d.l FPU_I0_ALU_I1 Latency 4 WriteFCvtI64ToF64
//  fcvt.d.wu FPU_I0_ALU_I1 Latency 4 WriteFCvtI32ToF64
//  fcvt.d.w FPU_I0_ALU_I1 Latency 4 WriteFCvtI32ToF64
//  fcvt.s.lu FPU_I0_ALU_I1 Latency 4 WriteFCvtI64ToF32
//  fcvt.s.l FPU_I0_ALU_I1 Latency 4 WriteFCvtI64ToF32
//  fcvt.s.wu FPU_I0_ALU_I1 Latency 4 WriteFCvtI32ToF32
//  fcvt.s.w FPU_I0_ALU_I1 Latency 4 WriteFCvtI32ToF32

let Latency = 4 in {
  // TODO: Can only dual issue in I0 alongside an ALU instruction in I1 - not sure yet how we enforce this.
  def : WriteRes<WriteFMovI16ToF16, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFMovI32ToF32, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFMovI64ToF64, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFCvtI32ToF32, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFCvtI32ToF64, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFCvtI64ToF32, [L730PipeI0, L730FPU]>;
  def : WriteRes<WriteFCvtI64ToF64, [L730PipeI0, L730FPU]>;
}

// LOAD issue class - executed by the LSU

//  fld FPU_I0_ALU_I1 Latency 4 WriteFLD64
//  flw FPU_I0_ALU_I1 Latency 4 WriteFLD32
//  flh FPU_I0_ALU_I1 Latency 4 WriteFLD16
//
//  ld LSU_ALU_DUAL Latency 3 WriteLDD
//  lwu LSU_ALU_DUAL Latency 3 WriteLDW
//  c.lhu LSU_ALU_DUAL Latency 3 WriteLDH
//  lhu LSU_ALU_DUAL Latency 3 WriteLDH
//  c.lbu LSU_ALU_DUAL Latency 3 WriteLDB
//  lbu LSU_ALU_DUAL Latency 3 WriteLDB
//  lw LSU_ALU_DUAL Latency 3 WriteLDW
//  c.lh LSU_ALU_DUAL Latency 3 WriteLDH
//  lh LSU_ALU_DUAL Latency 3 WriteLDH
//  lb LSU_ALU_DUAL Latency 3 WriteLDB
//  c.ld LSU_ALU_DUAL Latency 3 WriteLDD
//  c.lw LSU_ALU_DUAL Latency 3 WriteLDW
//
//  c.fld FPU_I0_ALU_I1 Latency 4 WriteFLD64
//  c.fldsp FPU_I0_ALU_I1 Latency 4 WriteFLD64
//
//  c.ldsp LSU_ALU_DUAL Latency 3 WriteLDD
//  c.lwsp LSU_ALU_DUAL Latency 3 WriteLDW

let Latency = 3 in {
  // TODO: Can only dual issue alongside an ALU instruction - not sure yet how we enforce this.
  def : WriteRes<WriteLDD, [L730PipeI0I1, L730LSU]>;
  def : WriteRes<WriteLDW, [L730PipeI0I1, L730LSU]>;
  def : WriteRes<WriteLDH, [L730PipeI0I1, L730LSU]>;
  def : WriteRes<WriteLDB, [L730PipeI0I1, L730LSU]>;
}

let Latency = 4 in {
  // TODO: Can only dual issue alongside an ALU instruction in I1 - not sure yet how we enforce this.
  // NOTE: These instructions are annotated as FPU_I0_ALU_I1 but documentation says these are executed by the LSU.
  def : WriteRes<WriteFLD64, [L730PipeI0, L730LSU]>;
  def : WriteRes<WriteFLD32, [L730PipeI0, L730LSU]>;
  def : WriteRes<WriteFLD16, [L730PipeI0, L730LSU]>;
}

// LSU_SPECIAL issue class - executed by the LSU
// TODO: No sched annotations

// MUL issue class - executed by the Mul unit

//  mulw MUL_ALU_DUAL Latency 3 WriteIMul32
//  mulhu MUL_ALU_DUAL Latency 3 WriteIMul
//  mulhsu MUL_ALU_DUAL Latency 3 WriteIMul
//  mulh MUL_ALU_DUAL Latency 3 WriteIMul
//  c.mul MUL_ALU_DUAL Latency 3 WriteIMul
//  mul MUL_ALU_DUAL Latency 3 WriteIMul

// Mul is a pipelined unit so it doesn't block.
let Latency = 3 in {
  // TODO: Can only dual issue alongside an ALU instruction - not sure yet how we enforce this.
  def : WriteRes<WriteIMul, [L730PipeI0I1, L730Mul]>;
  def : WriteRes<WriteIMul32, [L730PipeI0I1, L730Mul]>;
}

// PLC issue class - executed by the CSR unit

// STORE issue class - executed by the LSU

//  fsd FPU_I0_ALU_I1 Latency N/A? WriteFST64
//  fsw FPU_I0_ALU_I1 Latency N/A? WriteFST32
//  fsh FPU_I0_ALU_I1 Latency N/A? WriteFST16
//  sd LSU_ALU_DUAL Latency N/A? WriteSTD
//  sw LSU_ALU_DUAL Latency N/A? WriteSTW
//  c.sh LSU_ALU_DUAL Latency N/A? WriteSTH
//  sh LSU_ALU_DUAL Latency N/A? WriteSTH
//  c.sb LSU_ALU_DUAL Latency N/A? WriteSTB
//  sb LSU_ALU_DUAL Latency N/A? WriteSTB
//  c.sd LSU_ALU_DUAL Latency N/A? WriteSTD
//  c.sw LSU_ALU_DUAL Latency N/A? WriteSTW
//  c.fsd FPU_I0_ALU_I1 Latency N/A? WriteFST64
//  c.fsdsp FPU_I0_ALU_I1 Latency N/A? WriteFST64
//  c.sdsp LSU_ALU_DUAL Latency N/A? WriteSTD
//  c.swsp LSU_ALU_DUAL Latency N/A? WriteSTW

let Latency = 0 in {
  // TODO: Can only dual issue alongside an ALU instruction - not sure yet how we enforce this.
  def : WriteRes<WriteSTD, [L730PipeI0I1, L730LSU]>;
  def : WriteRes<WriteSTW, [L730PipeI0I1, L730LSU]>;
  def : WriteRes<WriteSTH, [L730PipeI0I1, L730LSU]>;
  def : WriteRes<WriteSTB, [L730PipeI0I1, L730LSU]>;

  // TODO: Can only dual issue alongside an ALU instruction in I1 - not sure yet how we enforce this.
  // NOTE: These instructions are annotated as FPU_I0_ALU_I1 but documentation says these are executed by the LSU.
  def : WriteRes<WriteFST64, [L730PipeI0, L730LSU]>;
  def : WriteRes<WriteFST32, [L730PipeI0, L730LSU]>;
  def : WriteRes<WriteFST16, [L730PipeI0, L730LSU]>;
}

// Unsupported extensions
defm : UnsupportedSchedSFB;
defm : UnsupportedSchedZabha;
defm : UnsupportedSchedZbc;
defm : UnsupportedSchedZfa;
defm : UnsupportedSchedZfh;
defm : UnsupportedSchedZvk;
defm : UnsupportedSchedV;
defm : UnsupportedSchedXsfvcp;
defm : UnsupportedSchedZCheriPurecap;

// Bypass based on ability to issue to late ALU. This gets a little bit closer
// to the behaviour we want, but there seems to be no way of indicating that,
// if these bypasses are utilised, there is an additional latency on the user
// (since it has to go to the late ALU).
// TODO: Currently there is no way to specify more than one kind of bypass/delay
// for a given SchedRead. We've chosen to just apply the MulDivDelay for ALU
// instructions for now on the basis that it seems to impact the performance the most.
//def : CodasipL730EarlyToLateBypass<ReadIALU>;
//def : CodasipL730EarlyToLateBypass<ReadIALU32>;
//def : CodasipL730EarlyToLateBypass<ReadPACK>;
//def : CodasipL730EarlyToLateBypass<ReadPACK32>;
//def : CodasipL730EarlyToLateBypass<ReadZIP>;
//def : CodasipL730EarlyToLateBypass<ReadCPOP>;
//def : CodasipL730EarlyToLateBypass<ReadCPOP32>;
//def : CodasipL730EarlyToLateBypass<ReadCLZ>;
//def : CodasipL730EarlyToLateBypass<ReadCLZ32>;
//def : CodasipL730EarlyToLateBypass<ReadCTZ>;
//def : CodasipL730EarlyToLateBypass<ReadCTZ32>;
//def : CodasipL730EarlyToLateBypass<ReadSHXADD>;
//def : CodasipL730EarlyToLateBypass<ReadSHXADD32>;
//def : CodasipL730EarlyToLateBypass<ReadShiftReg>;
//def : CodasipL730EarlyToLateBypass<ReadShiftReg32>;
//def : CodasipL730EarlyToLateBypass<ReadShiftImm>;
//def : CodasipL730EarlyToLateBypass<ReadShiftImm32>;
//def : CodasipL730EarlyToLateBypass<ReadRotateReg>;
//def : CodasipL730EarlyToLateBypass<ReadRotateReg32>;
//def : CodasipL730EarlyToLateBypass<ReadRotateImm>;
//def : CodasipL730EarlyToLateBypass<ReadRotateImm32>;
//def : CodasipL730EarlyToLateBypass<ReadORCB>;
//def : CodasipL730EarlyToLateBypass<ReadIMinMax>;
//def : CodasipL730EarlyToLateBypass<ReadREV8>;
//def : CodasipL730EarlyToLateBypass<ReadBREV8>;
//def : CodasipL730EarlyToLateBypass<ReadXPERM>;
//def : CodasipL730EarlyToLateBypass<ReadSingleBit>;
//def : CodasipL730EarlyToLateBypass<ReadSingleBitImm>;

// TODO: Other bypass types?

//def : CodasipL730LoadDelay<ReadIALU>;
//def : CodasipL730LoadDelay<ReadIALU32>;
//def : CodasipL730LoadDelay<ReadSingleBit>;
//def : CodasipL730LoadDelay<ReadSingleBitImm>;
//def : CodasipL730LoadDelay<ReadZIP>;
//def : CodasipL730LoadDelay<ReadREV8>;
//def : CodasipL730LoadDelay<ReadBREV8>;
//def : CodasipL730LoadDelay<ReadORCB>;
//def : CodasipL730LoadDelay<ReadRotateReg>;
//def : CodasipL730LoadDelay<ReadRotateImm>;
//def : CodasipL730LoadDelay<ReadRotateReg32>;
//def : CodasipL730LoadDelay<ReadRotateImm32>;
//def : CodasipL730LoadDelay<ReadPACK>;
//def : CodasipL730LoadDelay<ReadPACK32>;
//def : CodasipL730LoadDelay<ReadXPERM>;
//def : CodasipL730LoadDelay<ReadIMinMax>;
//def : CodasipL730LoadDelay<ReadCTZ>;
//def : CodasipL730LoadDelay<ReadCLZ>;
//def : CodasipL730LoadDelay<ReadCTZ32>;
//def : CodasipL730LoadDelay<ReadCLZ32>;
//def : CodasipL730LoadDelay<ReadShiftReg>;
//def : CodasipL730LoadDelay<ReadShiftImm>;
//def : CodasipL730LoadDelay<ReadShiftReg32>;
//def : CodasipL730LoadDelay<ReadShiftImm32>;
//def : CodasipL730LoadDelay<ReadCPOP>;
//def : CodasipL730LoadDelay<ReadCPOP32>;
//def : CodasipL730LoadDelay<ReadSHXADD>;
//def : CodasipL730LoadDelay<ReadSHXADD32>;

def : CodasipL730LoadDelay<ReadMemBase>;

def : CodasipL730MulDivDelay<ReadIALU>;
def : CodasipL730MulDivDelay<ReadIALU32>;
def : CodasipL730MulDivDelay<ReadSingleBit>;
def : CodasipL730MulDivDelay<ReadSingleBitImm>;
def : CodasipL730MulDivDelay<ReadZIP>;
def : CodasipL730MulDivDelay<ReadREV8>;
def : CodasipL730MulDivDelay<ReadBREV8>;
def : CodasipL730MulDivDelay<ReadORCB>;
def : CodasipL730MulDivDelay<ReadRotateReg>;
def : CodasipL730MulDivDelay<ReadRotateImm>;
def : CodasipL730MulDivDelay<ReadRotateReg32>;
def : CodasipL730MulDivDelay<ReadRotateImm32>;
def : CodasipL730MulDivDelay<ReadPACK>;
def : CodasipL730MulDivDelay<ReadPACK32>;
def : CodasipL730MulDivDelay<ReadXPERM>;
def : CodasipL730MulDivDelay<ReadIMinMax>;
def : CodasipL730MulDivDelay<ReadCTZ>;
def : CodasipL730MulDivDelay<ReadCLZ>;
def : CodasipL730MulDivDelay<ReadCTZ32>;
def : CodasipL730MulDivDelay<ReadCLZ32>;
def : CodasipL730MulDivDelay<ReadShiftReg>;
def : CodasipL730MulDivDelay<ReadShiftImm>;
def : CodasipL730MulDivDelay<ReadShiftReg32>;
def : CodasipL730MulDivDelay<ReadShiftImm32>;
def : CodasipL730MulDivDelay<ReadCPOP>;
def : CodasipL730MulDivDelay<ReadCPOP32>;
def : CodasipL730MulDivDelay<ReadSHXADD>;
def : CodasipL730MulDivDelay<ReadSHXADD32>;

def : CodasipL730MulDivDelay<ReadStoreData>;

def : CodasipL730MulDivDelay<ReadIDiv>;
def : CodasipL730MulDivDelay<ReadIDiv32>;
def : CodasipL730MulDivDelay<ReadIRem>;
def : CodasipL730MulDivDelay<ReadIRem32>;
def : CodasipL730MulDivDelay<ReadIMul>;
def : CodasipL730MulDivDelay<ReadIMul32>;

def : ReadAdvance<ReadJmp, 0>;
def : ReadAdvance<ReadJalr, 0>;
def : ReadAdvance<ReadCSR, 0>;
def : ReadAdvance<ReadAtomicWA, 0>;
def : ReadAdvance<ReadAtomicWD, 0>;
def : ReadAdvance<ReadAtomicDA, 0>;
def : ReadAdvance<ReadAtomicDD, 0>;
def : ReadAdvance<ReadAtomicLDW, 0>;
def : ReadAdvance<ReadAtomicLDD, 0>;
def : ReadAdvance<ReadAtomicSTW, 0>;
def : ReadAdvance<ReadAtomicSTD, 0>;
def : ReadAdvance<ReadFStoreData, 0>;
def : ReadAdvance<ReadFMemBase, 0>;
def : ReadAdvance<ReadFAdd32, 0>;
def : ReadAdvance<ReadFAdd64, 0>;
def : ReadAdvance<ReadFMul32, 0>;
def : ReadAdvance<ReadFMul64, 0>;
def : ReadAdvance<ReadFMA32, 0>;
def : ReadAdvance<ReadFMA32Addend, 0>;
def : ReadAdvance<ReadFMA64, 0>;
def : ReadAdvance<ReadFMA64Addend, 0>;
def : ReadAdvance<ReadFDiv32, 0>;
def : ReadAdvance<ReadFDiv64, 0>;
def : ReadAdvance<ReadFSqrt32, 0>;
def : ReadAdvance<ReadFSqrt64, 0>;
def : ReadAdvance<ReadFCmp32, 0>;
def : ReadAdvance<ReadFCmp64, 0>;
def : ReadAdvance<ReadFSGNJ32, 0>;
def : ReadAdvance<ReadFSGNJ64, 0>;
def : ReadAdvance<ReadFMinMax32, 0>;
def : ReadAdvance<ReadFMinMax64, 0>;
def : ReadAdvance<ReadFCvtF32ToI32, 0>;
def : ReadAdvance<ReadFCvtF32ToI64, 0>;
def : ReadAdvance<ReadFCvtF64ToI32, 0>;
def : ReadAdvance<ReadFCvtF64ToI64, 0>;
def : ReadAdvance<ReadFCvtI32ToF32, 0>;
def : ReadAdvance<ReadFCvtI32ToF64, 0>;
def : ReadAdvance<ReadFCvtI64ToF32, 0>;
def : ReadAdvance<ReadFCvtI64ToF64, 0>;
def : ReadAdvance<ReadFCvtF32ToF64, 0>;
def : ReadAdvance<ReadFCvtF64ToF32, 0>;
def : ReadAdvance<ReadFCvtF16ToF32, 0>;
def : ReadAdvance<ReadFCvtF32ToF16, 0>;
def : ReadAdvance<ReadFCvtF16ToF64, 0>;
def : ReadAdvance<ReadFCvtF64ToF16, 0>;
def : ReadAdvance<ReadFMovF16ToI16, 0>;
def : ReadAdvance<ReadFMovI16ToF16, 0>;
def : ReadAdvance<ReadFMovF32ToI32, 0>;
def : ReadAdvance<ReadFMovI32ToF32, 0>;
def : ReadAdvance<ReadFMovF64ToI64, 0>;
def : ReadAdvance<ReadFMovI64ToF64, 0>;
def : ReadAdvance<ReadFClass32, 0>;
def : ReadAdvance<ReadFClass64, 0>;
} // SchedModel = CodasipL730Model
