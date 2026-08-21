/*
 * File: blinky.h
 *
 * Code generated for Simulink model 'blinky'.
 *
 * Model version                  : 1.58
 * Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
 * C/C++ source code generated on : Fri Aug 21 14:09:21 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
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

#include "blinky_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmCounterLimit
#define rtmCounterLimit(rtm, idx)      ((rtm)->Timing.TaskCounters.cLimit[(idx)])
#endif

#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmStepTask
#define rtmStepTask(rtm, idx)          ((rtm)->Timing.TaskCounters.TID[(idx)] == 0)
#endif

#ifndef rtmTaskCounter
#define rtmTaskCounter(rtm, idx)       ((rtm)->Timing.TaskCounters.TID[(idx)])
#endif

#define blinky_M                       (rtM)

/* Exported data define */

/* Definition for custom storage class: Define */
#define SPEED_Kd                       0.0F                      /* Referenced by: '<S31>/Derivative Gain' */
#define SPEED_Ki                       1.0F                      /* Referenced by: '<S35>/Integral Gain' */
#define SPEED_Kp                       6.0F                      /* Referenced by: '<S43>/Proportional Gain' */

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  fusion_internal_simulink_imufil obj; /* '<S1>/IMU Filter' */
  real_T y;
  real32_T Ppost[81];
  real32_T H[27];
  real32_T y_m[27];
  real32_T y_c[27];
  real32_T y_tmp[27];
  real32_T b_I[9];
  real32_T Rpost[9];
  real32_T h1[9];
  real32_T fv[3];
  real32_T VectorConcatenate1[3];      /* '<S1>/Vector Concatenate1' */
  real32_T VectorConcatenate2[3];      /* '<S1>/Vector Concatenate2' */
  real32_T Reast[3];
  real32_T accelIn[3];
  real32_T Integrator_DSTATE;          /* '<S38>/Integrator' */
  real32_T Filter_DSTATE;              /* '<S33>/Filter' */
  real32_T accelMeasNoiseVar;
  real32_T FilterCoefficient;          /* '<S41>/Filter Coefficient' */
  real32_T e_b;
  real32_T e_c;
  real32_T e_d;
  real32_T deltaq_c;
  real32_T deltaq_d;
  real32_T n;
  real32_T maxval;
  real32_T a21;
  real32_T h1_k;
  real32_T Rpost_c;
  real32_T pGyroOffsetIn_idx_0;
  real32_T pLinAccelPostIn_idx_1;
  real32_T pGyroOffsetIn_idx_1;
  int32_T i;
  int32_T r1;
  int32_T r3;
  int32_T rtemp;
  int32_T xpageoffset;
  int8_T Integrator_PrevResetState;    /* '<S38>/Integrator' */
  int8_T Filter_PrevResetState;        /* '<S33>/Filter' */
} DW;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real32_T motor_a_actual_speed;       /* '<Root>/motor_a_actual_speed' */
  real32_T speed_a_target;             /* '<Root>/speed_a_target' */
  uint8_T running_flag;                /* '<Root>/running_flag' */
  real32_T gx;                         /* '<Root>/gx' */
  real32_T gy;                         /* '<Root>/gy' */
  real32_T ax;                         /* '<Root>/ax' */
  real32_T ay;                         /* '<Root>/ay' */
  real32_T az;                         /* '<Root>/az' */
  real32_T gz;                         /* '<Root>/gz' */
} ExtU;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  int16_T motor_a_pwm;                 /* '<Root>/motor_a_pwm' */
  real32_T roll;                       /* '<Root>/roll' */
  real32_T pitch;                      /* '<Root>/pitch' */
  real32_T Heading;                    /* '<Root>/Heading' */
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
    struct {
      uint8_T TID[2];
      uint8_T cLimit[2];
    } TaskCounters;
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
extern void blinky_step0(void);        /* Sample time: [0.001s, 0.0s] */
extern void blinky_step1(void);        /* Sample time: [0.005s, 0.0s] */
extern void blinky_terminate(void);

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
 * '<S3>'   : 'blinky/task_5ms/MATLAB Function2'
 * '<S4>'   : 'blinky/task_5ms/Discrete PID Controller1/Anti-windup'
 * '<S5>'   : 'blinky/task_5ms/Discrete PID Controller1/D Gain'
 * '<S6>'   : 'blinky/task_5ms/Discrete PID Controller1/External Derivative'
 * '<S7>'   : 'blinky/task_5ms/Discrete PID Controller1/Filter'
 * '<S8>'   : 'blinky/task_5ms/Discrete PID Controller1/Filter ICs'
 * '<S9>'   : 'blinky/task_5ms/Discrete PID Controller1/I Gain'
 * '<S10>'  : 'blinky/task_5ms/Discrete PID Controller1/Ideal P Gain'
 * '<S11>'  : 'blinky/task_5ms/Discrete PID Controller1/Ideal P Gain Fdbk'
 * '<S12>'  : 'blinky/task_5ms/Discrete PID Controller1/Integrator'
 * '<S13>'  : 'blinky/task_5ms/Discrete PID Controller1/Integrator ICs'
 * '<S14>'  : 'blinky/task_5ms/Discrete PID Controller1/N Copy'
 * '<S15>'  : 'blinky/task_5ms/Discrete PID Controller1/N Gain'
 * '<S16>'  : 'blinky/task_5ms/Discrete PID Controller1/P Copy'
 * '<S17>'  : 'blinky/task_5ms/Discrete PID Controller1/Parallel P Gain'
 * '<S18>'  : 'blinky/task_5ms/Discrete PID Controller1/Reset Signal'
 * '<S19>'  : 'blinky/task_5ms/Discrete PID Controller1/Saturation'
 * '<S20>'  : 'blinky/task_5ms/Discrete PID Controller1/Saturation Fdbk'
 * '<S21>'  : 'blinky/task_5ms/Discrete PID Controller1/Sum'
 * '<S22>'  : 'blinky/task_5ms/Discrete PID Controller1/Sum Fdbk'
 * '<S23>'  : 'blinky/task_5ms/Discrete PID Controller1/Tracking Mode'
 * '<S24>'  : 'blinky/task_5ms/Discrete PID Controller1/Tracking Mode Sum'
 * '<S25>'  : 'blinky/task_5ms/Discrete PID Controller1/Tsamp - Integral'
 * '<S26>'  : 'blinky/task_5ms/Discrete PID Controller1/Tsamp - Ngain'
 * '<S27>'  : 'blinky/task_5ms/Discrete PID Controller1/postSat Signal'
 * '<S28>'  : 'blinky/task_5ms/Discrete PID Controller1/preInt Signal'
 * '<S29>'  : 'blinky/task_5ms/Discrete PID Controller1/preSat Signal'
 * '<S30>'  : 'blinky/task_5ms/Discrete PID Controller1/Anti-windup/Passthrough'
 * '<S31>'  : 'blinky/task_5ms/Discrete PID Controller1/D Gain/Internal Parameters'
 * '<S32>'  : 'blinky/task_5ms/Discrete PID Controller1/External Derivative/Error'
 * '<S33>'  : 'blinky/task_5ms/Discrete PID Controller1/Filter/Disc. Forward Euler Filter'
 * '<S34>'  : 'blinky/task_5ms/Discrete PID Controller1/Filter ICs/Internal IC - Filter'
 * '<S35>'  : 'blinky/task_5ms/Discrete PID Controller1/I Gain/Internal Parameters'
 * '<S36>'  : 'blinky/task_5ms/Discrete PID Controller1/Ideal P Gain/Passthrough'
 * '<S37>'  : 'blinky/task_5ms/Discrete PID Controller1/Ideal P Gain Fdbk/Disabled'
 * '<S38>'  : 'blinky/task_5ms/Discrete PID Controller1/Integrator/Discrete'
 * '<S39>'  : 'blinky/task_5ms/Discrete PID Controller1/Integrator ICs/Internal IC'
 * '<S40>'  : 'blinky/task_5ms/Discrete PID Controller1/N Copy/Disabled'
 * '<S41>'  : 'blinky/task_5ms/Discrete PID Controller1/N Gain/Internal Parameters'
 * '<S42>'  : 'blinky/task_5ms/Discrete PID Controller1/P Copy/Disabled'
 * '<S43>'  : 'blinky/task_5ms/Discrete PID Controller1/Parallel P Gain/Internal Parameters'
 * '<S44>'  : 'blinky/task_5ms/Discrete PID Controller1/Reset Signal/External Reset'
 * '<S45>'  : 'blinky/task_5ms/Discrete PID Controller1/Saturation/Passthrough'
 * '<S46>'  : 'blinky/task_5ms/Discrete PID Controller1/Saturation Fdbk/Disabled'
 * '<S47>'  : 'blinky/task_5ms/Discrete PID Controller1/Sum/Sum_PID'
 * '<S48>'  : 'blinky/task_5ms/Discrete PID Controller1/Sum Fdbk/Disabled'
 * '<S49>'  : 'blinky/task_5ms/Discrete PID Controller1/Tracking Mode/Disabled'
 * '<S50>'  : 'blinky/task_5ms/Discrete PID Controller1/Tracking Mode Sum/Passthrough'
 * '<S51>'  : 'blinky/task_5ms/Discrete PID Controller1/Tsamp - Integral/TsSignalSpecification'
 * '<S52>'  : 'blinky/task_5ms/Discrete PID Controller1/Tsamp - Ngain/Passthrough'
 * '<S53>'  : 'blinky/task_5ms/Discrete PID Controller1/postSat Signal/Forward_Path'
 * '<S54>'  : 'blinky/task_5ms/Discrete PID Controller1/preInt Signal/Internal PreInt'
 * '<S55>'  : 'blinky/task_5ms/Discrete PID Controller1/preSat Signal/Forward_Path'
 */
#endif                                 /* blinky_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
