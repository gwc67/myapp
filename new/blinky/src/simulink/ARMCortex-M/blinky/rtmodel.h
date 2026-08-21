/*
 * File: rtmodel.h
 *
 * Code generated for Simulink model 'blinky'.
 *
 * Model version                  : 1.59
 * Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
 * C/C++ source code generated on : Fri Aug 21 19:38:48 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#ifndef rtmodel_h_
#define rtmodel_h_
#include "blinky.h"

/* Macros generated for backwards compatibility  */
#ifndef rtmGetStopRequested
#define rtmGetStopRequested(rtm)       ((void*) 0)
#endif

/* Model wrapper function */
/* Use this function only if you need to maintain compatibility with an existing static main program. */
extern void blinky_step(int_T tid);

#endif                                 /* rtmodel_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
