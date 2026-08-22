/*
 * File: blinky.c
 *
 * Code generated for Simulink model 'blinky'.
 *
 * Model version                  : 1.63
 * Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
 * C/C++ source code generated on : Fri Aug 21 21:50:39 2026
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
#include <string.h>

/* Exported block parameters */
real_T BALANCE_KD = 1.0;               /* Variable: BALANCE_KD
                                        * Referenced by: '<S4>/BALANCE_KD'
                                        */
real_T BALANCE_KP = 20.0;              /* Variable: BALANCE_KP
                                        * Referenced by: '<S4>/BALANCE_KP'
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
static real_T xnrm2(int32_T n, const real_T x[8], int32_T ix0);
static real_T rt_hypotd_o(real_T u0, real_T u1);
static void qrFactor(const real_T A[4], const real_T S[4], const real_T Ns[4],
                     real_T b_S[4]);
static void trisolve(const real_T A[4], real_T B_0[4]);
static void trisolve_o(const real_T A[4], real_T B_2[4]);
static real_T xnrm2(int32_T n, const real_T x[8], int32_T ix0)
{
  real_T y;
  int32_T k;

  /* Start for MATLABSystem: '<S7>/MATLAB System' incorporates:
   *  MATLABSystem: '<S9>/MATLAB System'
   */
  y = 0.0;
  if (n >= 1) {
    if (n == 1) {
      y = fabs(x[ix0 - 1]);
    } else {
      int32_T kend;
      rtDW.scale = 3.3121686421112381E-170;
      kend = ix0 + n;
      for (k = ix0; k < kend; k++) {
        rtDW.absxk = fabs(x[k - 1]);
        if (rtDW.absxk > rtDW.scale) {
          rtDW.t = rtDW.scale / rtDW.absxk;
          y = y * rtDW.t * rtDW.t + 1.0;
          rtDW.scale = rtDW.absxk;
        } else {
          rtDW.t = rtDW.absxk / rtDW.scale;
          y += rtDW.t * rtDW.t;
        }
      }

      y = rtDW.scale * sqrt(y);
    }
  }

  /* End of Start for MATLABSystem: '<S7>/MATLAB System' */
  return y;
}

static real_T rt_hypotd_o(real_T u0, real_T u1)
{
  real_T b;
  real_T y;
  rtDW.a = fabs(u0);
  b = fabs(u1);
  if (rtDW.a < b) {
    rtDW.a /= b;
    y = sqrt(rtDW.a * rtDW.a + 1.0) * b;
  } else if (rtDW.a > b) {
    b /= rtDW.a;
    y = sqrt(b * b + 1.0) * rtDW.a;
  } else {
    y = rtDW.a * 1.4142135623730951;
  }

  return y;
}

static void qrFactor(const real_T A[4], const real_T S[4], const real_T Ns[4],
                     real_T b_S[4])
{
  int32_T M_tmp;
  int32_T d;
  int32_T exitg1;
  int32_T i;
  int32_T iac;
  int32_T ii;
  int32_T jA;
  int32_T knt;
  int32_T lastc;
  int32_T lastv;
  boolean_T exitg2;

  /* Start for MATLABSystem: '<S7>/MATLAB System' incorporates:
   *  MATLABSystem: '<S9>/MATLAB System'
   */
  rtDW.b_atmp = S[1];
  rtDW.xnorm = S[0];
  rtDW.S = S[3];
  rtDW.S_g = S[2];
  for (i = 0; i < 2; i++) {
    rtDW.A_c = A[i + 2];
    rtDW.A_f = A[i];
    M_tmp = i << 2;
    rtDW.M[M_tmp] = rtDW.A_c * rtDW.b_atmp + rtDW.xnorm * rtDW.A_f;
    rtDW.M[M_tmp + 2] = Ns[i];
    rtDW.M[M_tmp + 1] = rtDW.A_c * rtDW.S + rtDW.S_g * rtDW.A_f;
    rtDW.M[M_tmp + 3] = Ns[i + 2];
    rtDW.b_tau[i] = 0.0;
    rtDW.work[i] = 0.0;
  }

  for (M_tmp = 0; M_tmp < 2; M_tmp++) {
    ii = (M_tmp << 2) + M_tmp;
    lastv = ii + 2;
    rtDW.b_atmp = rtDW.M[ii];
    rtDW.b_tau[M_tmp] = 0.0;
    rtDW.xnorm = xnrm2(3 - M_tmp, rtDW.M, ii + 2);
    if (rtDW.xnorm != 0.0) {
      rtDW.S = rtDW.M[ii];
      rtDW.xnorm = rt_hypotd_o(rtDW.S, rtDW.xnorm);
      if (rtDW.S >= 0.0) {
        rtDW.xnorm = -rtDW.xnorm;
      }

      if (fabs(rtDW.xnorm) < 1.0020841800044864E-292) {
        knt = -1;
        do {
          knt++;
          i = ii - M_tmp;
          for (lastc = lastv; lastc <= i + 4; lastc++) {
            rtDW.M[lastc - 1] *= 9.9792015476736E+291;
          }

          rtDW.xnorm *= 9.9792015476736E+291;
          rtDW.b_atmp *= 9.9792015476736E+291;
        } while ((fabs(rtDW.xnorm) < 1.0020841800044864E-292) && (knt + 1 < 20));

        rtDW.xnorm = rt_hypotd_o(rtDW.b_atmp, xnrm2(3 - M_tmp, rtDW.M, ii + 2));
        if (rtDW.b_atmp >= 0.0) {
          rtDW.xnorm = -rtDW.xnorm;
        }

        rtDW.b_tau[M_tmp] = (rtDW.xnorm - rtDW.b_atmp) / rtDW.xnorm;
        rtDW.b_atmp = 1.0 / (rtDW.b_atmp - rtDW.xnorm);
        for (lastc = lastv; lastc <= i + 4; lastc++) {
          rtDW.M[lastc - 1] *= rtDW.b_atmp;
        }

        for (lastc = 0; lastc <= knt; lastc++) {
          rtDW.xnorm *= 1.0020841800044864E-292;
        }

        rtDW.b_atmp = rtDW.xnorm;
      } else {
        rtDW.b_tau[M_tmp] = (rtDW.xnorm - rtDW.S) / rtDW.xnorm;
        rtDW.b_atmp = 1.0 / (rtDW.S - rtDW.xnorm);
        i = ii - M_tmp;
        for (lastc = lastv; lastc <= i + 4; lastc++) {
          rtDW.M[lastc - 1] *= rtDW.b_atmp;
        }

        rtDW.b_atmp = rtDW.xnorm;
      }
    }

    rtDW.M[ii] = rtDW.b_atmp;
    if (M_tmp + 1 < 2) {
      rtDW.M[ii] = 1.0;
      knt = ii + 5;
      if (rtDW.b_tau[M_tmp] != 0.0) {
        lastv = 4 - M_tmp;
        i = ii - M_tmp;
        while ((lastv > 0) && (rtDW.M[i + 3] == 0.0)) {
          lastv--;
          i--;
        }

        lastc = -M_tmp;
        exitg2 = false;
        while ((!exitg2) && (lastc + 1 > 0)) {
          i = (lastc << 2) + ii;
          jA = i + 5;
          do {
            exitg1 = 0;
            if (jA <= (i + lastv) + 4) {
              if (rtDW.M[jA - 1] != 0.0) {
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
        lastv = 0;
        lastc = -1;
      }

      if (lastv > 0) {
        if (lastc + 1 != 0) {
          if (lastc >= 0) {
            memset(&rtDW.work[0], 0, (uint32_T)(lastc + 1) * sizeof(real_T));
          }

          i = (lastc << 2) + ii;
          for (iac = knt; iac <= i + 5; iac += 4) {
            rtDW.xnorm = 0.0;
            d = iac + lastv;
            for (jA = iac; jA < d; jA++) {
              rtDW.xnorm += rtDW.M[(ii + jA) - iac] * rtDW.M[jA - 1];
            }

            jA = ((iac - ii) - 5) >> 2;
            rtDW.work[jA] += rtDW.xnorm;
          }
        }

        if (-rtDW.b_tau[M_tmp] != 0.0) {
          jA = ii;
          for (knt = 0; knt <= lastc; knt++) {
            rtDW.xnorm = rtDW.work[knt];
            if (rtDW.xnorm != 0.0) {
              rtDW.xnorm *= -rtDW.b_tau[M_tmp];
              i = jA + 5;
              iac = (lastv + jA) + 4;
              for (d = i; d <= iac; d++) {
                rtDW.M[d - 1] += rtDW.M[((ii + d) - jA) - 5] * rtDW.xnorm;
              }
            }

            jA += 4;
          }
        }
      }

      rtDW.M[ii] = rtDW.b_atmp;
    }
  }

  for (ii = 0; ii < 2; ii++) {
    for (M_tmp = 0; M_tmp <= ii; M_tmp++) {
      /* Start for MATLABSystem: '<S7>/MATLAB System' incorporates:
       *  MATLABSystem: '<S9>/MATLAB System'
       */
      rtDW.b_R[M_tmp + (ii << 1)] = rtDW.M[(ii << 2) + M_tmp];
    }

    if (ii <= 0) {
      rtDW.b_R[(ii << 1) + 1] = 0.0;
    }
  }

  /* Start for MATLABSystem: '<S7>/MATLAB System' incorporates:
   *  MATLABSystem: '<S9>/MATLAB System'
   */
  b_S[0] = rtDW.b_R[0];
  b_S[1] = rtDW.b_R[2];
  b_S[2] = rtDW.b_R[1];
  b_S[3] = rtDW.b_R[3];
}

static void trisolve(const real_T A[4], real_T B_0[4])
{
  int32_T b_j;
  int32_T b_k;
  int32_T i;

  /* Start for MATLABSystem: '<S7>/MATLAB System' */
  for (b_j = 0; b_j < 2; b_j++) {
    int32_T jBcol;
    jBcol = (b_j << 1) - 1;
    for (b_k = 0; b_k < 2; b_k++) {
      real_T B_1;
      int32_T B_tmp;
      int32_T k;
      int32_T kAcol;
      k = b_k + 1;
      kAcol = (b_k << 1) - 1;
      B_tmp = (b_k + jBcol) + 1;
      B_1 = B_0[B_tmp];
      if (B_1 != 0.0) {
        B_0[B_tmp] = B_1 / A[(b_k + kAcol) + 1];
        for (i = k + 1; i < 3; i++) {
          B_0[jBcol + 2] -= A[kAcol + 2] * B_0[B_tmp];
        }
      }
    }
  }

  /* End of Start for MATLABSystem: '<S7>/MATLAB System' */
}

static void trisolve_o(const real_T A[4], real_T B_2[4])
{
  int32_T b_i;
  int32_T b_j;
  int32_T k;

  /* Start for MATLABSystem: '<S7>/MATLAB System' */
  for (b_j = 0; b_j < 2; b_j++) {
    int32_T jBcol;
    jBcol = b_j << 1;
    for (k = 1; k >= 0; k--) {
      real_T tmp;
      int32_T kAcol;
      int32_T tmp_0;
      kAcol = k << 1;
      tmp_0 = k + jBcol;
      tmp = B_2[tmp_0];
      if (tmp != 0.0) {
        B_2[tmp_0] = tmp / A[k + kAcol];
        for (b_i = 0; b_i < k; b_i++) {
          B_2[jBcol] -= B_2[tmp_0] * A[kAcol];
        }
      }
    }
  }

  /* End of Start for MATLABSystem: '<S7>/MATLAB System' */
}

/* Model step function for TID0 */
void blinky_step0(void)                /* Sample time: [0.001s, 0.0s] */
{
  /* Update the flag to indicate when data transfers from
   *  Sample time: [0.001s, 0.0s] to Sample time: [0.005s, 0.0s]  */
  (rtM->Timing.RateInteraction.TID0_1)++;
  if ((rtM->Timing.RateInteraction.TID0_1) > 4) {
    rtM->Timing.RateInteraction.TID0_1 = 0;
  }

  /* UnitDelay: '<Root>/Unit Delay' */
  if (rtM->Timing.RateInteraction.TID0_1 == 1) {
    /* UnitDelay: '<Root>/Unit Delay' */
    rtDW.UnitDelay = rtDW.UnitDelay_DSTATE;

    /* UnitDelay: '<Root>/Unit Delay1' */
    rtDW.UnitDelay1 = rtDW.UnitDelay1_DSTATE;
  }

  /* End of UnitDelay: '<Root>/Unit Delay' */

  /* Outputs for Atomic SubSystem: '<Root>/task_1ms' */
  /* Outport: '<Root>/motor_a_pwm' incorporates:
   *  SignalConversion generated from: '<S1>/motor_a'
   */
  rtY.motor_a_pwm = rtDW.UnitDelay;

  /* Outport: '<Root>/motor_b_pwm' incorporates:
   *  SignalConversion generated from: '<S1>/motor_b'
   */
  rtY.motor_b_pwm = rtDW.UnitDelay1;

  /* End of Outputs for SubSystem: '<Root>/task_1ms' */
}

/* Model step function for TID1 */
void blinky_step1(void)                /* Sample time: [0.005s, 0.0s] */
{
  int32_T A_tmp;
  int32_T b_i;
  int32_T i;
  int16_T rtb_Switch;

  /* Outputs for Atomic SubSystem: '<Root>/task_5ms' */
  /* Outputs for Enabled SubSystem: '<S6>/Correct1' incorporates:
   *  EnablePort: '<S7>/Enable'
   */
  /* MATLABSystem: '<S7>/MATLAB System' incorporates:
   *  Constant: '<S6>/R1'
   *  DataStoreRead: '<S7>/Data Store ReadP'
   *  DataStoreRead: '<S7>/Data Store ReadX'
   */
  /* x 就是一开始状态转移函数定义的状态向量 */
  /* x = [picth;roll]; */
  /*  u = [pitch_acc;roll_acc] */
  rtDW.epsilon = fmax(1.4901161193847656E-8, 1.4901161193847656E-8 * fabs
                      (rtDW.x[0]));

  /* x 就是一开始状态转移函数定义的状态向量 */
  /* x = [picth;roll]; */
  /*  u = [pitch_acc;roll_acc] */
  rtDW.b_dHdx[0] = ((rtDW.x[0] + rtDW.epsilon) - rtDW.x[0]) / rtDW.epsilon;
  rtDW.b_dHdx[1] = 0.0 / rtDW.epsilon;
  rtDW.epsilon = fmax(1.4901161193847656E-8, 1.4901161193847656E-8 * fabs
                      (rtDW.x[1]));

  /* x 就是一开始状态转移函数定义的状态向量 */
  /* x = [picth;roll]; */
  /*  u = [pitch_acc;roll_acc] */
  rtDW.b_dHdx[2] = 0.0 / rtDW.epsilon;
  rtDW.b_dHdx[3] = ((rtDW.x[1] + rtDW.epsilon) - rtDW.x[1]) / rtDW.epsilon;

  /* x 就是一开始状态转移函数定义的状态向量 */
  /* x = [picth;roll]; */
  /*  u = [pitch_acc;roll_acc] */
  qrFactor(rtDW.b_dHdx, rtDW.P_i, rtConstP.R1_Value, rtDW.Sy);
  for (i = 0; i < 2; i++) {
    /* Start for MATLABSystem: '<S7>/MATLAB System' incorporates:
     *  DataStoreRead: '<S7>/Data Store ReadP'
     */
    rtDW.epsilon = rtDW.P_i[i];
    rtDW.Gain1 = rtDW.P_i[0] * rtDW.epsilon;
    rtDW.K_idx_1 = rtDW.P_i[1] * rtDW.epsilon;
    b_i = i << 1;
    rtDW.A[b_i] = 0.0;
    rtDW.epsilon = rtDW.P_i[i + 2];
    rtDW.K[b_i] = rtDW.P_i[2] * rtDW.epsilon + rtDW.Gain1;
    rtDW.K[b_i + 1] = rtDW.P_i[3] * rtDW.epsilon + rtDW.K_idx_1;
    rtDW.A[b_i + 1] = 0.0;
  }

  /* Start for MATLABSystem: '<S7>/MATLAB System' */
  rtDW.epsilon = rtDW.K[0];
  rtDW.Gain1 = rtDW.K[1];
  rtDW.K_idx_1 = rtDW.K[2];
  rtDW.z_idx_0 = rtDW.K[3];
  for (i = 0; i < 2; i++) {
    rtDW.K_idx_3 = rtDW.b_dHdx[i];
    b_i = i << 1;
    rtDW.z_idx_1 = rtDW.epsilon * rtDW.K_idx_3 + rtDW.A[b_i];
    rtDW.rtb_DataTypeConversion2_idx_1 = rtDW.A[b_i + 1] + rtDW.Gain1 *
      rtDW.K_idx_3;
    rtDW.K_idx_3 = rtDW.b_dHdx[i + 2];
    rtDW.A[b_i] = rtDW.K_idx_1 * rtDW.K_idx_3 + rtDW.z_idx_1;
    rtDW.A[b_i + 1] = rtDW.z_idx_0 * rtDW.K_idx_3 +
      rtDW.rtb_DataTypeConversion2_idx_1;
  }

  /* MATLABSystem: '<S7>/MATLAB System' */
  rtDW.K[0] = rtDW.A[0];
  rtDW.K[1] = rtDW.A[2];
  rtDW.K[2] = rtDW.A[1];
  rtDW.K[3] = rtDW.A[3];

  /* Start for MATLABSystem: '<S7>/MATLAB System' */
  trisolve(rtDW.Sy, rtDW.K);

  /* MATLABSystem: '<S7>/MATLAB System' */
  rtDW.A[0] = rtDW.Sy[0];
  rtDW.A[1] = rtDW.Sy[2];
  rtDW.A[2] = rtDW.Sy[1];
  rtDW.A[3] = rtDW.Sy[3];
  rtDW.Sy[0] = rtDW.K[0];
  rtDW.Sy[1] = rtDW.K[1];
  rtDW.Sy[2] = rtDW.K[2];
  rtDW.Sy[3] = rtDW.K[3];

  /* Start for MATLABSystem: '<S7>/MATLAB System' */
  trisolve_o(rtDW.A, rtDW.Sy);

  /* MATLABSystem: '<S7>/MATLAB System' */
  rtDW.K[0] = rtDW.Sy[0];
  rtDW.K[1] = rtDW.Sy[2];
  rtDW.K[2] = rtDW.Sy[1];
  rtDW.K[3] = rtDW.Sy[3];

  /* Start for MATLABSystem: '<S7>/MATLAB System' incorporates:
   *  DataStoreRead: '<S7>/Data Store ReadX'
   *  DataTypeConversion: '<S2>/Data Type Conversion2'
   *  Inport: '<Root>/accx'
   *  Inport: '<Root>/accy'
   *  Inport: '<Root>/accz'
   *  MATLAB Function: '<S2>/MATLAB Function3'
   * */
  rtDW.Gain1 = -rtDW.Sy[0];
  rtDW.K_idx_1 = -rtDW.Sy[2];
  rtDW.z_idx_0 = -rtDW.Sy[1];
  rtDW.K_idx_3 = -rtDW.Sy[3];
  rtDW.z_idx_1 = atan2f(-rtU.accx, sqrtf(rtU.accy * rtU.accy + rtU.accz *
    rtU.accz)) - rtDW.x[0];
  rtDW.rtb_DataTypeConversion2_idx_1 = atan2f(rtU.accy, rtU.accz) - rtDW.x[1];
  for (b_i = 0; b_i < 2; b_i++) {
    /* MATLABSystem: '<S7>/MATLAB System' incorporates:
     *  Constant: '<S6>/R1'
     */
    i = b_i << 1;
    rtDW.epsilon = rtDW.b_dHdx[i];
    rtDW.A_b = rtDW.Gain1 * rtDW.epsilon;
    rtDW.A_p = rtDW.K_idx_1 * rtDW.epsilon;
    rtDW.epsilon = rtDW.b_dHdx[i + 1];
    rtDW.A[i] = rtDW.z_idx_0 * rtDW.epsilon + rtDW.A_b;
    rtDW.A[i + 1] = rtDW.K_idx_3 * rtDW.epsilon + rtDW.A_p;
    A_tmp = i + b_i;
    rtDW.A[A_tmp]++;

    /* Start for MATLABSystem: '<S7>/MATLAB System' incorporates:
     *  Constant: '<S6>/R1'
     */
    rtDW.epsilon = rtConstP.R1_Value[i];
    rtDW.A_b = rtDW.K[0] * rtDW.epsilon;
    rtDW.A_p = rtDW.K[1] * rtDW.epsilon;
    rtDW.epsilon = rtConstP.R1_Value[i + 1];
    rtDW.Sy[i] = rtDW.K[2] * rtDW.epsilon + rtDW.A_b;
    rtDW.Sy[i + 1] = rtDW.K[3] * rtDW.epsilon + rtDW.A_p;

    /* DataStoreWrite: '<S7>/Data Store WriteX' incorporates:
     *  DataStoreRead: '<S7>/Data Store ReadX'
     *  MATLABSystem: '<S7>/MATLAB System'
     * */
    rtDW.x[b_i] += rtDW.K[b_i + 2] * rtDW.rtb_DataTypeConversion2_idx_1 +
      rtDW.K[b_i] * rtDW.z_idx_1;
  }

  /* End of Outputs for SubSystem: '<S6>/Correct1' */
  /* End of Outputs for SubSystem: '<Root>/task_5ms' */
  for (i = 0; i < 4; i++) {
    /* Outputs for Atomic SubSystem: '<Root>/task_5ms' */
    /* Outputs for Enabled SubSystem: '<S6>/Correct1' incorporates:
     *  EnablePort: '<S7>/Enable'
     */
    /* DataStoreRead: '<S7>/Data Store ReadP' */
    rtDW.K[i] = rtDW.P_i[i];

    /* End of Outputs for SubSystem: '<S6>/Correct1' */
    /* End of Outputs for SubSystem: '<Root>/task_5ms' */
  }

  /* Outputs for Atomic SubSystem: '<Root>/task_5ms' */
  /* Outputs for Enabled SubSystem: '<S6>/Correct1' incorporates:
   *  EnablePort: '<S7>/Enable'
   */
  /* Start for MATLABSystem: '<S7>/MATLAB System' incorporates:
   *  DataStoreWrite: '<S7>/Data Store WriteP'
   */
  qrFactor(rtDW.A, rtDW.K, rtDW.Sy, rtDW.P_i);

  /* End of Outputs for SubSystem: '<S6>/Correct1' */

  /* Outport: '<Root>/roll' incorporates:
   *  DataStoreRead: '<S8>/Data Store Read'
   *  Gain: '<S2>/Gain'
   */
  rtY.roll = -57.295779513082323 * rtDW.x[1];

  /* Gain: '<S2>/Gain1' incorporates:
   *  DataStoreRead: '<S8>/Data Store Read'
   */
  rtDW.Gain1 = -57.295779513082323 * rtDW.x[0];

  /* Switch: '<S4>/Switch' incorporates:
   *  Constant: '<S4>/BALANCE_KD'
   *  Constant: '<S4>/BALANCE_KP'
   *  Constant: '<S4>/BALANCE_OFFSET'
   *  Constant: '<S4>/Constant'
   *  Inport: '<Root>/gyro'
   *  Inport: '<Root>/running_flag'
   *  Product: '<S4>/Product'
   *  Product: '<S4>/Product1'
   *  Sum: '<S4>/Add'
   *  Sum: '<S4>/Sum'
   */
  if (rtU.running_flag > 0) {
    rtb_Switch = (int16_T)floor((rtDW.Gain1 - BALANCE_OFFSET) * BALANCE_KP +
      rtU.gyro[1] * BALANCE_KD);
  } else {
    rtb_Switch = 0;
  }

  /* End of Switch: '<S4>/Switch' */

  /* Saturate: '<S4>/Saturation' */
  if (rtb_Switch > ((int16_T)PWM_MAX)) {
    rtb_Switch = ((int16_T)PWM_MAX);
  } else if (rtb_Switch < ((int16_T)PWM_MIN)) {
    rtb_Switch = ((int16_T)PWM_MIN);
  }

  /* End of Saturate: '<S4>/Saturation' */

  /* Outputs for Atomic SubSystem: '<S6>/Predict' */
  /* Start for MATLABSystem: '<S9>/MATLAB System' incorporates:
   *  Inport: '<Root>/gyro'
   */
  /*  x = [pitch; roll]  (2×1) */
  /*  u = [gx; gy]       (2×1) */
  rtDW.K_idx_1 = rtU.gyro[1] * 0.005;

  /* MATLABSystem: '<S9>/MATLAB System' incorporates:
   *  DataStoreRead: '<S9>/Data Store ReadX'
   */
  rtDW.z_idx_0 = rtDW.K_idx_1 + rtDW.x[0];

  /* Start for MATLABSystem: '<S9>/MATLAB System' incorporates:
   *  Inport: '<Root>/gyro'
   */
  rtDW.K_idx_3 = rtU.gyro[0] * 0.005;

  /* MATLABSystem: '<S9>/MATLAB System' incorporates:
   *  DataStoreRead: '<S9>/Data Store ReadX'
   */
  rtDW.z_idx_1 = rtDW.K_idx_3 + rtDW.x[1];

  /*  2×1 */
  rtDW.epsilon = fmax(1.4901161193847656E-8, 1.4901161193847656E-8 * fabs
                      (rtDW.x[0]));

  /*  x = [pitch; roll]  (2×1) */
  /*  u = [gx; gy]       (2×1) */
  /*  2×1 */
  rtDW.b_dHdx[0] = (((rtDW.x[0] + rtDW.epsilon) + rtDW.K_idx_1) - rtDW.z_idx_0) /
    rtDW.epsilon;
  rtDW.b_dHdx[1] = (rtDW.z_idx_1 - rtDW.z_idx_1) / rtDW.epsilon;
  rtDW.epsilon = fmax(1.4901161193847656E-8, 1.4901161193847656E-8 * fabs
                      (rtDW.x[1]));

  /*  x = [pitch; roll]  (2×1) */
  /*  u = [gx; gy]       (2×1) */
  /*  2×1 */
  rtDW.b_dHdx[2] = (rtDW.z_idx_0 - rtDW.z_idx_0) / rtDW.epsilon;
  rtDW.b_dHdx[3] = (((rtDW.x[1] + rtDW.epsilon) + rtDW.K_idx_3) - rtDW.z_idx_1) /
    rtDW.epsilon;

  /* End of Outputs for SubSystem: '<S6>/Predict' */
  /* End of Outputs for SubSystem: '<Root>/task_5ms' */
  /*  x = [pitch; roll]  (2×1) */
  /*  u = [gx; gy]       (2×1) */
  /*  2×1 */
  for (i = 0; i < 4; i++) {
    /* Outputs for Atomic SubSystem: '<Root>/task_5ms' */
    /* Outputs for Atomic SubSystem: '<S6>/Predict' */
    /* DataStoreRead: '<S9>/Data Store ReadP' */
    rtDW.K[i] = rtDW.P_i[i];

    /* End of Outputs for SubSystem: '<S6>/Predict' */
    /* End of Outputs for SubSystem: '<Root>/task_5ms' */
  }

  /* Outputs for Atomic SubSystem: '<Root>/task_5ms' */
  /* Outputs for Atomic SubSystem: '<S6>/Predict' */
  /* Start for MATLABSystem: '<S9>/MATLAB System' incorporates:
   *  Constant: '<S6>/Q'
   *  DataStoreWrite: '<S9>/Data Store WriteP'
   */
  qrFactor(rtDW.b_dHdx, rtDW.K, rtConstP.Q_Value, rtDW.P_i);

  /* DataStoreWrite: '<S9>/Data Store WriteX' incorporates:
   *  MATLABSystem: '<S9>/MATLAB System'
   * */
  rtDW.x[0] = rtDW.z_idx_0;
  rtDW.x[1] = rtDW.z_idx_1;

  /* End of Outputs for SubSystem: '<S6>/Predict' */
  /* End of Outputs for SubSystem: '<Root>/task_5ms' */

  /* Outport: '<Root>/pitch' */
  rtY.pitch = rtDW.Gain1;

  /* Update for UnitDelay: '<Root>/Unit Delay' incorporates:
   *  Sum: '<Root>/motor_a'
   */
  rtDW.UnitDelay_DSTATE = rtb_Switch;

  /* Update for UnitDelay: '<Root>/Unit Delay1' incorporates:
   *  Sum: '<Root>/motor_b'
   */
  rtDW.UnitDelay1_DSTATE = rtb_Switch;
}

/* Model initialize function */
void blinky_initialize(void)
{
  /* Registration code */

  /* Set task counter limit used by the static main program */
  (rtM)->Timing.TaskCounters.cLimit[0] = 1;
  (rtM)->Timing.TaskCounters.cLimit[1] = 5;

  /* SystemInitialize for Atomic SubSystem: '<Root>/task_5ms' */
  /* Start for DataStoreMemory: '<S6>/DataStoreMemory - P' */
  rtDW.P_i[0] = 1.0;
  rtDW.P_i[1] = 0.0;
  rtDW.P_i[2] = 0.0;
  rtDW.P_i[3] = 1.0;

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
