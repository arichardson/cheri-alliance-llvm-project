# RUN: llvm-mc --triple=riscv64 --mattr=+experimental-y,+cap-mode --show-encoding < %s \
# RUN:   | FileCheck --check-prefix=RVY %s
# RUN: llvm-mc --triple=riscv64 --mattr=+zcheripurecap,+cap-mode --show-encoding < %s \
# RUN:   | FileCheck --check-prefix=ZCHERI %s

## In capability mode the standard loads/stores take a capability base
## register. The encodings are identical for RVY and Zcheripurecap; only the
## spelling of the base register differs (RVY uses the plain "x"/ABI names,
## Zcheripurecap uses the "c"-prefixed ones). Upstream picks the base operand
## type with RegClassByHwMode; this fork shares one definition between both
## and selects the spelling in RISCVInstPrinter::printV9CR instead.
##
## Note: the base register is checked as part of a whole "0(reg)" match so
## that the RVY expectation cannot also match the "c"-prefixed spelling.

lb a0, 0(a1)
# RVY:    lb a0, 0(a1) # encoding: [0x03,0x85,0x05,0x00]
# ZCHERI: lb a0, 0(ca1) # encoding: [0x03,0x85,0x05,0x00]

lw a0, 4(a1)
# RVY:    lw a0, 4(a1) # encoding: [0x03,0xa5,0x45,0x00]
# ZCHERI: lw a0, 4(ca1) # encoding: [0x03,0xa5,0x45,0x00]

ld a0, 8(a1)
# RVY:    ld a0, 8(a1) # encoding: [0x03,0xb5,0x85,0x00]
# ZCHERI: ld a0, 8(ca1) # encoding: [0x03,0xb5,0x85,0x00]

sb a0, 0(a1)
# RVY:    sb a0, 0(a1) # encoding: [0x23,0x80,0xa5,0x00]
# ZCHERI: sb a0, 0(ca1) # encoding: [0x23,0x80,0xa5,0x00]

sd a0, 8(a1)
# RVY:    sd a0, 8(a1) # encoding: [0x23,0xb4,0xa5,0x00]
# ZCHERI: sd a0, 8(ca1) # encoding: [0x23,0xb4,0xa5,0x00]
