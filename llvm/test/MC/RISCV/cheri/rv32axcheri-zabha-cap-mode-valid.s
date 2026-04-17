# RUN: llvm-mc -triple=riscv32 -mattr=+a,+zabha,+xcheri,+cap-mode -riscv-no-aliases -show-encoding < %s\
# RUN:     | FileCheck -check-prefixes=CHECK-ASM,CHECK-ASM-AND-OBJ %s
# RUN: llvm-mc -triple=riscv64 -mattr=+a,+zabha,+xcheri,+cap-mode -riscv-no-aliases -show-encoding < %s\
# RUN:     | FileCheck -check-prefixes=CHECK-ASM,CHECK-ASM-AND-OBJ %s
# RUN: llvm-mc -filetype=obj -triple=riscv32 -mattr=+a,+zabha,+xcheri,+cap-mode < %s \
# RUN:     | llvm-objdump --mattr=+a,+zabha,+xcheri,+cap-mode -M no-aliases -d -r - \
# RUN:     | FileCheck -check-prefixes=CHECK-ASM-AND-OBJ %s
# RUN: llvm-mc -filetype=obj -triple=riscv64 -mattr=+a,+zabha,+xcheri,+cap-mode < %s \
# RUN:     | llvm-objdump --mattr=+a,+zabha,+xcheri,+cap-mode -M no-aliases -d -r - \
# RUN:     | FileCheck -check-prefixes=CHECK-ASM-AND-OBJ %s

## Same test again without the "c" prefix on all lines
# RUN: sed -e 's/^c//' < %s > %t.s
# RUN: llvm-mc %t.s -triple=riscv32 -mattr=+a,+zabha,+xcheri,+cap-mode -riscv-no-aliases -show-encoding \
# RUN:     | FileCheck -check-prefixes=CHECK-ASM,CHECK-ASM-AND-OBJ %s
# RUN: llvm-mc %t.s -triple=riscv64 -mattr=+a,+zabha,+xcheri,+cap-mode -riscv-no-aliases -show-encoding \
# RUN:     | FileCheck -check-prefixes=CHECK-ASM,CHECK-ASM-AND-OBJ %s
# RUN: llvm-mc -filetype=obj -triple=riscv32 -mattr=+a,+zabha,+xcheri,+cap-mode < %t.s \
# RUN:     | llvm-objdump --mattr=+a,+zabha,+xcheri,+cap-mode -M no-aliases -d -r - \
# RUN:     | FileCheck -check-prefixes=CHECK-ASM-AND-OBJ %s
# RUN: llvm-mc -filetype=obj -triple=riscv64 -mattr=+a,+zabha,+xcheri,+cap-mode < %t.s \
# RUN:     | llvm-objdump --mattr=+a,+zabha,+xcheri,+cap-mode -M no-aliases -d -r - \
# RUN:     | FileCheck -check-prefixes=CHECK-ASM-AND-OBJ %s

# CHECK-ASM-AND-OBJ: {{[[:<:]]}}amoswap.b t0, t1, (ct2)
# CHECK-ASM: encoding: [0xaf,0x82,0x63,0x08]
camoswap.b t0, t1, (ct2)
# CHECK-ASM-AND-OBJ: {{[[:<:]]}}amoadd.b t1, t2, (ct3)
# CHECK-ASM: encoding: [0x2f,0x03,0x7e,0x00]
camoadd.b t1, t2, (ct3)
# CHECK-ASM-AND-OBJ: {{[[:<:]]}}amoxor.b t2, t3, (ct4)
# CHECK-ASM: encoding: [0xaf,0x83,0xce,0x21]
camoxor.b t2, t3, (ct4)
# CHECK-ASM-AND-OBJ: {{[[:<:]]}}amoand.b t3, t4, (ct5)
# CHECK-ASM: encoding: [0x2f,0x0e,0xdf,0x61]
camoand.b t3, t4, (ct5)
# CHECK-ASM-AND-OBJ: {{[[:<:]]}}amoor.b t4, t5, (ct6)
# CHECK-ASM: encoding: [0xaf,0x8e,0xef,0x41]
camoor.b t4, t5, (ct6)
# CHECK-ASM-AND-OBJ: {{[[:<:]]}}amomin.b t5, t6, (ca0)
# CHECK-ASM: encoding: [0x2f,0x0f,0xf5,0x81]
camomin.b t5, t6, (ca0)
# CHECK-ASM-AND-OBJ: {{[[:<:]]}}amomax.b t6, a0, (ca1)
# CHECK-ASM: encoding: [0xaf,0x8f,0xa5,0xa0]
camomax.b t6, a0, (ca1)
# CHECK-ASM-AND-OBJ: {{[[:<:]]}}amominu.b a0, a1, (ca2)
# CHECK-ASM: encoding: [0x2f,0x05,0xb6,0xc0]
camominu.b a0, a1, (ca2)
# CHECK-ASM-AND-OBJ: {{[[:<:]]}}amomaxu.b a1, a2, (ca3)
# CHECK-ASM: encoding: [0xaf,0x85,0xc6,0xe0]
camomaxu.b a1, a2, (ca3)

# CHECK-ASM-AND-OBJ: {{[[:<:]]}}amoswap.h t0, t1, (ct2)
# CHECK-ASM: encoding: [0xaf,0x92,0x63,0x08]
camoswap.h t0, t1, (ct2)
# CHECK-ASM-AND-OBJ: {{[[:<:]]}}amoadd.h t1, t2, (ct3)
# CHECK-ASM: encoding: [0x2f,0x13,0x7e,0x00]
camoadd.h t1, t2, (ct3)
# CHECK-ASM-AND-OBJ: {{[[:<:]]}}amoxor.h t2, t3, (ct4)
# CHECK-ASM: encoding: [0xaf,0x93,0xce,0x21]
camoxor.h t2, t3, (ct4)
# CHECK-ASM-AND-OBJ: {{[[:<:]]}}amoand.h t3, t4, (ct5)
# CHECK-ASM: encoding: [0x2f,0x1e,0xdf,0x61]
camoand.h t3, t4, (ct5)
# CHECK-ASM-AND-OBJ: {{[[:<:]]}}amoor.h t4, t5, (ct6)
# CHECK-ASM: encoding: [0xaf,0x9e,0xef,0x41]
camoor.h t4, t5, (ct6)
# CHECK-ASM-AND-OBJ: {{[[:<:]]}}amomin.h t5, t6, (ca0)
# CHECK-ASM: encoding: [0x2f,0x1f,0xf5,0x81]
camomin.h t5, t6, (ca0)
# CHECK-ASM-AND-OBJ: {{[[:<:]]}}amomax.h t6, a0, (ca1)
# CHECK-ASM: encoding: [0xaf,0x9f,0xa5,0xa0]
camomax.h t6, a0, (ca1)
# CHECK-ASM-AND-OBJ: {{[[:<:]]}}amominu.h a0, a1, (ca2)
# CHECK-ASM: encoding: [0x2f,0x15,0xb6,0xc0]
camominu.h a0, a1, (ca2)
# CHECK-ASM-AND-OBJ: {{[[:<:]]}}amomaxu.h a1, a2, (ca3)
# CHECK-ASM: encoding: [0xaf,0x95,0xc6,0xe0]
camomaxu.h a1, a2, (ca3)
