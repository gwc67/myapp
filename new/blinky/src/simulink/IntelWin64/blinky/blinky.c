/*
 * File: blinky.c
 *
 * Code generated for Simulink model 'blinky'.
 *
 * Model version                  : 1.54
 * Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
 * C/C++ source code generated on : Fri Aug 21 10:16:37 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include "blinky.h"
#include "rtwtypes.h"
#include <string.h>
#include <math.h>
#include <emmintrin.h>

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
static real_T norm(const real_T x[3]);
static void inv(const real_T x[9], real_T y[9]);
static void mrdiv_hw4(real_T A[72], const real_T B_3[36]);
static void mrdiv_hw(real_T A[108], const real_T B_2[81]);

/* Function for MATLAB Function: '<S3>/EKF' */
static real_T norm(const real_T x[3])
{
  real_T absxk;
  real_T scale;
  real_T t;
  real_T y;
  scale = 3.3121686421112381E-170;
  absxk = fabs(x[0]);
  if (absxk > 3.3121686421112381E-170) {
    y = 1.0;
    scale = absxk;
  } else {
    t = absxk / 3.3121686421112381E-170;
    y = t * t;
  }

  absxk = fabs(x[1]);
  if (absxk > scale) {
    t = scale / absxk;
    y = y * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    y += t * t;
  }

  absxk = fabs(x[2]);
  if (absxk > scale) {
    t = scale / absxk;
    y = y * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    y += t * t;
  }

  return scale * sqrt(y);
}

/* Function for MATLAB Function: '<S3>/EKF' */
static void inv(const real_T x[9], real_T y[9])
{
  __m128d tmp;
  real_T b_x[9];
  real_T absx11;
  real_T absx21;
  real_T absx31;
  int32_T p1;
  int32_T p2;
  int32_T p3;
  memcpy(&b_x[0], &x[0], 9U * sizeof(real_T));
  p1 = 0;
  p2 = 3;
  p3 = 6;
  absx11 = fabs(x[0]);
  absx21 = fabs(x[1]);
  absx31 = fabs(x[2]);
  if ((absx21 > absx11) && (absx21 > absx31)) {
    p1 = 3;
    p2 = 0;
    b_x[0] = x[1];
    b_x[1] = x[0];
    b_x[3] = x[4];
    b_x[4] = x[3];
    b_x[6] = x[7];
    b_x[7] = x[6];
  } else if (absx31 > absx11) {
    p1 = 6;
    p3 = 0;
    b_x[0] = x[2];
    b_x[2] = x[0];
    b_x[3] = x[5];
    b_x[5] = x[3];
    b_x[6] = x[8];
    b_x[8] = x[6];
  }

  tmp = _mm_div_pd(_mm_loadu_pd(&b_x[1]), _mm_set1_pd(b_x[0]));
  _mm_storeu_pd(&b_x[1], tmp);
  tmp = _mm_sub_pd(_mm_loadu_pd(&b_x[4]), _mm_mul_pd(_mm_loadu_pd(&b_x[1]),
    _mm_set1_pd(b_x[3])));
  _mm_storeu_pd(&b_x[4], tmp);
  tmp = _mm_sub_pd(_mm_loadu_pd(&b_x[7]), _mm_mul_pd(_mm_loadu_pd(&b_x[1]),
    _mm_set1_pd(b_x[6])));
  _mm_storeu_pd(&b_x[7], tmp);
  if (fabs(b_x[5]) > fabs(b_x[4])) {
    int32_T itmp;
    itmp = p2;
    p2 = p3;
    p3 = itmp;
    absx11 = b_x[1];
    b_x[1] = b_x[2];
    b_x[2] = absx11;
    absx11 = b_x[4];
    b_x[4] = b_x[5];
    b_x[5] = absx11;
    absx11 = b_x[7];
    b_x[7] = b_x[8];
    b_x[8] = absx11;
  }

  b_x[5] /= b_x[4];
  b_x[8] -= b_x[5] * b_x[7];
  absx11 = (b_x[1] * b_x[5] - b_x[2]) / b_x[8];
  absx21 = -(b_x[7] * absx11 + b_x[1]) / b_x[4];
  y[p1] = ((1.0 - b_x[3] * absx21) - b_x[6] * absx11) / b_x[0];
  y[p1 + 1] = absx21;
  y[p1 + 2] = absx11;
  absx11 = -b_x[5] / b_x[8];
  absx21 = (1.0 - b_x[7] * absx11) / b_x[4];
  y[p2] = -(b_x[3] * absx21 + b_x[6] * absx11) / b_x[0];
  y[p2 + 1] = absx21;
  y[p2 + 2] = absx11;
  absx11 = 1.0 / b_x[8];
  absx21 = -b_x[7] * absx11 / b_x[4];
  y[p3] = -(b_x[3] * absx21 + b_x[6] * absx11) / b_x[0];
  y[p3 + 1] = absx21;
  y[p3 + 2] = absx11;
}

/* Function for MATLAB Function: '<S3>/EKF' */
static void mrdiv_hw4(real_T A[72], const real_T B_3[36])
{
  __m128d tmp;
  real_T b_A[36];
  real_T smax;
  int32_T b_ix;
  int32_T c_k;
  int32_T d_j;
  int32_T ix;
  int32_T iy;
  int32_T jj;
  int32_T vectorUB;
  int8_T ipiv[6];
  memcpy(&b_A[0], &B_3[0], 36U * sizeof(real_T));
  for (vectorUB = 0; vectorUB < 6; vectorUB++) {
    ipiv[vectorUB] = (int8_T)(vectorUB + 1);
  }

  for (d_j = 0; d_j < 5; d_j++) {
    jj = d_j * 7;
    iy = 7 - d_j;
    b_ix = 0;
    smax = fabs(b_A[jj]);
    for (c_k = 2; c_k < iy; c_k++) {
      real_T s;
      s = fabs(b_A[(jj + c_k) - 1]);
      if (s > smax) {
        b_ix = c_k - 1;
        smax = s;
      }
    }

    if (b_A[jj + b_ix] != 0.0) {
      if (b_ix != 0) {
        iy = d_j + b_ix;
        ipiv[d_j] = (int8_T)(iy + 1);
        for (ix = 0; ix < 6; ix++) {
          b_ix = ix * 6 + d_j;
          smax = b_A[b_ix];
          b_A[b_ix] = b_A[iy];
          b_A[iy] = smax;
          iy += 6;
        }
      }

      iy = (jj - d_j) + 6;
      c_k = (((((iy - jj) - 1) / 2) << 1) + jj) + 2;
      vectorUB = c_k - 2;
      for (b_ix = jj + 2; b_ix <= vectorUB; b_ix += 2) {
        tmp = _mm_loadu_pd(&b_A[b_ix - 1]);
        _mm_storeu_pd(&b_A[b_ix - 1], _mm_div_pd(tmp, _mm_set1_pd(b_A[jj])));
      }

      for (b_ix = c_k; b_ix <= iy; b_ix++) {
        b_A[b_ix - 1] /= b_A[jj];
      }
    }

    iy = 4 - d_j;
    b_ix = jj + 8;
    for (c_k = 0; c_k <= iy; c_k++) {
      smax = b_A[(c_k * 6 + jj) + 6];
      if (smax != 0.0) {
        ix = (b_ix - d_j) + 4;
        for (vectorUB = b_ix; vectorUB <= ix; vectorUB++) {
          b_A[vectorUB - 1] += b_A[((jj + vectorUB) - b_ix) + 1] * -smax;
        }
      }

      b_ix += 6;
    }
  }

  for (d_j = 0; d_j < 6; d_j++) {
    jj = 12 * d_j;
    iy = 6 * d_j;
    for (b_ix = 0; b_ix < d_j; b_ix++) {
      ix = 12 * b_ix;
      smax = b_A[b_ix + iy];
      if (smax != 0.0) {
        for (c_k = 0; c_k < 12; c_k++) {
          vectorUB = c_k + jj;
          A[vectorUB] -= A[c_k + ix] * smax;
        }
      }
    }

    smax = 1.0 / b_A[d_j + iy];
    for (iy = 0; iy <= 10; iy += 2) {
      vectorUB = iy + jj;
      tmp = _mm_loadu_pd(&A[vectorUB]);
      _mm_storeu_pd(&A[vectorUB], _mm_mul_pd(tmp, _mm_set1_pd(smax)));
    }
  }

  for (d_j = 5; d_j >= 0; d_j--) {
    jj = 12 * d_j;
    iy = 6 * d_j - 1;
    for (b_ix = d_j + 2; b_ix < 7; b_ix++) {
      ix = (b_ix - 1) * 12;
      smax = b_A[b_ix + iy];
      if (smax != 0.0) {
        for (c_k = 0; c_k < 12; c_k++) {
          vectorUB = c_k + jj;
          A[vectorUB] -= A[c_k + ix] * smax;
        }
      }
    }
  }

  for (d_j = 4; d_j >= 0; d_j--) {
    int8_T ipiv_0;
    ipiv_0 = ipiv[d_j];
    if (d_j + 1 != ipiv_0) {
      for (iy = 0; iy < 12; iy++) {
        b_ix = 12 * d_j + iy;
        smax = A[b_ix];
        vectorUB = (ipiv_0 - 1) * 12 + iy;
        A[b_ix] = A[vectorUB];
        A[vectorUB] = smax;
      }
    }
  }
}

/* Function for MATLAB Function: '<S3>/EKF' */
static void mrdiv_hw(real_T A[108], const real_T B_2[81])
{
  __m128d tmp;
  real_T b_A[81];
  real_T smax;
  int32_T b_ix;
  int32_T c_k;
  int32_T d_j;
  int32_T ix;
  int32_T iy;
  int32_T jj;
  int32_T vectorUB;
  int8_T ipiv[9];
  memcpy(&b_A[0], &B_2[0], 81U * sizeof(real_T));
  for (vectorUB = 0; vectorUB < 9; vectorUB++) {
    ipiv[vectorUB] = (int8_T)(vectorUB + 1);
  }

  for (d_j = 0; d_j < 8; d_j++) {
    jj = d_j * 10;
    iy = 10 - d_j;
    b_ix = 0;
    smax = fabs(b_A[jj]);
    for (c_k = 2; c_k < iy; c_k++) {
      real_T s;
      s = fabs(b_A[(jj + c_k) - 1]);
      if (s > smax) {
        b_ix = c_k - 1;
        smax = s;
      }
    }

    if (b_A[jj + b_ix] != 0.0) {
      if (b_ix != 0) {
        iy = d_j + b_ix;
        ipiv[d_j] = (int8_T)(iy + 1);
        for (ix = 0; ix < 9; ix++) {
          b_ix = ix * 9 + d_j;
          smax = b_A[b_ix];
          b_A[b_ix] = b_A[iy];
          b_A[iy] = smax;
          iy += 9;
        }
      }

      iy = (jj - d_j) + 9;
      c_k = (((((iy - jj) - 1) / 2) << 1) + jj) + 2;
      vectorUB = c_k - 2;
      for (b_ix = jj + 2; b_ix <= vectorUB; b_ix += 2) {
        tmp = _mm_loadu_pd(&b_A[b_ix - 1]);
        _mm_storeu_pd(&b_A[b_ix - 1], _mm_div_pd(tmp, _mm_set1_pd(b_A[jj])));
      }

      for (b_ix = c_k; b_ix <= iy; b_ix++) {
        b_A[b_ix - 1] /= b_A[jj];
      }
    }

    iy = 7 - d_j;
    b_ix = jj + 11;
    for (c_k = 0; c_k <= iy; c_k++) {
      smax = b_A[(c_k * 9 + jj) + 9];
      if (smax != 0.0) {
        ix = (b_ix - d_j) + 7;
        for (vectorUB = b_ix; vectorUB <= ix; vectorUB++) {
          b_A[vectorUB - 1] += b_A[((jj + vectorUB) - b_ix) + 1] * -smax;
        }
      }

      b_ix += 9;
    }
  }

  for (d_j = 0; d_j < 9; d_j++) {
    jj = 12 * d_j;
    iy = 9 * d_j;
    for (b_ix = 0; b_ix < d_j; b_ix++) {
      ix = 12 * b_ix;
      smax = b_A[b_ix + iy];
      if (smax != 0.0) {
        for (c_k = 0; c_k < 12; c_k++) {
          vectorUB = c_k + jj;
          A[vectorUB] -= A[c_k + ix] * smax;
        }
      }
    }

    smax = 1.0 / b_A[d_j + iy];
    for (iy = 0; iy <= 10; iy += 2) {
      vectorUB = iy + jj;
      tmp = _mm_loadu_pd(&A[vectorUB]);
      _mm_storeu_pd(&A[vectorUB], _mm_mul_pd(tmp, _mm_set1_pd(smax)));
    }
  }

  for (d_j = 8; d_j >= 0; d_j--) {
    jj = 12 * d_j;
    iy = 9 * d_j - 1;
    for (b_ix = d_j + 2; b_ix < 10; b_ix++) {
      ix = (b_ix - 1) * 12;
      smax = b_A[b_ix + iy];
      if (smax != 0.0) {
        for (c_k = 0; c_k < 12; c_k++) {
          vectorUB = c_k + jj;
          A[vectorUB] -= A[c_k + ix] * smax;
        }
      }
    }
  }

  for (d_j = 7; d_j >= 0; d_j--) {
    int8_T ipiv_0;
    ipiv_0 = ipiv[d_j];
    if (d_j + 1 != ipiv_0) {
      for (iy = 0; iy < 12; iy++) {
        b_ix = 12 * d_j + iy;
        smax = A[b_ix];
        vectorUB = (ipiv_0 - 1) * 12 + iy;
        A[b_ix] = A[vectorUB];
        A[vectorUB] = smax;
      }
    }
  }
}

/* Model step function for TID0 */
void blinky_step0(void)                /* Sample time: [0.001s, 0.0s] */
{
  /* (no output/update code required) */
}

/* Model step function for TID1 */
void blinky_step1(void)                /* Sample time: [0.005s, 0.0s] */
{
  __m128d tmp;
  __m128d tmp_0;
  __m128d tmp_2;
  __m128d tmp_3;
  real_T P_apr[144];
  real_T P_apr_0[144];
  real_T P_apr_1[144];
  real_T b_S_k_tmp_0[108];
  real_T c_K_k[108];
  real_T b_S_k[81];
  real_T K_k[72];
  real_T S_k_tmp_0[72];
  real_T O_tmp_0[36];
  real_T S_k[36];
  real_T b_K_k[36];
  real_T v[12];
  real_T x_apr[12];
  real_T O[9];
  real_T O_0[9];
  real_T b_I_0[9];
  real_T z[9];
  real_T zek_tmp[9];
  real_T zek_tmp_0[9];
  real_T S_k_tmp_1[6];
  real_T z_0[6];
  real_T z_1[6];
  real_T muk[3];
  real_T z_2[3];
  real_T zek[3];
  real_T tmp_1[2];
  real_T O_1;
  real_T O_2;
  real_T O_3;
  real_T b_idx_1;
  real_T maxval;
  real_T wak_idx_0;
  real_T wak_idx_1;
  real_T wak_idx_2;
  int32_T P_apr_tmp_tmp;
  int32_T P_apr_tmp_tmp_0;
  int32_T P_apr_tmp_tmp_1;
  int32_T i;
  int32_T r2;
  int32_T r3;
  int32_T rtemp;
  real32_T rtb_FilterCoefficient;
  real32_T rtb_Sum2;
  int16_T rtb_Switch;
  int8_T c_I[144];
  int8_T b_S_k_tmp[108];
  int8_T S_k_tmp[72];
  int8_T O_tmp[36];
  int8_T b_I[9];
  int8_T O_tmp_1;
  int8_T O_tmp_2;
  int8_T O_tmp_3;
  boolean_T rtb_LogicalOperator;
  static const real_T d[9] = { 0.0018, 0.0, 0.0, 0.0, 0.0018, 0.0, 0.0, 0.0,
    0.0018 };

  static const int8_T e[36] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  static const int8_T f[108] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };

  static const int8_T c_a[108] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };

  static const int8_T g[36] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  static const int8_T d_a[36] = { 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  static const int8_T h[72] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0 };

  static const int8_T e_a[72] = { 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0 };

  static const int8_T i_0[72] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1 };

  static const int8_T f_a[72] = { 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 1 };

  /* Outputs for Atomic SubSystem: '<Root>/task_5ms' */
  /* MATLAB Function: '<S3>/EKF' incorporates:
   *  Constant: '<S1>/Constant1'
   *  Inport: '<Root>/ax'
   *  Inport: '<Root>/ay'
   *  Inport: '<Root>/az'
   *  Inport: '<Root>/gx'
   *  Inport: '<Root>/gy'
   *  Inport: '<Root>/gz'
   *  Math: '<S3>/Transpose2'
   */
  if (!rtDW.AHRSEKF_not_empty) {
    rtDW.AHRSEKF.approx_prediction = false;
    rtDW.AHRSEKF.use_inertia_matrix = false;
    rtDW.AHRSEKF.eulerAngles[0] = 0.0;
    rtDW.AHRSEKF.eulerAngles[1] = 0.0;
    rtDW.AHRSEKF.eulerAngles[2] = 0.0;
    rtDW.AHRSEKF.q_rotSpeed = 0.0001;
    rtDW.AHRSEKF.q_rotAcc = 0.08;
    rtDW.AHRSEKF.q_acc = 0.009;
    rtDW.AHRSEKF.q_mag = 0.005;
    rtDW.AHRSEKF.r_gyro = 0.0008;
    rtDW.AHRSEKF.r_accel = 10000.0;
    rtDW.AHRSEKF.r_mag = 100.0;
    memset(&rtDW.AHRSEKF.x_apo[0], 0, 12U * sizeof(real_T));
    memset(&rtDW.AHRSEKF.P_apo[0], 0, 144U * sizeof(real_T));
    rtDW.AHRSEKF.zFlag[0] = 1.0;
    rtDW.AHRSEKF.zFlag[1] = 1.0;
    rtDW.AHRSEKF.zFlag[2] = 1.0;
    memcpy(&rtDW.AHRSEKF.J[0], &d[0], 9U * sizeof(real_T));
    rtDW.AHRSEKF_not_empty = true;
  }

  maxval = norm(rtConstB.Transpose2);
  if (maxval != 0.0) {
    z[0] = rtU.gx;
    z[1] = rtU.gy;
    z[2] = rtU.gz;
    z[3] = rtU.ax;
    z[4] = rtU.ay;
    z[5] = rtU.az;
    maxval = 0.0 / maxval;
    z[6] = maxval;
    z[7] = maxval;
    z[8] = maxval;
    if (!rtDW.Ji_not_empty) {
      inv(rtDW.AHRSEKF.J, rtDW.Ji);
      rtDW.Ji_not_empty = true;
    }

    if (rtDW.AHRSEKF.use_inertia_matrix) {
      zek[0] = rtDW.AHRSEKF.x_apo[3];
      zek[1] = rtDW.AHRSEKF.x_apo[4];
      zek[2] = rtDW.AHRSEKF.x_apo[5];
      wak_idx_2 = 0.0;
      b_idx_1 = 0.0;
      wak_idx_0 = 0.0;
      for (i = 0; i < 3; i++) {
        tmp_3 = _mm_add_pd(_mm_mul_pd(_mm_loadu_pd(&rtDW.AHRSEKF.J[3 * i]),
          _mm_set1_pd(zek[i])), _mm_set_pd(b_idx_1, wak_idx_2));
        _mm_storeu_pd(&tmp_1[0], tmp_3);
        wak_idx_2 = tmp_1[0];
        b_idx_1 = tmp_1[1];
        wak_idx_0 += rtDW.AHRSEKF.J[3 * i + 2] * zek[i];
      }

      zek[0] = -(rtDW.AHRSEKF.x_apo[4] * wak_idx_0 - b_idx_1 *
                 rtDW.AHRSEKF.x_apo[5]);
      zek[1] = -(wak_idx_2 * rtDW.AHRSEKF.x_apo[5] - rtDW.AHRSEKF.x_apo[3] *
                 wak_idx_0);
      zek[2] = -(rtDW.AHRSEKF.x_apo[3] * b_idx_1 - wak_idx_2 *
                 rtDW.AHRSEKF.x_apo[4]);
      wak_idx_0 = 0.0;
      wak_idx_1 = 0.0;
      wak_idx_2 = 0.0;
      for (i = 0; i < 3; i++) {
        tmp_3 = _mm_add_pd(_mm_mul_pd(_mm_loadu_pd(&rtDW.Ji[3 * i]), _mm_set1_pd
          (zek[i])), _mm_set_pd(wak_idx_1, wak_idx_0));
        _mm_storeu_pd(&tmp_1[0], tmp_3);
        wak_idx_0 = tmp_1[0];
        wak_idx_1 = tmp_1[1];
        wak_idx_2 += rtDW.Ji[3 * i + 2] * zek[i];
      }

      tmp_3 = _mm_add_pd(_mm_mul_pd(_mm_set_pd(wak_idx_1, wak_idx_0),
        _mm_set1_pd(0.005)), _mm_loadu_pd(&rtDW.AHRSEKF.x_apo[3]));
      _mm_storeu_pd(&tmp_1[0], tmp_3);
      wak_idx_0 = tmp_1[0];
      wak_idx_1 = tmp_1[1];
      wak_idx_2 = wak_idx_2 * 0.005 + rtDW.AHRSEKF.x_apo[5];
    } else {
      wak_idx_0 = rtDW.AHRSEKF.x_apo[3];
      wak_idx_1 = rtDW.AHRSEKF.x_apo[4];
      wak_idx_2 = rtDW.AHRSEKF.x_apo[5];
    }

    O[0] = 0.0;
    O[1] = -rtDW.AHRSEKF.x_apo[2];
    O[2] = rtDW.AHRSEKF.x_apo[1];
    O[3] = rtDW.AHRSEKF.x_apo[2];
    O[4] = 0.0;
    O[5] = -rtDW.AHRSEKF.x_apo[0];
    O[6] = -rtDW.AHRSEKF.x_apo[1];
    O[7] = rtDW.AHRSEKF.x_apo[0];
    O[8] = 0.0;
    if (rtDW.AHRSEKF.approx_prediction) {
      for (i = 0; i < 9; i++) {
        b_I[i] = 0;
      }

      b_I[0] = 1;
      b_I[4] = 1;
      b_I[8] = 1;
      for (i = 0; i <= 6; i += 2) {
        tmp_3 = _mm_set1_pd(0.005);
        tmp_2 = _mm_mul_pd(_mm_loadu_pd(&O[i]), tmp_3);
        _mm_storeu_pd(&zek_tmp[i], tmp_2);
        _mm_storeu_pd(&zek_tmp_0[i], _mm_add_pd(tmp_2, _mm_set_pd(b_I[i + 1],
          b_I[i])));
      }

      for (i = 8; i < 9; i++) {
        b_idx_1 = O[i] * 0.005;
        zek_tmp[i] = b_idx_1;
        zek_tmp_0[i] = b_idx_1 + (real_T)b_I[i];
      }

      muk[0] = rtDW.AHRSEKF.x_apo[6];
      muk[1] = rtDW.AHRSEKF.x_apo[7];
      muk[2] = rtDW.AHRSEKF.x_apo[8];
      b_idx_1 = 0.0;
      O_1 = 0.0;
      O_2 = 0.0;
      for (i = 0; i < 3; i++) {
        tmp_3 = _mm_add_pd(_mm_mul_pd(_mm_loadu_pd(&zek_tmp_0[3 * i]),
          _mm_set1_pd(muk[i])), _mm_set_pd(O_1, b_idx_1));
        _mm_storeu_pd(&tmp_1[0], tmp_3);
        b_idx_1 = tmp_1[0];
        O_1 = tmp_1[1];
        O_2 += zek_tmp_0[3 * i + 2] * muk[i];
      }

      zek[2] = O_2;
      zek[1] = O_1;
      zek[0] = b_idx_1;
      for (i = 0; i < 9; i++) {
        b_I[i] = 0;
      }

      b_I[0] = 1;
      b_I[4] = 1;
      b_I[8] = 1;
      for (i = 0; i <= 6; i += 2) {
        tmp_3 = _mm_add_pd(_mm_loadu_pd(&zek_tmp[i]), _mm_set_pd(b_I[i + 1],
          b_I[i]));
        _mm_storeu_pd(&zek_tmp_0[i], tmp_3);
      }

      for (i = 8; i < 9; i++) {
        zek_tmp_0[i] = zek_tmp[i] + (real_T)b_I[i];
      }

      muk[0] = rtDW.AHRSEKF.x_apo[9];
      muk[1] = rtDW.AHRSEKF.x_apo[10];
      muk[2] = rtDW.AHRSEKF.x_apo[11];
      b_idx_1 = 0.0;
      O_1 = 0.0;
      O_2 = 0.0;
      for (i = 0; i < 3; i++) {
        tmp_3 = _mm_add_pd(_mm_mul_pd(_mm_loadu_pd(&zek_tmp_0[3 * i]),
          _mm_set1_pd(muk[i])), _mm_set_pd(O_1, b_idx_1));
        _mm_storeu_pd(&tmp_1[0], tmp_3);
        b_idx_1 = tmp_1[0];
        O_1 = tmp_1[1];
        O_2 += zek_tmp_0[3 * i + 2] * muk[i];
      }

      muk[2] = O_2;
      muk[1] = O_1;
      muk[0] = b_idx_1;
    } else {
      for (i = 0; i < 9; i++) {
        b_I[i] = 0;
      }

      b_I[0] = 1;
      b_I[4] = 1;
      b_I[8] = 1;
      for (i = 0; i <= 6; i += 2) {
        tmp_3 = _mm_loadu_pd(&O[i]);
        _mm_storeu_pd(&zek_tmp[i], _mm_mul_pd(tmp_3, _mm_set1_pd(0.005)));
      }

      for (i = 8; i < 9; i++) {
        zek_tmp[i] = O[i] * 0.005;
      }

      for (i = 0; i < 3; i++) {
        O_1 = 0.0;
        O_2 = 0.0;
        O_3 = 0.0;
        for (r2 = 0; r2 < 3; r2++) {
          b_idx_1 = O[3 * i + r2];
          tmp_3 = _mm_add_pd(_mm_mul_pd(_mm_loadu_pd(&O[3 * r2]), _mm_set1_pd
            (b_idx_1)), _mm_set_pd(O_2, O_1));
          _mm_storeu_pd(&tmp_1[0], tmp_3);
          O_1 = tmp_1[0];
          O_2 = tmp_1[1];
          O_3 += O[3 * r2 + 2] * b_idx_1;
        }

        O_0[3 * i + 2] = O_3;
        O_0[3 * i + 1] = O_2;
        O_0[3 * i] = O_1;
      }

      for (i = 0; i <= 6; i += 2) {
        tmp_3 = _mm_set1_pd(1.25E-5);
        tmp_2 = _mm_mul_pd(tmp_3, _mm_loadu_pd(&O_0[i]));
        _mm_storeu_pd(&zek_tmp_0[i], tmp_2);
        tmp_3 = _mm_add_pd(_mm_add_pd(_mm_set_pd(b_I[i + 1], b_I[i]),
          _mm_loadu_pd(&zek_tmp[i])), tmp_2);
        _mm_storeu_pd(&b_I_0[i], tmp_3);
      }

      for (i = 8; i < 9; i++) {
        b_idx_1 = 1.25E-5 * O_0[i];
        zek_tmp_0[i] = b_idx_1;
        b_I_0[i] = ((real_T)b_I[i] + zek_tmp[i]) + b_idx_1;
      }

      muk[0] = rtDW.AHRSEKF.x_apo[6];
      muk[1] = rtDW.AHRSEKF.x_apo[7];
      muk[2] = rtDW.AHRSEKF.x_apo[8];
      b_idx_1 = 0.0;
      O_1 = 0.0;
      O_2 = 0.0;
      for (i = 0; i < 3; i++) {
        tmp_3 = _mm_add_pd(_mm_mul_pd(_mm_loadu_pd(&b_I_0[3 * i]), _mm_set1_pd
          (muk[i])), _mm_set_pd(O_1, b_idx_1));
        _mm_storeu_pd(&tmp_1[0], tmp_3);
        b_idx_1 = tmp_1[0];
        O_1 = tmp_1[1];
        O_2 += b_I_0[3 * i + 2] * muk[i];
      }

      zek[2] = O_2;
      zek[1] = O_1;
      zek[0] = b_idx_1;
      for (i = 0; i < 9; i++) {
        b_I[i] = 0;
      }

      b_I[0] = 1;
      b_I[4] = 1;
      b_I[8] = 1;
      for (i = 0; i <= 6; i += 2) {
        tmp_3 = _mm_add_pd(_mm_add_pd(_mm_set_pd(b_I[i + 1], b_I[i]),
          _mm_loadu_pd(&zek_tmp[i])), _mm_loadu_pd(&zek_tmp_0[i]));
        _mm_storeu_pd(&b_I_0[i], tmp_3);
      }

      for (i = 8; i < 9; i++) {
        b_I_0[i] = ((real_T)b_I[i] + zek_tmp[i]) + zek_tmp_0[i];
      }

      muk[0] = rtDW.AHRSEKF.x_apo[9];
      muk[1] = rtDW.AHRSEKF.x_apo[10];
      muk[2] = rtDW.AHRSEKF.x_apo[11];
      b_idx_1 = 0.0;
      O_1 = 0.0;
      O_2 = 0.0;
      for (i = 0; i < 3; i++) {
        tmp_3 = _mm_add_pd(_mm_mul_pd(_mm_loadu_pd(&b_I_0[3 * i]), _mm_set1_pd
          (muk[i])), _mm_set_pd(O_1, b_idx_1));
        _mm_storeu_pd(&tmp_1[0], tmp_3);
        b_idx_1 = tmp_1[0];
        O_1 = tmp_1[1];
        O_2 += b_I_0[3 * i + 2] * muk[i];
      }

      muk[2] = O_2;
      muk[1] = O_1;
      muk[0] = b_idx_1;
    }

    tmp_3 = _mm_set1_pd(0.005);
    tmp_2 = _mm_add_pd(_mm_mul_pd(tmp_3, _mm_set_pd(wak_idx_1, wak_idx_0)),
                       _mm_loadu_pd(&rtDW.AHRSEKF.x_apo[0]));
    _mm_storeu_pd(&x_apr[0], tmp_2);
    x_apr[2] = 0.005 * wak_idx_2 + rtDW.AHRSEKF.x_apo[2];
    x_apr[3] = wak_idx_0;
    x_apr[6] = zek[0];
    x_apr[9] = muk[0];
    x_apr[4] = wak_idx_1;
    x_apr[7] = zek[1];
    x_apr[10] = muk[1];
    x_apr[5] = wak_idx_2;
    x_apr[8] = zek[2];
    x_apr[11] = muk[2];
    memset(&c_I[0], 0, 144U * sizeof(int8_T));
    for (i = 0; i < 12; i++) {
      c_I[i + 12 * i] = 1;
    }

    for (i = 0; i <= 6; i += 2) {
      tmp_2 = _mm_loadu_pd(&O[i]);
      _mm_storeu_pd(&O[i], _mm_mul_pd(tmp_2, tmp_3));
    }

    for (i = 8; i < 9; i++) {
      O[i] *= 0.005;
    }

    zek_tmp[0] = 0.0;
    _mm_storeu_pd(&zek_tmp[1], _mm_mul_pd(_mm_set_pd(-rtDW.AHRSEKF.x_apo[7],
      rtDW.AHRSEKF.x_apo[8]), tmp_3));
    zek_tmp[3] = -rtDW.AHRSEKF.x_apo[8] * 0.005;
    zek_tmp[4] = 0.0;
    tmp_2 = _mm_mul_pd(_mm_loadu_pd(&rtDW.AHRSEKF.x_apo[6]), tmp_3);
    _mm_storeu_pd(&zek_tmp[5], tmp_2);
    zek_tmp[7] = -rtDW.AHRSEKF.x_apo[6] * 0.005;
    zek_tmp[8] = 0.0;
    O_0[0] = 0.0;
    _mm_storeu_pd(&O_0[1], _mm_mul_pd(_mm_set_pd(-rtDW.AHRSEKF.x_apo[10],
      rtDW.AHRSEKF.x_apo[11]), tmp_3));
    O_0[3] = -rtDW.AHRSEKF.x_apo[11] * 0.005;
    O_0[4] = 0.0;
    tmp_3 = _mm_mul_pd(_mm_loadu_pd(&rtDW.AHRSEKF.x_apo[9]), tmp_3);
    _mm_storeu_pd(&O_0[5], tmp_3);
    O_0[7] = -rtDW.AHRSEKF.x_apo[9] * 0.005;
    O_0[8] = 0.0;
    for (i = 0; i < 12; i++) {
      P_apr[12 * i] = (real_T)e[3 * i] * 0.005 + (real_T)c_I[12 * i];
      rtemp = 12 * i + 3;
      P_apr[rtemp] = c_I[rtemp];
      rtemp = 12 * i + 1;
      P_apr[rtemp] = (real_T)e[3 * i + 1] * 0.005 + (real_T)c_I[rtemp];
      rtemp = 12 * i + 4;
      P_apr[rtemp] = c_I[rtemp];
      rtemp = 12 * i + 2;
      P_apr[rtemp] = (real_T)e[3 * i + 2] * 0.005 + (real_T)c_I[rtemp];
      rtemp = 12 * i + 5;
      P_apr[rtemp] = c_I[rtemp];
    }

    for (i = 0; i < 3; i++) {
      rtemp = 12 * i + 6;
      P_apr[rtemp] = zek_tmp[3 * i] + (real_T)c_I[rtemp];
      P_apr_tmp_tmp = (i + 3) * 12;
      P_apr[P_apr_tmp_tmp + 6] = c_I[P_apr_tmp_tmp + 6];
      O_1 = O[3 * i];
      P_apr_tmp_tmp_0 = (i + 6) * 12;
      P_apr[P_apr_tmp_tmp_0 + 6] = (real_T)c_I[P_apr_tmp_tmp_0 + 6] + O_1;
      P_apr_tmp_tmp_1 = (i + 9) * 12;
      P_apr[P_apr_tmp_tmp_1 + 6] = c_I[P_apr_tmp_tmp_1 + 6];
      rtemp = 12 * i + 9;
      P_apr[rtemp] = O_0[3 * i] + (real_T)c_I[rtemp];
      P_apr[P_apr_tmp_tmp + 9] = c_I[P_apr_tmp_tmp + 9];
      P_apr[P_apr_tmp_tmp_0 + 9] = c_I[P_apr_tmp_tmp_0 + 9];
      r2 = 12 * i + 7;
      r3 = 3 * i + 1;
      _mm_storeu_pd(&tmp_1[0], _mm_add_pd(_mm_set_pd(c_I[r2],
        c_I[P_apr_tmp_tmp_1 + 9]), _mm_set_pd(zek_tmp[r3], O_1)));
      P_apr[P_apr_tmp_tmp_1 + 9] = tmp_1[0];
      P_apr[r2] = tmp_1[1];
      P_apr[P_apr_tmp_tmp + 7] = c_I[P_apr_tmp_tmp + 7];
      O_1 = O[r3];
      P_apr[P_apr_tmp_tmp_0 + 7] = (real_T)c_I[P_apr_tmp_tmp_0 + 7] + O_1;
      P_apr[P_apr_tmp_tmp_1 + 7] = c_I[P_apr_tmp_tmp_1 + 7];
      rtemp = 12 * i + 10;
      P_apr[rtemp] = (real_T)c_I[rtemp] + O_0[r3];
      P_apr[P_apr_tmp_tmp + 10] = c_I[P_apr_tmp_tmp + 10];
      P_apr[P_apr_tmp_tmp_0 + 10] = c_I[P_apr_tmp_tmp_0 + 10];
      r2 = 12 * i + 8;
      r3 = 3 * i + 2;
      _mm_storeu_pd(&tmp_1[0], _mm_add_pd(_mm_set_pd(c_I[r2],
        c_I[P_apr_tmp_tmp_1 + 10]), _mm_set_pd(zek_tmp[r3], O_1)));
      P_apr[P_apr_tmp_tmp_1 + 10] = tmp_1[0];
      P_apr[r2] = tmp_1[1];
      P_apr[P_apr_tmp_tmp + 8] = c_I[P_apr_tmp_tmp + 8];
      O_1 = O[r3];
      P_apr[P_apr_tmp_tmp_0 + 8] = (real_T)c_I[P_apr_tmp_tmp_0 + 8] + O_1;
      P_apr[P_apr_tmp_tmp_1 + 8] = c_I[P_apr_tmp_tmp_1 + 8];
      rtemp = 12 * i + 11;
      P_apr[rtemp] = (real_T)c_I[rtemp] + O_0[r3];
      P_apr[P_apr_tmp_tmp + 11] = c_I[P_apr_tmp_tmp + 11];
      P_apr[P_apr_tmp_tmp_0 + 11] = c_I[P_apr_tmp_tmp_0 + 11];
      P_apr[P_apr_tmp_tmp_1 + 11] = (real_T)c_I[P_apr_tmp_tmp_1 + 11] + O_1;
    }

    if (!rtDW.Q_not_empty) {
      v[0] = rtDW.AHRSEKF.q_rotSpeed;
      v[1] = rtDW.AHRSEKF.q_rotSpeed;
      v[2] = rtDW.AHRSEKF.q_rotSpeed;
      v[3] = rtDW.AHRSEKF.q_rotAcc;
      v[4] = rtDW.AHRSEKF.q_rotAcc;
      v[5] = rtDW.AHRSEKF.q_rotAcc;
      v[6] = rtDW.AHRSEKF.q_acc;
      v[7] = rtDW.AHRSEKF.q_acc;
      v[8] = rtDW.AHRSEKF.q_acc;
      v[9] = rtDW.AHRSEKF.q_mag;
      v[10] = rtDW.AHRSEKF.q_mag;
      v[11] = rtDW.AHRSEKF.q_mag;
      memset(&rtDW.Q[0], 0, 144U * sizeof(real_T));
      for (i = 0; i < 12; i++) {
        rtDW.Q[i + 12 * i] = v[i];
      }

      rtDW.Q_not_empty = true;
    }

    for (i = 0; i < 12; i++) {
      memset(&P_apr_0[i * 12], 0, 12U * sizeof(real_T));
      for (r2 = 0; r2 < 12; r2++) {
        b_idx_1 = rtDW.AHRSEKF.P_apo[12 * i + r2];
        for (r3 = 0; r3 <= 10; r3 += 2) {
          tmp_3 = _mm_loadu_pd(&P_apr[12 * r2 + r3]);
          rtemp = 12 * i + r3;
          tmp_2 = _mm_loadu_pd(&P_apr_0[rtemp]);
          _mm_storeu_pd(&P_apr_0[rtemp], _mm_add_pd(_mm_mul_pd(tmp_3,
            _mm_set1_pd(b_idx_1)), tmp_2));
        }
      }
    }

    for (i = 0; i < 12; i++) {
      for (r2 = 0; r2 < 12; r2++) {
        b_idx_1 = 0.0;
        for (r3 = 0; r3 < 12; r3++) {
          b_idx_1 += P_apr_0[12 * r3 + i] * P_apr[12 * r3 + r2];
        }

        rtemp = 12 * r2 + i;
        P_apr_1[rtemp] = rtDW.Q[rtemp] + b_idx_1;
      }
    }

    memcpy(&P_apr[0], &P_apr_1[0], 144U * sizeof(real_T));
    if ((rtDW.AHRSEKF.zFlag[0] == 1.0) && (rtDW.AHRSEKF.zFlag[1] == 1.0) &&
        (rtDW.AHRSEKF.zFlag[2] == 1.0)) {
      for (i = 0; i < 108; i++) {
        c_K_k[i] = f[i];
        b_S_k_tmp[i] = c_a[i];
      }

      for (i = 0; i < 12; i++) {
        memset(&b_S_k_tmp_0[i * 9], 0, 9U * sizeof(real_T));
        for (r2 = 0; r2 < 12; r2++) {
          b_idx_1 = P_apr[12 * i + r2];
          for (r3 = 0; r3 < 9; r3++) {
            rtemp = 9 * i + r3;
            b_S_k_tmp_0[rtemp] += (real_T)b_S_k_tmp[9 * r2 + r3] * b_idx_1;
          }
        }
      }

      for (i = 0; i < 9; i++) {
        memset(&b_S_k[i * 9], 0, 9U * sizeof(real_T));
        for (r2 = 0; r2 < 12; r2++) {
          r3 = (int32_T)c_K_k[12 * i + r2];
          for (rtemp = 0; rtemp <= 6; rtemp += 2) {
            tmp_3 = _mm_loadu_pd(&b_S_k_tmp_0[9 * r2 + rtemp]);
            P_apr_tmp_tmp = 9 * i + rtemp;
            tmp_2 = _mm_loadu_pd(&b_S_k[P_apr_tmp_tmp]);
            _mm_storeu_pd(&b_S_k[P_apr_tmp_tmp], _mm_add_pd(_mm_mul_pd(tmp_3,
              _mm_set1_pd(r3)), tmp_2));
          }

          for (rtemp = 8; rtemp < 9; rtemp++) {
            P_apr_tmp_tmp = 9 * i + rtemp;
            b_S_k[P_apr_tmp_tmp] += b_S_k_tmp_0[9 * r2 + rtemp] * (real_T)r3;
          }
        }
      }

      zek_tmp[0] = rtDW.AHRSEKF.r_gyro;
      zek_tmp[1] = rtDW.AHRSEKF.r_gyro;
      zek_tmp[2] = rtDW.AHRSEKF.r_gyro;
      zek_tmp[3] = rtDW.AHRSEKF.r_accel;
      zek_tmp[4] = rtDW.AHRSEKF.r_accel;
      zek_tmp[5] = rtDW.AHRSEKF.r_accel;
      zek_tmp[6] = rtDW.AHRSEKF.r_mag;
      zek_tmp[7] = rtDW.AHRSEKF.r_mag;
      zek_tmp[8] = rtDW.AHRSEKF.r_mag;
      for (i = 0; i < 9; i++) {
        b_S_k[10 * i] += zek_tmp[i];
        memset(&b_S_k_tmp_0[i * 12], 0, 12U * sizeof(real_T));
        for (r2 = 0; r2 < 12; r2++) {
          r3 = (int32_T)c_K_k[12 * i + r2];
          for (rtemp = 0; rtemp <= 10; rtemp += 2) {
            tmp_3 = _mm_loadu_pd(&P_apr[12 * r2 + rtemp]);
            P_apr_tmp_tmp = 12 * i + rtemp;
            tmp_2 = _mm_loadu_pd(&b_S_k_tmp_0[P_apr_tmp_tmp]);
            _mm_storeu_pd(&b_S_k_tmp_0[P_apr_tmp_tmp], _mm_add_pd(_mm_mul_pd
              (tmp_3, _mm_set1_pd(r3)), tmp_2));
          }
        }
      }

      memcpy(&c_K_k[0], &b_S_k_tmp_0[0], 108U * sizeof(real_T));
      mrdiv_hw(c_K_k, b_S_k);
      for (i = 0; i < 9; i++) {
        b_idx_1 = 0.0;
        for (r2 = 0; r2 < 12; r2++) {
          b_idx_1 += (real_T)b_S_k_tmp[9 * r2 + i] * x_apr[r2];
        }

        O[i] = z[i] - b_idx_1;
      }

      for (i = 0; i < 12; i++) {
        b_idx_1 = 0.0;
        for (r2 = 0; r2 < 9; r2++) {
          b_idx_1 += c_K_k[12 * r2 + i] * O[r2];
        }

        v[i] = x_apr[i] + b_idx_1;
      }

      memset(&c_I[0], 0, 144U * sizeof(int8_T));
      for (i = 0; i < 12; i++) {
        c_I[i + 12 * i] = 1;
      }

      for (i = 0; i < 12; i++) {
        for (r2 = 0; r2 < 12; r2++) {
          b_idx_1 = 0.0;
          for (r3 = 0; r3 < 9; r3++) {
            b_idx_1 += c_K_k[12 * r3 + i] * (real_T)b_S_k_tmp[9 * r2 + r3];
          }

          r3 = 12 * r2 + i;
          P_apr_0[r3] = (real_T)c_I[r3] - b_idx_1;
          P_apr_1[r2 + 12 * i] = 0.0;
        }
      }

      for (i = 0; i < 12; i++) {
        for (r2 = 0; r2 < 12; r2++) {
          b_idx_1 = P_apr[12 * i + r2];
          for (r3 = 0; r3 <= 10; r3 += 2) {
            tmp_3 = _mm_loadu_pd(&P_apr_0[12 * r2 + r3]);
            rtemp = 12 * i + r3;
            tmp_2 = _mm_loadu_pd(&P_apr_1[rtemp]);
            _mm_storeu_pd(&P_apr_1[rtemp], _mm_add_pd(_mm_mul_pd(tmp_3,
              _mm_set1_pd(b_idx_1)), tmp_2));
          }
        }
      }

      memcpy(&P_apr[0], &P_apr_1[0], 144U * sizeof(real_T));
    } else if ((rtDW.AHRSEKF.zFlag[0] == 1.0) && (rtDW.AHRSEKF.zFlag[1] == 0.0) &&
               (rtDW.AHRSEKF.zFlag[2] == 0.0)) {
      for (i = 0; i < 36; i++) {
        b_K_k[i] = g[i];
        O_tmp[i] = d_a[i];
      }

      for (i = 0; i < 12; i++) {
        maxval = 0.0;
        wak_idx_0 = 0.0;
        wak_idx_1 = 0.0;
        for (r2 = 0; r2 < 12; r2++) {
          b_idx_1 = P_apr[12 * i + r2];
          _mm_storeu_pd(&tmp_1[0], _mm_add_pd(_mm_mul_pd(_mm_set_pd(O_tmp[3 * r2
            + 1], O_tmp[3 * r2]), _mm_set1_pd(b_idx_1)), _mm_set_pd(wak_idx_0,
            maxval)));
          maxval = tmp_1[0];
          wak_idx_0 = tmp_1[1];
          wak_idx_1 += (real_T)O_tmp[3 * r2 + 2] * b_idx_1;
        }

        O_tmp_0[3 * i + 2] = wak_idx_1;
        O_tmp_0[3 * i + 1] = wak_idx_0;
        O_tmp_0[3 * i] = maxval;
      }

      for (i = 0; i < 3; i++) {
        O_1 = 0.0;
        O_2 = 0.0;
        O_3 = 0.0;
        for (r2 = 0; r2 < 12; r2++) {
          b_idx_1 = (int32_T)b_K_k[12 * i + r2];
          tmp_3 = _mm_add_pd(_mm_mul_pd(_mm_loadu_pd(&O_tmp_0[3 * r2]),
            _mm_set1_pd(b_idx_1)), _mm_set_pd(O_2, O_1));
          _mm_storeu_pd(&tmp_1[0], tmp_3);
          O_1 = tmp_1[0];
          O_2 = tmp_1[1];
          O_3 += O_tmp_0[3 * r2 + 2] * b_idx_1;
        }

        O[3 * i + 2] = O_3;
        O[3 * i + 1] = O_2;
        O[3 * i] = O_1;
      }

      _mm_storeu_pd(&tmp_1[0], _mm_add_pd(_mm_set_pd(O[4], O[0]), _mm_set1_pd
        (rtDW.AHRSEKF.r_gyro)));
      O[0] = tmp_1[0];
      O[4] = tmp_1[1];
      O[8] += rtDW.AHRSEKF.r_gyro;
      for (i = 0; i < 12; i++) {
        S_k[i] = 0.0;
        S_k[i + 12] = 0.0;
        S_k[i + 24] = 0.0;
      }

      for (i = 0; i < 3; i++) {
        for (r2 = 0; r2 < 12; r2++) {
          r3 = (int32_T)b_K_k[12 * i + r2];
          for (rtemp = 0; rtemp <= 10; rtemp += 2) {
            tmp_3 = _mm_loadu_pd(&P_apr[12 * r2 + rtemp]);
            P_apr_tmp_tmp = 12 * i + rtemp;
            tmp_2 = _mm_loadu_pd(&S_k[P_apr_tmp_tmp]);
            _mm_storeu_pd(&S_k[P_apr_tmp_tmp], _mm_add_pd(_mm_mul_pd(tmp_3,
              _mm_set1_pd(r3)), tmp_2));
          }
        }
      }

      i = 0;
      r2 = 1;
      r3 = 2;
      maxval = fabs(tmp_1[0]);
      wak_idx_0 = fabs(O[1]);
      if (wak_idx_0 > maxval) {
        maxval = wak_idx_0;
        i = 1;
        r2 = 0;
      }

      if (fabs(O[2]) > maxval) {
        i = 2;
        r2 = 1;
        r3 = 0;
      }

      O[r2] /= O[i];
      O[r3] /= O[i];
      O[r2 + 3] -= O[i + 3] * O[r2];
      O[r3 + 3] -= O[i + 3] * O[r3];
      O[r2 + 6] -= O[i + 6] * O[r2];
      O[r3 + 6] -= O[i + 6] * O[r3];
      if (fabs(O[r3 + 3]) > fabs(O[r2 + 3])) {
        rtemp = r2;
        r2 = r3;
        r3 = rtemp;
      }

      O[r3 + 3] /= O[r2 + 3];
      O[r3 + 6] -= O[r3 + 3] * O[r2 + 6];
      for (rtemp = 0; rtemp < 12; rtemp++) {
        P_apr_tmp_tmp = 12 * i + rtemp;
        b_K_k[P_apr_tmp_tmp] = S_k[rtemp] / O[i];
        P_apr_tmp_tmp_0 = 12 * r2 + rtemp;
        b_K_k[P_apr_tmp_tmp_0] = S_k[rtemp + 12] - O[i + 3] *
          b_K_k[P_apr_tmp_tmp];
        P_apr_tmp_tmp_1 = 12 * r3 + rtemp;
        b_K_k[P_apr_tmp_tmp_1] = S_k[rtemp + 24] - O[i + 6] *
          b_K_k[P_apr_tmp_tmp];
        b_K_k[P_apr_tmp_tmp_0] /= O[r2 + 3];
        b_K_k[P_apr_tmp_tmp_1] -= O[r2 + 6] * b_K_k[P_apr_tmp_tmp_0];
        b_K_k[P_apr_tmp_tmp_1] /= O[r3 + 6];
        b_K_k[P_apr_tmp_tmp_0] -= O[r3 + 3] * b_K_k[P_apr_tmp_tmp_1];
        b_K_k[P_apr_tmp_tmp] -= b_K_k[P_apr_tmp_tmp_1] * O[r3];
        b_K_k[P_apr_tmp_tmp] -= b_K_k[P_apr_tmp_tmp_0] * O[r2];
      }

      for (i = 0; i < 3; i++) {
        b_idx_1 = 0.0;
        for (r2 = 0; r2 < 12; r2++) {
          b_idx_1 += (real_T)O_tmp[3 * r2 + i] * x_apr[r2];
        }

        z_2[i] = z[i] - b_idx_1;
      }

      maxval = z_2[1];
      wak_idx_0 = z_2[0];
      wak_idx_1 = z_2[2];
      for (i = 0; i <= 10; i += 2) {
        tmp_3 = _mm_loadu_pd(&b_K_k[i + 12]);
        tmp_2 = _mm_loadu_pd(&b_K_k[i]);
        tmp = _mm_loadu_pd(&b_K_k[i + 24]);
        tmp_0 = _mm_loadu_pd(&x_apr[i]);
        _mm_storeu_pd(&v[i], _mm_add_pd(_mm_add_pd(_mm_add_pd(_mm_mul_pd(tmp_3,
          _mm_set1_pd(maxval)), _mm_mul_pd(tmp_2, _mm_set1_pd(wak_idx_0))),
          _mm_mul_pd(tmp, _mm_set1_pd(wak_idx_1))), tmp_0));
      }

      memset(&c_I[0], 0, 144U * sizeof(int8_T));
      for (i = 0; i < 12; i++) {
        c_I[i + 12 * i] = 1;
      }

      for (i = 0; i < 12; i++) {
        O_tmp_1 = O_tmp[3 * i + 1];
        O_tmp_2 = O_tmp[3 * i];
        O_tmp_3 = O_tmp[3 * i + 2];
        for (r2 = 0; r2 < 12; r2++) {
          r3 = 12 * i + r2;
          P_apr_0[r3] = (real_T)c_I[r3] - ((b_K_k[r2 + 12] * (real_T)O_tmp_1 +
            (real_T)O_tmp_2 * b_K_k[r2]) + b_K_k[r2 + 24] * (real_T)O_tmp_3);
          P_apr_1[i + 12 * r2] = 0.0;
        }
      }

      for (i = 0; i < 12; i++) {
        for (r2 = 0; r2 < 12; r2++) {
          b_idx_1 = P_apr[12 * i + r2];
          for (r3 = 0; r3 <= 10; r3 += 2) {
            tmp_3 = _mm_loadu_pd(&P_apr_0[12 * r2 + r3]);
            rtemp = 12 * i + r3;
            tmp_2 = _mm_loadu_pd(&P_apr_1[rtemp]);
            _mm_storeu_pd(&P_apr_1[rtemp], _mm_add_pd(_mm_mul_pd(tmp_3,
              _mm_set1_pd(b_idx_1)), tmp_2));
          }
        }
      }

      memcpy(&P_apr[0], &P_apr_1[0], 144U * sizeof(real_T));
    } else if ((rtDW.AHRSEKF.zFlag[0] == 1.0) && (rtDW.AHRSEKF.zFlag[1] == 1.0) &&
               (rtDW.AHRSEKF.zFlag[2] == 0.0)) {
      for (i = 0; i < 72; i++) {
        K_k[i] = h[i];
        S_k_tmp[i] = e_a[i];
      }

      for (i = 0; i < 12; i++) {
        for (r2 = 0; r2 < 6; r2++) {
          S_k_tmp_0[r2 + 6 * i] = 0.0;
        }

        for (r2 = 0; r2 < 12; r2++) {
          b_idx_1 = P_apr[12 * i + r2];
          for (r3 = 0; r3 < 6; r3++) {
            rtemp = 6 * i + r3;
            S_k_tmp_0[rtemp] += (real_T)S_k_tmp[6 * r2 + r3] * b_idx_1;
          }
        }
      }

      for (i = 0; i < 6; i++) {
        for (r2 = 0; r2 < 6; r2++) {
          S_k[r2 + 6 * i] = 0.0;
        }

        for (r2 = 0; r2 < 12; r2++) {
          r3 = (int32_T)K_k[12 * i + r2];
          for (rtemp = 0; rtemp <= 4; rtemp += 2) {
            tmp_3 = _mm_loadu_pd(&S_k_tmp_0[6 * r2 + rtemp]);
            P_apr_tmp_tmp = 6 * i + rtemp;
            tmp_2 = _mm_loadu_pd(&S_k[P_apr_tmp_tmp]);
            _mm_storeu_pd(&S_k[P_apr_tmp_tmp], _mm_add_pd(_mm_mul_pd(tmp_3,
              _mm_set1_pd(r3)), tmp_2));
          }
        }
      }

      z_0[0] = rtDW.AHRSEKF.r_gyro;
      z_0[1] = rtDW.AHRSEKF.r_gyro;
      z_0[2] = rtDW.AHRSEKF.r_gyro;
      z_0[3] = rtDW.AHRSEKF.r_accel;
      z_0[4] = rtDW.AHRSEKF.r_accel;
      z_0[5] = rtDW.AHRSEKF.r_accel;
      for (i = 0; i < 6; i++) {
        S_k[7 * i] += z_0[i];
        memset(&S_k_tmp_0[i * 12], 0, 12U * sizeof(real_T));
        for (r2 = 0; r2 < 12; r2++) {
          r3 = (int32_T)K_k[12 * i + r2];
          for (rtemp = 0; rtemp <= 10; rtemp += 2) {
            tmp_3 = _mm_loadu_pd(&P_apr[12 * r2 + rtemp]);
            P_apr_tmp_tmp = 12 * i + rtemp;
            tmp_2 = _mm_loadu_pd(&S_k_tmp_0[P_apr_tmp_tmp]);
            _mm_storeu_pd(&S_k_tmp_0[P_apr_tmp_tmp], _mm_add_pd(_mm_mul_pd(tmp_3,
              _mm_set1_pd(r3)), tmp_2));
          }
        }
      }

      memcpy(&K_k[0], &S_k_tmp_0[0], 72U * sizeof(real_T));
      mrdiv_hw4(K_k, S_k);
      for (i = 0; i < 6; i++) {
        b_idx_1 = 0.0;
        for (r2 = 0; r2 < 12; r2++) {
          b_idx_1 += (real_T)S_k_tmp[6 * r2 + i] * x_apr[r2];
        }

        z_0[i] = z[i] - b_idx_1;
      }

      for (i = 0; i < 12; i++) {
        b_idx_1 = 0.0;
        for (r2 = 0; r2 < 6; r2++) {
          b_idx_1 += K_k[12 * r2 + i] * z_0[r2];
        }

        v[i] = x_apr[i] + b_idx_1;
      }

      memset(&c_I[0], 0, 144U * sizeof(int8_T));
      for (i = 0; i < 12; i++) {
        c_I[i + 12 * i] = 1;
      }

      for (i = 0; i < 12; i++) {
        for (r2 = 0; r2 < 12; r2++) {
          b_idx_1 = 0.0;
          for (r3 = 0; r3 < 6; r3++) {
            b_idx_1 += K_k[12 * r3 + i] * (real_T)S_k_tmp[6 * r2 + r3];
          }

          r3 = 12 * r2 + i;
          P_apr_0[r3] = (real_T)c_I[r3] - b_idx_1;
          P_apr_1[r2 + 12 * i] = 0.0;
        }
      }

      for (i = 0; i < 12; i++) {
        for (r2 = 0; r2 < 12; r2++) {
          b_idx_1 = P_apr[12 * i + r2];
          for (r3 = 0; r3 <= 10; r3 += 2) {
            tmp_3 = _mm_loadu_pd(&P_apr_0[12 * r2 + r3]);
            rtemp = 12 * i + r3;
            tmp_2 = _mm_loadu_pd(&P_apr_1[rtemp]);
            _mm_storeu_pd(&P_apr_1[rtemp], _mm_add_pd(_mm_mul_pd(tmp_3,
              _mm_set1_pd(b_idx_1)), tmp_2));
          }
        }
      }

      memcpy(&P_apr[0], &P_apr_1[0], 144U * sizeof(real_T));
    } else if ((rtDW.AHRSEKF.zFlag[0] == 1.0) && (rtDW.AHRSEKF.zFlag[1] == 0.0) &&
               (rtDW.AHRSEKF.zFlag[2] == 1.0)) {
      for (i = 0; i < 72; i++) {
        K_k[i] = i_0[i];
        S_k_tmp[i] = f_a[i];
      }

      for (i = 0; i < 12; i++) {
        for (r2 = 0; r2 < 6; r2++) {
          S_k_tmp_0[r2 + 6 * i] = 0.0;
        }

        for (r2 = 0; r2 < 12; r2++) {
          b_idx_1 = P_apr[12 * i + r2];
          for (r3 = 0; r3 < 6; r3++) {
            rtemp = 6 * i + r3;
            S_k_tmp_0[rtemp] += (real_T)S_k_tmp[6 * r2 + r3] * b_idx_1;
          }
        }
      }

      for (i = 0; i < 6; i++) {
        for (r2 = 0; r2 < 6; r2++) {
          S_k[r2 + 6 * i] = 0.0;
        }

        for (r2 = 0; r2 < 12; r2++) {
          r3 = (int32_T)K_k[12 * i + r2];
          for (rtemp = 0; rtemp <= 4; rtemp += 2) {
            tmp_3 = _mm_loadu_pd(&S_k_tmp_0[6 * r2 + rtemp]);
            P_apr_tmp_tmp = 6 * i + rtemp;
            tmp_2 = _mm_loadu_pd(&S_k[P_apr_tmp_tmp]);
            _mm_storeu_pd(&S_k[P_apr_tmp_tmp], _mm_add_pd(_mm_mul_pd(tmp_3,
              _mm_set1_pd(r3)), tmp_2));
          }
        }
      }

      z_0[0] = rtDW.AHRSEKF.r_gyro;
      z_0[1] = rtDW.AHRSEKF.r_gyro;
      z_0[2] = rtDW.AHRSEKF.r_gyro;
      z_0[3] = rtDW.AHRSEKF.r_mag;
      z_0[4] = rtDW.AHRSEKF.r_mag;
      z_0[5] = rtDW.AHRSEKF.r_mag;
      for (i = 0; i < 6; i++) {
        S_k[7 * i] += z_0[i];
        memset(&S_k_tmp_0[i * 12], 0, 12U * sizeof(real_T));
        for (r2 = 0; r2 < 12; r2++) {
          r3 = (int32_T)K_k[12 * i + r2];
          for (rtemp = 0; rtemp <= 10; rtemp += 2) {
            tmp_3 = _mm_loadu_pd(&P_apr[12 * r2 + rtemp]);
            P_apr_tmp_tmp = 12 * i + rtemp;
            tmp_2 = _mm_loadu_pd(&S_k_tmp_0[P_apr_tmp_tmp]);
            _mm_storeu_pd(&S_k_tmp_0[P_apr_tmp_tmp], _mm_add_pd(_mm_mul_pd(tmp_3,
              _mm_set1_pd(r3)), tmp_2));
          }
        }
      }

      memcpy(&K_k[0], &S_k_tmp_0[0], 72U * sizeof(real_T));
      mrdiv_hw4(K_k, S_k);
      z_0[0] = rtU.gx;
      z_0[3] = maxval;
      z_0[1] = rtU.gy;
      z_0[4] = maxval;
      z_0[2] = rtU.gz;
      z_0[5] = maxval;
      for (i = 0; i < 6; i++) {
        S_k_tmp_1[i] = 0.0;
      }

      for (i = 0; i < 12; i++) {
        b_idx_1 = x_apr[i];
        for (r2 = 0; r2 < 6; r2++) {
          S_k_tmp_1[r2] += (real_T)S_k_tmp[6 * i + r2] * b_idx_1;
        }
      }

      for (i = 0; i <= 4; i += 2) {
        tmp_3 = _mm_loadu_pd(&z_0[i]);
        tmp_2 = _mm_loadu_pd(&S_k_tmp_1[i]);
        _mm_storeu_pd(&z_1[i], _mm_sub_pd(tmp_3, tmp_2));
      }

      for (i = 0; i < 12; i++) {
        b_idx_1 = 0.0;
        for (r2 = 0; r2 < 6; r2++) {
          b_idx_1 += K_k[12 * r2 + i] * z_1[r2];
        }

        v[i] = x_apr[i] + b_idx_1;
      }

      memset(&c_I[0], 0, 144U * sizeof(int8_T));
      for (i = 0; i < 12; i++) {
        c_I[i + 12 * i] = 1;
      }

      for (i = 0; i < 12; i++) {
        for (r2 = 0; r2 < 12; r2++) {
          b_idx_1 = 0.0;
          for (r3 = 0; r3 < 6; r3++) {
            b_idx_1 += K_k[12 * r3 + i] * (real_T)S_k_tmp[6 * r2 + r3];
          }

          r3 = 12 * r2 + i;
          P_apr_0[r3] = (real_T)c_I[r3] - b_idx_1;
          P_apr_1[r2 + 12 * i] = 0.0;
        }
      }

      for (i = 0; i < 12; i++) {
        for (r2 = 0; r2 < 12; r2++) {
          b_idx_1 = P_apr[12 * i + r2];
          for (r3 = 0; r3 <= 10; r3 += 2) {
            tmp_3 = _mm_loadu_pd(&P_apr_0[12 * r2 + r3]);
            rtemp = 12 * i + r3;
            tmp_2 = _mm_loadu_pd(&P_apr_1[rtemp]);
            _mm_storeu_pd(&P_apr_1[rtemp], _mm_add_pd(_mm_mul_pd(tmp_3,
              _mm_set1_pd(b_idx_1)), tmp_2));
          }
        }
      }

      memcpy(&P_apr[0], &P_apr_1[0], 144U * sizeof(real_T));
    } else {
      memcpy(&v[0], &x_apr[0], 12U * sizeof(real_T));
    }

    tmp_3 = _mm_set_pd(norm(&v[9]), norm(&v[6]));
    _mm_storeu_pd(&tmp_1[0], _mm_div_pd(_mm_set_pd(v[9], -v[6]), tmp_3));
    wak_idx_0 = tmp_1[0];
    wak_idx_2 = tmp_1[1];
    _mm_storeu_pd(&tmp_1[0], _mm_div_pd(_mm_set_pd(v[10], -v[7]), tmp_3));
    wak_idx_1 = tmp_1[0];
    b_idx_1 = tmp_1[1];
    _mm_storeu_pd(&tmp_1[0], _mm_div_pd(_mm_set_pd(v[11], -v[8]), tmp_3));
    muk[0] = wak_idx_1 * tmp_1[1] - b_idx_1 * tmp_1[0];
    muk[1] = wak_idx_2 * tmp_1[0] - wak_idx_0 * tmp_1[1];
    muk[2] = wak_idx_0 * b_idx_1 - wak_idx_2 * wak_idx_1;
    b_idx_1 = norm(muk);
    tmp_3 = _mm_div_pd(_mm_loadu_pd(&muk[0]), _mm_set1_pd(b_idx_1));
    _mm_storeu_pd(&muk[0], tmp_3);
    muk[2] /= b_idx_1;
    _mm_storeu_pd(&zek[0], _mm_sub_pd(_mm_mul_pd(_mm_set_pd(wak_idx_0, muk[1]),
      _mm_set_pd(muk[2], tmp_1[0])), _mm_mul_pd(_mm_set_pd(muk[0], wak_idx_1),
      _mm_set_pd(tmp_1[0], muk[2]))));
    zek[2] = muk[0] * wak_idx_1 - wak_idx_0 * muk[1];
    memcpy(&rtDW.AHRSEKF.xa_apo[0], &v[0], 12U * sizeof(real_T));
    memcpy(&rtDW.AHRSEKF.Pa_apo[0], &P_apr[0], 144U * sizeof(real_T));
    rtDW.AHRSEKF.eulerAngles[0] = atan2(wak_idx_1, tmp_1[0]);
    rtDW.AHRSEKF.eulerAngles[1] = -asin(wak_idx_0);
    rtDW.AHRSEKF.eulerAngles[2] = atan2(muk[0], zek[0] / norm(zek));
    memcpy(&rtDW.AHRSEKF.x_apo[0], &rtDW.AHRSEKF.xa_apo[0], 12U * sizeof(real_T));
    memcpy(&rtDW.AHRSEKF.P_apo[0], &rtDW.AHRSEKF.Pa_apo[0], 144U * sizeof(real_T));
  }

  /* End of MATLAB Function: '<S3>/EKF' */

  /* Sum: '<S1>/Sum2' incorporates:
   *  Inport: '<Root>/motor_a_actual_speed'
   *  Inport: '<Root>/speed_a_target'
   */
  rtb_Sum2 = rtU.speed_a_target - rtU.motor_a_actual_speed;

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
  rtb_FilterCoefficient = (SPEED_Kd * rtb_Sum2 - rtDW.Filter_DSTATE) * 100.0F;

  /* Switch: '<S1>/Switch' incorporates:
   *  Constant: '<S1>/Constant'
   *  DataTypeConversion: '<S1>/Data Type Conversion1'
   *  DiscreteIntegrator: '<S38>/Integrator'
   *  Gain: '<S43>/Proportional Gain'
   *  Inport: '<Root>/running_flag'
   *  Sum: '<S47>/Sum'
   */
  if (rtU.running_flag > 0) {
    rtb_Switch = (int16_T)floorf((SPEED_Kp * rtb_Sum2 + rtDW.Integrator_DSTATE)
      + rtb_FilterCoefficient);
  } else {
    rtb_Switch = 0;
  }

  /* End of Switch: '<S1>/Switch' */

  /* Update for DiscreteIntegrator: '<S38>/Integrator' incorporates:
   *  Gain: '<S35>/Integral Gain'
   */
  rtDW.Integrator_DSTATE += SPEED_Ki * rtb_Sum2;
  rtDW.Integrator_PrevResetState = (int8_T)rtb_LogicalOperator;

  /* Update for DiscreteIntegrator: '<S33>/Filter' incorporates:
   *  DiscreteIntegrator: '<S38>/Integrator'
   */
  rtDW.Filter_DSTATE += 0.005F * rtb_FilterCoefficient;
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
  /* End of Outputs for SubSystem: '<Root>/task_5ms' */
}

/* Model initialize function */
void blinky_initialize(void)
{
  /* (no initialization code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
