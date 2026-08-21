/*
 * File: blinky_types.h
 *
 * Code generated for Simulink model 'blinky'.
 *
 * Model version                  : 1.58
 * Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
 * C/C++ source code generated on : Fri Aug 21 14:09:21 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#ifndef blinky_types_h_
#define blinky_types_h_
#include "rtwtypes.h"
#ifndef struct_tag_x0Pf112fqOjrWwtjJwjSZE
#define struct_tag_x0Pf112fqOjrWwtjJwjSZE

struct tag_x0Pf112fqOjrWwtjJwjSZE
{
  real32_T a;
  real32_T b;
  real32_T c;
  real32_T d;
};

#endif                                 /* struct_tag_x0Pf112fqOjrWwtjJwjSZE */

#ifndef typedef_d_quaternion
#define typedef_d_quaternion

typedef struct tag_x0Pf112fqOjrWwtjJwjSZE d_quaternion;

#endif                                 /* typedef_d_quaternion */

#ifndef struct_tag_rE0p9RRszDGqA6Mm8JUyIH
#define struct_tag_rE0p9RRszDGqA6Mm8JUyIH

struct tag_rE0p9RRszDGqA6Mm8JUyIH
{
  int32_T __dummy;
};

#endif                                 /* struct_tag_rE0p9RRszDGqA6Mm8JUyIH */

#ifndef typedef_c_fusion_internal_frames_NED
#define typedef_c_fusion_internal_frames_NED

typedef struct tag_rE0p9RRszDGqA6Mm8JUyIH c_fusion_internal_frames_NED;

#endif                                /* typedef_c_fusion_internal_frames_NED */

#ifndef struct_tag_aCff97A7IWREVCT4uH4jxB
#define struct_tag_aCff97A7IWREVCT4uH4jxB

struct tag_aCff97A7IWREVCT4uH4jxB
{
  int32_T isInitialized;
  boolean_T TunablePropsChanged;
  real_T AccelerometerNoise;
  real_T GyroscopeNoise;
  real_T GyroscopeDriftNoise;
  real_T LinearAccelerationNoise;
  real_T LinearAccelerationDecayFactor;
  real32_T pQw[81];
  real32_T pQv[9];
  d_quaternion pOrientPost;
  boolean_T pFirstTime;
  c_fusion_internal_frames_NED pRefSys;
  real32_T pSensorPeriod;
  real32_T pKalmanPeriod;
  real32_T pGyroOffset[3];
  real32_T pLinAccelPost[3];
  real32_T pInputPrototype[3];
};

#endif                                 /* struct_tag_aCff97A7IWREVCT4uH4jxB */

#ifndef typedef_fusion_internal_simulink_imufil
#define typedef_fusion_internal_simulink_imufil

typedef struct tag_aCff97A7IWREVCT4uH4jxB fusion_internal_simulink_imufil;

#endif                             /* typedef_fusion_internal_simulink_imufil */

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;

#endif                                 /* blinky_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
