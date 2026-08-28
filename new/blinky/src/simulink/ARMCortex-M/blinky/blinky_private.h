/*
 * File: blinky_private.h
 *
 * Code generated for Simulink model 'blinky'.
 *
 * Model version                  : 1.79
 * Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
 * C/C++ source code generated on : Fri Aug 28 17:40:41 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#ifndef blinky_private_h_
#define blinky_private_h_
#include "rtwtypes.h"
#include "blinky_types.h"
#include "blinky.h"

extern real32_T rt_hypotf(real32_T u0, real32_T u1);
extern uint32_T plook_u32ff_binc(real32_T u, const real32_T bp[], uint32_T
  maxIndex, real32_T *fraction);
extern real32_T intrp2d_fu32fl(const uint32_T bpIndex[], const real32_T frac[],
  const real32_T table[], const uint32_T stride);
extern uint32_T binsearch_u32f(real32_T u, const real32_T bp[], uint32_T
  startIndex, uint32_T maxIndex);

#endif                                 /* blinky_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
