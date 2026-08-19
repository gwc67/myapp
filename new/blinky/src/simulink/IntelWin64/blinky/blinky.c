/*
 * File: blinky.c
 *
 * Code generated for Simulink model 'blinky'.
 *
 * Model version                  : 1.34
 * Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
 * C/C++ source code generated on : Wed Aug 19 21:44:33 2026
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
#include <math.h>

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
static void task_5ms_high(void);

/* Function for Chart: '<Root>/Chart' */
static void task_5ms_high(void)
{
  real32_T Filter;
  real32_T Integrator;
  real32_T rtb_FilterCoefficient;
  real32_T rtb_Sum2;
  real32_T u0;
  uint32_T FunctionCallSubsystem_ELAPS_T;
  if (rtDW.task_5ms_count == 1) {
    /* Outputs for Function Call SubSystem: '<Root>/Function-Call Subsystem' */
    if (rtDW.FunctionCallSubsystem_RESET_ELA) {
      FunctionCallSubsystem_ELAPS_T = 0U;
    } else {
      FunctionCallSubsystem_ELAPS_T = rtM->Timing.clockTick0 -
        rtDW.FunctionCallSubsystem_PREV_T;
    }

    rtDW.FunctionCallSubsystem_PREV_T = rtM->Timing.clockTick0;
    rtDW.FunctionCallSubsystem_RESET_ELA = false;

    /* Sum: '<S2>/Sum2' incorporates:
     *  Inport: '<Root>/motor_a_actual_speed'
     *  Inport: '<Root>/speed_a_target'
     */
    rtb_Sum2 = rtU.speed_a_target - rtU.motor_a_actual_speed;

    /* DiscreteIntegrator: '<S33>/Filter' */
    if (rtDW.Filter_SYSTEM_ENABLE != 0) {
      /* DiscreteIntegrator: '<S33>/Filter' */
      Filter = rtDW.Filter_DSTATE;
    } else {
      /* DiscreteIntegrator: '<S33>/Filter' */
      Filter = 0.001F * (real32_T)FunctionCallSubsystem_ELAPS_T
        * rtDW.Filter_PREV_U + rtDW.Filter_DSTATE;
    }

    /* End of DiscreteIntegrator: '<S33>/Filter' */

    /* Gain: '<S41>/Filter Coefficient' incorporates:
     *  Gain: '<S31>/Derivative Gain'
     *  Sum: '<S33>/SumD'
     */
    rtb_FilterCoefficient = (0.0F - Filter) * 100.0F;

    /* DiscreteIntegrator: '<S38>/Integrator' */
    if (rtDW.Integrator_SYSTEM_ENABLE != 0) {
      /* DiscreteIntegrator: '<S38>/Integrator' */
      Integrator = rtDW.Integrator_DSTATE;
    } else {
      /* DiscreteIntegrator: '<S38>/Integrator' */
      Integrator = 0.001F * (real32_T)FunctionCallSubsystem_ELAPS_T
        * rtDW.Integrator_PREV_U + rtDW.Integrator_DSTATE;
    }

    /* End of DiscreteIntegrator: '<S38>/Integrator' */

    /* Abs: '<S2>/Abs' incorporates:
     *  Sum: '<S47>/Sum'
     */
    u0 = fabsf((rtb_Sum2 + Integrator) + rtb_FilterCoefficient);

    /* Saturate: '<S2>/Saturation' */
    if (u0 > 1000.0F) {
      /* Outport: '<Root>/motor_a_pwm' */
      rtY.motor_a_pwm = 1000.0F;
    } else {
      /* Outport: '<Root>/motor_a_pwm' */
      rtY.motor_a_pwm = u0;
    }

    /* End of Saturate: '<S2>/Saturation' */

    /* Update for DiscreteIntegrator: '<S33>/Filter' */
    rtDW.Filter_SYSTEM_ENABLE = 0U;
    rtDW.Filter_DSTATE = Filter;
    rtDW.Filter_PREV_U = rtb_FilterCoefficient;

    /* Update for DiscreteIntegrator: '<S38>/Integrator' */
    rtDW.Integrator_SYSTEM_ENABLE = 0U;
    rtDW.Integrator_DSTATE = Integrator;
    rtDW.Integrator_PREV_U = rtb_Sum2;

    /* End of Outputs for SubSystem: '<Root>/Function-Call Subsystem' */
  }
}

/* Model step function */
void blinky_step(void)
{
  uint32_T tmp;

  /* Chart: '<Root>/Chart' */
  tmp = rtDW.task_5ms_count + 1U;
  if (rtDW.task_5ms_count + 1U > 255U) {
    tmp = 255U;
  }

  rtDW.task_5ms_count = (uint8_T)tmp;
  if ((uint8_T)(rtDW.task_5ms_count - (uint32_T)((int32_T)(rtDW.task_5ms_count /
         5U) * 5)) == 0) {
    rtDW.task_5ms_count = 0U;
  }

  tmp = rtDW.task_10ms_count + 1U;
  if (rtDW.task_10ms_count + 1U > 255U) {
    tmp = 255U;
  }

  rtDW.task_10ms_count = (uint8_T)tmp;
  if ((uint8_T)(rtDW.task_10ms_count - (uint32_T)((int32_T)(rtDW.task_10ms_count
         / 10U) * 10)) == 0) {
    rtDW.task_10ms_count = 0U;
  }

  tmp = rtDW.task_20ms_count + 1U;
  if (rtDW.task_20ms_count + 1U > 255U) {
    tmp = 255U;
  }

  rtDW.task_20ms_count = (uint8_T)tmp;
  if ((uint8_T)(rtDW.task_20ms_count - (uint32_T)((int32_T)(rtDW.task_20ms_count
         / 20U) * 20)) == 0) {
    rtDW.task_20ms_count = 0U;
  }

  tmp = rtDW.task_50ms_count + 1U;
  if (rtDW.task_50ms_count + 1U > 255U) {
    tmp = 255U;
  }

  rtDW.task_50ms_count = (uint8_T)tmp;
  if ((uint8_T)(rtDW.task_50ms_count - (uint32_T)((int32_T)(rtDW.task_50ms_count
         / 50U) * 50)) == 0) {
    rtDW.task_50ms_count = 0U;
  }

  task_5ms_high();

  /* End of Chart: '<Root>/Chart' */

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The resolution of this integer timer is 0.001, which is the step size
   * of the task. Size of "clockTick0" ensures timer will not overflow during the
   * application lifespan selected.
   */
  rtM->Timing.clockTick0++;
}

/* Model initialize function */
void blinky_initialize(void)
{
  /* Enable for Chart: '<Root>/Chart' incorporates:
   *  SubSystem: '<Root>/Function-Call Subsystem'
   */
  rtDW.FunctionCallSubsystem_RESET_ELA = true;

  /* Enable for DiscreteIntegrator: '<S33>/Filter' */
  rtDW.Filter_SYSTEM_ENABLE = 1U;

  /* Enable for DiscreteIntegrator: '<S38>/Integrator' */
  rtDW.Integrator_SYSTEM_ENABLE = 1U;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
