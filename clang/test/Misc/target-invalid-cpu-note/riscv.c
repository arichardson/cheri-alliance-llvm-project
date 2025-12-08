// This test uses '<prefix>-SAME: {{^}}' to start matching immediately where the
// previous check finished matching (specifically, caret is not treated as
// matching a start of line when used like this in FileCheck).

// RUN: not %clang_cc1 -triple riscv32 -target-cpu not-a-cpu -fsyntax-only %s 2>&1 | FileCheck %s --check-prefix RISCV32
// RISCV32: error: unknown target CPU 'not-a-cpu'
// RISCV32-NEXT: note: valid target CPU values are:
// RISCV32-SAME: {{^}} codasip-v735-amber
// RISCV32-SAME: {{^}}, codasip-l730-amethyst
// RISCV32-SAME: {{^}}, codasip-l730-diamond
// RISCV32-SAME: {{^}}, codasip-l730-glim
// RISCV32-SAME: {{^}}, codasip-l735-ruby
// RISCV32-SAME: {{^}}, codasip-l735-ruby-codal
// RISCV32-SAME: {{^}}, codasip-v730-shine
// RISCV32-SAME: {{^}}, codasip-v739-spinel
// RISCV32-SAME: {{^}}, codasip-l739-topaz
// RISCV32-SAME: {{^}}, codasip-l730-zephire
// RISCV32-SAME: {{^}}, generic-rv32
// RISCV32-SAME: {{^}}, rocket-rv32
// RISCV32-SAME: {{^}}, rp2350-hazard3
// RISCV32-SAME: {{^}}, sifive-e20
// RISCV32-SAME: {{^}}, sifive-e21
// RISCV32-SAME: {{^}}, sifive-e24
// RISCV32-SAME: {{^}}, sifive-e31
// RISCV32-SAME: {{^}}, sifive-e34
// RISCV32-SAME: {{^}}, sifive-e76
// RISCV32-SAME: {{^}}, syntacore-scr1-base
// RISCV32-SAME: {{^}}, syntacore-scr1-max
// RISCV32-SAME: {{^}}, syntacore-scr3-rv32
// RISCV32-SAME: {{^}}, syntacore-scr4-rv32
// RISCV32-SAME: {{^}}, syntacore-scr5-rv32
// RISCV32-SAME: {{$}}

// RUN: not %clang_cc1 -triple riscv64 -target-cpu not-a-cpu -fsyntax-only %s 2>&1 | FileCheck %s --check-prefix RISCV64
// RISCV64: error: unknown target CPU 'not-a-cpu'
// RISCV64-NEXT: note: valid target CPU values are:
// RISCV64-SAME: {{^}} codasip-a730-aura
// RISCV64-SAME: {{^}}, codasip-a730-aura-codal
// RISCV64-SAME: {{^}}, codasip-a730-flint
// RISCV64-SAME: {{^}}, codasip-x730-jade
// RISCV64-SAME: {{^}}, codasip-x730-lapis
// RISCV64-SAME: {{^}}, codasip-x730-lunox
// RISCV64-SAME: {{^}}, codasip-x730-lux
// RISCV64-SAME: {{^}}, codasip-x730-spark
// RISCV64-SAME: {{^}}, generic-rv64
// RISCV64-SAME: {{^}}, mips-p8700
// RISCV64-SAME: {{^}}, rocket-rv64
// RISCV64-SAME: {{^}}, sifive-p450
// RISCV64-SAME: {{^}}, sifive-p470
// RISCV64-SAME: {{^}}, sifive-p550
// RISCV64-SAME: {{^}}, sifive-p670
// RISCV64-SAME: {{^}}, sifive-s21
// RISCV64-SAME: {{^}}, sifive-s51
// RISCV64-SAME: {{^}}, sifive-s54
// RISCV64-SAME: {{^}}, sifive-s76
// RISCV64-SAME: {{^}}, sifive-u54
// RISCV64-SAME: {{^}}, sifive-u74
// RISCV64-SAME: {{^}}, sifive-x280
// RISCV64-SAME: {{^}}, spacemit-x60
// RISCV64-SAME: {{^}}, syntacore-scr3-rv64
// RISCV64-SAME: {{^}}, syntacore-scr4-rv64
// RISCV64-SAME: {{^}}, syntacore-scr5-rv64
// RISCV64-SAME: {{^}}, syntacore-scr7
// RISCV64-SAME: {{^}}, tt-ascalon-d8
// RISCV64-SAME: {{^}}, veyron-v1
// RISCV64-SAME: {{^}}, xiangshan-nanhu
// RISCV64-SAME: {{$}}

// RUN: not %clang_cc1 -triple riscv32 -tune-cpu not-a-cpu -fsyntax-only %s 2>&1 | FileCheck %s --check-prefix TUNE-RISCV32
// TUNE-RISCV32: error: unknown target CPU 'not-a-cpu'
// TUNE-RISCV32-NEXT: note: valid target CPU values are:
// TUNE-RISCV32-SAME: {{^}} codasip-v735-amber
// TUNE-RISCV32-SAME: {{^}}, codasip-l730-amethyst
// TUNE-RISCV32-SAME: {{^}}, codasip-l730-diamond
// TUNE-RISCV32-SAME: {{^}}, codasip-l730-glim
// TUNE-RISCV32-SAME: {{^}}, codasip-l735-ruby
// TUNE-RISCV32-SAME: {{^}}, codasip-l735-ruby-codal
// TUNE-RISCV32-SAME: {{^}}, codasip-v730-shine
// TUNE-RISCV32-SAME: {{^}}, codasip-v739-spinel
// TUNE-RISCV32-SAME: {{^}}, codasip-l739-topaz
// TUNE-RISCV32-SAME: {{^}}, codasip-l730-zephire
// TUNE-RISCV32-SAME: {{^}}, generic-rv32
// TUNE-RISCV32-SAME: {{^}}, rocket-rv32
// TUNE-RISCV32-SAME: {{^}}, rp2350-hazard3
// TUNE-RISCV32-SAME: {{^}}, sifive-e20
// TUNE-RISCV32-SAME: {{^}}, sifive-e21
// TUNE-RISCV32-SAME: {{^}}, sifive-e24
// TUNE-RISCV32-SAME: {{^}}, sifive-e31
// TUNE-RISCV32-SAME: {{^}}, sifive-e34
// TUNE-RISCV32-SAME: {{^}}, sifive-e76
// TUNE-RISCV32-SAME: {{^}}, syntacore-scr1-base
// TUNE-RISCV32-SAME: {{^}}, syntacore-scr1-max
// TUNE-RISCV32-SAME: {{^}}, syntacore-scr3-rv32
// TUNE-RISCV32-SAME: {{^}}, syntacore-scr4-rv32
// TUNE-RISCV32-SAME: {{^}}, syntacore-scr5-rv32
// TUNE-RISCV32-SAME: {{^}}, generic
// TUNE-RISCV32-SAME: {{^}}, rocket
// TUNE-RISCV32-SAME: {{^}}, sifive-7-series
// TUNE-RISCV32-SAME: {{$}}

// RUN: not %clang_cc1 -triple riscv64 -tune-cpu not-a-cpu -fsyntax-only %s 2>&1 | FileCheck %s --check-prefix TUNE-RISCV64
// TUNE-RISCV64: error: unknown target CPU 'not-a-cpu'
// TUNE-RISCV64-NEXT: note: valid target CPU values are:
// TUNE-RISCV64-SAME: {{^}} codasip-a730-aura
// TUNE-RISCV64-SAME: {{^}}, codasip-a730-aura-codal
// TUNE-RISCV64-SAME: {{^}}, codasip-a730-flint
// TUNE-RISCV64-SAME: {{^}}, codasip-x730-jade
// TUNE-RISCV64-SAME: {{^}}, codasip-x730-lapis
// TUNE-RISCV64-SAME: {{^}}, codasip-x730-lunox
// TUNE-RISCV64-SAME: {{^}}, codasip-x730-lux
// TUNE-RISCV64-SAME: {{^}}, codasip-x730-spark
// TUNE-RISCV64-SAME: {{^}}, generic-rv64
// TUNE-RISCV64-SAME: {{^}}, mips-p8700
// TUNE-RISCV64-SAME: {{^}}, rocket-rv64
// TUNE-RISCV64-SAME: {{^}}, sifive-p450
// TUNE-RISCV64-SAME: {{^}}, sifive-p470
// TUNE-RISCV64-SAME: {{^}}, sifive-p550
// TUNE-RISCV64-SAME: {{^}}, sifive-p670
// TUNE-RISCV64-SAME: {{^}}, sifive-s21
// TUNE-RISCV64-SAME: {{^}}, sifive-s51
// TUNE-RISCV64-SAME: {{^}}, sifive-s54
// TUNE-RISCV64-SAME: {{^}}, sifive-s76
// TUNE-RISCV64-SAME: {{^}}, sifive-u54
// TUNE-RISCV64-SAME: {{^}}, sifive-u74
// TUNE-RISCV64-SAME: {{^}}, sifive-x280
// TUNE-RISCV64-SAME: {{^}}, spacemit-x60
// TUNE-RISCV64-SAME: {{^}}, syntacore-scr3-rv64
// TUNE-RISCV64-SAME: {{^}}, syntacore-scr4-rv64
// TUNE-RISCV64-SAME: {{^}}, syntacore-scr5-rv64
// TUNE-RISCV64-SAME: {{^}}, syntacore-scr7
// TUNE-RISCV64-SAME: {{^}}, tt-ascalon-d8
// TUNE-RISCV64-SAME: {{^}}, veyron-v1
// TUNE-RISCV64-SAME: {{^}}, xiangshan-nanhu
// TUNE-RISCV64-SAME: {{^}}, generic
// TUNE-RISCV64-SAME: {{^}}, rocket
// TUNE-RISCV64-SAME: {{^}}, sifive-7-series
// TUNE-RISCV64-SAME: {{$}}
