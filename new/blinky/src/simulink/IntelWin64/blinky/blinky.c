/*
 * File: blinky.c
 *
 * Code generated for Simulink model 'blinky'.
 *
 * Model version                  : 1.2
 * Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
 * C/C++ source code generated on : Tue Aug 18 14:34:33 2026
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

/* Named constants for Chart: '<Root>/led_state' */
#define IN_led0_off                    ((uint8_T)1U)
#define IN_led0_on                     ((uint8_T)2U)

/* Block signals and states (default storage) */
DW rtDW;

/* External outputs (root outports fed by signals with default storage) */
ExtY rtY;

/* Real-time model */
static RT_MODEL rtM_;
RT_MODEL *const rtM = &rtM_;

/* Model step function */
void blinky_step(void)
{
  /* Chart: '<Root>/led_state' */
  if (rtDW.temporalCounter_i1 < 3) {
    rtDW.temporalCounter_i1++;
  }

  if (rtDW.is_active_c3_blinky == 0) {
    rtDW.is_active_c3_blinky = 1U;
    rtDW.temporalCounter_i1 = 0U;
    rtDW.is_c3_blinky = IN_led0_on;

    /* Outport: '<Root>/led_output' */
    rtY.led_output = 1U;
  } else if (rtDW.is_c3_blinky == IN_led0_off) {
    /* Outport: '<Root>/led_output' */
    rtY.led_output = 0U;
    if (rtDW.temporalCounter_i1 >= 3) {
      rtDW.temporalCounter_i1 = 0U;
      rtDW.is_c3_blinky = IN_led0_on;

      /* Outport: '<Root>/led_output' */
      rtY.led_output = 1U;
    }
  } else {
    /* Outport: '<Root>/led_output' */
    /* case IN_led0_on: */
    rtY.led_output = 1U;
    if (rtDW.temporalCounter_i1 >= 3) {
      rtDW.temporalCounter_i1 = 0U;
      rtDW.is_c3_blinky = IN_led0_off;

      /* Outport: '<Root>/led_output' */
      rtY.led_output = 0U;
    }
  }

  /* End of Chart: '<Root>/led_state' */
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
