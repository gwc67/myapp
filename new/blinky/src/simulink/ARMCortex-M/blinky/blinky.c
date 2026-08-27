/*
 * File: blinky.c
 *
 * Code generated for Simulink model 'blinky'.
 *
 * Model version                  : 1.76
 * Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
 * C/C++ source code generated on : Thu Aug 27 11:11:38 2026
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
                                        * Referenced by: '<S114>/Constant1'
                                        */
real_T BALANCE_KP = 35.0;              /* Variable: BALANCE_KP
                                        * Referenced by:
                                        *   '<S114>/Constant3'
                                        *   '<S114>/bal_kp'
                                        */
real_T SPD_KD = 0.0;                   /* Variable: SPD_KD
                                        * Referenced by: '<S34>/Derivative Gain'
                                        */
real_T SPD_KI = 0.0012;                /* Variable: SPD_KI
                                        * Referenced by: '<S38>/Integral Gain'
                                        */
real_T SPD_KP = 0.465;                 /* Variable: SPD_KP
                                        * Referenced by: '<S46>/Proportional Gain'
                                        */
real32_T TURN_KD = 1.0F;               /* Variable: TURN_KD
                                        * Referenced by: '<S87>/Derivative Gain'
                                        */
real32_T TURN_KP = 15.0F;              /* Variable: TURN_KP
                                        * Referenced by: '<S99>/Proportional Gain'
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
static real_T trimf(real_T x, const real_T params[3]);
static void trimf_b(const real_T x[101], const real_T params[3], real_T y[101]);
static real_T xnrm2(int32_T n, const real_T x[8], int32_T ix0)
{
  real_T y;
  int32_T k;

  /* Start for MATLABSystem: '<S123>/MATLAB System' incorporates:
   *  MATLABSystem: '<S125>/MATLAB System'
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

  /* End of Start for MATLABSystem: '<S123>/MATLAB System' */
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

  /* Start for MATLABSystem: '<S123>/MATLAB System' incorporates:
   *  MATLABSystem: '<S125>/MATLAB System'
   */
  rtDW.b_atmp = S[1];
  rtDW.xnorm = S[0];
  rtDW.S = S[3];
  rtDW.S_f = S[2];
  for (i = 0; i < 2; i++) {
    rtDW.A_p = A[i + 2];
    rtDW.A_c = A[i];
    M_tmp = i << 2;
    rtDW.M[M_tmp] = rtDW.A_p * rtDW.b_atmp + rtDW.xnorm * rtDW.A_c;
    rtDW.M[M_tmp + 2] = Ns[i];
    rtDW.M[M_tmp + 1] = rtDW.A_p * rtDW.S + rtDW.S_f * rtDW.A_c;
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
      /* Start for MATLABSystem: '<S123>/MATLAB System' incorporates:
       *  MATLABSystem: '<S125>/MATLAB System'
       */
      rtDW.b_R[M_tmp + (ii << 1)] = rtDW.M[(ii << 2) + M_tmp];
    }

    if (ii <= 0) {
      rtDW.b_R[(ii << 1) + 1] = 0.0;
    }
  }

  /* Start for MATLABSystem: '<S123>/MATLAB System' incorporates:
   *  MATLABSystem: '<S125>/MATLAB System'
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

  /* Start for MATLABSystem: '<S123>/MATLAB System' */
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

  /* End of Start for MATLABSystem: '<S123>/MATLAB System' */
}

static void trisolve_o(const real_T A[4], real_T B_2[4])
{
  int32_T b_i;
  int32_T b_j;
  int32_T k;

  /* Start for MATLABSystem: '<S123>/MATLAB System' */
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

  /* End of Start for MATLABSystem: '<S123>/MATLAB System' */
}

/* Function for MATLAB Function: '<S118>/Evaluate Rule Antecedents' */
static real_T trimf(real_T x, const real_T params[3])
{
  real_T y;
  y = 0.0;
  if ((params[0] != params[1]) && (params[0] < x) && (x < params[1])) {
    y = 1.0 / (params[1] - params[0]) * (x - params[0]);
  }

  if ((params[1] != params[2]) && (params[1] < x) && (x < params[2])) {
    y = 1.0 / (params[2] - params[1]) * (params[2] - x);
  }

  if (x == params[1]) {
    y = 1.0;
  }

  return y;
}

/* Function for MATLAB Function: '<S118>/Evaluate Rule Consequents' */
static void trimf_b(const real_T x[101], const real_T params[3], real_T y[101])
{
  real_T a;
  real_T b;
  real_T c;
  int32_T i;
  a = params[0];
  b = params[1];
  c = params[2];
  for (i = 0; i < 101; i++) {
    real_T x_0;
    y[i] = 0.0;
    if (a != b) {
      x_0 = x[i];
      if ((a < x_0) && (x_0 < b)) {
        y[i] = 1.0 / (b - a) * (x_0 - a);
      }
    }

    if (b != c) {
      x_0 = x[i];
      if ((b < x_0) && (x_0 < c)) {
        y[i] = 1.0 / (c - b) * (c - x_0);
      }
    }

    if (x[i] == b) {
      y[i] = 1.0;
    }
  }
}

/* Model step function for TID0 */
void blinky_step0(void)                /* Sample time: [0.001s, 0.0s] */
{
  int16_T rtb_Switch1;
  uint8_T rtb_running_success_flag;

  /* Update the flag to indicate when data transfers from
   *  Sample time: [0.001s, 0.0s] to Sample time: [0.005s, 0.0s]  */
  (rtM->Timing.RateInteraction.TID0_1)++;
  if ((rtM->Timing.RateInteraction.TID0_1) > 4) {
    rtM->Timing.RateInteraction.TID0_1 = 0;
  }

  /* Outport: '<Root>/motor_a_pwm' incorporates:
   *  UnitDelay: '<Root>/Unit Delay'
   */
  rtY.motor_a_pwm = rtDW.UnitDelay_DSTATE;

  /* Outport: '<Root>/motor_b_pwm' incorporates:
   *  UnitDelay: '<Root>/Unit Delay1'
   */
  rtY.motor_b_pwm = rtDW.UnitDelay1_DSTATE;

  /* UnitDelay: '<Root>/Unit Delay3' */
  if (rtM->Timing.RateInteraction.TID0_1 == 1) {
    /* UnitDelay: '<Root>/Unit Delay3' */
    rtDW.UnitDelay3 = rtDW.UnitDelay3_DSTATE;
  }

  /* End of UnitDelay: '<Root>/Unit Delay3' */

  /* Chart: '<Root>/output_flag' incorporates:
   *  Inport: '<Root>/running_flag'
   */
  rtb_running_success_flag = (uint8_T)((rtU.running_flag > 0) &&
    (rtDW.UnitDelay3 > pitch_min) && (rtDW.UnitDelay3 < pitch_max));

  /* Outport: '<Root>/running_success_flag' */
  rtY.running_success_flag = rtb_running_success_flag;

  /* UnitDelay: '<Root>/Unit Delay5' */
  rtDW.UnitDelay5 = rtDW.UnitDelay5_DSTATE;

  /* UnitDelay: '<Root>/Unit Delay6' */
  rtDW.UnitDelay6 = rtDW.UnitDelay6_DSTATE;

  /* Outputs for Atomic SubSystem: '<Root>/task_5ms' */
  /* DiscreteIntegrator: '<S114>/Discrete-Time Integrator' incorporates:
   *  Logic: '<S114>/NOT'
   *  UnitDelay: '<Root>/Unit Delay4'
   */
  if (rtDW.UnitDelay4_DSTATE == 0) {
    rtDW.DiscreteTimeIntegrator_DSTATE = 0.0F;

    /* Switch: '<S114>/Switch1' incorporates:
     *  Constant: '<S114>/Constant4'
     */
    rtb_Switch1 = 0;
  } else {
    /* Switch: '<S114>/Switch1' incorporates:
     *  DataTypeConversion: '<S114>/Data Type Conversion7'
     *  Product: '<S114>/Product2'
     *  Sum: '<S114>/Add'
     */
    rtb_Switch1 = (int16_T)floor((rtDW.DiscreteTimeIntegrator_DSTATE * rtDW.Add2
      + rtDW.Product1) + rtDW.Product);
  }

  /* End of DiscreteIntegrator: '<S114>/Discrete-Time Integrator' */

  /* Saturate: '<S114>/Saturation1' */
  if (rtb_Switch1 > ((int16_T)PWM_MAX)) {
    rtb_Switch1 = ((int16_T)PWM_MAX);
  } else if (rtb_Switch1 < ((int16_T)PWM_MIN)) {
    rtb_Switch1 = ((int16_T)PWM_MIN);
  }

  /* End of Saturate: '<S114>/Saturation1' */

  /* Update for DiscreteIntegrator: '<S114>/Discrete-Time Integrator' */
  rtDW.DiscreteTimeIntegrator_DSTATE += 0.001F * rtDW.DataTypeConversion4;
  if (rtDW.DiscreteTimeIntegrator_DSTATE > 200.0F) {
    rtDW.DiscreteTimeIntegrator_DSTATE = 200.0F;
  } else if (rtDW.DiscreteTimeIntegrator_DSTATE < -200.0F) {
    rtDW.DiscreteTimeIntegrator_DSTATE = -200.0F;
  }

  /* End of Update for DiscreteIntegrator: '<S114>/Discrete-Time Integrator' */
  /* End of Outputs for SubSystem: '<Root>/task_5ms' */

  /* Update for UnitDelay: '<Root>/Unit Delay' incorporates:
   *  Sum: '<Root>/motor_a'
   */
  rtDW.UnitDelay_DSTATE = (int16_T)(rtb_Switch1 - rtDW.DataTypeConversion);

  /* Update for UnitDelay: '<Root>/Unit Delay1' incorporates:
   *  Sum: '<Root>/motor_b'
   */
  rtDW.UnitDelay1_DSTATE = (int16_T)(rtb_Switch1 + rtDW.DataTypeConversion);

  /* Update for UnitDelay: '<Root>/Unit Delay4' */
  rtDW.UnitDelay4_DSTATE = rtb_running_success_flag;

  /* Update for UnitDelay: '<Root>/Unit Delay5' */
  rtDW.UnitDelay5_DSTATE = rtb_running_success_flag;

  /* Update for UnitDelay: '<Root>/Unit Delay6' */
  rtDW.UnitDelay6_DSTATE = rtb_running_success_flag;
}

/* Model step function for TID1 */
void blinky_step1(void)                /* Sample time: [0.005s, 0.0s] */
{
  int32_T A_tmp;
  int32_T b_i;
  int32_T i;
  int32_T rtb_DataTypeConversion2_idx_0_t;
  static const real_T h[3] = { -1.713, -0.5713, 0.5715 };

  static const real_T g[3] = { 0.5715, 1.713, 2.857 };

  static const real_T f[3] = { -4.0, -2.857, -1.713 };

  static const real_T e[3] = { -2.857, -1.713, -0.5713 };

  static const real_T d[3] = { -0.5713, 0.5715, 1.713 };

  static const real_T c[3] = { 1.713, 2.857, 4.0 };

  static const int8_T b[98] = { 1, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 4, 4, 4, 5, 5,
    5, 5, 5, 5, 5, 2, 2, 2, 2, 2, 2, 2, 6, 6, 6, 6, 6, 6, 6, 3, 3, 3, 3, 3, 3, 3,
    7, 7, 7, 7, 7, 7, 7, 1, 4, 5, 2, 6, 3, 7, 1, 4, 5, 2, 6, 3, 7, 1, 4, 5, 2, 6,
    3, 7, 1, 4, 5, 2, 6, 3, 7, 1, 4, 5, 2, 6, 3, 7, 1, 4, 5, 2, 6, 3, 7, 1, 4, 5,
    2, 6, 3, 3 };

  static const int8_T b_0[147] = { 7, 7, 3, 3, 6, 2, 2, 7, 7, 3, 6, 6, 2, 5, 3,
    3, 3, 6, 2, 5, 5, 3, 3, 6, 2, 5, 1, 1, 6, 6, 2, 5, 5, 1, 1, 6, 2, 5, 1, 1, 1,
    4, 2, 2, 1, 1, 1, 4, 4, 1, 1, 4, 4, 5, 2, 2, 1, 1, 4, 5, 5, 2, 2, 1, 4, 5, 5,
    2, 6, 6, 4, 4, 5, 2, 6, 3, 3, 4, 5, 2, 6, 6, 3, 7, 2, 2, 6, 6, 3, 7, 7, 2, 2,
    6, 3, 3, 7, 7, 6, 5, 1, 1, 1, 4, 6, 6, 5, 1, 4, 4, 5, 2, 2, 5, 4, 4, 5, 5, 2,
    2, 5, 5, 5, 5, 5, 2, 2, 2, 2, 2, 2, 2, 2, 7, 5, 6, 6, 6, 6, 7, 7, 3, 3, 3, 6,
    6, 7 };

  /* Outputs for Atomic SubSystem: '<Root>/task_5ms' */
  /* Outputs for Enabled SubSystem: '<S122>/Correct1' incorporates:
   *  EnablePort: '<S123>/Enable'
   */
  /* MATLABSystem: '<S123>/MATLAB System' incorporates:
   *  Constant: '<S122>/R1'
   *  DataStoreRead: '<S123>/Data Store ReadP'
   *  DataStoreRead: '<S123>/Data Store ReadX'
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
    /* Start for MATLABSystem: '<S123>/MATLAB System' incorporates:
     *  DataStoreRead: '<S123>/Data Store ReadP'
     */
    rtDW.epsilon = rtDW.P_i[i];
    rtDW.rtb_DataTypeConversion2_idx_1 = rtDW.P_i[0] * rtDW.epsilon;
    rtDW.area = rtDW.P_i[1] * rtDW.epsilon;
    b_i = i << 1;
    rtDW.A[b_i] = 0.0;
    rtDW.epsilon = rtDW.P_i[i + 2];
    rtDW.K[b_i] = rtDW.P_i[2] * rtDW.epsilon +
      rtDW.rtb_DataTypeConversion2_idx_1;
    rtDW.K[b_i + 1] = rtDW.P_i[3] * rtDW.epsilon + rtDW.area;
    rtDW.A[b_i + 1] = 0.0;
  }

  /* Start for MATLABSystem: '<S123>/MATLAB System' */
  rtDW.epsilon = rtDW.K[0];
  rtDW.rtb_DataTypeConversion2_idx_1 = rtDW.K[1];
  rtDW.area = rtDW.K[2];
  rtDW.Gain1 = rtDW.K[3];
  for (i = 0; i < 2; i++) {
    rtDW.TSamp = rtDW.b_dHdx[i];
    b_i = i << 1;
    rtDW.TSamp_f = rtDW.epsilon * rtDW.TSamp + rtDW.A[b_i];
    rtDW.K_idx_3 = rtDW.A[b_i + 1] + rtDW.rtb_DataTypeConversion2_idx_1 *
      rtDW.TSamp;
    rtDW.TSamp = rtDW.b_dHdx[i + 2];
    rtDW.A[b_i] = rtDW.area * rtDW.TSamp + rtDW.TSamp_f;
    rtDW.A[b_i + 1] = rtDW.Gain1 * rtDW.TSamp + rtDW.K_idx_3;
  }

  /* MATLABSystem: '<S123>/MATLAB System' */
  rtDW.K[0] = rtDW.A[0];
  rtDW.K[1] = rtDW.A[2];
  rtDW.K[2] = rtDW.A[1];
  rtDW.K[3] = rtDW.A[3];

  /* Start for MATLABSystem: '<S123>/MATLAB System' */
  trisolve(rtDW.Sy, rtDW.K);

  /* MATLABSystem: '<S123>/MATLAB System' */
  rtDW.A[0] = rtDW.Sy[0];
  rtDW.A[1] = rtDW.Sy[2];
  rtDW.A[2] = rtDW.Sy[1];
  rtDW.A[3] = rtDW.Sy[3];
  rtDW.Sy[0] = rtDW.K[0];
  rtDW.Sy[1] = rtDW.K[1];
  rtDW.Sy[2] = rtDW.K[2];
  rtDW.Sy[3] = rtDW.K[3];

  /* Start for MATLABSystem: '<S123>/MATLAB System' */
  trisolve_o(rtDW.A, rtDW.Sy);

  /* MATLABSystem: '<S123>/MATLAB System' */
  rtDW.K[0] = rtDW.Sy[0];
  rtDW.K[1] = rtDW.Sy[2];
  rtDW.K[2] = rtDW.Sy[1];
  rtDW.K[3] = rtDW.Sy[3];

  /* Start for MATLABSystem: '<S123>/MATLAB System' incorporates:
   *  DataStoreRead: '<S123>/Data Store ReadX'
   *  DataTypeConversion: '<S5>/Data Type Conversion2'
   *  Inport: '<Root>/accx'
   *  Inport: '<Root>/accy'
   *  Inport: '<Root>/accz'
   *  MATLAB Function: '<S5>/MATLAB Function3'
   * */
  rtDW.Gain1 = -rtDW.Sy[0];
  rtDW.TSamp = -rtDW.Sy[2];
  rtDW.TSamp_f = -rtDW.Sy[1];
  rtDW.K_idx_3 = -rtDW.Sy[3];
  rtDW.area = atan2f(-rtU.accx, sqrtf(rtU.accy * rtU.accy + rtU.accz * rtU.accz))
    - rtDW.x[0];
  rtDW.rtb_DataTypeConversion2_idx_1 = atan2f(rtU.accy, rtU.accz) - rtDW.x[1];
  for (b_i = 0; b_i < 2; b_i++) {
    /* MATLABSystem: '<S123>/MATLAB System' incorporates:
     *  Constant: '<S122>/R1'
     */
    i = b_i << 1;
    rtDW.epsilon = rtDW.b_dHdx[i];
    rtDW.z_idx_1 = rtDW.Gain1 * rtDW.epsilon;
    rtDW.A_b = rtDW.TSamp * rtDW.epsilon;
    rtDW.epsilon = rtDW.b_dHdx[i + 1];
    rtDW.A[i] = rtDW.TSamp_f * rtDW.epsilon + rtDW.z_idx_1;
    rtDW.A[i + 1] = rtDW.K_idx_3 * rtDW.epsilon + rtDW.A_b;
    A_tmp = i + b_i;
    rtDW.A[A_tmp]++;

    /* Start for MATLABSystem: '<S123>/MATLAB System' incorporates:
     *  Constant: '<S122>/R1'
     */
    rtDW.epsilon = rtConstP.R1_Value[i];
    rtDW.z_idx_1 = rtDW.K[0] * rtDW.epsilon;
    rtDW.A_b = rtDW.K[1] * rtDW.epsilon;
    rtDW.epsilon = rtConstP.R1_Value[i + 1];
    rtDW.Sy[i] = rtDW.K[2] * rtDW.epsilon + rtDW.z_idx_1;
    rtDW.Sy[i + 1] = rtDW.K[3] * rtDW.epsilon + rtDW.A_b;

    /* DataStoreWrite: '<S123>/Data Store WriteX' incorporates:
     *  DataStoreRead: '<S123>/Data Store ReadX'
     *  MATLABSystem: '<S123>/MATLAB System'
     * */
    rtDW.x[b_i] += rtDW.K[b_i + 2] * rtDW.rtb_DataTypeConversion2_idx_1 +
      rtDW.K[b_i] * rtDW.area;
  }

  /* End of Outputs for SubSystem: '<S122>/Correct1' */
  /* End of Outputs for SubSystem: '<Root>/task_5ms' */
  for (i = 0; i < 4; i++) {
    /* Outputs for Atomic SubSystem: '<Root>/task_5ms' */
    /* Outputs for Enabled SubSystem: '<S122>/Correct1' incorporates:
     *  EnablePort: '<S123>/Enable'
     */
    /* DataStoreRead: '<S123>/Data Store ReadP' */
    rtDW.K[i] = rtDW.P_i[i];

    /* End of Outputs for SubSystem: '<S122>/Correct1' */
    /* End of Outputs for SubSystem: '<Root>/task_5ms' */
  }

  /* Outputs for Atomic SubSystem: '<Root>/task_5ms' */
  /* Outputs for Enabled SubSystem: '<S122>/Correct1' incorporates:
   *  EnablePort: '<S123>/Enable'
   */
  /* Start for MATLABSystem: '<S123>/MATLAB System' incorporates:
   *  DataStoreWrite: '<S123>/Data Store WriteP'
   */
  qrFactor(rtDW.A, rtDW.K, rtDW.Sy, rtDW.P_i);

  /* End of Outputs for SubSystem: '<S122>/Correct1' */

  /* Outport: '<Root>/roll' incorporates:
   *  DataStoreRead: '<S124>/Data Store Read'
   *  Gain: '<S5>/Gain'
   */
  rtY.roll = 57.295779513082323 * rtDW.x[1];

  /* Gain: '<S5>/Gain1' incorporates:
   *  DataStoreRead: '<S124>/Data Store Read'
   */
  rtDW.Gain1 = 57.295779513082323 * rtDW.x[0];

  /* Sum: '<S114>/Sum2' */
  rtDW.epsilon = rtDW.UnitDelay2 - rtDW.Gain1;

  /* SampleTimeMath: '<S117>/TSamp'
   *
   * About '<S117>/TSamp':
   *  y = u * K where K = 1 / ( w * Ts )
   *   */
  rtDW.TSamp = rtDW.epsilon * 200.0;

  /* Outputs for Atomic SubSystem: '<S114>/Fuzzy Logic  Controller' */
  /* SignalConversion generated from: '<S120>/ SFunction ' incorporates:
   *  Gain: '<S114>/gain'
   *  Gain: '<S114>/gain2'
   *  MATLAB Function: '<S118>/Evaluate Rule Antecedents'
   *  Sum: '<S117>/Diff'
   *  UnitDelay: '<S117>/UD'
   *
   * Block description for '<S117>/Diff':
   *
   *  Add in CPU
   *
   * Block description for '<S117>/UD':
   *
   *  Store in Global RAM
   */
  rtDW.area = e_factor * rtDW.epsilon;
  rtDW.rtb_DataTypeConversion2_idx_1 = (rtDW.TSamp - rtDW.UD_DSTATE) * ec_factor;

  /* MATLAB Function: '<S118>/Evaluate Rule Antecedents' */
  rtDW.TSamp_f = 0.0;
  rtDW.dv2[0] = -4.5;
  rtDW.dv2[1] = -3.5;
  rtDW.dv2[2] = -2.5;
  rtDW.inputMFCache[0] = trimf(rtDW.area, rtDW.dv2);
  rtDW.dv2[0] = -1.5;
  rtDW.dv2[1] = -0.5;
  rtDW.dv2[2] = 0.5;
  rtDW.inputMFCache[1] = trimf(rtDW.area, rtDW.dv2);
  rtDW.dv2[0] = 0.5;
  rtDW.dv2[1] = 1.5;
  rtDW.dv2[2] = 2.5;
  rtDW.inputMFCache[2] = trimf(rtDW.area, rtDW.dv2);
  rtDW.dv2[0] = -3.5;
  rtDW.dv2[1] = -2.5;
  rtDW.dv2[2] = -1.5;
  rtDW.inputMFCache[3] = trimf(rtDW.area, rtDW.dv2);
  rtDW.dv2[0] = -2.5;
  rtDW.dv2[1] = -1.5;
  rtDW.dv2[2] = -0.5;
  rtDW.inputMFCache[4] = trimf(rtDW.area, rtDW.dv2);
  rtDW.dv2[0] = -0.5;
  rtDW.dv2[1] = 0.5;
  rtDW.dv2[2] = 1.5;
  rtDW.inputMFCache[5] = trimf(rtDW.area, rtDW.dv2);
  rtDW.dv2[0] = 1.5;
  rtDW.dv2[1] = 2.5;
  rtDW.dv2[2] = 3.5;
  rtDW.inputMFCache[6] = trimf(rtDW.area, rtDW.dv2);
  rtDW.dv2[0] = -5.143;
  rtDW.dv2[1] = -4.0;
  rtDW.dv2[2] = -2.857;
  rtDW.inputMFCache[7] = trimf(rtDW.rtb_DataTypeConversion2_idx_1, rtDW.dv2);
  rtDW.inputMFCache[8] = trimf(rtDW.rtb_DataTypeConversion2_idx_1, h);
  rtDW.inputMFCache[9] = trimf(rtDW.rtb_DataTypeConversion2_idx_1, g);
  rtDW.inputMFCache[10] = trimf(rtDW.rtb_DataTypeConversion2_idx_1, f);
  rtDW.inputMFCache[11] = trimf(rtDW.rtb_DataTypeConversion2_idx_1, e);
  rtDW.inputMFCache[12] = trimf(rtDW.rtb_DataTypeConversion2_idx_1, d);
  rtDW.inputMFCache[13] = trimf(rtDW.rtb_DataTypeConversion2_idx_1, c);
  for (b_i = 0; b_i < 49; b_i++) {
    rtDW.rtb_DataTypeConversion2_idx_1 = rtDW.inputMFCache[b[b_i] - 1];
    if (rtDW.rtb_DataTypeConversion2_idx_1 < 1.0) {
      rtDW.area = rtDW.rtb_DataTypeConversion2_idx_1;
    } else {
      rtDW.area = 1.0;
    }

    rtDW.rtb_DataTypeConversion2_idx_1 = rtDW.inputMFCache[b[b_i + 49] + 6];
    if (rtDW.area > rtDW.rtb_DataTypeConversion2_idx_1) {
      rtDW.area = rtDW.rtb_DataTypeConversion2_idx_1;
    }

    rtDW.antecedentOutputs[b_i] = rtDW.area;
    rtDW.TSamp_f += rtDW.area;
  }

  /* MATLAB Function: '<S118>/Evaluate Rule Consequents' incorporates:
   *  Constant: '<S118>/Output Sample Points'
   */
  memset(&rtDW.aggregatedOutputs[0], 0, 303U * sizeof(real_T));
  for (i = 0; i < 101; i++) {
    rtDW.dv[i] = rtConstP.OutputSamplePoints_Value[3 * i];
  }

  rtDW.dv2[0] = 0.0;
  rtDW.dv2[1] = 1.0;
  rtDW.dv2[2] = 2.0;
  trimf_b(rtDW.dv, rtDW.dv2, rtDW.dv1);
  for (i = 0; i < 101; i++) {
    rtDW.outputMFCache[21 * i] = rtDW.dv1[i];
  }

  for (i = 0; i < 101; i++) {
    rtDW.dv[i] = rtConstP.OutputSamplePoints_Value[3 * i];
  }

  rtDW.dv2[0] = 2.0;
  rtDW.dv2[1] = 3.0;
  rtDW.dv2[2] = 4.0;
  trimf_b(rtDW.dv, rtDW.dv2, rtDW.dv1);
  for (i = 0; i < 101; i++) {
    rtDW.outputMFCache[21 * i + 1] = rtDW.dv1[i];
  }

  for (i = 0; i < 101; i++) {
    rtDW.dv[i] = rtConstP.OutputSamplePoints_Value[3 * i];
  }

  rtDW.dv2[0] = 4.0;
  rtDW.dv2[1] = 5.0;
  rtDW.dv2[2] = 6.0;
  trimf_b(rtDW.dv, rtDW.dv2, rtDW.dv1);
  for (i = 0; i < 101; i++) {
    rtDW.outputMFCache[21 * i + 2] = rtDW.dv1[i];
  }

  for (i = 0; i < 101; i++) {
    rtDW.dv[i] = rtConstP.OutputSamplePoints_Value[3 * i];
  }

  rtDW.dv2[0] = -1.0;
  rtDW.dv2[1] = 0.0;
  rtDW.dv2[2] = 1.0;
  trimf_b(rtDW.dv, rtDW.dv2, rtDW.dv1);
  for (i = 0; i < 101; i++) {
    rtDW.outputMFCache[21 * i + 3] = rtDW.dv1[i];
  }

  for (i = 0; i < 101; i++) {
    rtDW.dv[i] = rtConstP.OutputSamplePoints_Value[3 * i];
  }

  rtDW.dv2[0] = 1.0;
  rtDW.dv2[1] = 2.0;
  rtDW.dv2[2] = 3.0;
  trimf_b(rtDW.dv, rtDW.dv2, rtDW.dv1);
  for (i = 0; i < 101; i++) {
    rtDW.outputMFCache[21 * i + 4] = rtDW.dv1[i];
  }

  for (i = 0; i < 101; i++) {
    rtDW.dv[i] = rtConstP.OutputSamplePoints_Value[3 * i];
  }

  rtDW.dv2[0] = 3.0;
  rtDW.dv2[1] = 4.0;
  rtDW.dv2[2] = 5.0;
  trimf_b(rtDW.dv, rtDW.dv2, rtDW.dv1);
  for (i = 0; i < 101; i++) {
    rtDW.outputMFCache[21 * i + 5] = rtDW.dv1[i];
  }

  for (i = 0; i < 101; i++) {
    rtDW.dv[i] = rtConstP.OutputSamplePoints_Value[3 * i];
  }

  rtDW.dv2[0] = 5.0;
  rtDW.dv2[1] = 6.0;
  rtDW.dv2[2] = 7.0;
  trimf_b(rtDW.dv, rtDW.dv2, rtDW.dv1);
  for (i = 0; i < 101; i++) {
    rtDW.outputMFCache[21 * i + 6] = rtDW.dv1[i];
  }

  for (i = 0; i < 101; i++) {
    rtDW.dv[i] = rtConstP.OutputSamplePoints_Value[3 * i + 1];
  }

  rtDW.dv2[0] = -1.0;
  rtDW.dv2[1] = 0.0;
  rtDW.dv2[2] = 1.0;
  trimf_b(rtDW.dv, rtDW.dv2, rtDW.dv1);
  for (i = 0; i < 101; i++) {
    rtDW.outputMFCache[21 * i + 7] = rtDW.dv1[i];
  }

  for (i = 0; i < 101; i++) {
    rtDW.dv[i] = rtConstP.OutputSamplePoints_Value[3 * i + 1];
  }

  rtDW.dv2[0] = 2.0;
  rtDW.dv2[1] = 3.0;
  rtDW.dv2[2] = 4.0;
  trimf_b(rtDW.dv, rtDW.dv2, rtDW.dv1);
  for (i = 0; i < 101; i++) {
    rtDW.outputMFCache[21 * i + 8] = rtDW.dv1[i];
  }

  for (i = 0; i < 101; i++) {
    rtDW.dv[i] = rtConstP.OutputSamplePoints_Value[3 * i + 1];
  }

  rtDW.dv2[0] = 4.0;
  rtDW.dv2[1] = 5.0;
  rtDW.dv2[2] = 6.0;
  trimf_b(rtDW.dv, rtDW.dv2, rtDW.dv1);
  for (i = 0; i < 101; i++) {
    rtDW.outputMFCache[21 * i + 9] = rtDW.dv1[i];
  }

  for (i = 0; i < 101; i++) {
    rtDW.dv[i] = rtConstP.OutputSamplePoints_Value[3 * i + 1];
  }

  rtDW.dv2[0] = 0.0;
  rtDW.dv2[1] = 1.0;
  rtDW.dv2[2] = 2.0;
  trimf_b(rtDW.dv, rtDW.dv2, rtDW.dv1);
  for (i = 0; i < 101; i++) {
    rtDW.outputMFCache[21 * i + 10] = rtDW.dv1[i];
  }

  for (i = 0; i < 101; i++) {
    rtDW.dv[i] = rtConstP.OutputSamplePoints_Value[3 * i + 1];
  }

  rtDW.dv2[0] = 1.0;
  rtDW.dv2[1] = 2.0;
  rtDW.dv2[2] = 3.0;
  trimf_b(rtDW.dv, rtDW.dv2, rtDW.dv1);
  for (i = 0; i < 101; i++) {
    rtDW.outputMFCache[21 * i + 11] = rtDW.dv1[i];
  }

  for (i = 0; i < 101; i++) {
    rtDW.dv[i] = rtConstP.OutputSamplePoints_Value[3 * i + 1];
  }

  rtDW.dv2[0] = 3.0;
  rtDW.dv2[1] = 4.0;
  rtDW.dv2[2] = 5.0;
  trimf_b(rtDW.dv, rtDW.dv2, rtDW.dv1);
  for (i = 0; i < 101; i++) {
    rtDW.outputMFCache[21 * i + 12] = rtDW.dv1[i];
  }

  for (i = 0; i < 101; i++) {
    rtDW.dv[i] = rtConstP.OutputSamplePoints_Value[3 * i + 1];
  }

  rtDW.dv2[0] = 5.0;
  rtDW.dv2[1] = 6.0;
  rtDW.dv2[2] = 7.0;
  trimf_b(rtDW.dv, rtDW.dv2, rtDW.dv1);
  for (i = 0; i < 101; i++) {
    rtDW.outputMFCache[21 * i + 13] = rtDW.dv1[i];
  }

  for (i = 0; i < 101; i++) {
    rtDW.dv[i] = rtConstP.OutputSamplePoints_Value[3 * i + 2];
  }

  rtDW.dv2[0] = -1.0;
  rtDW.dv2[1] = 0.0;
  rtDW.dv2[2] = 1.0;
  trimf_b(rtDW.dv, rtDW.dv2, rtDW.dv1);
  for (i = 0; i < 101; i++) {
    rtDW.outputMFCache[21 * i + 14] = rtDW.dv1[i];
  }

  for (i = 0; i < 101; i++) {
    rtDW.dv[i] = rtConstP.OutputSamplePoints_Value[3 * i + 2];
  }

  rtDW.dv2[0] = 2.0;
  rtDW.dv2[1] = 3.0;
  rtDW.dv2[2] = 4.0;
  trimf_b(rtDW.dv, rtDW.dv2, rtDW.dv1);
  for (i = 0; i < 101; i++) {
    rtDW.outputMFCache[21 * i + 15] = rtDW.dv1[i];
  }

  for (i = 0; i < 101; i++) {
    rtDW.dv[i] = rtConstP.OutputSamplePoints_Value[3 * i + 2];
  }

  rtDW.dv2[0] = 4.0;
  rtDW.dv2[1] = 5.0;
  rtDW.dv2[2] = 6.0;
  trimf_b(rtDW.dv, rtDW.dv2, rtDW.dv1);
  for (i = 0; i < 101; i++) {
    rtDW.outputMFCache[21 * i + 16] = rtDW.dv1[i];
  }

  for (i = 0; i < 101; i++) {
    rtDW.dv[i] = rtConstP.OutputSamplePoints_Value[3 * i + 2];
  }

  rtDW.dv2[0] = 0.0;
  rtDW.dv2[1] = 1.0;
  rtDW.dv2[2] = 2.0;
  trimf_b(rtDW.dv, rtDW.dv2, rtDW.dv1);
  for (i = 0; i < 101; i++) {
    rtDW.outputMFCache[21 * i + 17] = rtDW.dv1[i];
  }

  for (i = 0; i < 101; i++) {
    rtDW.dv[i] = rtConstP.OutputSamplePoints_Value[3 * i + 2];
  }

  rtDW.dv2[0] = 1.0;
  rtDW.dv2[1] = 2.0;
  rtDW.dv2[2] = 3.0;
  trimf_b(rtDW.dv, rtDW.dv2, rtDW.dv1);
  for (i = 0; i < 101; i++) {
    rtDW.outputMFCache[21 * i + 18] = rtDW.dv1[i];
  }

  for (i = 0; i < 101; i++) {
    rtDW.dv[i] = rtConstP.OutputSamplePoints_Value[3 * i + 2];
  }

  rtDW.dv2[0] = 3.0;
  rtDW.dv2[1] = 4.0;
  rtDW.dv2[2] = 5.0;
  trimf_b(rtDW.dv, rtDW.dv2, rtDW.dv1);
  for (i = 0; i < 101; i++) {
    rtDW.outputMFCache[21 * i + 19] = rtDW.dv1[i];
  }

  for (i = 0; i < 101; i++) {
    rtDW.dv[i] = rtConstP.OutputSamplePoints_Value[3 * i + 2];
  }

  rtDW.dv2[0] = 5.0;
  rtDW.dv2[1] = 6.0;
  rtDW.dv2[2] = 7.0;
  trimf_b(rtDW.dv, rtDW.dv2, rtDW.dv1);
  for (i = 0; i < 101; i++) {
    rtDW.outputMFCache[21 * i + 20] = rtDW.dv1[i];
  }

  for (A_tmp = 0; A_tmp < 3; A_tmp++) {
    for (b_i = 0; b_i < 49; b_i++) {
      for (i = 0; i < 101; i++) {
        rtDW.area = rtDW.outputMFCache[((b_0[49 * A_tmp + b_i] + 7 * A_tmp) + 21
          * i) - 1];
        rtDW.rtb_DataTypeConversion2_idx_1 = rtDW.antecedentOutputs[b_i];
        rtb_DataTypeConversion2_idx_0_t = 101 * A_tmp + i;
        if (rtDW.area <= rtDW.rtb_DataTypeConversion2_idx_1) {
          rtDW.rtb_DataTypeConversion2_idx_1 = rtDW.area;
        }

        if (rtDW.aggregatedOutputs[rtb_DataTypeConversion2_idx_0_t] <
            rtDW.rtb_DataTypeConversion2_idx_1) {
          rtDW.aggregatedOutputs[rtb_DataTypeConversion2_idx_0_t] =
            rtDW.rtb_DataTypeConversion2_idx_1;
        }
      }
    }
  }

  /* End of MATLAB Function: '<S118>/Evaluate Rule Consequents' */

  /* MATLAB Function: '<S118>/Defuzzify Outputs' incorporates:
   *  Constant: '<S118>/Output Sample Points'
   *  MATLAB Function: '<S118>/Evaluate Rule Antecedents'
   */
  if (rtDW.TSamp_f == 0.0) {
    rtDW.defuzzifiedOutputs[0] = 3.5;
    rtDW.defuzzifiedOutputs[1] = 3.5;
    rtDW.defuzzifiedOutputs[2] = 3.5;
  } else {
    for (A_tmp = 0; A_tmp < 3; A_tmp++) {
      rtDW.rtb_DataTypeConversion2_idx_1 = 0.0;
      rtDW.area = 0.0;
      for (i = 0; i < 101; i++) {
        rtDW.area += rtDW.aggregatedOutputs[101 * A_tmp + i];
      }

      if (rtDW.area == 0.0) {
        rtDW.defuzzifiedOutputs[A_tmp] =
          (rtConstP.OutputSamplePoints_Value[A_tmp + 300] +
           rtConstP.OutputSamplePoints_Value[A_tmp]) / 2.0;
      } else {
        for (b_i = 0; b_i < 101; b_i++) {
          rtDW.rtb_DataTypeConversion2_idx_1 +=
            rtConstP.OutputSamplePoints_Value[3 * b_i + A_tmp] *
            rtDW.aggregatedOutputs[101 * A_tmp + b_i];
        }

        rtDW.defuzzifiedOutputs[A_tmp] = 1.0 / rtDW.area *
          rtDW.rtb_DataTypeConversion2_idx_1;
      }
    }
  }

  /* End of MATLAB Function: '<S118>/Defuzzify Outputs' */
  /* End of Outputs for SubSystem: '<S114>/Fuzzy Logic  Controller' */

  /* Sum: '<S114>/Add2' incorporates:
   *  Gain: '<S114>/Gain1'
   */
  rtDW.Add2 = bal_ki_factor * rtDW.defuzzifiedOutputs[1];

  /* DataTypeConversion: '<S114>/Data Type Conversion4' */
  rtDW.DataTypeConversion4 = (real32_T)rtDW.epsilon;

  /* SampleTimeMath: '<S116>/TSamp' incorporates:
   *  Gain: '<S114>/gain1'
   *
   * About '<S116>/TSamp':
   *  y = u * K where K = 1 / ( w * Ts )
   *   */
  rtDW.TSamp_f = 0.005 * rtDW.epsilon * 200.0;

  /* Product: '<S114>/Product' incorporates:
   *  Constant: '<S114>/Constant1'
   *  Gain: '<S114>/Gain2'
   *  Sum: '<S114>/Add1'
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
  rtDW.Product = (bal_kd_factor * rtDW.defuzzifiedOutputs[2] + BALANCE_KD) *
    (rtDW.TSamp_f - rtDW.UD_DSTATE_f);

  /* Product: '<S114>/Product1' incorporates:
   *  Constant: '<S114>/Constant3'
   *  Gain: '<S114>/Gain'
   *  Gain: '<S114>/bal_kp'
   *  Sum: '<S114>/Add3'
   */
  rtDW.Product1 = (bal_kp_factor * rtDW.defuzzifiedOutputs[0] + BALANCE_KP) *
    (BALANCE_KP * rtDW.epsilon);

  /* Outputs for Atomic SubSystem: '<S122>/Predict' */
  /* Start for MATLABSystem: '<S125>/MATLAB System' incorporates:
   *  DataTypeConversion: '<S5>/Data Type Conversion3'
   *  Inport: '<Root>/gyroy'
   */
  /*  x = [pitch; roll]  (2×1) */
  /*  u = [gx; gy]       (2×1) */
  rtDW.rtb_DataTypeConversion2_idx_1 = rtU.gyroy * 0.005;

  /* MATLABSystem: '<S125>/MATLAB System' incorporates:
   *  DataStoreRead: '<S125>/Data Store ReadX'
   */
  rtDW.area = rtDW.rtb_DataTypeConversion2_idx_1 + rtDW.x[0];

  /* Start for MATLABSystem: '<S125>/MATLAB System' incorporates:
   *  DataTypeConversion: '<S5>/Data Type Conversion3'
   *  Inport: '<Root>/gyrox'
   */
  rtDW.K_idx_3 = rtU.gyrox * 0.005;

  /* MATLABSystem: '<S125>/MATLAB System' incorporates:
   *  DataStoreRead: '<S125>/Data Store ReadX'
   */
  rtDW.z_idx_1 = rtDW.K_idx_3 + rtDW.x[1];

  /*  2×1 */
  rtDW.epsilon = fmax(1.4901161193847656E-8, 1.4901161193847656E-8 * fabs
                      (rtDW.x[0]));

  /*  x = [pitch; roll]  (2×1) */
  /*  u = [gx; gy]       (2×1) */
  /*  2×1 */
  rtDW.b_dHdx[0] = (((rtDW.x[0] + rtDW.epsilon) +
                     rtDW.rtb_DataTypeConversion2_idx_1) - rtDW.area) /
    rtDW.epsilon;
  rtDW.b_dHdx[1] = (rtDW.z_idx_1 - rtDW.z_idx_1) / rtDW.epsilon;
  rtDW.epsilon = fmax(1.4901161193847656E-8, 1.4901161193847656E-8 * fabs
                      (rtDW.x[1]));

  /*  x = [pitch; roll]  (2×1) */
  /*  u = [gx; gy]       (2×1) */
  /*  2×1 */
  rtDW.b_dHdx[2] = (rtDW.area - rtDW.area) / rtDW.epsilon;
  rtDW.b_dHdx[3] = (((rtDW.x[1] + rtDW.epsilon) + rtDW.K_idx_3) - rtDW.z_idx_1) /
    rtDW.epsilon;

  /* End of Outputs for SubSystem: '<S122>/Predict' */
  /* End of Outputs for SubSystem: '<Root>/task_5ms' */
  /*  x = [pitch; roll]  (2×1) */
  /*  u = [gx; gy]       (2×1) */
  /*  2×1 */
  for (i = 0; i < 4; i++) {
    /* Outputs for Atomic SubSystem: '<Root>/task_5ms' */
    /* Outputs for Atomic SubSystem: '<S122>/Predict' */
    /* DataStoreRead: '<S125>/Data Store ReadP' */
    rtDW.K[i] = rtDW.P_i[i];

    /* End of Outputs for SubSystem: '<S122>/Predict' */
    /* End of Outputs for SubSystem: '<Root>/task_5ms' */
  }

  /* Outputs for Atomic SubSystem: '<Root>/task_5ms' */
  /* Outputs for Atomic SubSystem: '<S122>/Predict' */
  /* Start for MATLABSystem: '<S125>/MATLAB System' incorporates:
   *  Constant: '<S122>/Q'
   *  DataStoreWrite: '<S125>/Data Store WriteP'
   */
  qrFactor(rtDW.b_dHdx, rtDW.K, rtConstP.Q_Value, rtDW.P_i);

  /* DataStoreWrite: '<S125>/Data Store WriteX' incorporates:
   *  MATLABSystem: '<S125>/MATLAB System'
   * */
  rtDW.x[0] = rtDW.area;
  rtDW.x[1] = rtDW.z_idx_1;

  /* End of Outputs for SubSystem: '<S122>/Predict' */

  /* Update for UnitDelay: '<S117>/UD'
   *
   * Block description for '<S117>/UD':
   *
   *  Store in Global RAM
   */
  rtDW.UD_DSTATE = rtDW.TSamp;

  /* Update for UnitDelay: '<S116>/UD'
   *
   * Block description for '<S116>/UD':
   *
   *  Store in Global RAM
   */
  rtDW.UD_DSTATE_f = rtDW.TSamp_f;

  /* End of Outputs for SubSystem: '<Root>/task_5ms' */

  /* Outport: '<Root>/pitch' */
  rtY.pitch = rtDW.Gain1;

  /* Update for UnitDelay: '<Root>/Unit Delay3' */
  rtDW.UnitDelay3_DSTATE = rtDW.Gain1;
}

/* Model step function for TID2 */
void blinky_step2(void)                /* Sample time: [0.01s, 0.0s] */
{
  real_T rtb_FilterCoefficient;
  real_T rtb_IntegralGain;
  real_T rtb_Sum;
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
  rtb_Sum = (rtU.motor_a_speed + rtU.motor_b_speed) / 2.0 - rtU.target_speed;

  /* Logic: '<S2>/NOT' */
  rtb_NOT_c = (rtDW.UnitDelay5 == 0);

  /* DiscreteIntegrator: '<S36>/Filter' */
  if (rtb_NOT_c || (rtDW.Filter_PrevResetState_a != 0)) {
    rtDW.Filter_DSTATE = 0.0;
  }

  /* Gain: '<S38>/Integral Gain' */
  rtb_IntegralGain = SPD_KI * rtb_Sum;

  /* DiscreteIntegrator: '<S41>/Integrator' */
  if (rtb_NOT_c || (rtDW.Integrator_PrevResetState != 0)) {
    rtDW.Integrator_DSTATE = 0.0;
  }

  /* Gain: '<S44>/Filter Coefficient' incorporates:
   *  DiscreteIntegrator: '<S36>/Filter'
   *  Gain: '<S34>/Derivative Gain'
   *  Sum: '<S36>/SumD'
   */
  rtb_FilterCoefficient = (SPD_KD * rtb_Sum - rtDW.Filter_DSTATE) * 100.0;

  /* Sum: '<S50>/Sum' incorporates:
   *  DiscreteIntegrator: '<S41>/Integrator'
   *  Gain: '<S46>/Proportional Gain'
   */
  rtb_Sum = (SPD_KP * rtb_Sum + rtDW.Integrator_DSTATE) + rtb_FilterCoefficient;

  /* Update for DiscreteIntegrator: '<S36>/Filter' */
  rtDW.Filter_DSTATE += 0.01 * rtb_FilterCoefficient;
  rtDW.Filter_PrevResetState_a = (int8_T)rtb_NOT_c;

  /* Update for DiscreteIntegrator: '<S41>/Integrator' incorporates:
   *  DiscreteIntegrator: '<S36>/Filter'
   */
  rtDW.Integrator_DSTATE += rtb_IntegralGain;
  if (rtDW.Integrator_DSTATE > ANGLE_INTERGRAL_MAX) {
    rtDW.Integrator_DSTATE = ANGLE_INTERGRAL_MAX;
  } else if (rtDW.Integrator_DSTATE < ANGLE_INTERGRAL_MIN) {
    rtDW.Integrator_DSTATE = ANGLE_INTERGRAL_MIN;
  }

  rtDW.Integrator_PrevResetState = (int8_T)rtb_NOT_c;

  /* End of Update for DiscreteIntegrator: '<S41>/Integrator' */

  /* Saturate: '<S48>/Saturation' */
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

  /* End of Saturate: '<S48>/Saturation' */
  /* End of Outputs for SubSystem: '<Root>/task_10ms' */
}

/* Model step function for TID3 */
void blinky_step3(void)                /* Sample time: [0.02s, 0.0s] */
{
  real32_T rtb_FilterCoefficient;
  real32_T u0;
  boolean_T rtb_NOT_n;

  /* Outputs for Atomic SubSystem: '<Root>/task_20ms' */
  /* Logic: '<S4>/NOT' */
  rtb_NOT_n = (rtDW.UnitDelay6 == 0);

  /* DiscreteIntegrator: '<S89>/Filter' */
  if (rtb_NOT_n || (rtDW.Filter_PrevResetState != 0)) {
    rtDW.Filter_DSTATE_j = 0.0F;
  }

  /* Gain: '<S97>/Filter Coefficient' incorporates:
   *  DiscreteIntegrator: '<S89>/Filter'
   */
  rtb_FilterCoefficient = 100.0F * rtDW.Filter_DSTATE_j;

  /* Sum: '<S103>/Sum' incorporates:
   *  Gain: '<S99>/Proportional Gain'
   *  Inport: '<Root>/turn_target'
   */
  u0 = TURN_KP * rtU.turn_target + rtb_FilterCoefficient;

  /* Saturate: '<S101>/Saturation' */
  if (u0 > 500.0F) {
    u0 = 500.0F;
  } else if (u0 < -500.0F) {
    u0 = -500.0F;
  }

  /* DataTypeConversion: '<S4>/Data Type Conversion' incorporates:
   *  Saturate: '<S101>/Saturation'
   */
  rtDW.DataTypeConversion = (int16_T)floorf(u0);

  /* Update for DiscreteIntegrator: '<S89>/Filter' incorporates:
   *  Gain: '<S87>/Derivative Gain'
   *  Inport: '<Root>/gyroz'
   *  Sum: '<S89>/SumD'
   */
  rtDW.Filter_DSTATE_j += (TURN_KD * rtU.gyroz - rtb_FilterCoefficient) * 0.02F;
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
  /* Start for DataStoreMemory: '<S122>/DataStoreMemory - P' */
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
