/*
 * File: blinky.c
 *
 * Code generated for Simulink model 'blinky'.
 *
 * Model version                  : 1.76
 * Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
 * C/C++ source code generated on : Thu Aug 27 13:27:30 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include "blinky.h"
#include "rtwtypes.h"
#include <math.h>
#include "blinky_private.h"
#include <string.h>

/* Exported block parameters */
real32_T BALANCE_KD = 1.0F;            /* Variable: BALANCE_KD
                                        * Referenced by: '<S113>/Constant1'
                                        */
real32_T BALANCE_KP = 35.0F;           /* Variable: BALANCE_KP
                                        * Referenced by:
                                        *   '<S113>/Constant3'
                                        *   '<S113>/bal_kp'
                                        */
real32_T SPD_KD = 0.0F;                /* Variable: SPD_KD
                                        * Referenced by: '<S33>/Derivative Gain'
                                        */
real32_T SPD_KI = 0.0012F;             /* Variable: SPD_KI
                                        * Referenced by: '<S37>/Integral Gain'
                                        */
real32_T SPD_KP = 0.465F;              /* Variable: SPD_KP
                                        * Referenced by: '<S45>/Proportional Gain'
                                        */
real32_T TURN_KD = 1.0F;               /* Variable: TURN_KD
                                        * Referenced by: '<S86>/Derivative Gain'
                                        */
real32_T TURN_KP = 15.0F;              /* Variable: TURN_KP
                                        * Referenced by: '<S98>/Proportional Gain'
                                        */

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
static real32_T xnrm2(int32_T n, const real32_T x[8], int32_T ix0);
static void qrFactor(const real32_T A[4], const real32_T S[4], const real32_T
                     Ns[4], real32_T b_S[4]);
static void trisolve(const real32_T A[4], real32_T B_0[4]);
static void trisolve_o(const real32_T A[4], real32_T B_2[4]);
uint32_T plook_u32ff_binc(real32_T u, const real32_T bp[], uint32_T maxIndex,
  real32_T *fraction)
{
  uint32_T bpIndex;

  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Clip'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u <= bp[0U]) {
    bpIndex = 0U;
    *fraction = 0.0F;
  } else if (u < bp[maxIndex]) {
    bpIndex = binsearch_u32f(u, bp, maxIndex >> 1U, maxIndex);
    *fraction = (u - bp[bpIndex]) / (bp[bpIndex + 1U] - bp[bpIndex]);
  } else {
    bpIndex = maxIndex - 1U;
    *fraction = 1.0F;
  }

  return bpIndex;
}

real32_T intrp2d_fu32fl(const uint32_T bpIndex[], const real32_T frac[], const
  real32_T table[], const uint32_T stride)
{
  real32_T yL_0d0;
  real32_T yL_0d1;
  uint32_T offset_1d;

  /* Column-major Interpolation 2-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'off'
     Overflow mode: 'wrapping'
   */
  offset_1d = bpIndex[1U] * stride + bpIndex[0U];
  yL_0d0 = table[offset_1d];
  yL_0d0 += (table[offset_1d + 1U] - yL_0d0) * frac[0U];
  offset_1d += stride;
  yL_0d1 = table[offset_1d];
  return (((table[offset_1d + 1U] - yL_0d1) * frac[0U] + yL_0d1) - yL_0d0) *
    frac[1U] + yL_0d0;
}

uint32_T binsearch_u32f(real32_T u, const real32_T bp[], uint32_T startIndex,
  uint32_T maxIndex)
{
  uint32_T bpIdx;
  uint32_T bpIndex;
  uint32_T iRght;

  /* Binary Search */
  bpIdx = startIndex;
  bpIndex = 0U;
  iRght = maxIndex;
  while (iRght - bpIndex > 1U) {
    if (u < bp[bpIdx]) {
      iRght = bpIdx;
    } else {
      bpIndex = bpIdx;
    }

    bpIdx = (iRght + bpIndex) >> 1U;
  }

  return bpIndex;
}

static real32_T xnrm2(int32_T n, const real32_T x[8], int32_T ix0)
{
  int32_T k;
  real32_T y;

  /* Start for MATLABSystem: '<S119>/MATLAB System' incorporates:
   *  MATLABSystem: '<S121>/MATLAB System'
   */
  y = 0.0F;
  if (n >= 1) {
    if (n == 1) {
      y = fabsf(x[ix0 - 1]);
    } else {
      int32_T kend;
      real32_T scale;
      scale = 1.29246971E-26F;
      kend = ix0 + n;
      for (k = ix0; k < kend; k++) {
        real32_T absxk;
        absxk = fabsf(x[k - 1]);
        if (absxk > scale) {
          real32_T t;
          t = scale / absxk;
          y = y * t * t + 1.0F;
          scale = absxk;
        } else {
          real32_T t;
          t = absxk / scale;
          y += t * t;
        }
      }

      y = scale * sqrtf(y);
    }
  }

  /* End of Start for MATLABSystem: '<S119>/MATLAB System' */
  return y;
}

real32_T rt_hypotf(real32_T u0, real32_T u1)
{
  real32_T a;
  real32_T b;
  real32_T y;
  a = fabsf(u0);
  b = fabsf(u1);
  if (a < b) {
    a /= b;
    y = sqrtf(a * a + 1.0F) * b;
  } else if (a > b) {
    b /= a;
    y = sqrtf(b * b + 1.0F) * a;
  } else {
    y = a * 1.41421354F;
  }

  return y;
}

static void qrFactor(const real32_T A[4], const real32_T S[4], const real32_T
                     Ns[4], real32_T b_S[4])
{
  int32_T M_tmp;
  int32_T d;
  int32_T exitg1;
  int32_T i;
  int32_T iac;
  int32_T jA;
  int32_T lastc;
  real32_T A_0;
  real32_T A_1;
  real32_T S_0;
  real32_T S_1;
  boolean_T exitg2;

  /* Start for MATLABSystem: '<S119>/MATLAB System' incorporates:
   *  MATLABSystem: '<S121>/MATLAB System'
   */
  rtDW.b_atmp = S[1];
  rtDW.xnorm = S[0];
  S_0 = S[3];
  S_1 = S[2];
  for (i = 0; i < 2; i++) {
    A_0 = A[i + 2];
    A_1 = A[i];
    M_tmp = i << 2;
    rtDW.M[M_tmp] = A_0 * rtDW.b_atmp + rtDW.xnorm * A_1;
    rtDW.M[M_tmp + 2] = Ns[i];
    rtDW.M[M_tmp + 1] = A_0 * S_0 + S_1 * A_1;
    rtDW.M[M_tmp + 3] = Ns[i + 2];
    rtDW.b_tau[i] = 0.0F;
    rtDW.work[i] = 0.0F;
  }

  for (M_tmp = 0; M_tmp < 2; M_tmp++) {
    rtDW.ii = (M_tmp << 2) + M_tmp;
    rtDW.lastv = rtDW.ii + 2;
    rtDW.b_atmp = rtDW.M[rtDW.ii];
    rtDW.b_tau[M_tmp] = 0.0F;
    rtDW.xnorm = xnrm2(3 - M_tmp, rtDW.M, rtDW.ii + 2);
    if (rtDW.xnorm != 0.0F) {
      S_0 = rtDW.M[rtDW.ii];
      rtDW.xnorm = rt_hypotf(S_0, rtDW.xnorm);
      if (S_0 >= 0.0F) {
        rtDW.xnorm = -rtDW.xnorm;
      }

      if (fabsf(rtDW.xnorm) < 9.86076132E-32F) {
        rtDW.knt = -1;
        do {
          rtDW.knt++;
          i = rtDW.ii - M_tmp;
          for (lastc = rtDW.lastv; lastc <= i + 4; lastc++) {
            rtDW.M[lastc - 1] *= 1.01412048E+31F;
          }

          rtDW.xnorm *= 1.01412048E+31F;
          rtDW.b_atmp *= 1.01412048E+31F;
        } while ((fabsf(rtDW.xnorm) < 9.86076132E-32F) && (rtDW.knt + 1 < 20));

        rtDW.xnorm = rt_hypotf(rtDW.b_atmp, xnrm2(3 - M_tmp, rtDW.M, rtDW.ii + 2));
        if (rtDW.b_atmp >= 0.0F) {
          rtDW.xnorm = -rtDW.xnorm;
        }

        rtDW.b_tau[M_tmp] = (rtDW.xnorm - rtDW.b_atmp) / rtDW.xnorm;
        rtDW.b_atmp = 1.0F / (rtDW.b_atmp - rtDW.xnorm);
        for (lastc = rtDW.lastv; lastc <= i + 4; lastc++) {
          rtDW.M[lastc - 1] *= rtDW.b_atmp;
        }

        for (lastc = 0; lastc <= rtDW.knt; lastc++) {
          rtDW.xnorm *= 9.86076132E-32F;
        }

        rtDW.b_atmp = rtDW.xnorm;
      } else {
        rtDW.b_tau[M_tmp] = (rtDW.xnorm - S_0) / rtDW.xnorm;
        rtDW.b_atmp = 1.0F / (S_0 - rtDW.xnorm);
        i = rtDW.ii - M_tmp;
        for (lastc = rtDW.lastv; lastc <= i + 4; lastc++) {
          rtDW.M[lastc - 1] *= rtDW.b_atmp;
        }

        rtDW.b_atmp = rtDW.xnorm;
      }
    }

    rtDW.M[rtDW.ii] = rtDW.b_atmp;
    if (M_tmp + 1 < 2) {
      rtDW.M[rtDW.ii] = 1.0F;
      rtDW.knt = rtDW.ii + 5;
      if (rtDW.b_tau[M_tmp] != 0.0F) {
        rtDW.lastv = 4 - M_tmp;
        i = rtDW.ii - M_tmp;
        while ((rtDW.lastv > 0) && (rtDW.M[i + 3] == 0.0F)) {
          rtDW.lastv--;
          i--;
        }

        lastc = -M_tmp;
        exitg2 = false;
        while ((!exitg2) && (lastc + 1 > 0)) {
          i = (lastc << 2) + rtDW.ii;
          jA = i + 5;
          do {
            exitg1 = 0;
            if (jA <= (i + rtDW.lastv) + 4) {
              if (rtDW.M[jA - 1] != 0.0F) {
                exitg1 = 1;
              } else {
                jA++;
              }
            } else {
              lastc--;
              exitg1 = 2;
            }
          } while (exitg1 == 0);

          if (exitg1 == 1) {
            exitg2 = true;
          }
        }
      } else {
        rtDW.lastv = 0;
        lastc = -1;
      }

      if (rtDW.lastv > 0) {
        if (lastc + 1 != 0) {
          if (lastc >= 0) {
            memset(&rtDW.work[0], 0, (uint32_T)(lastc + 1) * sizeof(real32_T));
          }

          i = (lastc << 2) + rtDW.ii;
          for (iac = rtDW.knt; iac <= i + 5; iac += 4) {
            rtDW.xnorm = 0.0F;
            d = iac + rtDW.lastv;
            for (jA = iac; jA < d; jA++) {
              rtDW.xnorm += rtDW.M[(rtDW.ii + jA) - iac] * rtDW.M[jA - 1];
            }

            jA = ((iac - rtDW.ii) - 5) >> 2;
            rtDW.work[jA] += rtDW.xnorm;
          }
        }

        if (-rtDW.b_tau[M_tmp] != 0.0F) {
          jA = rtDW.ii;
          for (rtDW.knt = 0; rtDW.knt <= lastc; rtDW.knt++) {
            rtDW.xnorm = rtDW.work[rtDW.knt];
            if (rtDW.xnorm != 0.0F) {
              rtDW.xnorm *= -rtDW.b_tau[M_tmp];
              i = jA + 5;
              iac = (rtDW.lastv + jA) + 4;
              for (d = i; d <= iac; d++) {
                rtDW.M[d - 1] += rtDW.M[((rtDW.ii + d) - jA) - 5] * rtDW.xnorm;
              }
            }

            jA += 4;
          }
        }
      }

      rtDW.M[rtDW.ii] = rtDW.b_atmp;
    }
  }

  for (rtDW.ii = 0; rtDW.ii < 2; rtDW.ii++) {
    for (M_tmp = 0; M_tmp <= rtDW.ii; M_tmp++) {
      /* Start for MATLABSystem: '<S119>/MATLAB System' incorporates:
       *  MATLABSystem: '<S121>/MATLAB System'
       */
      rtDW.b_R[M_tmp + (rtDW.ii << 1)] = rtDW.M[(rtDW.ii << 2) + M_tmp];
    }

    if (rtDW.ii <= 0) {
      /* Start for MATLABSystem: '<S119>/MATLAB System' incorporates:
       *  MATLABSystem: '<S121>/MATLAB System'
       */
      rtDW.b_R[(rtDW.ii << 1) + 1] = 0.0F;
    }
  }

  /* Start for MATLABSystem: '<S119>/MATLAB System' incorporates:
   *  MATLABSystem: '<S121>/MATLAB System'
   */
  b_S[0] = rtDW.b_R[0];
  b_S[1] = rtDW.b_R[2];
  b_S[2] = rtDW.b_R[1];
  b_S[3] = rtDW.b_R[3];
}

static void trisolve(const real32_T A[4], real32_T B_0[4])
{
  int32_T b_j;
  int32_T b_k;
  int32_T i;

  /* Start for MATLABSystem: '<S119>/MATLAB System' */
  for (b_j = 0; b_j < 2; b_j++) {
    int32_T jBcol;
    jBcol = (b_j << 1) - 1;
    for (b_k = 0; b_k < 2; b_k++) {
      int32_T B_tmp;
      int32_T k;
      int32_T kAcol;
      real32_T B_1;
      k = b_k + 1;
      kAcol = (b_k << 1) - 1;
      B_tmp = (b_k + jBcol) + 1;
      B_1 = B_0[B_tmp];
      if (B_1 != 0.0F) {
        B_0[B_tmp] = B_1 / A[(b_k + kAcol) + 1];
        for (i = k + 1; i < 3; i++) {
          B_0[jBcol + 2] -= A[kAcol + 2] * B_0[B_tmp];
        }
      }
    }
  }

  /* End of Start for MATLABSystem: '<S119>/MATLAB System' */
}

static void trisolve_o(const real32_T A[4], real32_T B_2[4])
{
  int32_T b_i;
  int32_T b_j;
  int32_T k;

  /* Start for MATLABSystem: '<S119>/MATLAB System' */
  for (b_j = 0; b_j < 2; b_j++) {
    int32_T jBcol;
    jBcol = b_j << 1;
    for (k = 1; k >= 0; k--) {
      int32_T kAcol;
      int32_T tmp_0;
      real32_T tmp;
      kAcol = k << 1;
      tmp_0 = k + jBcol;
      tmp = B_2[tmp_0];
      if (tmp != 0.0F) {
        B_2[tmp_0] = tmp / A[k + kAcol];
        for (b_i = 0; b_i < k; b_i++) {
          B_2[jBcol] -= B_2[tmp_0] * A[kAcol];
        }
      }
    }
  }

  /* End of Start for MATLABSystem: '<S119>/MATLAB System' */
}

/* Model step function for TID0 */
void blinky_step0(void)                /* Sample time: [0.001s, 0.0s] */
{
  /* (no output/update code required) */
}

/* Model step function for TID1 */
void blinky_step1(void)                /* Sample time: [0.005s, 0.0s] */
{
  int16_T rtb_Switch1;
  uint8_T rtb_UnitDelay4;

  /* UnitDelay: '<Root>/Unit Delay4' */
  rtb_UnitDelay4 = rtDW.UnitDelay4_DSTATE;

  /* Outputs for Atomic SubSystem: '<Root>/task_5ms' */
  /* Outputs for Enabled SubSystem: '<S118>/Correct1' incorporates:
   *  EnablePort: '<S119>/Enable'
   */
  /* MATLABSystem: '<S119>/MATLAB System' incorporates:
   *  Constant: '<S118>/R1'
   *  DataStoreRead: '<S119>/Data Store ReadP'
   *  DataStoreRead: '<S119>/Data Store ReadX'
   */
  /* x 就是一开始状态转移函数定义的状态向量 */
  /* x = [picth;roll]; */
  /*  u = [pitch_acc;roll_acc] */
  rtDW.epsilon = fmaxf(0.000345266977F, 0.000345266977F * fabsf(rtDW.x[0]));

  /* x 就是一开始状态转移函数定义的状态向量 */
  /* x = [picth;roll]; */
  /*  u = [pitch_acc;roll_acc] */
  rtDW.b_dHdx[0] = ((rtDW.x[0] + rtDW.epsilon) - rtDW.x[0]) / rtDW.epsilon;
  rtDW.b_dHdx[1] = 0.0F / rtDW.epsilon;
  rtDW.epsilon = fmaxf(0.000345266977F, 0.000345266977F * fabsf(rtDW.x[1]));

  /* x 就是一开始状态转移函数定义的状态向量 */
  /* x = [picth;roll]; */
  /*  u = [pitch_acc;roll_acc] */
  rtDW.b_dHdx[2] = 0.0F / rtDW.epsilon;
  rtDW.b_dHdx[3] = ((rtDW.x[1] + rtDW.epsilon) - rtDW.x[1]) / rtDW.epsilon;

  /* x 就是一开始状态转移函数定义的状态向量 */
  /* x = [picth;roll]; */
  /*  u = [pitch_acc;roll_acc] */
  qrFactor(rtDW.b_dHdx, rtDW.P_i, rtConstP.R1_Value, rtDW.Sy);
  for (rtDW.i = 0; rtDW.i < 2; rtDW.i++) {
    /* Start for MATLABSystem: '<S119>/MATLAB System' incorporates:
     *  DataStoreRead: '<S119>/Data Store ReadP'
     */
    rtDW.Gain1 = rtDW.P_i[rtDW.i];
    rtDW.TSamp = rtDW.P_i[0] * rtDW.Gain1;
    rtDW.K_idx_1 = rtDW.P_i[1] * rtDW.Gain1;
    rtDW.b_i = rtDW.i << 1;
    rtDW.A[rtDW.b_i] = 0.0F;
    rtDW.Gain1 = rtDW.P_i[rtDW.i + 2];
    rtDW.K[rtDW.b_i] = rtDW.P_i[2] * rtDW.Gain1 + rtDW.TSamp;
    rtDW.K[rtDW.b_i + 1] = rtDW.P_i[3] * rtDW.Gain1 + rtDW.K_idx_1;
    rtDW.A[rtDW.b_i + 1] = 0.0F;
  }

  /* Start for MATLABSystem: '<S119>/MATLAB System' */
  rtDW.Gain1 = rtDW.K[0];
  rtDW.TSamp = rtDW.K[1];
  rtDW.K_idx_1 = rtDW.K[2];
  rtDW.TSamp_f = rtDW.K[3];
  for (rtDW.i = 0; rtDW.i < 2; rtDW.i++) {
    rtDW.epsilon = rtDW.b_dHdx[rtDW.i];
    rtDW.b_i = rtDW.i << 1;
    rtDW.rtb_FISLookupTableData_idx_0 = rtDW.Gain1 * rtDW.epsilon +
      rtDW.A[rtDW.b_i];
    rtDW.rtb_FISLookupTableData_idx_1 = rtDW.A[rtDW.b_i + 1] + rtDW.TSamp *
      rtDW.epsilon;
    rtDW.epsilon = rtDW.b_dHdx[rtDW.i + 2];
    rtDW.A[rtDW.b_i] = rtDW.K_idx_1 * rtDW.epsilon +
      rtDW.rtb_FISLookupTableData_idx_0;
    rtDW.A[rtDW.b_i + 1] = rtDW.TSamp_f * rtDW.epsilon +
      rtDW.rtb_FISLookupTableData_idx_1;
  }

  /* MATLABSystem: '<S119>/MATLAB System' */
  rtDW.K[0] = rtDW.A[0];
  rtDW.K[1] = rtDW.A[2];
  rtDW.K[2] = rtDW.A[1];
  rtDW.K[3] = rtDW.A[3];

  /* Start for MATLABSystem: '<S119>/MATLAB System' */
  trisolve(rtDW.Sy, rtDW.K);

  /* MATLABSystem: '<S119>/MATLAB System' */
  rtDW.A[0] = rtDW.Sy[0];
  rtDW.A[1] = rtDW.Sy[2];
  rtDW.A[2] = rtDW.Sy[1];
  rtDW.A[3] = rtDW.Sy[3];
  rtDW.Sy[0] = rtDW.K[0];
  rtDW.Sy[1] = rtDW.K[1];
  rtDW.Sy[2] = rtDW.K[2];
  rtDW.Sy[3] = rtDW.K[3];

  /* Start for MATLABSystem: '<S119>/MATLAB System' */
  trisolve_o(rtDW.A, rtDW.Sy);

  /* MATLABSystem: '<S119>/MATLAB System' */
  rtDW.K[0] = rtDW.Sy[0];
  rtDW.K[1] = rtDW.Sy[2];
  rtDW.K[2] = rtDW.Sy[1];
  rtDW.K[3] = rtDW.Sy[3];

  /* Start for MATLABSystem: '<S119>/MATLAB System' incorporates:
   *  DataStoreRead: '<S119>/Data Store ReadX'
   *  Inport: '<Root>/accx'
   *  Inport: '<Root>/accy'
   *  Inport: '<Root>/accz'
   *  MATLAB Function: '<S4>/MATLAB Function3'
   */
  rtDW.TSamp = -rtDW.Sy[0];
  rtDW.K_idx_1 = -rtDW.Sy[2];
  rtDW.TSamp_f = -rtDW.Sy[1];
  rtDW.epsilon = -rtDW.Sy[3];
  rtDW.rtb_FISLookupTableData_idx_0 = atan2f(-rtU.accx, sqrtf(rtU.accy *
    rtU.accy + rtU.accz * rtU.accz)) - rtDW.x[0];
  rtDW.rtb_FISLookupTableData_idx_1 = atan2f(rtU.accy, rtU.accz) - rtDW.x[1];
  for (rtDW.b_i = 0; rtDW.b_i < 2; rtDW.b_i++) {
    /* MATLABSystem: '<S119>/MATLAB System' incorporates:
     *  Constant: '<S118>/R1'
     */
    rtDW.i = rtDW.b_i << 1;
    rtDW.Gain1 = rtDW.b_dHdx[rtDW.i];
    rtDW.rtb_FISLookupTableData_idx_2 = rtDW.TSamp * rtDW.Gain1;
    rtDW.z_idx_1 = rtDW.K_idx_1 * rtDW.Gain1;
    rtDW.Gain1 = rtDW.b_dHdx[rtDW.i + 1];
    rtDW.A[rtDW.i] = rtDW.TSamp_f * rtDW.Gain1 +
      rtDW.rtb_FISLookupTableData_idx_2;
    rtDW.A[rtDW.i + 1] = rtDW.epsilon * rtDW.Gain1 + rtDW.z_idx_1;
    rtDW.A_tmp = rtDW.i + rtDW.b_i;
    rtDW.A[rtDW.A_tmp]++;

    /* Start for MATLABSystem: '<S119>/MATLAB System' incorporates:
     *  Constant: '<S118>/R1'
     */
    rtDW.Gain1 = rtConstP.R1_Value[rtDW.i];
    rtDW.rtb_FISLookupTableData_idx_2 = rtDW.K[0] * rtDW.Gain1;
    rtDW.z_idx_1 = rtDW.K[1] * rtDW.Gain1;
    rtDW.Gain1 = rtConstP.R1_Value[rtDW.i + 1];
    rtDW.Sy[rtDW.i] = rtDW.K[2] * rtDW.Gain1 + rtDW.rtb_FISLookupTableData_idx_2;
    rtDW.Sy[rtDW.i + 1] = rtDW.K[3] * rtDW.Gain1 + rtDW.z_idx_1;

    /* DataStoreWrite: '<S119>/Data Store WriteX' incorporates:
     *  DataStoreRead: '<S119>/Data Store ReadX'
     *  MATLABSystem: '<S119>/MATLAB System'
     * */
    rtDW.x[rtDW.b_i] += rtDW.K[rtDW.b_i + 2] * rtDW.rtb_FISLookupTableData_idx_1
      + rtDW.K[rtDW.b_i] * rtDW.rtb_FISLookupTableData_idx_0;
  }

  /* End of Outputs for SubSystem: '<S118>/Correct1' */
  /* End of Outputs for SubSystem: '<Root>/task_5ms' */
  for (rtDW.i = 0; rtDW.i < 4; rtDW.i++) {
    /* Outputs for Atomic SubSystem: '<Root>/task_5ms' */
    /* Outputs for Enabled SubSystem: '<S118>/Correct1' incorporates:
     *  EnablePort: '<S119>/Enable'
     */
    /* DataStoreRead: '<S119>/Data Store ReadP' */
    rtDW.K[rtDW.i] = rtDW.P_i[rtDW.i];

    /* End of Outputs for SubSystem: '<S118>/Correct1' */
    /* End of Outputs for SubSystem: '<Root>/task_5ms' */
  }

  /* Outputs for Atomic SubSystem: '<Root>/task_5ms' */
  /* Outputs for Enabled SubSystem: '<S118>/Correct1' incorporates:
   *  EnablePort: '<S119>/Enable'
   */
  /* Start for MATLABSystem: '<S119>/MATLAB System' incorporates:
   *  DataStoreWrite: '<S119>/Data Store WriteP'
   */
  qrFactor(rtDW.A, rtDW.K, rtDW.Sy, rtDW.P_i);

  /* End of Outputs for SubSystem: '<S118>/Correct1' */

  /* Outport: '<Root>/roll' incorporates:
   *  DataStoreRead: '<S120>/Data Store Read'
   *  Gain: '<S4>/Gain'
   */
  rtY.roll = 57.2957802F * rtDW.x[1];

  /* Gain: '<S4>/Gain1' incorporates:
   *  DataStoreRead: '<S120>/Data Store Read'
   */
  rtDW.Gain1 = 57.2957802F * rtDW.x[0];

  /* Sum: '<S113>/Sum2' */
  rtDW.epsilon = rtDW.UnitDelay2 - rtDW.Gain1;

  /* SampleTimeMath: '<S116>/TSamp'
   *
   * About '<S116>/TSamp':
   *  y = u * K where K = 1 / ( w * Ts )
   *   */
  rtDW.TSamp = rtDW.epsilon * 200.0F;

  /* DiscreteIntegrator: '<S113>/Discrete-Time Integrator' incorporates:
   *  Logic: '<S113>/NOT'
   */
  if (rtb_UnitDelay4 == 0) {
    rtDW.DiscreteTimeIntegrator_DSTATE = 0.0F;
  }

  /* Gain: '<S113>/gain1' incorporates:
   *  DiscreteIntegrator: '<S113>/Discrete-Time Integrator'
   */
  rtDW.K_idx_1 = 0.005F * rtDW.epsilon;

  /* SampleTimeMath: '<S115>/TSamp' incorporates:
   *  Gain: '<S113>/gain1'
   *
   * About '<S115>/TSamp':
   *  y = u * K where K = 1 / ( w * Ts )
   *   */
  rtDW.TSamp_f = rtDW.K_idx_1 * 200.0F;

  /* Switch: '<S113>/Switch1' incorporates:
   *  Constant: '<S113>/Constant1'
   *  Constant: '<S113>/Constant3'
   *  Constant: '<S113>/Constant4'
   *  Constant: '<S117>/OutDims'
   *  DataTypeConversion: '<S113>/Data Type Conversion7'
   *  DiscreteIntegrator: '<S113>/Discrete-Time Integrator'
   *  Gain: '<S113>/Gain'
   *  Gain: '<S113>/Gain1'
   *  Gain: '<S113>/Gain2'
   *  Gain: '<S113>/bal_kp'
   *  Interpolation_n-D: '<S117>/FISLookupTableData'
   *  Product: '<S113>/Product'
   *  Product: '<S113>/Product1'
   *  Product: '<S113>/Product2'
   *  Sum: '<S113>/Add'
   *  Sum: '<S113>/Add1'
   *  Sum: '<S113>/Add2'
   *  Sum: '<S113>/Add3'
   *  Sum: '<S115>/Diff'
   *  UnitDelay: '<S115>/UD'
   *
   * Block description for '<S115>/Diff':
   *
   *  Add in CPU
   *
   * Block description for '<S115>/UD':
   *
   *  Store in Global RAM
   */
  if (rtb_UnitDelay4 > 0) {
    /* Outputs for Atomic SubSystem: '<S113>/Fuzzy Logic  Controller' */
    /* Interpolation_n-D: '<S117>/FISLookupTableData' incorporates:
     *  Gain: '<S113>/gain'
     *  Gain: '<S113>/gain2'
     *  PreLookup: '<S117>/FISLookupTableIndex1'
     *  PreLookup: '<S117>/FISLookupTableIndex2'
     *  Sum: '<S116>/Diff'
     *  UnitDelay: '<S116>/UD'
     *
     * Block description for '<S116>/Diff':
     *
     *  Add in CPU
     *
     * Block description for '<S116>/UD':
     *
     *  Store in Global RAM
     */
    rtDW.bpIndex[0] = plook_u32ff_binc(e_factor * rtDW.epsilon,
      rtConstP.FISLookupTableIndex1_Breakpoint, 20U, &rtDW.frac[0]);
    rtDW.bpIndex[1] = plook_u32ff_binc((rtDW.TSamp - rtDW.UD_DSTATE) * ec_factor,
      rtConstP.FISLookupTableIndex2_Breakpoint, 20U, &rtDW.frac[1]);
    rtDW.bpIndex[2] = 0U;

    /* Interpolation_n-D: '<S117>/FISLookupTableData' incorporates:
     *  Constant: '<S117>/OutDims'
     */
    rtDW.rtb_FISLookupTableData_idx_0 = intrp2d_fu32fl(rtDW.bpIndex, rtDW.frac,
      &rtConstP.FISLookupTableData_Table[0U], 21U);
    rtDW.bpIndex[2] = 1U;

    /* Interpolation_n-D: '<S117>/FISLookupTableData' incorporates:
     *  Constant: '<S117>/OutDims'
     */
    rtDW.rtb_FISLookupTableData_idx_1 = intrp2d_fu32fl(rtDW.bpIndex, rtDW.frac,
      &rtConstP.FISLookupTableData_Table[441U], 21U);
    rtDW.bpIndex[2] = 2U;

    /* Interpolation_n-D: '<S117>/FISLookupTableData' incorporates:
     *  Constant: '<S117>/OutDims'
     */
    rtDW.rtb_FISLookupTableData_idx_2 = intrp2d_fu32fl(rtDW.bpIndex, rtDW.frac,
      &rtConstP.FISLookupTableData_Table[882U], 21U);

    /* End of Outputs for SubSystem: '<S113>/Fuzzy Logic  Controller' */
    rtb_Switch1 = (int16_T)floorf(((bal_kp_factor *
      rtDW.rtb_FISLookupTableData_idx_0 + BALANCE_KP) * (BALANCE_KP *
      rtDW.epsilon) + (real32_T)((real_T)(bal_ki_factor *
      rtDW.rtb_FISLookupTableData_idx_1) * rtDW.DiscreteTimeIntegrator_DSTATE))
      + (bal_kd_factor * rtDW.rtb_FISLookupTableData_idx_2 + BALANCE_KD) *
      (rtDW.TSamp_f - rtDW.UD_DSTATE_f));
  } else {
    rtb_Switch1 = 0;
  }

  /* End of Switch: '<S113>/Switch1' */

  /* Saturate: '<S113>/Saturation1' */
  if (rtb_Switch1 > ((int16_T)PWM_MAX)) {
    rtb_Switch1 = ((int16_T)PWM_MAX);
  } else if (rtb_Switch1 < ((int16_T)PWM_MIN)) {
    rtb_Switch1 = ((int16_T)PWM_MIN);
  }

  /* End of Saturate: '<S113>/Saturation1' */

  /* Outputs for Atomic SubSystem: '<S118>/Predict' */
  /* Start for MATLABSystem: '<S121>/MATLAB System' incorporates:
   *  Inport: '<Root>/gyroy'
   */
  /*  x = [pitch; roll]  (2×1) */
  /*  u = [gx; gy]       (2×1) */
  rtDW.rtb_FISLookupTableData_idx_0 = rtU.gyroy * 0.005F;

  /* MATLABSystem: '<S121>/MATLAB System' incorporates:
   *  DataStoreRead: '<S121>/Data Store ReadX'
   */
  rtDW.rtb_FISLookupTableData_idx_1 = rtDW.rtb_FISLookupTableData_idx_0 +
    rtDW.x[0];

  /* Start for MATLABSystem: '<S121>/MATLAB System' incorporates:
   *  Inport: '<Root>/gyrox'
   */
  rtDW.rtb_FISLookupTableData_idx_2 = rtU.gyrox * 0.005F;

  /* MATLABSystem: '<S121>/MATLAB System' incorporates:
   *  DataStoreRead: '<S121>/Data Store ReadX'
   */
  rtDW.z_idx_1 = rtDW.rtb_FISLookupTableData_idx_2 + rtDW.x[1];

  /*  2×1 */
  rtDW.epsilon = fmaxf(0.000345266977F, 0.000345266977F * fabsf(rtDW.x[0]));

  /*  x = [pitch; roll]  (2×1) */
  /*  u = [gx; gy]       (2×1) */
  /*  2×1 */
  rtDW.b_dHdx[0] = (((rtDW.x[0] + rtDW.epsilon) +
                     rtDW.rtb_FISLookupTableData_idx_0) -
                    rtDW.rtb_FISLookupTableData_idx_1) / rtDW.epsilon;
  rtDW.b_dHdx[1] = (rtDW.z_idx_1 - rtDW.z_idx_1) / rtDW.epsilon;
  rtDW.epsilon = fmaxf(0.000345266977F, 0.000345266977F * fabsf(rtDW.x[1]));

  /*  x = [pitch; roll]  (2×1) */
  /*  u = [gx; gy]       (2×1) */
  /*  2×1 */
  rtDW.b_dHdx[2] = (rtDW.rtb_FISLookupTableData_idx_1 -
                    rtDW.rtb_FISLookupTableData_idx_1) / rtDW.epsilon;
  rtDW.b_dHdx[3] = (((rtDW.x[1] + rtDW.epsilon) +
                     rtDW.rtb_FISLookupTableData_idx_2) - rtDW.z_idx_1) /
    rtDW.epsilon;

  /* End of Outputs for SubSystem: '<S118>/Predict' */
  /* End of Outputs for SubSystem: '<Root>/task_5ms' */
  /*  x = [pitch; roll]  (2×1) */
  /*  u = [gx; gy]       (2×1) */
  /*  2×1 */
  for (rtDW.i = 0; rtDW.i < 4; rtDW.i++) {
    /* Outputs for Atomic SubSystem: '<Root>/task_5ms' */
    /* Outputs for Atomic SubSystem: '<S118>/Predict' */
    /* DataStoreRead: '<S121>/Data Store ReadP' */
    rtDW.K[rtDW.i] = rtDW.P_i[rtDW.i];

    /* End of Outputs for SubSystem: '<S118>/Predict' */
    /* End of Outputs for SubSystem: '<Root>/task_5ms' */
  }

  /* Outputs for Atomic SubSystem: '<Root>/task_5ms' */
  /* Outputs for Atomic SubSystem: '<S118>/Predict' */
  /* Start for MATLABSystem: '<S121>/MATLAB System' incorporates:
   *  Constant: '<S118>/Q'
   *  DataStoreWrite: '<S121>/Data Store WriteP'
   */
  qrFactor(rtDW.b_dHdx, rtDW.K, rtConstP.Q_Value, rtDW.P_i);

  /* DataStoreWrite: '<S121>/Data Store WriteX' incorporates:
   *  MATLABSystem: '<S121>/MATLAB System'
   * */
  rtDW.x[0] = rtDW.rtb_FISLookupTableData_idx_1;
  rtDW.x[1] = rtDW.z_idx_1;

  /* End of Outputs for SubSystem: '<S118>/Predict' */

  /* Update for UnitDelay: '<S116>/UD'
   *
   * Block description for '<S116>/UD':
   *
   *  Store in Global RAM
   */
  rtDW.UD_DSTATE = rtDW.TSamp;

  /* Update for DiscreteIntegrator: '<S113>/Discrete-Time Integrator' */
  rtDW.DiscreteTimeIntegrator_DSTATE += rtDW.K_idx_1;
  if (rtDW.DiscreteTimeIntegrator_DSTATE > 200.0F) {
    rtDW.DiscreteTimeIntegrator_DSTATE = 200.0F;
  } else if (rtDW.DiscreteTimeIntegrator_DSTATE < -200.0F) {
    rtDW.DiscreteTimeIntegrator_DSTATE = -200.0F;
  }

  /* Update for UnitDelay: '<S115>/UD'
   *
   * Block description for '<S115>/UD':
   *
   *  Store in Global RAM
   */
  rtDW.UD_DSTATE_f = rtDW.TSamp_f;

  /* End of Outputs for SubSystem: '<Root>/task_5ms' */

  /* Outport: '<Root>/pitch' */
  rtY.pitch = rtDW.Gain1;

  /* Outport: '<Root>/motor_a_pwm' incorporates:
   *  UnitDelay: '<Root>/Unit Delay'
   */
  rtY.motor_a_pwm = rtDW.UnitDelay_DSTATE;

  /* Outport: '<Root>/motor_b_pwm' incorporates:
   *  UnitDelay: '<Root>/Unit Delay1'
   */
  rtY.motor_b_pwm = rtDW.UnitDelay1_DSTATE;

  /* UnitDelay: '<Root>/Unit Delay3' */
  rtDW.epsilon = rtDW.UnitDelay3_DSTATE;

  /* Chart: '<Root>/output_flag' incorporates:
   *  Inport: '<Root>/running_flag'
   */
  rtb_UnitDelay4 = (uint8_T)((rtU.running_flag > 0) && (rtDW.epsilon > pitch_min)
    && (rtDW.epsilon < pitch_max));

  /* Outport: '<Root>/running_success_flag' */
  rtY.running_success_flag = rtb_UnitDelay4;

  /* UnitDelay: '<Root>/Unit Delay5' */
  rtDW.UnitDelay5 = rtDW.UnitDelay5_DSTATE;

  /* UnitDelay: '<Root>/Unit Delay6' */
  rtDW.UnitDelay6 = rtDW.UnitDelay6_DSTATE;

  /* Update for UnitDelay: '<Root>/Unit Delay4' */
  rtDW.UnitDelay4_DSTATE = rtb_UnitDelay4;

  /* Update for UnitDelay: '<Root>/Unit Delay' incorporates:
   *  Sum: '<Root>/motor_a'
   */
  rtDW.UnitDelay_DSTATE = (int16_T)(rtb_Switch1 - rtDW.DataTypeConversion);

  /* Update for UnitDelay: '<Root>/Unit Delay1' incorporates:
   *  Sum: '<Root>/motor_b'
   */
  rtDW.UnitDelay1_DSTATE = (int16_T)(rtb_Switch1 + rtDW.DataTypeConversion);

  /* Update for UnitDelay: '<Root>/Unit Delay3' */
  rtDW.UnitDelay3_DSTATE = rtDW.Gain1;

  /* Update for UnitDelay: '<Root>/Unit Delay5' */
  rtDW.UnitDelay5_DSTATE = rtb_UnitDelay4;

  /* Update for UnitDelay: '<Root>/Unit Delay6' */
  rtDW.UnitDelay6_DSTATE = rtb_UnitDelay4;
}

/* Model step function for TID2 */
void blinky_step2(void)                /* Sample time: [0.01s, 0.0s] */
{
  real32_T rtb_FilterCoefficient;
  real32_T rtb_IntegralGain;
  real32_T rtb_Sum;
  boolean_T rtb_NOT_c;

  /* UnitDelay: '<Root>/Unit Delay2' */
  rtDW.UnitDelay2 = rtDW.UnitDelay2_DSTATE;

  /* Outport: '<Root>/angle_target' */
  rtY.angle_target = rtDW.UnitDelay2;

  /* Outputs for Atomic SubSystem: '<Root>/task_10ms' */
  /* Sum: '<S2>/Sum' incorporates:
   *  Constant: '<S2>/Constant'
   *  Inport: '<Root>/motor_a_speed'
   *  Inport: '<Root>/motor_b_speed'
   *  Inport: '<Root>/target_speed'
   *  Product: '<S2>/Divide'
   *  Sum: '<S2>/Add'
   */
  rtb_Sum = (rtU.motor_a_speed + rtU.motor_b_speed) / 2.0F - rtU.target_speed;

  /* Logic: '<S2>/NOT' */
  rtb_NOT_c = (rtDW.UnitDelay5 == 0);

  /* DiscreteIntegrator: '<S35>/Filter' */
  if (rtb_NOT_c || (rtDW.Filter_PrevResetState_a != 0)) {
    rtDW.Filter_DSTATE_n = 0.0F;
  }

  /* Gain: '<S37>/Integral Gain' */
  rtb_IntegralGain = SPD_KI * rtb_Sum;

  /* DiscreteIntegrator: '<S40>/Integrator' */
  if (rtb_NOT_c || (rtDW.Integrator_PrevResetState != 0)) {
    rtDW.Integrator_DSTATE = 0.0F;
  }

  /* Gain: '<S43>/Filter Coefficient' incorporates:
   *  DiscreteIntegrator: '<S35>/Filter'
   *  Gain: '<S33>/Derivative Gain'
   *  Sum: '<S35>/SumD'
   */
  rtb_FilterCoefficient = (SPD_KD * rtb_Sum - rtDW.Filter_DSTATE_n) * 100.0F;

  /* Sum: '<S49>/Sum' incorporates:
   *  DiscreteIntegrator: '<S40>/Integrator'
   *  Gain: '<S45>/Proportional Gain'
   */
  rtb_Sum = (SPD_KP * rtb_Sum + rtDW.Integrator_DSTATE) + rtb_FilterCoefficient;

  /* Update for DiscreteIntegrator: '<S35>/Filter' */
  rtDW.Filter_DSTATE_n += 0.01F * rtb_FilterCoefficient;
  rtDW.Filter_PrevResetState_a = (int8_T)rtb_NOT_c;

  /* Update for DiscreteIntegrator: '<S40>/Integrator' incorporates:
   *  DiscreteIntegrator: '<S35>/Filter'
   */
  rtDW.Integrator_DSTATE += rtb_IntegralGain;
  if (rtDW.Integrator_DSTATE > ANGLE_INTERGRAL_MAX) {
    rtDW.Integrator_DSTATE = ANGLE_INTERGRAL_MAX;
  } else if (rtDW.Integrator_DSTATE < ANGLE_INTERGRAL_MIN) {
    rtDW.Integrator_DSTATE = ANGLE_INTERGRAL_MIN;
  }

  rtDW.Integrator_PrevResetState = (int8_T)rtb_NOT_c;

  /* End of Update for DiscreteIntegrator: '<S40>/Integrator' */

  /* Saturate: '<S47>/Saturation' */
  if (rtb_Sum > ANGLE_MAX) {
    /* Update for UnitDelay: '<Root>/Unit Delay2' */
    rtDW.UnitDelay2_DSTATE = ANGLE_MAX;
  } else if (rtb_Sum < ANGLE_MIN) {
    /* Update for UnitDelay: '<Root>/Unit Delay2' */
    rtDW.UnitDelay2_DSTATE = ANGLE_MIN;
  } else {
    /* Update for UnitDelay: '<Root>/Unit Delay2' */
    rtDW.UnitDelay2_DSTATE = rtb_Sum;
  }

  /* End of Saturate: '<S47>/Saturation' */
  /* End of Outputs for SubSystem: '<Root>/task_10ms' */
}

/* Model step function for TID3 */
void blinky_step3(void)                /* Sample time: [0.02s, 0.0s] */
{
  real32_T rtb_FilterCoefficient;
  real32_T u0;
  boolean_T rtb_NOT_n;

  /* Outputs for Atomic SubSystem: '<Root>/task_20ms' */
  /* Logic: '<S3>/NOT' */
  rtb_NOT_n = (rtDW.UnitDelay6 == 0);

  /* DiscreteIntegrator: '<S88>/Filter' */
  if (rtb_NOT_n || (rtDW.Filter_PrevResetState != 0)) {
    rtDW.Filter_DSTATE = 0.0F;
  }

  /* Gain: '<S96>/Filter Coefficient' incorporates:
   *  DiscreteIntegrator: '<S88>/Filter'
   */
  rtb_FilterCoefficient = 100.0F * rtDW.Filter_DSTATE;

  /* Sum: '<S102>/Sum' incorporates:
   *  Gain: '<S98>/Proportional Gain'
   *  Inport: '<Root>/turn_target'
   */
  u0 = TURN_KP * rtU.turn_target + rtb_FilterCoefficient;

  /* Saturate: '<S100>/Saturation' */
  if (u0 > 500.0F) {
    u0 = 500.0F;
  } else if (u0 < -500.0F) {
    u0 = -500.0F;
  }

  /* DataTypeConversion: '<S3>/Data Type Conversion' incorporates:
   *  Saturate: '<S100>/Saturation'
   */
  rtDW.DataTypeConversion = (int16_T)floorf(u0);

  /* Update for DiscreteIntegrator: '<S88>/Filter' incorporates:
   *  Gain: '<S86>/Derivative Gain'
   *  Inport: '<Root>/gyroz'
   *  Sum: '<S88>/SumD'
   */
  rtDW.Filter_DSTATE += (TURN_KD * rtU.gyroz - rtb_FilterCoefficient) * 0.02F;
  rtDW.Filter_PrevResetState = (int8_T)rtb_NOT_n;

  /* End of Outputs for SubSystem: '<Root>/task_20ms' */
}

/* Model initialize function */
void blinky_initialize(void)
{
  /* Registration code */

  /* Set task counter limit used by the static main program */
  (rtM)->Timing.TaskCounters.cLimit[0] = 1;
  (rtM)->Timing.TaskCounters.cLimit[1] = 5;
  (rtM)->Timing.TaskCounters.cLimit[2] = 10;
  (rtM)->Timing.TaskCounters.cLimit[3] = 20;

  /* SystemInitialize for Atomic SubSystem: '<Root>/task_5ms' */
  /* Start for DataStoreMemory: '<S118>/DataStoreMemory - P' */
  rtDW.P_i[0] = 1.0F;
  rtDW.P_i[1] = 0.0F;
  rtDW.P_i[2] = 0.0F;
  rtDW.P_i[3] = 1.0F;

  /* End of SystemInitialize for SubSystem: '<Root>/task_5ms' */
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
