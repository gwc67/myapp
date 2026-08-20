/*
 * File: blinky.h
 *
 * Code generated for Simulink model 'blinky'.
 *
 * Model version                  : 1.34
 * Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
 * C/C++ source code generated on : Thu Aug 20 12:12:22 2026
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
#define SPEED_Kd                       0.0F                      /* Referenced by: '<S31>/Derivative Gain' */
#define SPEED_Ki                       1.0F                      /* Referenced by: '<S35>/Integral Gain' */
#define SPEED_Kp                       6.0F                      /* Referenced by: '<S43>/Proportional Gain' */

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  real32_T Integrator_DSTATE;          /* '<S38>/Integrator' */
  real32_T Filter_DSTATE;              /* '<S33>/Filter' */
  real32_T Filter_PREV_U;              /* '<S33>/Filter' */
  uint32_T FunctionCallSubsystem_PREV_T;/* '<Root>/Function-Call Subsystem' */
  int8_T Integrator_PrevResetState;    /* '<S38>/Integrator' */
  int8_T Filter_PrevResetState;        /* '<S33>/Filter' */
  uint8_T task_10ms_count;             /* '<Root>/Chart' */
  uint8_T task_5ms_count;              /* '<Root>/Chart' */
  uint8_T task_50ms_count;             /* '<Root>/Chart' */
  uint8_T task_20ms_count;             /* '<Root>/Chart' */
  uint8_T Filter_SYSTEM_ENABLE;        /* '<S33>/Filter' */
  boolean_T FunctionCallSubsystem_RESET_ELA;/* '<Root>/Function-Call Subsystem' */
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

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
  } Timing;
};

/* Block signals and states (default storage) */
extern DW rtDW;

/* External inputs (root inport signals with default storage) */
extern ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY rtY;

/* Model entry point functions */
extern void blinky_initialize(void);
extern void blinky_step(void);

/* Real-time Model object */
extern RT_MODEL *const rtM;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S2>/Scope2' : Unused code path elimination
 * Block '<Root>/Scope2' : Unused code path elimination
 * Block '<S2>/Data Type Conversion' : Eliminate redundant data type conversion
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
 * '<S1>'   : 'blinky/Chart'
 * '<S2>'   : 'blinky/Function-Call Subsystem'
 * '<S3>'   : 'blinky/Function-Call Subsystem/Discrete PID Controller1'
 * '<S4>'   : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Anti-windup'
 * '<S5>'   : 'blinky/Function-Call Subsystem/Discrete PID Controller1/D Gain'
 * '<S6>'   : 'blinky/Function-Call Subsystem/Discrete PID Controller1/External Derivative'
 * '<S7>'   : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Filter'
 * '<S8>'   : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Filter ICs'
 * '<S9>'   : 'blinky/Function-Call Subsystem/Discrete PID Controller1/I Gain'
 * '<S10>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Ideal P Gain'
 * '<S11>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Ideal P Gain Fdbk'
 * '<S12>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Integrator'
 * '<S13>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Integrator ICs'
 * '<S14>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/N Copy'
 * '<S15>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/N Gain'
 * '<S16>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/P Copy'
 * '<S17>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Parallel P Gain'
 * '<S18>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Reset Signal'
 * '<S19>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Saturation'
 * '<S20>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Saturation Fdbk'
 * '<S21>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Sum'
 * '<S22>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Sum Fdbk'
 * '<S23>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Tracking Mode'
 * '<S24>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Tracking Mode Sum'
 * '<S25>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Tsamp - Integral'
 * '<S26>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Tsamp - Ngain'
 * '<S27>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/postSat Signal'
 * '<S28>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/preInt Signal'
 * '<S29>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/preSat Signal'
 * '<S30>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Anti-windup/Passthrough'
 * '<S31>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/D Gain/Internal Parameters'
 * '<S32>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/External Derivative/Error'
 * '<S33>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Filter/Disc. Forward Euler Filter'
 * '<S34>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Filter ICs/Internal IC - Filter'
 * '<S35>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/I Gain/Internal Parameters'
 * '<S36>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Ideal P Gain/Passthrough'
 * '<S37>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Ideal P Gain Fdbk/Disabled'
 * '<S38>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Integrator/Discrete'
 * '<S39>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Integrator ICs/Internal IC'
 * '<S40>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/N Copy/Disabled'
 * '<S41>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/N Gain/Internal Parameters'
 * '<S42>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/P Copy/Disabled'
 * '<S43>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Parallel P Gain/Internal Parameters'
 * '<S44>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Reset Signal/External Reset'
 * '<S45>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Saturation/Passthrough'
 * '<S46>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Saturation Fdbk/Disabled'
 * '<S47>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Sum/Sum_PID'
 * '<S48>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Sum Fdbk/Disabled'
 * '<S49>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Tracking Mode/Disabled'
 * '<S50>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Tracking Mode Sum/Passthrough'
 * '<S51>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Tsamp - Integral/TsSignalSpecification'
 * '<S52>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/Tsamp - Ngain/Passthrough'
 * '<S53>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/postSat Signal/Forward_Path'
 * '<S54>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/preInt Signal/Internal PreInt'
 * '<S55>'  : 'blinky/Function-Call Subsystem/Discrete PID Controller1/preSat Signal/Forward_Path'
 */
#endif                                 /* blinky_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
