/*
 * File: blinky.c
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

#include "blinky.h"
#include "blinky_types.h"
#include "rtwtypes.h"
#include <math.h>
#include <string.h>

/* Block signals and states (default storage) */
DW rtDW;

/* External inputs (root inport signals with default storage) */
ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
ExtY rtY;

/* Real-time model */
static RT_MODEL rtM_;
RT_MODEL *const rtM = &rtM_;

/* Forward declaration for local functions */
static void quaternion_quaternion(const real32_T varargin_1[9], real32_T *obj_a,
  real32_T *obj_b, real32_T *obj_c, real32_T *obj_d);
static void quaternion_quaternion_o(const real32_T varargin_1[3], real32_T
  *obj_a, real32_T *obj_b, real32_T *obj_c, real32_T *obj_d);
static void quaternionBase_rotmat(real32_T q_a, real32_T q_b, real32_T q_c,
  real32_T q_d, real32_T r[9]);
static void imufilter_stepImpl(fusion_internal_simulink_imufil *obj, const
  real32_T accelIn[3], const real32_T gyroIn[3], real32_T orientOut[9], real32_T
  angvel[3]);
static void quaternion_quaternion(const real32_T varargin_1[9], real32_T *obj_a,
  real32_T *obj_b, real32_T *obj_c, real32_T *obj_d)
{
  int32_T b_idx;
  real32_T psquared;
  real32_T psquared_idx_0;
  real32_T t;

  /* Start for MATLABSystem: '<S1>/IMU Filter' */
  t = (varargin_1[0] + varargin_1[4]) + varargin_1[8];
  psquared_idx_0 = (t * 2.0F + 1.0F) - t;
  b_idx = -1;
  psquared = (2.0F * varargin_1[0] + 1.0F) - t;
  if (psquared_idx_0 < psquared) {
    psquared_idx_0 = psquared;
    b_idx = 0;
  }

  psquared = (2.0F * varargin_1[4] + 1.0F) - t;
  if (psquared_idx_0 < psquared) {
    psquared_idx_0 = psquared;
    b_idx = 1;
  }

  psquared = (2.0F * varargin_1[8] + 1.0F) - t;
  if (psquared_idx_0 < psquared) {
    psquared_idx_0 = psquared;
    b_idx = 2;
  }

  switch (b_idx + 2) {
   case 1:
    psquared_idx_0 = sqrtf(psquared_idx_0);
    *obj_a = 0.5F * psquared_idx_0;
    psquared_idx_0 = 0.5F / psquared_idx_0;
    *obj_b = (varargin_1[7] - varargin_1[5]) * psquared_idx_0;
    *obj_c = (varargin_1[2] - varargin_1[6]) * psquared_idx_0;
    *obj_d = (varargin_1[3] - varargin_1[1]) * psquared_idx_0;
    break;

   case 2:
    psquared_idx_0 = sqrtf(psquared_idx_0);
    *obj_b = 0.5F * psquared_idx_0;
    psquared_idx_0 = 0.5F / psquared_idx_0;
    *obj_a = (varargin_1[7] - varargin_1[5]) * psquared_idx_0;
    *obj_c = (varargin_1[1] + varargin_1[3]) * psquared_idx_0;
    *obj_d = (varargin_1[2] + varargin_1[6]) * psquared_idx_0;
    break;

   case 3:
    psquared_idx_0 = sqrtf(psquared_idx_0);
    *obj_c = 0.5F * psquared_idx_0;
    psquared_idx_0 = 0.5F / psquared_idx_0;
    *obj_a = (varargin_1[2] - varargin_1[6]) * psquared_idx_0;
    *obj_b = (varargin_1[1] + varargin_1[3]) * psquared_idx_0;
    *obj_d = (varargin_1[5] + varargin_1[7]) * psquared_idx_0;
    break;

   default:
    psquared_idx_0 = sqrtf(psquared_idx_0);
    *obj_d = 0.5F * psquared_idx_0;
    psquared_idx_0 = 0.5F / psquared_idx_0;
    *obj_a = (varargin_1[3] - varargin_1[1]) * psquared_idx_0;
    *obj_b = (varargin_1[2] + varargin_1[6]) * psquared_idx_0;
    *obj_c = (varargin_1[5] + varargin_1[7]) * psquared_idx_0;
    break;
  }

  /* End of Start for MATLABSystem: '<S1>/IMU Filter' */
  if (*obj_a < 0.0F) {
    *obj_a = -*obj_a;
    *obj_b = -*obj_b;
    *obj_c = -*obj_c;
    *obj_d = -*obj_d;
  }
}

static void quaternion_quaternion_o(const real32_T varargin_1[3], real32_T
  *obj_a, real32_T *obj_b, real32_T *obj_c, real32_T *obj_d)
{
  real32_T st;
  real32_T st_tmp;
  real32_T theta;

  /* Start for MATLABSystem: '<S1>/IMU Filter' */
  *obj_a = 1.0F;
  *obj_b = 0.0F;
  *obj_c = 0.0F;
  *obj_d = 0.0F;
  theta = sqrtf((varargin_1[0] * varargin_1[0] + varargin_1[1] * varargin_1[1])
                + varargin_1[2] * varargin_1[2]);
  st_tmp = theta / 2.0F;
  st = sinf(st_tmp);
  if (theta != 0.0F) {
    /* Start for MATLABSystem: '<S1>/IMU Filter' */
    *obj_a = cosf(st_tmp);
    *obj_b = varargin_1[0] / theta * st;
    *obj_c = varargin_1[1] / theta * st;
    *obj_d = varargin_1[2] / theta * st;
  }
}

static void quaternionBase_rotmat(real32_T q_a, real32_T q_b, real32_T q_c,
  real32_T q_d, real32_T r[9])
{
  real32_T aasq;
  real32_T ac2;
  real32_T ad2;
  real32_T bc2;
  real32_T bd2;
  real32_T cd2;
  real32_T n;
  real32_T z;
  real32_T z_0;
  real32_T z_1;

  /* Start for MATLABSystem: '<S1>/IMU Filter' */
  n = sqrtf(((q_a * q_a + q_b * q_b) + q_c * q_c) + q_d * q_d);
  q_a /= n;
  z = q_b / n;
  z_0 = q_c / n;
  z_1 = q_d / n;
  n = q_a * z * 2.0F;
  ac2 = q_a * z_0 * 2.0F;
  ad2 = q_a * z_1 * 2.0F;
  bc2 = z * z_0 * 2.0F;
  bd2 = z * z_1 * 2.0F;
  cd2 = z_0 * z_1 * 2.0F;
  aasq = q_a * q_a * 2.0F - 1.0F;
  r[0] = z * z * 2.0F + aasq;
  r[3] = bc2 + ad2;
  r[6] = bd2 - ac2;
  r[1] = bc2 - ad2;
  r[4] = z_0 * z_0 * 2.0F + aasq;
  r[7] = cd2 + n;
  r[2] = bd2 + ac2;
  r[5] = cd2 - n;
  r[8] = z_1 * z_1 * 2.0F + aasq;
}

static void imufilter_stepImpl(fusion_internal_simulink_imufil *obj, const
  real32_T accelIn[3], const real32_T gyroIn[3], real32_T orientOut[9], real32_T
  angvel[3])
{
  int32_T H_tmp;
  int32_T H_tmp_0;
  int32_T H_tmp_1;
  real32_T pGyroOffsetIn_idx_2;
  real32_T pLinAccelPostIn_idx_2;
  boolean_T b;
  static const int8_T tmp[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

  /* Start for MATLABSystem: '<S1>/IMU Filter' */
  b = obj->pFirstTime;
  rtDW.pGyroOffsetIn_idx_0 = obj->pGyroOffset[0];
  rtDW.pGyroOffsetIn_idx_1 = obj->pGyroOffset[1];
  pGyroOffsetIn_idx_2 = obj->pGyroOffset[2];
  if (obj->pFirstTime) {
    rtDW.Rpost[6] = accelIn[0];
    rtDW.Rpost[3] = 0.0F;
    rtDW.Rpost[7] = accelIn[1];
    rtDW.Rpost[4] = accelIn[2];
    rtDW.Rpost[8] = accelIn[2];
    rtDW.Rpost[5] = 0.0F - accelIn[1];
    rtDW.Rpost[0] = accelIn[2] * accelIn[2] - (0.0F - accelIn[1]) * accelIn[1];
    rtDW.Rpost[1] = (0.0F - accelIn[1]) * accelIn[0];
    rtDW.Rpost[2] = 0.0F - accelIn[0] * accelIn[2];
    for (rtDW.rtemp = 0; rtDW.rtemp < 9; rtDW.rtemp++) {
      rtDW.Rpost_c = rtDW.Rpost[rtDW.rtemp];
      rtDW.h1[rtDW.rtemp] = rtDW.Rpost_c * rtDW.Rpost_c;
    }

    for (rtDW.rtemp = 0; rtDW.rtemp < 3; rtDW.rtemp++) {
      rtDW.xpageoffset = rtDW.rtemp * 3;
      rtDW.Reast[rtDW.rtemp] = sqrtf((rtDW.h1[rtDW.xpageoffset + 1] +
        rtDW.h1[rtDW.xpageoffset]) + rtDW.h1[rtDW.xpageoffset + 2]);
    }

    for (rtDW.rtemp = 0; rtDW.rtemp < 9; rtDW.rtemp++) {
      rtDW.h1[rtDW.rtemp] = rtDW.Rpost[rtDW.rtemp];
    }

    for (rtDW.rtemp = 0; rtDW.rtemp < 3; rtDW.rtemp++) {
      rtDW.Rpost_c = rtDW.Reast[rtDW.rtemp];
      rtDW.Rpost[3 * rtDW.rtemp] = rtDW.h1[3 * rtDW.rtemp] / rtDW.Rpost_c;
      rtDW.xpageoffset = 3 * rtDW.rtemp + 1;
      rtDW.Rpost[rtDW.xpageoffset] = rtDW.h1[rtDW.xpageoffset] / rtDW.Rpost_c;
      rtDW.xpageoffset = 3 * rtDW.rtemp + 2;
      rtDW.Rpost[rtDW.xpageoffset] = rtDW.h1[rtDW.xpageoffset] / rtDW.Rpost_c;
    }

    b = false;
    quaternion_quaternion(rtDW.Rpost, &obj->pOrientPost.a, &obj->pOrientPost.b,
                          &obj->pOrientPost.c, &obj->pOrientPost.d);
  }

  rtDW.Reast[0] = (gyroIn[0] - obj->pGyroOffset[0]) * obj->pSensorPeriod;
  rtDW.Reast[1] = (gyroIn[1] - obj->pGyroOffset[1]) * obj->pSensorPeriod;
  rtDW.Reast[2] = (gyroIn[2] - obj->pGyroOffset[2]) * obj->pSensorPeriod;
  quaternion_quaternion_o(rtDW.Reast, &rtDW.maxval, &rtDW.a21, &rtDW.deltaq_c,
    &rtDW.deltaq_d);
  rtDW.n = ((obj->pOrientPost.a * rtDW.maxval - obj->pOrientPost.b * rtDW.a21) -
            obj->pOrientPost.c * rtDW.deltaq_c) - obj->pOrientPost.d *
    rtDW.deltaq_d;
  rtDW.e_b = ((obj->pOrientPost.a * rtDW.a21 + obj->pOrientPost.b * rtDW.maxval)
              + obj->pOrientPost.c * rtDW.deltaq_d) - obj->pOrientPost.d *
    rtDW.deltaq_c;
  rtDW.e_c = ((obj->pOrientPost.a * rtDW.deltaq_c - obj->pOrientPost.b *
               rtDW.deltaq_d) + obj->pOrientPost.c * rtDW.maxval) +
    obj->pOrientPost.d * rtDW.a21;
  rtDW.e_d = ((obj->pOrientPost.a * rtDW.deltaq_d + obj->pOrientPost.b *
               rtDW.deltaq_c) - obj->pOrientPost.c * rtDW.a21) +
    obj->pOrientPost.d * rtDW.maxval;
  if (((obj->pOrientPost.a * rtDW.maxval - obj->pOrientPost.b * rtDW.a21) -
       obj->pOrientPost.c * rtDW.deltaq_c) - obj->pOrientPost.d * rtDW.deltaq_d <
      0.0F) {
    rtDW.n = -rtDW.n;
    rtDW.e_b = -rtDW.e_b;
    rtDW.e_c = -rtDW.e_c;
    rtDW.e_d = -rtDW.e_d;
  }

  quaternionBase_rotmat(rtDW.n, rtDW.e_b, rtDW.e_c, rtDW.e_d, rtDW.h1);
  rtDW.Reast[0] = rtDW.h1[6] * 9.81F;
  rtDW.Rpost_c = (real32_T)obj->LinearAccelerationDecayFactor *
    obj->pLinAccelPost[0];
  rtDW.Reast[1] = rtDW.h1[7] * 9.81F;
  rtDW.pLinAccelPostIn_idx_1 = (real32_T)obj->LinearAccelerationDecayFactor *
    obj->pLinAccelPost[1];
  rtDW.Reast[2] = rtDW.h1[8] * 9.81F;
  pLinAccelPostIn_idx_2 = (real32_T)obj->LinearAccelerationDecayFactor *
    obj->pLinAccelPost[2];
  for (rtDW.rtemp = 0; rtDW.rtemp < 9; rtDW.rtemp++) {
    rtDW.h1[rtDW.rtemp] = 0.0F;
  }

  /* Start for MATLABSystem: '<S1>/IMU Filter' */
  rtDW.h1[3] = rtDW.Reast[2];
  rtDW.h1[6] = -rtDW.Reast[1];
  rtDW.h1[7] = rtDW.Reast[0];
  for (rtDW.rtemp = 0; rtDW.rtemp < 3; rtDW.rtemp++) {
    rtDW.Rpost[3 * rtDW.rtemp] = rtDW.h1[3 * rtDW.rtemp];
    rtDW.xpageoffset = 3 * rtDW.rtemp + 1;
    rtDW.Rpost[rtDW.xpageoffset] = rtDW.h1[rtDW.xpageoffset] -
      rtDW.h1[rtDW.rtemp + 3];
    rtDW.xpageoffset = 3 * rtDW.rtemp + 2;
    rtDW.Rpost[rtDW.xpageoffset] = rtDW.h1[rtDW.xpageoffset] -
      rtDW.h1[rtDW.rtemp + 6];
  }

  for (rtDW.rtemp = 0; rtDW.rtemp < 9; rtDW.rtemp++) {
    rtDW.h1_k = rtDW.Rpost[rtDW.rtemp];
    rtDW.H[rtDW.rtemp] = rtDW.h1_k;

    /* Start for MATLABSystem: '<S1>/IMU Filter' */
    rtDW.H[rtDW.rtemp + 9] = -rtDW.h1_k * obj->pKalmanPeriod;
    rtDW.H[rtDW.rtemp + 18] = tmp[rtDW.rtemp];
    rtDW.y_m[3 * rtDW.rtemp] = 0.0F;
    rtDW.y_m[3 * rtDW.rtemp + 1] = 0.0F;
    rtDW.y_m[3 * rtDW.rtemp + 2] = 0.0F;
  }

  for (rtDW.rtemp = 0; rtDW.rtemp < 9; rtDW.rtemp++) {
    rtDW.maxval = rtDW.y_m[3 * rtDW.rtemp];
    rtDW.r1 = 3 * rtDW.rtemp + 1;
    rtDW.a21 = rtDW.y_m[rtDW.r1];
    rtDW.r3 = 3 * rtDW.rtemp + 2;
    rtDW.deltaq_c = rtDW.y_m[rtDW.r3];
    for (rtDW.xpageoffset = 0; rtDW.xpageoffset < 9; rtDW.xpageoffset++) {
      /* Start for MATLABSystem: '<S1>/IMU Filter' */
      rtDW.h1_k = obj->pQw[9 * rtDW.rtemp + rtDW.xpageoffset];
      rtDW.maxval += rtDW.H[3 * rtDW.xpageoffset] * rtDW.h1_k;
      rtDW.a21 += rtDW.H[3 * rtDW.xpageoffset + 1] * rtDW.h1_k;
      rtDW.deltaq_c += rtDW.H[3 * rtDW.xpageoffset + 2] * rtDW.h1_k;
    }

    rtDW.y_m[rtDW.r3] = rtDW.deltaq_c;
    rtDW.y_m[rtDW.r1] = rtDW.a21;
    rtDW.y_m[3 * rtDW.rtemp] = rtDW.maxval;
  }

  /* Start for MATLABSystem: '<S1>/IMU Filter' */
  for (rtDW.rtemp = 0; rtDW.rtemp < 3; rtDW.rtemp++) {
    rtDW.h1_k = 0.0F;
    rtDW.maxval = 0.0F;
    rtDW.a21 = 0.0F;
    for (rtDW.xpageoffset = 0; rtDW.xpageoffset < 9; rtDW.xpageoffset++) {
      rtDW.deltaq_c = rtDW.H[3 * rtDW.xpageoffset + rtDW.rtemp];
      rtDW.y_tmp[rtDW.xpageoffset + 9 * rtDW.rtemp] = rtDW.deltaq_c;
      rtDW.h1_k += rtDW.y_m[3 * rtDW.xpageoffset] * rtDW.deltaq_c;
      rtDW.maxval += rtDW.y_m[3 * rtDW.xpageoffset + 1] * rtDW.deltaq_c;
      rtDW.a21 += rtDW.y_m[3 * rtDW.xpageoffset + 2] * rtDW.deltaq_c;
    }

    rtDW.h1[3 * rtDW.rtemp + 2] = rtDW.a21;
    rtDW.h1[3 * rtDW.rtemp + 1] = rtDW.maxval;
    rtDW.h1[3 * rtDW.rtemp] = rtDW.h1_k;
  }

  for (rtDW.rtemp = 0; rtDW.rtemp < 3; rtDW.rtemp++) {
    /* Start for MATLABSystem: '<S1>/IMU Filter' */
    rtDW.Rpost[3 * rtDW.rtemp] = rtDW.h1[rtDW.rtemp] + obj->pQv[rtDW.rtemp];
    rtDW.Rpost[3 * rtDW.rtemp + 1] = rtDW.h1[rtDW.rtemp + 3] + obj->
      pQv[rtDW.rtemp + 3];
    rtDW.Rpost[3 * rtDW.rtemp + 2] = rtDW.h1[rtDW.rtemp + 6] + obj->
      pQv[rtDW.rtemp + 6];
    for (rtDW.xpageoffset = 0; rtDW.xpageoffset < 9; rtDW.xpageoffset++) {
      rtDW.y_c[rtDW.xpageoffset + 9 * rtDW.rtemp] = 0.0F;
    }

    for (rtDW.xpageoffset = 0; rtDW.xpageoffset < 9; rtDW.xpageoffset++) {
      rtDW.h1_k = rtDW.y_tmp[9 * rtDW.rtemp + rtDW.xpageoffset];
      for (rtDW.r3 = 0; rtDW.r3 < 9; rtDW.r3++) {
        rtDW.r1 = 9 * rtDW.rtemp + rtDW.r3;

        /* Start for MATLABSystem: '<S1>/IMU Filter' */
        rtDW.y_c[rtDW.r1] += obj->pQw[9 * rtDW.xpageoffset + rtDW.r3] *
          rtDW.h1_k;
      }
    }
  }

  /* Start for MATLABSystem: '<S1>/IMU Filter' */
  rtDW.r1 = 0;
  rtDW.xpageoffset = 1;
  rtDW.r3 = 2;
  rtDW.maxval = fabsf(rtDW.Rpost[0]);
  rtDW.a21 = fabsf(rtDW.Rpost[1]);
  if (rtDW.a21 > rtDW.maxval) {
    rtDW.maxval = rtDW.a21;

    /* Start for MATLABSystem: '<S1>/IMU Filter' */
    rtDW.r1 = 1;
    rtDW.xpageoffset = 0;
  }

  /* Start for MATLABSystem: '<S1>/IMU Filter' */
  if (fabsf(rtDW.Rpost[2]) > rtDW.maxval) {
    rtDW.r1 = 2;
    rtDW.xpageoffset = 1;
    rtDW.r3 = 0;
  }

  rtDW.Rpost[rtDW.xpageoffset] /= rtDW.Rpost[rtDW.r1];
  rtDW.Rpost[rtDW.r3] /= rtDW.Rpost[rtDW.r1];
  rtDW.Rpost[rtDW.xpageoffset + 3] -= rtDW.Rpost[rtDW.r1 + 3] *
    rtDW.Rpost[rtDW.xpageoffset];
  rtDW.Rpost[rtDW.r3 + 3] -= rtDW.Rpost[rtDW.r1 + 3] * rtDW.Rpost[rtDW.r3];
  rtDW.Rpost[rtDW.xpageoffset + 6] -= rtDW.Rpost[rtDW.r1 + 6] *
    rtDW.Rpost[rtDW.xpageoffset];
  rtDW.Rpost[rtDW.r3 + 6] -= rtDW.Rpost[rtDW.r1 + 6] * rtDW.Rpost[rtDW.r3];

  /* Start for MATLABSystem: '<S1>/IMU Filter' */
  if (fabsf(rtDW.Rpost[rtDW.r3 + 3]) > fabsf(rtDW.Rpost[rtDW.xpageoffset + 3]))
  {
    rtDW.rtemp = rtDW.xpageoffset + 1;
    rtDW.xpageoffset = rtDW.r3;
    rtDW.r3 = rtDW.rtemp - 1;
  }

  rtDW.Rpost[rtDW.r3 + 3] /= rtDW.Rpost[rtDW.xpageoffset + 3];
  rtDW.Rpost[rtDW.r3 + 6] -= rtDW.Rpost[rtDW.r3 + 3] *
    rtDW.Rpost[rtDW.xpageoffset + 6];

  /* Start for MATLABSystem: '<S1>/IMU Filter' */
  rtDW.accelIn[0] = (accelIn[0] + rtDW.Rpost_c) - rtDW.Reast[0];
  rtDW.accelIn[1] = (accelIn[1] + rtDW.pLinAccelPostIn_idx_1) - rtDW.Reast[1];
  rtDW.accelIn[2] = (accelIn[2] + pLinAccelPostIn_idx_2) - rtDW.Reast[2];
  for (rtDW.rtemp = 0; rtDW.rtemp < 9; rtDW.rtemp++) {
    /* Start for MATLABSystem: '<S1>/IMU Filter' */
    H_tmp = 9 * rtDW.r1 + rtDW.rtemp;
    rtDW.H[H_tmp] = rtDW.y_c[rtDW.rtemp] / rtDW.Rpost[rtDW.r1];
    H_tmp_0 = 9 * rtDW.xpageoffset + rtDW.rtemp;
    rtDW.H[H_tmp_0] = rtDW.y_c[rtDW.rtemp + 9] - rtDW.Rpost[rtDW.r1 + 3] *
      rtDW.H[H_tmp];
    H_tmp_1 = 9 * rtDW.r3 + rtDW.rtemp;
    rtDW.H[H_tmp_1] = rtDW.y_c[rtDW.rtemp + 18] - rtDW.Rpost[rtDW.r1 + 6] *
      rtDW.H[H_tmp];
    rtDW.H[H_tmp_0] /= rtDW.Rpost[rtDW.xpageoffset + 3];
    rtDW.H[H_tmp_1] -= rtDW.Rpost[rtDW.xpageoffset + 6] * rtDW.H[H_tmp_0];
    rtDW.H[H_tmp_1] /= rtDW.Rpost[rtDW.r3 + 6];
    rtDW.H[H_tmp_0] -= rtDW.Rpost[rtDW.r3 + 3] * rtDW.H[H_tmp_1];
    rtDW.H[H_tmp] -= rtDW.H[H_tmp_1] * rtDW.Rpost[rtDW.r3];
    rtDW.H[H_tmp] -= rtDW.H[H_tmp_0] * rtDW.Rpost[rtDW.xpageoffset];
    rtDW.h1[rtDW.rtemp] = 0.0F;
  }

  for (rtDW.rtemp = 0; rtDW.rtemp < 3; rtDW.rtemp++) {
    rtDW.h1_k = rtDW.accelIn[rtDW.rtemp];
    for (rtDW.xpageoffset = 0; rtDW.xpageoffset < 9; rtDW.xpageoffset++) {
      /* Start for MATLABSystem: '<S1>/IMU Filter' */
      rtDW.h1[rtDW.xpageoffset] += rtDW.H[9 * rtDW.rtemp + rtDW.xpageoffset] *
        rtDW.h1_k;
    }
  }

  /* Start for MATLABSystem: '<S1>/IMU Filter' */
  quaternion_quaternion_o(&rtDW.h1[0], &rtDW.maxval, &rtDW.a21, &rtDW.deltaq_c,
    &rtDW.deltaq_d);
  rtDW.h1_k = ((rtDW.n * rtDW.maxval - rtDW.e_b * -rtDW.a21) - rtDW.e_c *
               -rtDW.deltaq_c) - rtDW.e_d * -rtDW.deltaq_d;
  obj->pOrientPost.a = rtDW.h1_k;
  obj->pOrientPost.b = ((rtDW.n * -rtDW.a21 + rtDW.e_b * rtDW.maxval) + rtDW.e_c
                        * -rtDW.deltaq_d) - rtDW.e_d * -rtDW.deltaq_c;
  obj->pOrientPost.c = ((rtDW.n * -rtDW.deltaq_c - rtDW.e_b * -rtDW.deltaq_d) +
                        rtDW.e_c * rtDW.maxval) + rtDW.e_d * -rtDW.a21;
  obj->pOrientPost.d = ((rtDW.n * -rtDW.deltaq_d + rtDW.e_b * -rtDW.deltaq_c) -
                        rtDW.e_c * -rtDW.a21) + rtDW.e_d * rtDW.maxval;
  if (rtDW.h1_k < 0.0F) {
    obj->pOrientPost.a = -obj->pOrientPost.a;
    obj->pOrientPost.b = -obj->pOrientPost.b;
    obj->pOrientPost.c = -obj->pOrientPost.c;
    obj->pOrientPost.d = -obj->pOrientPost.d;
  }

  /* Start for MATLABSystem: '<S1>/IMU Filter' */
  rtDW.n = sqrtf(((obj->pOrientPost.a * obj->pOrientPost.a + obj->pOrientPost.b *
                   obj->pOrientPost.b) + obj->pOrientPost.c * obj->pOrientPost.c)
                 + obj->pOrientPost.d * obj->pOrientPost.d);
  obj->pOrientPost.a /= rtDW.n;
  obj->pOrientPost.b /= rtDW.n;
  obj->pOrientPost.c /= rtDW.n;
  obj->pOrientPost.d /= rtDW.n;
  for (rtDW.rtemp = 0; rtDW.rtemp < 9; rtDW.rtemp++) {
    /* Start for MATLABSystem: '<S1>/IMU Filter' */
    rtDW.n = rtDW.H[rtDW.rtemp + 9];
    rtDW.e_b = rtDW.H[rtDW.rtemp];
    rtDW.e_c = rtDW.H[rtDW.rtemp + 18];
    for (rtDW.xpageoffset = 0; rtDW.xpageoffset < 9; rtDW.xpageoffset++) {
      /* Start for MATLABSystem: '<S1>/IMU Filter' */
      rtDW.r1 = 9 * rtDW.xpageoffset + rtDW.rtemp;
      rtDW.Ppost[rtDW.r1] = obj->pQw[rtDW.r1] - ((rtDW.y_m[3 * rtDW.xpageoffset
        + 1] * rtDW.n + rtDW.y_m[3 * rtDW.xpageoffset] * rtDW.e_b) + rtDW.y_m[3 *
        rtDW.xpageoffset + 2] * rtDW.e_c);
    }
  }

  memset(&obj->pQw[0], 0, 81U * sizeof(real32_T));

  /* Start for MATLABSystem: '<S1>/IMU Filter' */
  rtDW.n = obj->pKalmanPeriod * obj->pKalmanPeriod;
  rtDW.e_b = (real32_T)(obj->GyroscopeDriftNoise + obj->GyroscopeNoise);
  obj->pQw[0] = (rtDW.Ppost[30] + rtDW.e_b) * rtDW.n + rtDW.Ppost[0];
  obj->pQw[30] = rtDW.Ppost[30] + (real32_T)obj->GyroscopeDriftNoise;
  obj->pQw[10] = (rtDW.Ppost[40] + rtDW.e_b) * rtDW.n + rtDW.Ppost[10];
  obj->pQw[40] = rtDW.Ppost[40] + (real32_T)obj->GyroscopeDriftNoise;
  obj->pQw[20] = (rtDW.Ppost[50] + rtDW.e_b) * rtDW.n + rtDW.Ppost[20];
  obj->pQw[50] = rtDW.Ppost[50] + (real32_T)obj->GyroscopeDriftNoise;
  rtDW.Reast[0] = -obj->pKalmanPeriod * obj->pQw[30];
  rtDW.Reast[1] = -obj->pKalmanPeriod * obj->pQw[40];
  rtDW.Reast[2] = -obj->pKalmanPeriod * obj->pQw[50];
  rtDW.y = obj->LinearAccelerationDecayFactor *
    obj->LinearAccelerationDecayFactor;
  obj->pFirstTime = b;
  quaternionBase_rotmat(obj->pOrientPost.a, obj->pOrientPost.b,
                        obj->pOrientPost.c, obj->pOrientPost.d, orientOut);
  obj->pQw[3] = rtDW.Reast[0];
  obj->pQw[27] = rtDW.Reast[0];

  /* Start for MATLABSystem: '<S1>/IMU Filter' */
  obj->pQw[60] = (real32_T)rtDW.y * rtDW.Ppost[60] + (real32_T)
    obj->LinearAccelerationNoise;
  obj->pGyroOffset[0] -= rtDW.h1[3];
  obj->pLinAccelPost[0] = rtDW.Rpost_c - rtDW.h1[6];
  angvel[0] = gyroIn[0] - rtDW.pGyroOffsetIn_idx_0;
  obj->pQw[13] = rtDW.Reast[1];
  obj->pQw[37] = rtDW.Reast[1];

  /* Start for MATLABSystem: '<S1>/IMU Filter' */
  obj->pQw[70] = (real32_T)rtDW.y * rtDW.Ppost[70] + (real32_T)
    obj->LinearAccelerationNoise;
  obj->pGyroOffset[1] -= rtDW.h1[4];
  obj->pLinAccelPost[1] = rtDW.pLinAccelPostIn_idx_1 - rtDW.h1[7];
  angvel[1] = gyroIn[1] - rtDW.pGyroOffsetIn_idx_1;
  obj->pQw[23] = rtDW.Reast[2];
  obj->pQw[47] = rtDW.Reast[2];

  /* Start for MATLABSystem: '<S1>/IMU Filter' */
  obj->pQw[80] = (real32_T)rtDW.y * rtDW.Ppost[80] + (real32_T)
    obj->LinearAccelerationNoise;
  obj->pGyroOffset[2] -= rtDW.h1[5];
  obj->pLinAccelPost[2] = pLinAccelPostIn_idx_2 - rtDW.h1[8];
  angvel[2] = gyroIn[2] - pGyroOffsetIn_idx_2;
}

/* Model step function for TID0 */
void blinky_step0(void)                /* Sample time: [0.001s, 0.0s] */
{
  /* (no output/update code required) */
}

/* Model step function for TID1 */
void blinky_step1(void)                /* Sample time: [0.005s, 0.0s] */
{
  int16_T rtb_Switch;
  boolean_T rtb_LogicalOperator;

  /* Outputs for Atomic SubSystem: '<Root>/task_5ms' */
  /* SignalConversion generated from: '<S1>/Vector Concatenate1' incorporates:
   *  Inport: '<Root>/ax'
   */
  rtDW.VectorConcatenate1[0] = rtU.ax;

  /* SignalConversion generated from: '<S1>/Vector Concatenate1' incorporates:
   *  Inport: '<Root>/ay'
   */
  rtDW.VectorConcatenate1[1] = rtU.ay;

  /* SignalConversion generated from: '<S1>/Vector Concatenate1' incorporates:
   *  Inport: '<Root>/az'
   */
  rtDW.VectorConcatenate1[2] = rtU.az;

  /* SignalConversion generated from: '<S1>/Vector Concatenate2' incorporates:
   *  Inport: '<Root>/gx'
   */
  rtDW.VectorConcatenate2[0] = rtU.gx;

  /* SignalConversion generated from: '<S1>/Vector Concatenate2' incorporates:
   *  Inport: '<Root>/gy'
   */
  rtDW.VectorConcatenate2[1] = rtU.gy;

  /* SignalConversion generated from: '<S1>/Vector Concatenate2' incorporates:
   *  Inport: '<Root>/gz'
   */
  rtDW.VectorConcatenate2[2] = rtU.gz;

  /* MATLABSystem: '<S1>/IMU Filter' incorporates:
   *  Concatenate: '<S1>/Vector Concatenate1'
   *  Concatenate: '<S1>/Vector Concatenate2'
   */
  if (rtDW.obj.AccelerometerNoise != 0.0001924722) {
    if (rtDW.obj.isInitialized == 1) {
      rtDW.obj.TunablePropsChanged = true;
    }

    rtDW.obj.AccelerometerNoise = 0.0001924722;
  }

  if (rtDW.obj.GyroscopeNoise != 9.1385E-5) {
    if (rtDW.obj.isInitialized == 1) {
      rtDW.obj.TunablePropsChanged = true;
    }

    rtDW.obj.GyroscopeNoise = 9.1385E-5;
  }

  if (rtDW.obj.GyroscopeDriftNoise != 3.0462E-13) {
    if (rtDW.obj.isInitialized == 1) {
      rtDW.obj.TunablePropsChanged = true;
    }

    rtDW.obj.GyroscopeDriftNoise = 3.0462E-13;
  }

  if (rtDW.obj.LinearAccelerationNoise != 0.0096236100000000012) {
    if (rtDW.obj.isInitialized == 1) {
      rtDW.obj.TunablePropsChanged = true;
    }

    rtDW.obj.LinearAccelerationNoise = 0.0096236100000000012;
  }

  if (rtDW.obj.LinearAccelerationDecayFactor != 0.1) {
    if (rtDW.obj.isInitialized == 1) {
      rtDW.obj.TunablePropsChanged = true;
    }

    rtDW.obj.LinearAccelerationDecayFactor = 0.1;
  }

  if (rtDW.obj.TunablePropsChanged) {
    rtDW.obj.TunablePropsChanged = false;
    rtDW.obj.pSensorPeriod = 0.005F;
    rtDW.obj.pKalmanPeriod = 0.005F;
    for (rtDW.i = 0; rtDW.i < 9; rtDW.i++) {
      rtDW.b_I[rtDW.i] = 0.0F;
    }

    rtDW.b_I[0] = 1.0F;
    rtDW.b_I[4] = 1.0F;
    rtDW.b_I[8] = 1.0F;
    for (rtDW.i = 0; rtDW.i < 9; rtDW.i++) {
      rtDW.obj.pQv[rtDW.i] = 0.00981608406F * rtDW.b_I[rtDW.i];
    }
  }

  imufilter_stepImpl(&rtDW.obj, rtDW.VectorConcatenate1, rtDW.VectorConcatenate2,
                     rtDW.b_I, rtDW.fv);

  /* Sum: '<S1>/Sum2' incorporates:
   *  Inport: '<Root>/motor_a_actual_speed'
   *  Inport: '<Root>/speed_a_target'
   */
  rtDW.accelMeasNoiseVar = rtU.speed_a_target - rtU.motor_a_actual_speed;

  /* Logic: '<S1>/Logical Operator' incorporates:
   *  Inport: '<Root>/running_flag'
   */
  rtb_LogicalOperator = (rtU.running_flag == 0);

  /* DiscreteIntegrator: '<S38>/Integrator' */
  if (rtb_LogicalOperator || (rtDW.Integrator_PrevResetState != 0)) {
    rtDW.Integrator_DSTATE = 0.0F;
  }

  /* DiscreteIntegrator: '<S33>/Filter' */
  if (rtb_LogicalOperator || (rtDW.Filter_PrevResetState != 0)) {
    rtDW.Filter_DSTATE = 0.0F;
  }

  /* Gain: '<S41>/Filter Coefficient' incorporates:
   *  DiscreteIntegrator: '<S33>/Filter'
   *  Gain: '<S31>/Derivative Gain'
   *  Sum: '<S33>/SumD'
   */
  rtDW.FilterCoefficient = (SPEED_Kd * rtDW.accelMeasNoiseVar -
    rtDW.Filter_DSTATE) * 100.0F;

  /* Switch: '<S1>/Switch' incorporates:
   *  Constant: '<S1>/Constant'
   *  DataTypeConversion: '<S1>/Data Type Conversion1'
   *  DiscreteIntegrator: '<S38>/Integrator'
   *  Gain: '<S43>/Proportional Gain'
   *  Inport: '<Root>/running_flag'
   *  Sum: '<S47>/Sum'
   */
  if (rtU.running_flag > 0) {
    rtb_Switch = (int16_T)floorf((SPEED_Kp * rtDW.accelMeasNoiseVar +
      rtDW.Integrator_DSTATE) + rtDW.FilterCoefficient);
  } else {
    rtb_Switch = 0;
  }

  /* End of Switch: '<S1>/Switch' */

  /* Update for DiscreteIntegrator: '<S38>/Integrator' incorporates:
   *  Gain: '<S35>/Integral Gain'
   */
  rtDW.Integrator_DSTATE += SPEED_Ki * rtDW.accelMeasNoiseVar;
  rtDW.Integrator_PrevResetState = (int8_T)rtb_LogicalOperator;

  /* Update for DiscreteIntegrator: '<S33>/Filter' incorporates:
   *  DiscreteIntegrator: '<S38>/Integrator'
   */
  rtDW.Filter_DSTATE += 0.005F * rtDW.FilterCoefficient;
  rtDW.Filter_PrevResetState = (int8_T)rtb_LogicalOperator;

  /* Saturate: '<S1>/Saturation' */
  if (rtb_Switch > 1000) {
    /* Outport: '<Root>/motor_a_pwm' */
    rtY.motor_a_pwm = 1000;
  } else if (rtb_Switch < -1000) {
    /* Outport: '<Root>/motor_a_pwm' */
    rtY.motor_a_pwm = -1000;
  } else {
    /* Outport: '<Root>/motor_a_pwm' */
    rtY.motor_a_pwm = rtb_Switch;
  }

  /* End of Saturate: '<S1>/Saturation' */

  /* Outport: '<Root>/roll' incorporates:
   *  MATLAB Function: '<S1>/MATLAB Function2'
   *  MATLABSystem: '<S1>/IMU Filter'
   */
  rtY.roll = atan2f(rtDW.b_I[7], rtDW.b_I[8]);

  /* Outport: '<Root>/pitch' incorporates:
   *  MATLAB Function: '<S1>/MATLAB Function2'
   *  MATLABSystem: '<S1>/IMU Filter'
   */
  rtY.pitch = -asinf(rtDW.b_I[6]);

  /* Outport: '<Root>/Heading' incorporates:
   *  MATLAB Function: '<S1>/MATLAB Function2'
   *  MATLABSystem: '<S1>/IMU Filter'
   */
  rtY.Heading = atan2f(rtDW.b_I[3], rtDW.b_I[0]);

  /* End of Outputs for SubSystem: '<Root>/task_5ms' */
}

/* Model initialize function */
void blinky_initialize(void)
{
  /* Registration code */

  /* Set task counter limit used by the static main program */
  (rtM)->Timing.TaskCounters.cLimit[0] = 1;
  (rtM)->Timing.TaskCounters.cLimit[1] = 5;

  {
    int32_T i;
    int8_T b_I[9];
    static const real32_T tmp[81] = { 6.09234849E-6F, 0.0F, 0.0F, 0.0F, 0.0F,
      0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 6.09234849E-6F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
      0.0F, 0.0F, 0.0F, 0.0F, 6.09234849E-6F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
      0.0F, 0.0F, 0.0F, 7.61543561E-5F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
      0.0F, 0.0F, 7.61543561E-5F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
      0.0F, 7.61543561E-5F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
      0.00962361F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
      0.00962361F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
      0.00962361F };

    /* SystemInitialize for Atomic SubSystem: '<Root>/task_5ms' */
    /* Start for MATLABSystem: '<S1>/IMU Filter' */
    rtDW.obj.AccelerometerNoise = 0.0001924722;
    rtDW.obj.GyroscopeNoise = 9.1385E-5;
    rtDW.obj.GyroscopeDriftNoise = 3.0462E-13;
    rtDW.obj.LinearAccelerationNoise = 0.0096236100000000012;
    rtDW.obj.LinearAccelerationDecayFactor = 0.1;
    rtDW.obj.isInitialized = 1;
    rtDW.obj.pSensorPeriod = 0.005F;
    rtDW.obj.pKalmanPeriod = 0.005F;
    rtDW.obj.TunablePropsChanged = false;

    /* InitializeConditions for MATLABSystem: '<S1>/IMU Filter' */
    rtDW.obj.pOrientPost.a = 1.0F;
    rtDW.obj.pOrientPost.b = 0.0F;
    rtDW.obj.pOrientPost.c = 0.0F;
    rtDW.obj.pOrientPost.d = 0.0F;

    /* Start for MATLABSystem: '<S1>/IMU Filter' */
    rtDW.obj.pInputPrototype[0] = 0.0F;

    /* InitializeConditions for MATLABSystem: '<S1>/IMU Filter' */
    rtDW.obj.pGyroOffset[0] = 0.0F;

    /* Start for MATLABSystem: '<S1>/IMU Filter' */
    rtDW.obj.pInputPrototype[1] = 0.0F;

    /* InitializeConditions for MATLABSystem: '<S1>/IMU Filter' */
    rtDW.obj.pGyroOffset[1] = 0.0F;

    /* Start for MATLABSystem: '<S1>/IMU Filter' */
    rtDW.obj.pInputPrototype[2] = 0.0F;

    /* InitializeConditions for MATLABSystem: '<S1>/IMU Filter' */
    rtDW.obj.pGyroOffset[2] = 0.0F;
    for (i = 0; i < 9; i++) {
      b_I[i] = 0;
    }

    b_I[0] = 1;
    b_I[4] = 1;
    b_I[8] = 1;
    for (i = 0; i < 9; i++) {
      rtDW.obj.pQv[i] = 0.00981608406F * (real32_T)b_I[i];
    }

    memcpy(&rtDW.obj.pQw[0], &tmp[0], 81U * sizeof(real32_T));
    rtDW.obj.pLinAccelPost[0] = 0.0F;
    rtDW.obj.pLinAccelPost[1] = 0.0F;
    rtDW.obj.pLinAccelPost[2] = 0.0F;
    rtDW.obj.pFirstTime = true;

    /* End of SystemInitialize for SubSystem: '<Root>/task_5ms' */
  }
}

/* Model terminate function */
void blinky_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
