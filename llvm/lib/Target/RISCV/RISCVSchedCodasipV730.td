//==- RISCVSchedCodasipV730.td - V730 Scheduling Definitions -*- tablegen -*-=//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//

// TODO: Go over these.
class CodasipV730EarlyToLateBypass<SchedRead read, int cycles = 2>
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

class CodasipV730LoadDelay<SchedRead read, int cycles = -2>
    : ReadAdvance<read, cycles, [WriteLDD, WriteLDW, WriteLDH, WriteLDB]>;

class CodasipV730MulDivDelay<SchedRead read, int cycles = -3>
    : ReadAdvance<read, cycles, [WriteIMul, WriteIMul32, WriteIDiv, WriteIDiv32,
                                 WriteIRem, WriteIRem32]>;

def CodasipV730Model : SchedMachineModel {
  let MicroOpBufferSize = 0; // Explicitly set to zero since CodasipV730 is in-order.
  let IssueWidth = 2;
  let LoadLatency = 3;
  let MispredictPenalty = 3;
  let CompleteModel = 0;
  let EnableIntervals = true;
}

let SchedModel = CodasipV730Model in {

let BufferSize = 0 in {
  def V730PipeI0     : ProcResource<1>;
  def V730PipeI1     : ProcResource<1>;
  def V730Mul        : ProcResource<1>;
  def V730LSU        : ProcResource<1>;
  def V730EarlyALU   : ProcResource<2>;
  def V730LateALU    : ProcResource<2>;
  def V730FPU        : ProcResource<1>;
  def V730CCU        : ProcResource<1>;
  def V730CSR        : ProcResource<1>;
  def V730Div        : ProcResource<1>;
}

def V730PipeI0I1 : ProcResGroup<[V730PipeI0, V730PipeI1]>;

def V730EarlyLateALU : ProcResGroup<[V730EarlyALU, V730LateALU]>;


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
//  modesw.cap EARLY_LATE_ALU_DUAL Latency 1
//  modesw.int EARLY_LATE_ALU_DUAL Latency 1
//  scmode EARLY_LATE_ALU_DUAL Latency 1 WriteCHERIMod
//  gctype EARLY_LATE_ALU_DUAL Latency 1 WriteCHERIInspect
//  gcmode EARLY_LATE_ALU_DUAL Latency 1 WriteCHERIInspect
//  sceq EARLY_LATE_ALU_DUAL Latency 1 WriteCHERITest
//  schi EARLY_LATE_ALU_DUAL Latency 1 WriteCHERITest
//  gchi EARLY_LATE_ALU_DUAL Latency 1 WriteCHERIInspect
//  acperm EARLY_LATE_ALU_DUAL Latency 1
//  cmv EARLY_LATE_ALU_DUAL Latency 0 WriteCHERIMove
//  sentry EARLY_LATE_ALU_DUAL Latency 1 WriteCHERIMod
//  gcperm EARLY_LATE_ALU_DUAL Latency 1 WriteCHERIInspect
//  gctag EARLY_LATE_ALU_DUAL Latency 1 WriteCHERIInspect
//  scaddr EARLY_LATE_ALU_DUAL Latency 1 WriteCHERIMod
//  caddi EARLY_LATE_ALU_DUAL Latency 1 WriteCHERIMod
//  cadd EARLY_LATE_ALU_DUAL Latency 1 WriteCHERIMod

let Latency = 0 in
def V730SchedWriteIMat : SchedWriteRes<[V730PipeI0I1, V730EarlyALU]>;

def V730WriteIMat : SchedWriteVariant<[
  SchedVar<NoSchedPred, [V730SchedWriteIMat]>
]>;

def : InstRW<[V730WriteIMat], (instrs AUIPC, LUI, C_LUI, C_LI)>;

let Latency = 0 in
def : WriteRes<WriteCHERIMove, [V730PipeI0I1, V730EarlyLateALU]>;

let Latency = 1 in {
  def : WriteRes<WriteIALU, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteIALU32, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteNop, [V730PipeI0I1, V730EarlyLateALU]>;

  // TODO: Jumps cannot be dual-issued with other jumps.
  def : WriteRes<WriteJmp, [V730PipeI0I1, V730EarlyALU]>;
  def : WriteRes<WriteJal, [V730PipeI0I1, V730EarlyALU]>;
  def : WriteRes<WriteJalr, [V730PipeI0I1, V730EarlyALU]>;

  def : WriteRes<WritePACK, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WritePACK32, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteZIP, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteCPOP, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteCPOP32, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteCLZ, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteCLZ32, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteCTZ, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteCTZ32, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteSHXADD, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteSHXADD32, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteShiftReg, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteShiftReg32, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteShiftImm, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteShiftImm32, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteRotateReg, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteRotateReg32, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteRotateImm, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteRotateImm32, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteORCB, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteIMinMax, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteREV8, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteBREV8, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteXPERM, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteSingleBit, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteSingleBitImm, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteBEXT, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteBEXTI, [V730PipeI0I1, V730EarlyLateALU]>;

  def : WriteRes<WriteCHERIInspect, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteCHERIMod, [V730PipeI0I1, V730EarlyLateALU]>;
  def : WriteRes<WriteCHERITest, [V730PipeI0I1, V730EarlyLateALU]>;
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

//  amoswap.c SINGLE Latency 3 WriteCHERIAtomicC
//  sc.b SINGLE Latency 3 WriteCHERIAtomicSTB
//  sc.h SINGLE Latency 3 WriteCHERIAtomicSTH
//  sc.c SINGLE Latency 3 WriteCHERIAtomicSTC
//  lr.b SINGLE Latency 3 WriteCHERIAtomicLDB
//  lr.h SINGLE Latency 3 WriteCHERIAtomicLDH
//  lr.c SINGLE Latency 3 WriteCHERIAtomicLDC

let Latency = 3, SingleIssue = true in {
  def : WriteRes<WriteAtomicD, [V730PipeI0I1, V730LSU]>;
  def : WriteRes<WriteAtomicW, [V730PipeI0I1, V730LSU]>;
  def : WriteRes<WriteAtomicSTD, [V730PipeI0I1, V730LSU]>;
  def : WriteRes<WriteAtomicSTW, [V730PipeI0I1, V730LSU]>;
  def : WriteRes<WriteAtomicLDD, [V730PipeI0I1, V730LSU]>;
  def : WriteRes<WriteAtomicLDW, [V730PipeI0I1, V730LSU]>;

  def : WriteRes<WriteCHERIAtomicC, [V730PipeI0I1, V730LSU]>;
  def : WriteRes<WriteCHERIAtomicSTB, [V730PipeI0I1, V730LSU]>;
  def : WriteRes<WriteCHERIAtomicSTH, [V730PipeI0I1, V730LSU]>;
  def : WriteRes<WriteCHERIAtomicSTC, [V730PipeI0I1, V730LSU]>;
  def : WriteRes<WriteCHERIAtomicLDB, [V730PipeI0I1, V730LSU]>;
  def : WriteRes<WriteCHERIAtomicLDH, [V730PipeI0I1, V730LSU]>;
  def : WriteRes<WriteCHERIAtomicLDC, [V730PipeI0I1, V730LSU]>;
}

// CCU issue class - executed by the CCU

//  gclen CCU_I0_ALU_I1 Latency 3 WriteCHERIInspect !! Ambiguous SchedWrite !!
//  gcbase CCU_I0_ALU_I1 Latency 3 WriteCHERIInspect !! Ambiguous SchedWrite !!
//  cram CCU_I0_ALU_I1 Latency 3 WriteCHERIComputeMask
//  scbndsr CCU_I0_ALU_I1 Latency 3 WriteCHERIMod !! Ambiguous SchedWrite !!
//  scbndsi CCU_I0_ALU_I1 Latency 3 WriteCHERIMod !! Ambiguous SchedWrite !!
//  scbnds CCU_I0_ALU_I1 Latency 3 WriteCHERIMod !! Ambiguous SchedWrite !!
//  cbld CCU_I0_ALU_I1 Latency 3 WriteCHERIMod !! Ambiguous SchedWrite !!
//  scss CCU_I0_ALU_I1 Latency 3 WriteCHERITest !! Ambiguous SchedWrite !!

let Latency = 3 in
def V730SchedWriteCCU : SchedWriteRes<[V730PipeI0, V730CCU]>;

def V730WriteCHERICCU : SchedWriteVariant<[
  SchedVar<NoSchedPred, [V730SchedWriteCCU]>
]>;

def : InstRW<[V730WriteCHERICCU],
             (instrs GCLEN, GCBASE, SCBNDS, SCBNDSR, SCBNDSI, CBLD, SCSS)>;

let Latency = 3 in {
  def : WriteRes<WriteCHERIComputeMask, [V730PipeI0, V730CCU]>;
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
def V730SchedWriteCSRSimple : SchedWriteRes<[V730PipeI0, V730CSR]>;

let Latency = 3, SingleIssue = true in
def V730SchedWriteCSR : SchedWriteRes<[V730PipeI0I1, V730CSR]>;

def V730CSRIsSimplePred : MCSchedPredicate<
  CheckAny<[
    CheckAll<[CheckIsRegOperand<2>, CheckRegOperand<2, X0>]>,
    CheckAll<[CheckIsImmOperand<2>, CheckImmOperand<2, 0>]>
  ]>
>;

def V730WriteCSRSimple : SchedWriteVariant<[
  SchedVar<V730CSRIsSimplePred, [V730SchedWriteCSRSimple]>,
  SchedVar<NoSchedPred, [V730SchedWriteCSR]>
]>;

def : InstRW<[V730WriteCSRSimple], (instrs CSRRCI, CSRRSI, CSRRC, CSRRS)>;

let Latency = 3, SingleIssue = true in
def : WriteRes<WriteCSR, [V730PipeI0I1, V730CSR]>;

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
  def : WriteRes<WriteIDiv, [V730PipeI0I1, V730Div]>;
  def : WriteRes<WriteIDiv32, [V730PipeI0I1, V730Div]>;
  def : WriteRes<WriteIRem, [V730PipeI0I1, V730Div]>;
  def : WriteRes<WriteIRem32, [V730PipeI0I1, V730Div]>;
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
  def : WriteRes<WriteFCmp32, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFCmp64, [V730PipeI0, V730FPU]>;
}

let Latency = 4 in {
  // TODO: Can only dual issue in I0 alongside an ALU instruction in I1 - not sure yet how we enforce this.
  def : WriteRes<WriteFMinMax64, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFMinMax32, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFSGNJ64, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFSGNJ32, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFCvtF32ToF64, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFCvtF64ToF32, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFCvtF16ToF64, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFCvtF64ToF16, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFCvtF32ToF16, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFCvtF16ToF32, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFAdd64, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFAdd32, [V730PipeI0, V730FPU]>;
}

let Latency = 6 in {
  // TODO: Can only dual issue in I0 alongside an ALU instruction in I1 - not sure yet how we enforce this.
  def : WriteRes<WriteFMul64, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFMA64, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFMul32, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFMA32, [V730PipeI0, V730FPU]>;
}

let Latency = 32, ReleaseAtCycles = [32, 32], SingleIssue = true in {
  def : WriteRes<WriteFDiv64, [V730PipeI0I1, V730FPU]>;
  def : WriteRes<WriteFSqrt64, [V730PipeI0I1, V730FPU]>;
  def : WriteRes<WriteFDiv32, [V730PipeI0I1, V730FPU]>;
  def : WriteRes<WriteFSqrt32, [V730PipeI0I1, V730FPU]>;
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
  def : WriteRes<WriteFClass32, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFClass64, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFMovF16ToI16, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFMovF32ToI32, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFMovF64ToI64, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFCvtF32ToI32, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFCvtF32ToI64, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFCvtF64ToI32, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFCvtF64ToI64, [V730PipeI0, V730FPU]>;
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
  def : WriteRes<WriteFMovI16ToF16, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFMovI32ToF32, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFMovI64ToF64, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFCvtI32ToF32, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFCvtI32ToF64, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFCvtI64ToF32, [V730PipeI0, V730FPU]>;
  def : WriteRes<WriteFCvtI64ToF64, [V730PipeI0, V730FPU]>;
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
//
//  lc LSU_ALU_DUAL Latency 3 WriteCHERILoadCap

let Latency = 3 in {
  // TODO: Can only dual issue alongside an ALU instruction - not sure yet how we enforce this.
  def : WriteRes<WriteLDD, [V730PipeI0I1, V730LSU]>;
  def : WriteRes<WriteLDW, [V730PipeI0I1, V730LSU]>;
  def : WriteRes<WriteLDH, [V730PipeI0I1, V730LSU]>;
  def : WriteRes<WriteLDB, [V730PipeI0I1, V730LSU]>;
  def : WriteRes<WriteCHERILoadCap, [V730PipeI0I1, V730LSU]>;
}

let Latency = 4 in {
  // TODO: Can only dual issue alongside an ALU instruction in I1 - not sure yet how we enforce this.
  // NOTE: These instructions are annotated as FPU_I0_ALU_I1 but documentation says these are executed by the LSU.
  def : WriteRes<WriteFLD64, [V730PipeI0, V730LSU]>;
  def : WriteRes<WriteFLD32, [V730PipeI0, V730LSU]>;
  def : WriteRes<WriteFLD16, [V730PipeI0, V730LSU]>;
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
  def : WriteRes<WriteIMul, [V730PipeI0I1, V730Mul]>;
  def : WriteRes<WriteIMul32, [V730PipeI0I1, V730Mul]>;
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
//
//  lc LSU_ALU_DUAL Latency N/A WriteCHERIStoreCap

let Latency = 0 in {
  // TODO: Can only dual issue alongside an ALU instruction - not sure yet how we enforce this.
  def : WriteRes<WriteSTD, [V730PipeI0I1, V730LSU]>;
  def : WriteRes<WriteSTW, [V730PipeI0I1, V730LSU]>;
  def : WriteRes<WriteSTH, [V730PipeI0I1, V730LSU]>;
  def : WriteRes<WriteSTB, [V730PipeI0I1, V730LSU]>;
  def : WriteRes<WriteCHERIStoreCap, [V730PipeI0I1, V730LSU]>;

  // TODO: Can only dual issue alongside an ALU instruction in I1 - not sure yet how we enforce this.
  // NOTE: These instructions are annotated as FPU_I0_ALU_I1 but documentation says these are executed by the LSU.
  def : WriteRes<WriteFST64, [V730PipeI0, V730LSU]>;
  def : WriteRes<WriteFST32, [V730PipeI0, V730LSU]>;
  def : WriteRes<WriteFST16, [V730PipeI0, V730LSU]>;
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

// Bypass based on ability to issue to late ALU. This gets a little bit closer
// to the behaviour we want, but there seems to be no way of indicating that,
// if these bypasses are utilised, there is an additional latency on the user
// (since it has to go to the late ALU).
// TODO: Currently there is no way to specify more than one kind of bypass/delay
// for a given SchedRead. We've chosen to just apply the MulDivDelay for ALU
// instructions for now on the basis that it seems to impact the performance the most.
//def : CodasipV730EarlyToLateBypass<ReadIALU>;
//def : CodasipV730EarlyToLateBypass<ReadIALU32>;
//def : CodasipV730EarlyToLateBypass<ReadPACK>;
//def : CodasipV730EarlyToLateBypass<ReadPACK32>;
//def : CodasipV730EarlyToLateBypass<ReadZIP>;
//def : CodasipV730EarlyToLateBypass<ReadCPOP>;
//def : CodasipV730EarlyToLateBypass<ReadCPOP32>;
//def : CodasipV730EarlyToLateBypass<ReadCLZ>;
//def : CodasipV730EarlyToLateBypass<ReadCLZ32>;
//def : CodasipV730EarlyToLateBypass<ReadCTZ>;
//def : CodasipV730EarlyToLateBypass<ReadCTZ32>;
//def : CodasipV730EarlyToLateBypass<ReadSHXADD>;
//def : CodasipV730EarlyToLateBypass<ReadSHXADD32>;
//def : CodasipV730EarlyToLateBypass<ReadShiftReg>;
//def : CodasipV730EarlyToLateBypass<ReadShiftReg32>;
//def : CodasipV730EarlyToLateBypass<ReadShiftImm>;
//def : CodasipV730EarlyToLateBypass<ReadShiftImm32>;
//def : CodasipV730EarlyToLateBypass<ReadRotateReg>;
//def : CodasipV730EarlyToLateBypass<ReadRotateReg32>;
//def : CodasipV730EarlyToLateBypass<ReadRotateImm>;
//def : CodasipV730EarlyToLateBypass<ReadRotateImm32>;
//def : CodasipV730EarlyToLateBypass<ReadORCB>;
//def : CodasipV730EarlyToLateBypass<ReadIMinMax>;
//def : CodasipV730EarlyToLateBypass<ReadREV8>;
//def : CodasipV730EarlyToLateBypass<ReadBREV8>;
//def : CodasipV730EarlyToLateBypass<ReadXPERM>;
//def : CodasipV730EarlyToLateBypass<ReadSingleBit>;
//def : CodasipV730EarlyToLateBypass<ReadSingleBitImm>;

// TODO: Other bypass types?

//def : CodasipV730LoadDelay<ReadIALU>;
//def : CodasipV730LoadDelay<ReadIALU32>;
//def : CodasipV730LoadDelay<ReadSingleBit>;
//def : CodasipV730LoadDelay<ReadSingleBitImm>;
//def : CodasipV730LoadDelay<ReadZIP>;
//def : CodasipV730LoadDelay<ReadREV8>;
//def : CodasipV730LoadDelay<ReadBREV8>;
//def : CodasipV730LoadDelay<ReadORCB>;
//def : CodasipV730LoadDelay<ReadRotateReg>;
//def : CodasipV730LoadDelay<ReadRotateImm>;
//def : CodasipV730LoadDelay<ReadRotateReg32>;
//def : CodasipV730LoadDelay<ReadRotateImm32>;
//def : CodasipV730LoadDelay<ReadPACK>;
//def : CodasipV730LoadDelay<ReadPACK32>;
//def : CodasipV730LoadDelay<ReadXPERM>;
//def : CodasipV730LoadDelay<ReadIMinMax>;
//def : CodasipV730LoadDelay<ReadCTZ>;
//def : CodasipV730LoadDelay<ReadCLZ>;
//def : CodasipV730LoadDelay<ReadCTZ32>;
//def : CodasipV730LoadDelay<ReadCLZ32>;
//def : CodasipV730LoadDelay<ReadShiftReg>;
//def : CodasipV730LoadDelay<ReadShiftImm>;
//def : CodasipV730LoadDelay<ReadShiftReg32>;
//def : CodasipV730LoadDelay<ReadShiftImm32>;
//def : CodasipV730LoadDelay<ReadCPOP>;
//def : CodasipV730LoadDelay<ReadCPOP32>;
//def : CodasipV730LoadDelay<ReadSHXADD>;
//def : CodasipV730LoadDelay<ReadSHXADD32>;

def : CodasipV730LoadDelay<ReadMemBase>;

def : CodasipV730MulDivDelay<ReadIALU>;
def : CodasipV730MulDivDelay<ReadIALU32>;
def : CodasipV730MulDivDelay<ReadSingleBit>;
def : CodasipV730MulDivDelay<ReadSingleBitImm>;
def : CodasipV730MulDivDelay<ReadZIP>;
def : CodasipV730MulDivDelay<ReadREV8>;
def : CodasipV730MulDivDelay<ReadBREV8>;
def : CodasipV730MulDivDelay<ReadORCB>;
def : CodasipV730MulDivDelay<ReadRotateReg>;
def : CodasipV730MulDivDelay<ReadRotateImm>;
def : CodasipV730MulDivDelay<ReadRotateReg32>;
def : CodasipV730MulDivDelay<ReadRotateImm32>;
def : CodasipV730MulDivDelay<ReadPACK>;
def : CodasipV730MulDivDelay<ReadPACK32>;
def : CodasipV730MulDivDelay<ReadXPERM>;
def : CodasipV730MulDivDelay<ReadIMinMax>;
def : CodasipV730MulDivDelay<ReadCTZ>;
def : CodasipV730MulDivDelay<ReadCLZ>;
def : CodasipV730MulDivDelay<ReadCTZ32>;
def : CodasipV730MulDivDelay<ReadCLZ32>;
def : CodasipV730MulDivDelay<ReadShiftReg>;
def : CodasipV730MulDivDelay<ReadShiftImm>;
def : CodasipV730MulDivDelay<ReadShiftReg32>;
def : CodasipV730MulDivDelay<ReadShiftImm32>;
def : CodasipV730MulDivDelay<ReadCPOP>;
def : CodasipV730MulDivDelay<ReadCPOP32>;
def : CodasipV730MulDivDelay<ReadSHXADD>;
def : CodasipV730MulDivDelay<ReadSHXADD32>;

def : CodasipV730MulDivDelay<ReadStoreData>;

def : CodasipV730MulDivDelay<ReadIDiv>;
def : CodasipV730MulDivDelay<ReadIDiv32>;
def : CodasipV730MulDivDelay<ReadIRem>;
def : CodasipV730MulDivDelay<ReadIRem32>;
def : CodasipV730MulDivDelay<ReadIMul>;
def : CodasipV730MulDivDelay<ReadIMul32>;

def : ReadAdvance<ReadCHERIInspect, 0>;
def : ReadAdvance<ReadCHERIMod, 0>;
def : ReadAdvance<ReadCHERITest, 0>;
def : ReadAdvance<ReadCHERIMove, 0>;
def : ReadAdvance<ReadCHERIAtomicC, 0>;
def : ReadAdvance<ReadCHERIAtomicLDB, 0>;
def : ReadAdvance<ReadCHERIAtomicLDH, 0>;
def : ReadAdvance<ReadCHERIAtomicLDC, 0>;
def : ReadAdvance<ReadCHERIAtomicSTB, 0>;
def : ReadAdvance<ReadCHERIAtomicSTH, 0>;
def : ReadAdvance<ReadCHERIAtomicSTC, 0>;
def : ReadAdvance<ReadCHERIComputeMask, 0>;
def : ReadAdvance<ReadCHERILoadCap, 0>;
def : ReadAdvance<ReadCHERIStoreCap, 0>;
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
} // SchedModel = CodasipV730Model
