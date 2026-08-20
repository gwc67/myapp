/*
 * File: blinky.h
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

#ifndef blinky_h_
#define blinky_h_
#ifndef blinky_COMMON_INCLUDES_
#define blinky_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* blinky_COMMON_INCLUDES_ */

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Exported data define */

/* Definition for custom storage class: Define */
#define SPEED_Kd                       0.0F                      /* Referenced by: '<S30>/Derivative Gain' */
#define SPEED_Ki                       1.0F                      /* Referenced by: '<S34>/Integral Gain' */
#define SPEED_Kp                       6.0F                      /* Referenced by: '<S42>/Proportional Gain' */

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  real32_T Integrator_DSTATE;          /* '<S37>/Integrator' */
  real32_T Filter_DSTATE;              /* '<S32>/Filter' */
  int8_T Integrator_PrevResetState;    /* '<S37>/Integrator' */
  int8_T Filter_PrevResetState;        /* '<S32>/Filter' */
} DW;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real32_T motor_a_actual_speed;       /* '<Root>/motor_a_actual_speed' */
  real32_T speed_a_target;             /* '<Root>/speed_a_target' */
  uint8_T running_flag;                /* '<Root>/running_flag' */
} ExtU;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  int16_T motor_a_pwm;                 /* '<Root>/motor_a_pwm' */
} ExtY;

/* Real-time Model Data Structure */
struct tag_RTM {
  const char_T * volatile errorStatus;
};

/* Block signals and states (default storage) */
extern DW rtDW;

/* External inputs (root inport signals with default storage) */
extern ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY rtY;

/* Model entry point functions */
extern void blinky_initialize(void);
extern void blinky_step0(void);        /* Sample time: [0.001s, 0.0s] */
extern void blinky_step1(void);        /* Sample time: [0.005s, 0.0s] */

/* Real-time Model object */
extern RT_MODEL *const rtM;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<Root>/Scope4' : Unused code path elimination
 * Block '<S1>/Scope2' : Unused code path elimination
 * Block '<S1>/Data Type Conversion' : Eliminate redundant data type conversion
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'blinky'
 * '<S1>'   : 'blinky/task_5ms'
 * '<S2>'   : 'blinky/task_5ms/Discrete PID Controller1'
 * '<S3>'   : 'blinky/task_5ms/Discrete PID Controller1/Anti-windup'
 * '<S4>'   : 'blinky/task_5ms/Discrete PID Controller1/D Gain'
 * '<S5>'   : 'blinky/task_5ms/Discrete PID Controller1/External Derivative'
 * '<S6>'   : 'blinky/task_5ms/Discrete PID Controller1/Filter'
 * '<S7>'   : 'blinky/task_5ms/Discrete PID Controller1/Filter ICs'
 * '<S8>'   : 'blinky/task_5ms/Discrete PID Controller1/I Gain'
 * '<S9>'   : 'blinky/task_5ms/Discrete PID Controller1/Ideal P Gain'
 * '<S10>'  : 'blinky/task_5ms/Discrete PID Controller1/Ideal P Gain Fdbk'
 * '<S11>'  : 'blinky/task_5ms/Discrete PID Controller1/Integrator'
 * '<S12>'  : 'blinky/task_5ms/Discrete PID Controller1/Integrator ICs'
 * '<S13>'  : 'blinky/task_5ms/Discrete PID Controller1/N Copy'
 * '<S14>'  : 'blinky/task_5ms/Discrete PID Controller1/N Gain'
 * '<S15>'  : 'blinky/task_5ms/Discrete PID Controller1/P Copy'
 * '<S16>'  : 'blinky/task_5ms/Discrete PID Controller1/Parallel P Gain'
 * '<S17>'  : 'blinky/task_5ms/Discrete PID Controller1/Reset Signal'
 * '<S18>'  : 'blinky/task_5ms/Discrete PID Controller1/Saturation'
 * '<S19>'  : 'blinky/task_5ms/Discrete PID Controller1/Saturation Fdbk'
 * '<S20>'  : 'blinky/task_5ms/Discrete PID Controller1/Sum'
 * '<S21>'  : 'blinky/task_5ms/Discrete PID Controller1/Sum Fdbk'
 * '<S22>'  : 'blinky/task_5ms/Discrete PID Controller1/Tracking Mode'
 * '<S23>'  : 'blinky/task_5ms/Discrete PID Controller1/Tracking Mode Sum'
 * '<S24>'  : 'blinky/task_5ms/Discrete PID Controller1/Tsamp - Integral'
 * '<S25>'  : 'blinky/task_5ms/Discrete PID Controller1/Tsamp - Ngain'
 * '<S26>'  : 'blinky/task_5ms/Discrete PID Controller1/postSat Signal'
 * '<S27>'  : 'blinky/task_5ms/Discrete PID Controller1/preInt Signal'
 * '<S28>'  : 'blinky/task_5ms/Discrete PID Controller1/preSat Signal'
 * '<S29>'  : 'blinky/task_5ms/Discrete PID Controller1/Anti-windup/Passthrough'
 * '<S30>'  : 'blinky/task_5ms/Discrete PID Controller1/D Gain/Internal Parameters'
 * '<S31>'  : 'blinky/task_5ms/Discrete PID Controller1/External Derivative/Error'
 * '<S32>'  : 'blinky/task_5ms/Discrete PID Controller1/Filter/Disc. Forward Euler Filter'
 * '<S33>'  : 'blinky/task_5ms/Discrete PID Controller1/Filter ICs/Internal IC - Filter'
 * '<S34>'  : 'blinky/task_5ms/Discrete PID Controller1/I Gain/Internal Parameters'
 * '<S35>'  : 'blinky/task_5ms/Discrete PID Controller1/Ideal P Gain/Passthrough'
 * '<S36>'  : 'blinky/task_5ms/Discrete PID Controller1/Ideal P Gain Fdbk/Disabled'
 * '<S37>'  : 'blinky/task_5ms/Discrete PID Controller1/Integrator/Discrete'
 * '<S38>'  : 'blinky/task_5ms/Discrete PID Controller1/Integrator ICs/Internal IC'
 * '<S39>'  : 'blinky/task_5ms/Discrete PID Controller1/N Copy/Disabled'
 * '<S40>'  : 'blinky/task_5ms/Discrete PID Controller1/N Gain/Internal Parameters'
 * '<S41>'  : 'blinky/task_5ms/Discrete PID Controller1/P Copy/Disabled'
 * '<S42>'  : 'blinky/task_5ms/Discrete PID Controller1/Parallel P Gain/Internal Parameters'
 * '<S43>'  : 'blinky/task_5ms/Discrete PID Controller1/Reset Signal/External Reset'
 * '<S44>'  : 'blinky/task_5ms/Discrete PID Controller1/Saturation/Passthrough'
 * '<S45>'  : 'blinky/task_5ms/Discrete PID Controller1/Saturation Fdbk/Disabled'
 * '<S46>'  : 'blinky/task_5ms/Discrete PID Controller1/Sum/Sum_PID'
 * '<S47>'  : 'blinky/task_5ms/Discrete PID Controller1/Sum Fdbk/Disabled'
 * '<S48>'  : 'blinky/task_5ms/Discrete PID Controller1/Tracking Mode/Disabled'
 * '<S49>'  : 'blinky/task_5ms/Discrete PID Controller1/Tracking Mode Sum/Passthrough'
 * '<S50>'  : 'blinky/task_5ms/Discrete PID Controller1/Tsamp - Integral/TsSignalSpecification'
 * '<S51>'  : 'blinky/task_5ms/Discrete PID Controller1/Tsamp - Ngain/Passthrough'
 * '<S52>'  : 'blinky/task_5ms/Discrete PID Controller1/postSat Signal/Forward_Path'
 * '<S53>'  : 'blinky/task_5ms/Discrete PID Controller1/preInt Signal/Internal PreInt'
 * '<S54>'  : 'blinky/task_5ms/Discrete PID Controller1/preSat Signal/Forward_Path'
 */
#endif                                 /* blinky_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
