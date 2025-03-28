# NOTE: Assertions have been autogenerated by utils/update_mca_test_checks.py
# RUN: llvm-mca -mtriple=riscv64 -mcpu=sifive-p670 -iterations=1 < %s | FileCheck %s

# These instructions only work with e64

vsetvli zero, zero, e64, m1, tu, mu
vclmul.vv v4, v8, v12
vclmul.vx v4, v8, a0
vclmulh.vv v4, v8, v12
vclmulh.vx v4, v8, a0

vsetvli zero, zero, e64, m2, tu, mu
vclmul.vv v4, v8, v12
vclmul.vx v4, v8, a0
vclmulh.vv v4, v8, v12
vclmulh.vx v4, v8, a0

vsetvli zero, zero, e64, m4, tu, mu
vclmul.vv v4, v8, v12
vclmul.vx v4, v8, a0
vclmulh.vv v4, v8, v12
vclmulh.vx v4, v8, a0

vsetvli zero, zero, e64, m8, tu, mu
vclmul.vv  v8, v12, v24
vclmul.vx  v8, v12, a0
vclmulh.vv v8, v12, v24
vclmulh.vx v8, v12, a0

# CHECK:      Iterations:        1
# CHECK-NEXT: Instructions:      20
# CHECK-NEXT: Total Cycles:      64
# CHECK-NEXT: Total uOps:        20

# CHECK:      Dispatch Width:    4
# CHECK-NEXT: uOps Per Cycle:    0.31
# CHECK-NEXT: IPC:               0.31
# CHECK-NEXT: Block RThroughput: 60.0

# CHECK:      Instruction Info:
# CHECK-NEXT: [1]: #uOps
# CHECK-NEXT: [2]: Latency
# CHECK-NEXT: [3]: RThroughput
# CHECK-NEXT: [4]: MayLoad
# CHECK-NEXT: [5]: MayStore
# CHECK-NEXT: [6]: HasSideEffects (U)

# CHECK:      [1]    [2]    [3]    [4]    [5]    [6]    Instructions:
# CHECK-NEXT:  1      1     1.00                  U     vsetvli	zero, zero, e64, m1, tu, mu
# CHECK-NEXT:  1      2     1.00                        vclmul.vv	v4, v8, v12
# CHECK-NEXT:  1      2     1.00                        vclmul.vx	v4, v8, a0
# CHECK-NEXT:  1      2     1.00                        vclmulh.vv	v4, v8, v12
# CHECK-NEXT:  1      2     1.00                        vclmulh.vx	v4, v8, a0
# CHECK-NEXT:  1      1     1.00                  U     vsetvli	zero, zero, e64, m2, tu, mu
# CHECK-NEXT:  1      2     2.00                        vclmul.vv	v4, v8, v12
# CHECK-NEXT:  1      2     2.00                        vclmul.vx	v4, v8, a0
# CHECK-NEXT:  1      2     2.00                        vclmulh.vv	v4, v8, v12
# CHECK-NEXT:  1      2     2.00                        vclmulh.vx	v4, v8, a0
# CHECK-NEXT:  1      1     1.00                  U     vsetvli	zero, zero, e64, m4, tu, mu
# CHECK-NEXT:  1      4     4.00                        vclmul.vv	v4, v8, v12
# CHECK-NEXT:  1      4     4.00                        vclmul.vx	v4, v8, a0
# CHECK-NEXT:  1      4     4.00                        vclmulh.vv	v4, v8, v12
# CHECK-NEXT:  1      4     4.00                        vclmulh.vx	v4, v8, a0
# CHECK-NEXT:  1      1     1.00                  U     vsetvli	zero, zero, e64, m8, tu, mu
# CHECK-NEXT:  1      8     8.00                        vclmul.vv	v8, v12, v24
# CHECK-NEXT:  1      8     8.00                        vclmul.vx	v8, v12, a0
# CHECK-NEXT:  1      8     8.00                        vclmulh.vv	v8, v12, v24
# CHECK-NEXT:  1      8     8.00                        vclmulh.vx	v8, v12, a0

# CHECK:      Resources:
# CHECK-NEXT: [0]   - SiFiveP600Div
# CHECK-NEXT: [1]   - SiFiveP600FEXQ0
# CHECK-NEXT: [2]   - SiFiveP600FEXQ1
# CHECK-NEXT: [3]   - SiFiveP600FloatDiv
# CHECK-NEXT: [4]   - SiFiveP600IEXQ0
# CHECK-NEXT: [5]   - SiFiveP600IEXQ1
# CHECK-NEXT: [6]   - SiFiveP600IEXQ2
# CHECK-NEXT: [7]   - SiFiveP600IEXQ3
# CHECK-NEXT: [8.0] - SiFiveP600LDST
# CHECK-NEXT: [8.1] - SiFiveP600LDST
# CHECK-NEXT: [9]   - SiFiveP600VDiv
# CHECK-NEXT: [10]  - SiFiveP600VEXQ0
# CHECK-NEXT: [11]  - SiFiveP600VEXQ1
# CHECK-NEXT: [12]  - SiFiveP600VFloatDiv
# CHECK-NEXT: [13]  - SiFiveP600VLD
# CHECK-NEXT: [14]  - SiFiveP600VST

# CHECK:      Resource pressure per iteration:
# CHECK-NEXT: [0]    [1]    [2]    [3]    [4]    [5]    [6]    [7]    [8.0]  [8.1]  [9]    [10]   [11]   [12]   [13]   [14]
# CHECK-NEXT:  -      -      -      -     4.00    -      -      -      -      -      -     60.00   -      -      -      -

# CHECK:      Resource pressure by instruction:
# CHECK-NEXT: [0]    [1]    [2]    [3]    [4]    [5]    [6]    [7]    [8.0]  [8.1]  [9]    [10]   [11]   [12]   [13]   [14]   Instructions:
# CHECK-NEXT:  -      -      -      -     1.00    -      -      -      -      -      -      -      -      -      -      -     vsetvli	zero, zero, e64, m1, tu, mu
# CHECK-NEXT:  -      -      -      -      -      -      -      -      -      -      -     1.00    -      -      -      -     vclmul.vv	v4, v8, v12
# CHECK-NEXT:  -      -      -      -      -      -      -      -      -      -      -     1.00    -      -      -      -     vclmul.vx	v4, v8, a0
# CHECK-NEXT:  -      -      -      -      -      -      -      -      -      -      -     1.00    -      -      -      -     vclmulh.vv	v4, v8, v12
# CHECK-NEXT:  -      -      -      -      -      -      -      -      -      -      -     1.00    -      -      -      -     vclmulh.vx	v4, v8, a0
# CHECK-NEXT:  -      -      -      -     1.00    -      -      -      -      -      -      -      -      -      -      -     vsetvli	zero, zero, e64, m2, tu, mu
# CHECK-NEXT:  -      -      -      -      -      -      -      -      -      -      -     2.00    -      -      -      -     vclmul.vv	v4, v8, v12
# CHECK-NEXT:  -      -      -      -      -      -      -      -      -      -      -     2.00    -      -      -      -     vclmul.vx	v4, v8, a0
# CHECK-NEXT:  -      -      -      -      -      -      -      -      -      -      -     2.00    -      -      -      -     vclmulh.vv	v4, v8, v12
# CHECK-NEXT:  -      -      -      -      -      -      -      -      -      -      -     2.00    -      -      -      -     vclmulh.vx	v4, v8, a0
# CHECK-NEXT:  -      -      -      -     1.00    -      -      -      -      -      -      -      -      -      -      -     vsetvli	zero, zero, e64, m4, tu, mu
# CHECK-NEXT:  -      -      -      -      -      -      -      -      -      -      -     4.00    -      -      -      -     vclmul.vv	v4, v8, v12
# CHECK-NEXT:  -      -      -      -      -      -      -      -      -      -      -     4.00    -      -      -      -     vclmul.vx	v4, v8, a0
# CHECK-NEXT:  -      -      -      -      -      -      -      -      -      -      -     4.00    -      -      -      -     vclmulh.vv	v4, v8, v12
# CHECK-NEXT:  -      -      -      -      -      -      -      -      -      -      -     4.00    -      -      -      -     vclmulh.vx	v4, v8, a0
# CHECK-NEXT:  -      -      -      -     1.00    -      -      -      -      -      -      -      -      -      -      -     vsetvli	zero, zero, e64, m8, tu, mu
# CHECK-NEXT:  -      -      -      -      -      -      -      -      -      -      -     8.00    -      -      -      -     vclmul.vv	v8, v12, v24
# CHECK-NEXT:  -      -      -      -      -      -      -      -      -      -      -     8.00    -      -      -      -     vclmul.vx	v8, v12, a0
# CHECK-NEXT:  -      -      -      -      -      -      -      -      -      -      -     8.00    -      -      -      -     vclmulh.vv	v8, v12, v24
# CHECK-NEXT:  -      -      -      -      -      -      -      -      -      -      -     8.00    -      -      -      -     vclmulh.vx	v8, v12, a0
