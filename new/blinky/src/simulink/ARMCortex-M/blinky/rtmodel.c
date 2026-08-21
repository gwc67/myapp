/*
 * File: rtmodel.c
 *
 * Code generated for Simulink model 'blinky'.
 *
 * Model version                  : 1.59
 * Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
 * C/C++ source code generated on : Fri Aug 21 19:44:16 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include "rtmodel.h"

/* Use this function only if you need to maintain compatibility with an existing static main program. */
void blinky_step(int_T tid)
{
  switch (tid) {
   case 0 :
    blinky_step0();
    break;

   case 1 :
    blinky_step1();
    break;

   default :
    /* do nothing */
    break;
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
