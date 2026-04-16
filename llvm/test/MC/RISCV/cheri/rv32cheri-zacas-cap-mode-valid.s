# RUN: llvm-mc -triple=riscv32 -mattr=+a,+zacas,+xcheri,+cap-mode -riscv-no-aliases -show-encoding < %s\
# RUN:     | FileCheck -check-prefixes=CHECK-ASM,CHECK-ASM-AND-OBJ %s
# RUN: llvm-mc -triple=riscv64 -mattr=+a,+zacas,+xcheri,+cap-mode -riscv-no-aliases -show-encoding < %s\
# RUN:     | FileCheck -check-prefixes=CHECK-ASM,CHECK-ASM-AND-OBJ %s
# RUN: llvm-mc -filetype=obj -triple=riscv32 -mattr=+a,+zacas,+xcheri,+cap-mode < %s \
# RUN:     | llvm-objdump --mattr=+a,+zacas,+xcheri,+cap-mode -M no-aliases -d -r - \
# RUN:     | FileCheck -check-prefixes=CHECK-ASM-AND-OBJ %s
# RUN: llvm-mc -filetype=obj -triple=riscv64 -mattr=+a,+zacas,+xcheri,+cap-mode < %s \
# RUN:     | llvm-objdump --mattr=+a,+zacas,+xcheri,+cap-mode -M no-aliases -d -r - \
# RUN:     | FileCheck -check-prefixes=CHECK-ASM-AND-OBJ %s

## Same test again without the "c" prefix on all lines
# RUN: sed -e 's/^c//' < %s > %t.s
# RUN: llvm-mc %t.s -triple=riscv32 -mattr=+a,+zacas,+xcheri,+cap-mode -riscv-no-aliases -show-encoding \
# RUN:     | FileCheck -check-prefixes=CHECK-ASM,CHECK-ASM-AND-OBJ %s
# RUN: llvm-mc %t.s -triple=riscv64 -mattr=+a,+zacas,+xcheri,+cap-mode -riscv-no-aliases -show-encoding \
# RUN:     | FileCheck -check-prefixes=CHECK-ASM,CHECK-ASM-AND-OBJ %s
# RUN: llvm-mc -filetype=obj -triple=riscv32 -mattr=+a,+zacas,+xcheri,+cap-mode < %t.s \
# RUN:     | llvm-objdump --mattr=+a,+zacas,+xcheri,+cap-mode -M no-aliases -d -r - \
# RUN:     | FileCheck -check-prefixes=CHECK-ASM-AND-OBJ %s
# RUN: llvm-mc -filetype=obj -triple=riscv64 -mattr=+a,+zacas,+xcheri,+cap-mode < %t.s \
# RUN:     | llvm-objdump --mattr=+a,+zacas,+xcheri,+cap-mode -M no-aliases -d -r - \
# RUN:     | FileCheck -check-prefixes=CHECK-ASM-AND-OBJ %s

# CHECK-ASM-AND-OBJ: {{[[:<:]]}}amocas.w t0, t1, (ct2)
# CHECK-ASM: encoding: [0xaf,0xa2,0x63,0x28]
camocas.w t0, t1, (ct2)

# CHECK-ASM-AND-OBJ: {{[[:<:]]}}amocas.d t1, t3, (ct5)
# CHECK-ASM: encoding: [0x2f,0x33,0xcf,0x29]
camocas.d t1, t3, (ct5)
