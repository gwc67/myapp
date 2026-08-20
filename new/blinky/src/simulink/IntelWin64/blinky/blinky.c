/*
 * File: blinky.c
 *
 * Code generated for Simulink model 'blinky'.
 *
 * Model version                  : 1.35
 * Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
 * C/C++ source code generated on : Thu Aug 20 15:37:58 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include "blinky.h"
#include <math.h>
#include "rtwtypes.h"

/* Block signals and states (default storage) */
DW rtDW;

/* External inputs (root inport signals with default storage) */
ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
ExtY rtY;

/* Real-time model */
static RT_MODEL rtM_;
RT_MODEL *const rtM = &rtM_;

/* Model step function for TID0 */
void blinky_step0(void)                /* Sample time: [0.001s, 0.0s] */
{
  /* (no output/update code required) */
}

/* Model step function for TID1 */
void blinky_step1(void)                /* Sample time: [0.005s, 0.0s] */
{
  real32_T rtb_FilterCoefficient;
  real32_T rtb_Sum2;
  int16_T rtb_Switch;
  boolean_T rtb_LogicalOperator;

  /* Outputs for Atomic SubSystem: '<Root>/task_5ms' */
  /* Sum: '<S1>/Sum2' incorporates:
   *  Inport: '<Root>/motor_a_actual_speed'
   *  Inport: '<Root>/speed_a_target'
   */
  rtb_Sum2 = rtU.speed_a_target - rtU.motor_a_actual_speed;

  /* Logic: '<S1>/Logical Operator' incorporates:
   *  Inport: '<Root>/running_flag'
   */
  rtb_LogicalOperator = (rtU.running_flag == 0);

  /* DiscreteIntegrator: '<S37>/Integrator' */
  if (rtb_LogicalOperator || (rtDW.Integrator_PrevResetState != 0)) {
    rtDW.Integrator_DSTATE = 0.0F;
  }

  /* DiscreteIntegrator: '<S32>/Filter' */
  if (rtb_LogicalOperator || (rtDW.Filter_PrevResetState != 0)) {
    rtDW.Filter_DSTATE = 0.0F;
  }

  /* Gain: '<S40>/Filter Coefficient' incorporates:
   *  DiscreteIntegrator: '<S32>/Filter'
   *  Gain: '<S30>/Derivative Gain'
   *  Sum: '<S32>/SumD'
   */
  rtb_FilterCoefficient = (SPEED_Kd * rtb_Sum2 - rtDW.Filter_DSTATE) * 100.0F;

  /* Switch: '<S1>/Switch' incorporates:
   *  Constant: '<S1>/Constant'
   *  DataTypeConversion: '<S1>/Data Type Conversion1'
   *  DiscreteIntegrator: '<S37>/Integrator'
   *  Gain: '<S42>/Proportional Gain'
   *  Inport: '<Root>/running_flag'
   *  Sum: '<S46>/Sum'
   */
  if (rtU.running_flag > 0) {
    rtb_Switch = (int16_T)floorf((SPEED_Kp * rtb_Sum2 + rtDW.Integrator_DSTATE)
      + rtb_FilterCoefficient);
  } else {
    rtb_Switch = 0;
  }

  /* End of Switch: '<S1>/Switch' */

  /* Update for DiscreteIntegrator: '<S37>/Integrator' incorporates:
   *  Gain: '<S34>/Integral Gain'
   */
  rtDW.Integrator_DSTATE += SPEED_Ki * rtb_Sum2;
  rtDW.Integrator_PrevResetState = (int8_T)rtb_LogicalOperator;

  /* Update for DiscreteIntegrator: '<S32>/Filter' incorporates:
   *  DiscreteIntegrator: '<S37>/Integrator'
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
