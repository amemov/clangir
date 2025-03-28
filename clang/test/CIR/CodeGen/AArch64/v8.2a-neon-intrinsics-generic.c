// RUN: %clang_cc1 -triple arm64-none-linux-gnu -target-feature +fullfp16 -target-feature +v8.2a \
// RUN:    -fclangir -disable-O0-optnone \
// RUN:  -flax-vector-conversions=none -emit-cir -o %t.cir %s
// RUN: FileCheck --check-prefix=CIR --input-file=%t.cir %s

// RUN: %clang_cc1 -triple arm64-none-linux-gnu -target-feature +fullfp16 -target-feature +v8.2a \
// RUN:    -fclangir -disable-O0-optnone \
// RUN:  -flax-vector-conversions=none -emit-llvm -fno-clangir-call-conv-lowering -o - %s \
// RUN: | opt -S -passes=mem2reg,simplifycfg -o %t.ll
// RUN: FileCheck --check-prefix=LLVM --input-file=%t.ll %s

// REQUIRES: aarch64-registered-target || arm-registered-target

// This test mimics clang/test/CodeGen/AArch64/v8.2a-neon-intrinsics.c, which eventually
// CIR shall be able to support fully. Since this is going to take some time to converge,
// the unsupported/NYI code is commented out, so that we can incrementally improve this.
// The NYI filecheck used contains the LLVM output from OG codegen that should guide the
// correct result when implementing this into the CIR pipeline.

#include <arm_neon.h>

// CHECK-LABEL: define {{[^@]+}}@test_vbsl_f16
// CHECK-SAME: (<4 x i16> noundef [[A:%.*]], <4 x half> noundef [[B:%.*]], <4 x half> noundef [[C:%.*]]) #[[ATTR0:[0-9]+]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <4 x i16> [[A]] to <8 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <4 x half> [[B]] to <8 x i8>
// CHECK-NEXT:    [[TMP2:%.*]] = bitcast <4 x half> [[C]] to <8 x i8>
// CHECK-NEXT:    [[VBSL1_I:%.*]] = bitcast <8 x i8> [[TMP1]] to <4 x i16>
// CHECK-NEXT:    [[VBSL2_I:%.*]] = bitcast <8 x i8> [[TMP2]] to <4 x i16>
// CHECK-NEXT:    [[VBSL3_I:%.*]] = and <4 x i16> [[A]], [[VBSL1_I]]
// CHECK-NEXT:    [[TMP3:%.*]] = xor <4 x i16> [[A]], splat (i16 -1)
// CHECK-NEXT:    [[VBSL4_I:%.*]] = and <4 x i16> [[TMP3]], [[VBSL2_I]]
// CHECK-NEXT:    [[VBSL5_I:%.*]] = or <4 x i16> [[VBSL3_I]], [[VBSL4_I]]
// CHECK-NEXT:    [[TMP4:%.*]] = bitcast <4 x i16> [[VBSL5_I]] to <4 x half>
// CHECK-NEXT:    ret <4 x half> [[TMP4]]
//
// float16x4_t test_vbsl_f16(uint16x4_t a, float16x4_t b, float16x4_t c) {
//   return vbsl_f16(a, b, c);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vbslq_f16
// CHECK-SAME: (<8 x i16> noundef [[A:%.*]], <8 x half> noundef [[B:%.*]], <8 x half> noundef [[C:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <8 x i16> [[A]] to <16 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <8 x half> [[B]] to <16 x i8>
// CHECK-NEXT:    [[TMP2:%.*]] = bitcast <8 x half> [[C]] to <16 x i8>
// CHECK-NEXT:    [[VBSL1_I:%.*]] = bitcast <16 x i8> [[TMP1]] to <8 x i16>
// CHECK-NEXT:    [[VBSL2_I:%.*]] = bitcast <16 x i8> [[TMP2]] to <8 x i16>
// CHECK-NEXT:    [[VBSL3_I:%.*]] = and <8 x i16> [[A]], [[VBSL1_I]]
// CHECK-NEXT:    [[TMP3:%.*]] = xor <8 x i16> [[A]], splat (i16 -1)
// CHECK-NEXT:    [[VBSL4_I:%.*]] = and <8 x i16> [[TMP3]], [[VBSL2_I]]
// CHECK-NEXT:    [[VBSL5_I:%.*]] = or <8 x i16> [[VBSL3_I]], [[VBSL4_I]]
// CHECK-NEXT:    [[TMP4:%.*]] = bitcast <8 x i16> [[VBSL5_I]] to <8 x half>
// CHECK-NEXT:    ret <8 x half> [[TMP4]]
//
// float16x8_t test_vbslq_f16(uint16x8_t a, float16x8_t b, float16x8_t c) {
//   return vbslq_f16(a, b, c);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vzip_f16
// CHECK-SAME: (<4 x half> noundef [[A:%.*]], <4 x half> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[RETVAL_I:%.*]] = alloca [[STRUCT_FLOAT16X4X2_T:%.*]], align 8
// CHECK-NEXT:    [[RETVAL:%.*]] = alloca [[STRUCT_FLOAT16X4X2_T]], align 8
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <4 x half> [[A]] to <8 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <4 x half> [[B]] to <8 x i8>
// CHECK-NEXT:    [[VZIP_I:%.*]] = shufflevector <4 x half> [[A]], <4 x half> [[B]], <4 x i32> <i32 0, i32 4, i32 1, i32 5>
// CHECK-NEXT:    store <4 x half> [[VZIP_I]], ptr [[RETVAL_I]], align 8
// CHECK-NEXT:    [[TMP2:%.*]] = getelementptr inbounds <4 x half>, ptr [[RETVAL_I]], i32 1
// CHECK-NEXT:    [[VZIP1_I:%.*]] = shufflevector <4 x half> [[A]], <4 x half> [[B]], <4 x i32> <i32 2, i32 6, i32 3, i32 7>
// CHECK-NEXT:    store <4 x half> [[VZIP1_I]], ptr [[TMP2]], align 8
// CHECK-NEXT:    [[TMP3:%.*]] = load [[STRUCT_FLOAT16X4X2_T]], ptr [[RETVAL_I]], align 8
// CHECK-NEXT:    [[TMP4:%.*]] = getelementptr inbounds nuw [[STRUCT_FLOAT16X4X2_T]], ptr [[RETVAL]], i32 0, i32 0
// CHECK-NEXT:    [[TMP5:%.*]] = extractvalue [[STRUCT_FLOAT16X4X2_T]] [[TMP3]], 0
// CHECK-NEXT:    store [2 x <4 x half>] [[TMP5]], ptr [[TMP4]], align 8
// CHECK-NEXT:    [[TMP6:%.*]] = load [[STRUCT_FLOAT16X4X2_T]], ptr [[RETVAL]], align 8
// CHECK-NEXT:    ret [[STRUCT_FLOAT16X4X2_T]] [[TMP6]]
//
// float16x4x2_t test_vzip_f16(float16x4_t a, float16x4_t b) {
//   return vzip_f16(a, b);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vzipq_f16
// CHECK-SAME: (<8 x half> noundef [[A:%.*]], <8 x half> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[RETVAL_I:%.*]] = alloca [[STRUCT_FLOAT16X8X2_T:%.*]], align 16
// CHECK-NEXT:    [[RETVAL:%.*]] = alloca [[STRUCT_FLOAT16X8X2_T]], align 16
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <8 x half> [[A]] to <16 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <8 x half> [[B]] to <16 x i8>
// CHECK-NEXT:    [[VZIP_I:%.*]] = shufflevector <8 x half> [[A]], <8 x half> [[B]], <8 x i32> <i32 0, i32 8, i32 1, i32 9, i32 2, i32 10, i32 3, i32 11>
// CHECK-NEXT:    store <8 x half> [[VZIP_I]], ptr [[RETVAL_I]], align 16
// CHECK-NEXT:    [[TMP2:%.*]] = getelementptr inbounds <8 x half>, ptr [[RETVAL_I]], i32 1
// CHECK-NEXT:    [[VZIP1_I:%.*]] = shufflevector <8 x half> [[A]], <8 x half> [[B]], <8 x i32> <i32 4, i32 12, i32 5, i32 13, i32 6, i32 14, i32 7, i32 15>
// CHECK-NEXT:    store <8 x half> [[VZIP1_I]], ptr [[TMP2]], align 16
// CHECK-NEXT:    [[TMP3:%.*]] = load [[STRUCT_FLOAT16X8X2_T]], ptr [[RETVAL_I]], align 16
// CHECK-NEXT:    [[TMP4:%.*]] = getelementptr inbounds nuw [[STRUCT_FLOAT16X8X2_T]], ptr [[RETVAL]], i32 0, i32 0
// CHECK-NEXT:    [[TMP5:%.*]] = extractvalue [[STRUCT_FLOAT16X8X2_T]] [[TMP3]], 0
// CHECK-NEXT:    store [2 x <8 x half>] [[TMP5]], ptr [[TMP4]], align 16
// CHECK-NEXT:    [[TMP6:%.*]] = load [[STRUCT_FLOAT16X8X2_T]], ptr [[RETVAL]], align 16
// CHECK-NEXT:    ret [[STRUCT_FLOAT16X8X2_T]] [[TMP6]]
//
// float16x8x2_t test_vzipq_f16(float16x8_t a, float16x8_t b) {
//   return vzipq_f16(a, b);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vuzp_f16
// CHECK-SAME: (<4 x half> noundef [[A:%.*]], <4 x half> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[RETVAL_I:%.*]] = alloca [[STRUCT_FLOAT16X4X2_T:%.*]], align 8
// CHECK-NEXT:    [[RETVAL:%.*]] = alloca [[STRUCT_FLOAT16X4X2_T]], align 8
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <4 x half> [[A]] to <8 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <4 x half> [[B]] to <8 x i8>
// CHECK-NEXT:    [[VUZP_I:%.*]] = shufflevector <4 x half> [[A]], <4 x half> [[B]], <4 x i32> <i32 0, i32 2, i32 4, i32 6>
// CHECK-NEXT:    store <4 x half> [[VUZP_I]], ptr [[RETVAL_I]], align 8
// CHECK-NEXT:    [[TMP2:%.*]] = getelementptr inbounds <4 x half>, ptr [[RETVAL_I]], i32 1
// CHECK-NEXT:    [[VUZP1_I:%.*]] = shufflevector <4 x half> [[A]], <4 x half> [[B]], <4 x i32> <i32 1, i32 3, i32 5, i32 7>
// CHECK-NEXT:    store <4 x half> [[VUZP1_I]], ptr [[TMP2]], align 8
// CHECK-NEXT:    [[TMP3:%.*]] = load [[STRUCT_FLOAT16X4X2_T]], ptr [[RETVAL_I]], align 8
// CHECK-NEXT:    [[TMP4:%.*]] = getelementptr inbounds nuw [[STRUCT_FLOAT16X4X2_T]], ptr [[RETVAL]], i32 0, i32 0
// CHECK-NEXT:    [[TMP5:%.*]] = extractvalue [[STRUCT_FLOAT16X4X2_T]] [[TMP3]], 0
// CHECK-NEXT:    store [2 x <4 x half>] [[TMP5]], ptr [[TMP4]], align 8
// CHECK-NEXT:    [[TMP6:%.*]] = load [[STRUCT_FLOAT16X4X2_T]], ptr [[RETVAL]], align 8
// CHECK-NEXT:    ret [[STRUCT_FLOAT16X4X2_T]] [[TMP6]]
//
// float16x4x2_t test_vuzp_f16(float16x4_t a, float16x4_t b) {
//   return vuzp_f16(a, b);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vuzpq_f16
// CHECK-SAME: (<8 x half> noundef [[A:%.*]], <8 x half> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[RETVAL_I:%.*]] = alloca [[STRUCT_FLOAT16X8X2_T:%.*]], align 16
// CHECK-NEXT:    [[RETVAL:%.*]] = alloca [[STRUCT_FLOAT16X8X2_T]], align 16
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <8 x half> [[A]] to <16 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <8 x half> [[B]] to <16 x i8>
// CHECK-NEXT:    [[VUZP_I:%.*]] = shufflevector <8 x half> [[A]], <8 x half> [[B]], <8 x i32> <i32 0, i32 2, i32 4, i32 6, i32 8, i32 10, i32 12, i32 14>
// CHECK-NEXT:    store <8 x half> [[VUZP_I]], ptr [[RETVAL_I]], align 16
// CHECK-NEXT:    [[TMP2:%.*]] = getelementptr inbounds <8 x half>, ptr [[RETVAL_I]], i32 1
// CHECK-NEXT:    [[VUZP1_I:%.*]] = shufflevector <8 x half> [[A]], <8 x half> [[B]], <8 x i32> <i32 1, i32 3, i32 5, i32 7, i32 9, i32 11, i32 13, i32 15>
// CHECK-NEXT:    store <8 x half> [[VUZP1_I]], ptr [[TMP2]], align 16
// CHECK-NEXT:    [[TMP3:%.*]] = load [[STRUCT_FLOAT16X8X2_T]], ptr [[RETVAL_I]], align 16
// CHECK-NEXT:    [[TMP4:%.*]] = getelementptr inbounds nuw [[STRUCT_FLOAT16X8X2_T]], ptr [[RETVAL]], i32 0, i32 0
// CHECK-NEXT:    [[TMP5:%.*]] = extractvalue [[STRUCT_FLOAT16X8X2_T]] [[TMP3]], 0
// CHECK-NEXT:    store [2 x <8 x half>] [[TMP5]], ptr [[TMP4]], align 16
// CHECK-NEXT:    [[TMP6:%.*]] = load [[STRUCT_FLOAT16X8X2_T]], ptr [[RETVAL]], align 16
// CHECK-NEXT:    ret [[STRUCT_FLOAT16X8X2_T]] [[TMP6]]
//
// float16x8x2_t test_vuzpq_f16(float16x8_t a, float16x8_t b) {
//   return vuzpq_f16(a, b);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vtrn_f16
// CHECK-SAME: (<4 x half> noundef [[A:%.*]], <4 x half> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[RETVAL_I:%.*]] = alloca [[STRUCT_FLOAT16X4X2_T:%.*]], align 8
// CHECK-NEXT:    [[RETVAL:%.*]] = alloca [[STRUCT_FLOAT16X4X2_T]], align 8
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <4 x half> [[A]] to <8 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <4 x half> [[B]] to <8 x i8>
// CHECK-NEXT:    [[VTRN_I:%.*]] = shufflevector <4 x half> [[A]], <4 x half> [[B]], <4 x i32> <i32 0, i32 4, i32 2, i32 6>
// CHECK-NEXT:    store <4 x half> [[VTRN_I]], ptr [[RETVAL_I]], align 8
// CHECK-NEXT:    [[TMP2:%.*]] = getelementptr inbounds <4 x half>, ptr [[RETVAL_I]], i32 1
// CHECK-NEXT:    [[VTRN1_I:%.*]] = shufflevector <4 x half> [[A]], <4 x half> [[B]], <4 x i32> <i32 1, i32 5, i32 3, i32 7>
// CHECK-NEXT:    store <4 x half> [[VTRN1_I]], ptr [[TMP2]], align 8
// CHECK-NEXT:    [[TMP3:%.*]] = load [[STRUCT_FLOAT16X4X2_T]], ptr [[RETVAL_I]], align 8
// CHECK-NEXT:    [[TMP4:%.*]] = getelementptr inbounds nuw [[STRUCT_FLOAT16X4X2_T]], ptr [[RETVAL]], i32 0, i32 0
// CHECK-NEXT:    [[TMP5:%.*]] = extractvalue [[STRUCT_FLOAT16X4X2_T]] [[TMP3]], 0
// CHECK-NEXT:    store [2 x <4 x half>] [[TMP5]], ptr [[TMP4]], align 8
// CHECK-NEXT:    [[TMP6:%.*]] = load [[STRUCT_FLOAT16X4X2_T]], ptr [[RETVAL]], align 8
// CHECK-NEXT:    ret [[STRUCT_FLOAT16X4X2_T]] [[TMP6]]
//
// float16x4x2_t test_vtrn_f16(float16x4_t a, float16x4_t b) {
//   return vtrn_f16(a, b);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vtrnq_f16
// CHECK-SAME: (<8 x half> noundef [[A:%.*]], <8 x half> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[RETVAL_I:%.*]] = alloca [[STRUCT_FLOAT16X8X2_T:%.*]], align 16
// CHECK-NEXT:    [[RETVAL:%.*]] = alloca [[STRUCT_FLOAT16X8X2_T]], align 16
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <8 x half> [[A]] to <16 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <8 x half> [[B]] to <16 x i8>
// CHECK-NEXT:    [[VTRN_I:%.*]] = shufflevector <8 x half> [[A]], <8 x half> [[B]], <8 x i32> <i32 0, i32 8, i32 2, i32 10, i32 4, i32 12, i32 6, i32 14>
// CHECK-NEXT:    store <8 x half> [[VTRN_I]], ptr [[RETVAL_I]], align 16
// CHECK-NEXT:    [[TMP2:%.*]] = getelementptr inbounds <8 x half>, ptr [[RETVAL_I]], i32 1
// CHECK-NEXT:    [[VTRN1_I:%.*]] = shufflevector <8 x half> [[A]], <8 x half> [[B]], <8 x i32> <i32 1, i32 9, i32 3, i32 11, i32 5, i32 13, i32 7, i32 15>
// CHECK-NEXT:    store <8 x half> [[VTRN1_I]], ptr [[TMP2]], align 16
// CHECK-NEXT:    [[TMP3:%.*]] = load [[STRUCT_FLOAT16X8X2_T]], ptr [[RETVAL_I]], align 16
// CHECK-NEXT:    [[TMP4:%.*]] = getelementptr inbounds nuw [[STRUCT_FLOAT16X8X2_T]], ptr [[RETVAL]], i32 0, i32 0
// CHECK-NEXT:    [[TMP5:%.*]] = extractvalue [[STRUCT_FLOAT16X8X2_T]] [[TMP3]], 0
// CHECK-NEXT:    store [2 x <8 x half>] [[TMP5]], ptr [[TMP4]], align 16
// CHECK-NEXT:    [[TMP6:%.*]] = load [[STRUCT_FLOAT16X8X2_T]], ptr [[RETVAL]], align 16
// CHECK-NEXT:    ret [[STRUCT_FLOAT16X8X2_T]] [[TMP6]]
//
// float16x8x2_t test_vtrnq_f16(float16x8_t a, float16x8_t b) {
//   return vtrnq_f16(a, b);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vmov_n_f16
// CHECK-SAME: (half noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[VECINIT:%.*]] = insertelement <4 x half> poison, half [[A]], i32 0
// CHECK-NEXT:    [[VECINIT1:%.*]] = insertelement <4 x half> [[VECINIT]], half [[A]], i32 1
// CHECK-NEXT:    [[VECINIT2:%.*]] = insertelement <4 x half> [[VECINIT1]], half [[A]], i32 2
// CHECK-NEXT:    [[VECINIT3:%.*]] = insertelement <4 x half> [[VECINIT2]], half [[A]], i32 3
// CHECK-NEXT:    ret <4 x half> [[VECINIT3]]
//
// float16x4_t test_vmov_n_f16(float16_t a) {
//   return vmov_n_f16(a);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vmovq_n_f16
// CHECK-SAME: (half noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[VECINIT:%.*]] = insertelement <8 x half> poison, half [[A]], i32 0
// CHECK-NEXT:    [[VECINIT1:%.*]] = insertelement <8 x half> [[VECINIT]], half [[A]], i32 1
// CHECK-NEXT:    [[VECINIT2:%.*]] = insertelement <8 x half> [[VECINIT1]], half [[A]], i32 2
// CHECK-NEXT:    [[VECINIT3:%.*]] = insertelement <8 x half> [[VECINIT2]], half [[A]], i32 3
// CHECK-NEXT:    [[VECINIT4:%.*]] = insertelement <8 x half> [[VECINIT3]], half [[A]], i32 4
// CHECK-NEXT:    [[VECINIT5:%.*]] = insertelement <8 x half> [[VECINIT4]], half [[A]], i32 5
// CHECK-NEXT:    [[VECINIT6:%.*]] = insertelement <8 x half> [[VECINIT5]], half [[A]], i32 6
// CHECK-NEXT:    [[VECINIT7:%.*]] = insertelement <8 x half> [[VECINIT6]], half [[A]], i32 7
// CHECK-NEXT:    ret <8 x half> [[VECINIT7]]
//
// float16x8_t test_vmovq_n_f16(float16_t a) {
//   return vmovq_n_f16(a);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vdup_n_f16
// CHECK-SAME: (half noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[VECINIT:%.*]] = insertelement <4 x half> poison, half [[A]], i32 0
// CHECK-NEXT:    [[VECINIT1:%.*]] = insertelement <4 x half> [[VECINIT]], half [[A]], i32 1
// CHECK-NEXT:    [[VECINIT2:%.*]] = insertelement <4 x half> [[VECINIT1]], half [[A]], i32 2
// CHECK-NEXT:    [[VECINIT3:%.*]] = insertelement <4 x half> [[VECINIT2]], half [[A]], i32 3
// CHECK-NEXT:    ret <4 x half> [[VECINIT3]]
//
// float16x4_t test_vdup_n_f16(float16_t a) {
//   return vdup_n_f16(a);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vdupq_n_f16
// CHECK-SAME: (half noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[VECINIT:%.*]] = insertelement <8 x half> poison, half [[A]], i32 0
// CHECK-NEXT:    [[VECINIT1:%.*]] = insertelement <8 x half> [[VECINIT]], half [[A]], i32 1
// CHECK-NEXT:    [[VECINIT2:%.*]] = insertelement <8 x half> [[VECINIT1]], half [[A]], i32 2
// CHECK-NEXT:    [[VECINIT3:%.*]] = insertelement <8 x half> [[VECINIT2]], half [[A]], i32 3
// CHECK-NEXT:    [[VECINIT4:%.*]] = insertelement <8 x half> [[VECINIT3]], half [[A]], i32 4
// CHECK-NEXT:    [[VECINIT5:%.*]] = insertelement <8 x half> [[VECINIT4]], half [[A]], i32 5
// CHECK-NEXT:    [[VECINIT6:%.*]] = insertelement <8 x half> [[VECINIT5]], half [[A]], i32 6
// CHECK-NEXT:    [[VECINIT7:%.*]] = insertelement <8 x half> [[VECINIT6]], half [[A]], i32 7
// CHECK-NEXT:    ret <8 x half> [[VECINIT7]]
//
// float16x8_t test_vdupq_n_f16(float16_t a) {
//   return vdupq_n_f16(a);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vdup_lane_f16
// CHECK-SAME: (<4 x half> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <4 x half> [[A]] to <8 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x half>
// CHECK-NEXT:    [[LANE:%.*]] = shufflevector <4 x half> [[TMP1]], <4 x half> [[TMP1]], <4 x i32> <i32 3, i32 3, i32 3, i32 3>
// CHECK-NEXT:    ret <4 x half> [[LANE]]
//
// float16x4_t test_vdup_lane_f16(float16x4_t a) {
//   return vdup_lane_f16(a, 3);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vdupq_lane_f16
// CHECK-SAME: (<4 x half> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <4 x half> [[A]] to <8 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x half>
// CHECK-NEXT:    [[LANE:%.*]] = shufflevector <4 x half> [[TMP1]], <4 x half> [[TMP1]], <8 x i32> <i32 3, i32 3, i32 3, i32 3, i32 3, i32 3, i32 3, i32 3>
// CHECK-NEXT:    ret <8 x half> [[LANE]]
//
// float16x8_t test_vdupq_lane_f16(float16x4_t a) {
//   return vdupq_lane_f16(a, 3);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vdup_laneq_f16
// CHECK-SAME: (<8 x half> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <8 x half> [[A]] to <16 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x half>
// CHECK-NEXT:    [[LANE:%.*]] = shufflevector <8 x half> [[TMP1]], <8 x half> [[TMP1]], <4 x i32> <i32 1, i32 1, i32 1, i32 1>
// CHECK-NEXT:    ret <4 x half> [[LANE]]
//
// float16x4_t test_vdup_laneq_f16(float16x8_t a) {
//   return vdup_laneq_f16(a, 1);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vdupq_laneq_f16
// CHECK-SAME: (<8 x half> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <8 x half> [[A]] to <16 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x half>
// CHECK-NEXT:    [[LANE:%.*]] = shufflevector <8 x half> [[TMP1]], <8 x half> [[TMP1]], <8 x i32> <i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7>
// CHECK-NEXT:    ret <8 x half> [[LANE]]
//
// float16x8_t test_vdupq_laneq_f16(float16x8_t a) {
//   return vdupq_laneq_f16(a, 7);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vext_f16
// CHECK-SAME: (<4 x half> noundef [[A:%.*]], <4 x half> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <4 x half> [[A]] to <8 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <4 x half> [[B]] to <8 x i8>
// CHECK-NEXT:    [[TMP2:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x half>
// CHECK-NEXT:    [[TMP3:%.*]] = bitcast <8 x i8> [[TMP1]] to <4 x half>
// CHECK-NEXT:    [[VEXT:%.*]] = shufflevector <4 x half> [[TMP2]], <4 x half> [[TMP3]], <4 x i32> <i32 2, i32 3, i32 4, i32 5>
// CHECK-NEXT:    ret <4 x half> [[VEXT]]
//
// float16x4_t test_vext_f16(float16x4_t a, float16x4_t b) {
//   return vext_f16(a, b, 2);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vextq_f16
// CHECK-SAME: (<8 x half> noundef [[A:%.*]], <8 x half> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <8 x half> [[A]] to <16 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <8 x half> [[B]] to <16 x i8>
// CHECK-NEXT:    [[TMP2:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x half>
// CHECK-NEXT:    [[TMP3:%.*]] = bitcast <16 x i8> [[TMP1]] to <8 x half>
// CHECK-NEXT:    [[VEXT:%.*]] = shufflevector <8 x half> [[TMP2]], <8 x half> [[TMP3]], <8 x i32> <i32 5, i32 6, i32 7, i32 8, i32 9, i32 10, i32 11, i32 12>
// CHECK-NEXT:    ret <8 x half> [[VEXT]]
//
// float16x8_t test_vextq_f16(float16x8_t a, float16x8_t b) {
//   return vextq_f16(a, b, 5);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vrev64_f16
// CHECK-SAME: (<4 x half> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[SHUFFLE_I:%.*]] = shufflevector <4 x half> [[A]], <4 x half> [[A]], <4 x i32> <i32 3, i32 2, i32 1, i32 0>
// CHECK-NEXT:    ret <4 x half> [[SHUFFLE_I]]
//
// float16x4_t test_vrev64_f16(float16x4_t a) {
//   return vrev64_f16(a);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vrev64q_f16
// CHECK-SAME: (<8 x half> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[SHUFFLE_I:%.*]] = shufflevector <8 x half> [[A]], <8 x half> [[A]], <8 x i32> <i32 3, i32 2, i32 1, i32 0, i32 7, i32 6, i32 5, i32 4>
// CHECK-NEXT:    ret <8 x half> [[SHUFFLE_I]]
//
// float16x8_t test_vrev64q_f16(float16x8_t a) {
//   return vrev64q_f16(a);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vzip1_f16
// CHECK-SAME: (<4 x half> noundef [[A:%.*]], <4 x half> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[SHUFFLE_I:%.*]] = shufflevector <4 x half> [[A]], <4 x half> [[B]], <4 x i32> <i32 0, i32 4, i32 1, i32 5>
// CHECK-NEXT:    ret <4 x half> [[SHUFFLE_I]]
//
// float16x4_t test_vzip1_f16(float16x4_t a, float16x4_t b) {
//   return vzip1_f16(a, b);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vzip1q_f16
// CHECK-SAME: (<8 x half> noundef [[A:%.*]], <8 x half> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[SHUFFLE_I:%.*]] = shufflevector <8 x half> [[A]], <8 x half> [[B]], <8 x i32> <i32 0, i32 8, i32 1, i32 9, i32 2, i32 10, i32 3, i32 11>
// CHECK-NEXT:    ret <8 x half> [[SHUFFLE_I]]
//
// float16x8_t test_vzip1q_f16(float16x8_t a, float16x8_t b) {
//   return vzip1q_f16(a, b);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vzip2_f16
// CHECK-SAME: (<4 x half> noundef [[A:%.*]], <4 x half> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[SHUFFLE_I:%.*]] = shufflevector <4 x half> [[A]], <4 x half> [[B]], <4 x i32> <i32 2, i32 6, i32 3, i32 7>
// CHECK-NEXT:    ret <4 x half> [[SHUFFLE_I]]
//
// float16x4_t test_vzip2_f16(float16x4_t a, float16x4_t b) {
//   return vzip2_f16(a, b);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vzip2q_f16
// CHECK-SAME: (<8 x half> noundef [[A:%.*]], <8 x half> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[SHUFFLE_I:%.*]] = shufflevector <8 x half> [[A]], <8 x half> [[B]], <8 x i32> <i32 4, i32 12, i32 5, i32 13, i32 6, i32 14, i32 7, i32 15>
// CHECK-NEXT:    ret <8 x half> [[SHUFFLE_I]]
//
// float16x8_t test_vzip2q_f16(float16x8_t a, float16x8_t b) {
//   return vzip2q_f16(a, b);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vuzp1_f16
// CHECK-SAME: (<4 x half> noundef [[A:%.*]], <4 x half> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[SHUFFLE_I:%.*]] = shufflevector <4 x half> [[A]], <4 x half> [[B]], <4 x i32> <i32 0, i32 2, i32 4, i32 6>
// CHECK-NEXT:    ret <4 x half> [[SHUFFLE_I]]
//
// float16x4_t test_vuzp1_f16(float16x4_t a, float16x4_t b) {
//   return vuzp1_f16(a, b);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vuzp1q_f16
// CHECK-SAME: (<8 x half> noundef [[A:%.*]], <8 x half> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[SHUFFLE_I:%.*]] = shufflevector <8 x half> [[A]], <8 x half> [[B]], <8 x i32> <i32 0, i32 2, i32 4, i32 6, i32 8, i32 10, i32 12, i32 14>
// CHECK-NEXT:    ret <8 x half> [[SHUFFLE_I]]
//
// float16x8_t test_vuzp1q_f16(float16x8_t a, float16x8_t b) {
//   return vuzp1q_f16(a, b);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vuzp2_f16
// CHECK-SAME: (<4 x half> noundef [[A:%.*]], <4 x half> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[SHUFFLE_I:%.*]] = shufflevector <4 x half> [[A]], <4 x half> [[B]], <4 x i32> <i32 1, i32 3, i32 5, i32 7>
// CHECK-NEXT:    ret <4 x half> [[SHUFFLE_I]]
//
// float16x4_t test_vuzp2_f16(float16x4_t a, float16x4_t b) {
//   return vuzp2_f16(a, b);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vuzp2q_f16
// CHECK-SAME: (<8 x half> noundef [[A:%.*]], <8 x half> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[SHUFFLE_I:%.*]] = shufflevector <8 x half> [[A]], <8 x half> [[B]], <8 x i32> <i32 1, i32 3, i32 5, i32 7, i32 9, i32 11, i32 13, i32 15>
// CHECK-NEXT:    ret <8 x half> [[SHUFFLE_I]]
//
// float16x8_t test_vuzp2q_f16(float16x8_t a, float16x8_t b) {
//   return vuzp2q_f16(a, b);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vtrn1_f16
// CHECK-SAME: (<4 x half> noundef [[A:%.*]], <4 x half> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[SHUFFLE_I:%.*]] = shufflevector <4 x half> [[A]], <4 x half> [[B]], <4 x i32> <i32 0, i32 4, i32 2, i32 6>
// CHECK-NEXT:    ret <4 x half> [[SHUFFLE_I]]
//
// float16x4_t test_vtrn1_f16(float16x4_t a, float16x4_t b) {
//   return vtrn1_f16(a, b);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vtrn1q_f16
// CHECK-SAME: (<8 x half> noundef [[A:%.*]], <8 x half> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[SHUFFLE_I:%.*]] = shufflevector <8 x half> [[A]], <8 x half> [[B]], <8 x i32> <i32 0, i32 8, i32 2, i32 10, i32 4, i32 12, i32 6, i32 14>
// CHECK-NEXT:    ret <8 x half> [[SHUFFLE_I]]
//
// float16x8_t test_vtrn1q_f16(float16x8_t a, float16x8_t b) {
//   return vtrn1q_f16(a, b);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vtrn2_f16
// CHECK-SAME: (<4 x half> noundef [[A:%.*]], <4 x half> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[SHUFFLE_I:%.*]] = shufflevector <4 x half> [[A]], <4 x half> [[B]], <4 x i32> <i32 1, i32 5, i32 3, i32 7>
// CHECK-NEXT:    ret <4 x half> [[SHUFFLE_I]]
//
// float16x4_t test_vtrn2_f16(float16x4_t a, float16x4_t b) {
//   return vtrn2_f16(a, b);
// }

// CHECK-LABEL: define {{[^@]+}}@test_vtrn2q_f16
// CHECK-SAME: (<8 x half> noundef [[A:%.*]], <8 x half> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[SHUFFLE_I:%.*]] = shufflevector <8 x half> [[A]], <8 x half> [[B]], <8 x i32> <i32 1, i32 9, i32 3, i32 11, i32 5, i32 13, i32 7, i32 15>
// CHECK-NEXT:    ret <8 x half> [[SHUFFLE_I]]
//
// float16x8_t test_vtrn2q_f16(float16x8_t a, float16x8_t b) {
//   return vtrn2q_f16(a, b);
// }

float16_t test_vduph_laneq_f16(float16x8_t vec) {
  return vduph_laneq_f16(vec, 7);

  // CIR-LABEL: vduph_laneq_f16
  // CIR: [[TMP0:%.*]] = cir.const #cir.int<7> : !s32i
  // CIR: [[TMP1:%.*]] = cir.vec.extract {{.*}}[{{.*}} : !s32i] : !cir.vector<!cir.f16 x 8>

  // LLVM-LABEL: test_vduph_laneq_f16
  // LLVM-SAME: (<8 x half> [[VEC:%.*]])
  // LLVM: [[VGET_LANE:%.*]] = extractelement <8 x half> [[VEC]], i32 7
  // LLVM: ret half [[VGET_LANE]]
}

float16_t test_vduph_lane_f16(float16x4_t vec) {
  return vduph_lane_f16(vec, 3);

  // CIR-LABEL: vduph_lane_f16
  // CIR: [[TMP0:%.*]] = cir.const #cir.int<3> : !s32i
  // CIR: [[TMP1:%.*]] = cir.vec.extract {{.*}}[{{.*}} : !s32i] : !cir.vector<!cir.f16 x 4>

  // LLVM-LABEL: test_vduph_lane_f16
  // LLVM-SAME: (<4 x half> [[VEC:%.*]])
  // LLVM: [[VGET_LANE:%.*]] = extractelement <4 x half> [[VEC]], i32 3
  // LLVM: ret half [[VGET_LANE]]
}
