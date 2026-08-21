/*
 * File: blinky.h
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
#define SPEED_Kd                       0.0F                      /* Referenced by: '<S32>/Derivative Gain' */
#define SPEED_Ki                       1.0F                      /* Referenced by: '<S36>/Integral Gain' */
#define SPEED_Kp                       6.0F                      /* Referenced by: '<S44>/Proportional Gain' */

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  real_T P_i[4];                       /* '<S57>/DataStoreMemory - P' */
  real_T x[2];                         /* '<S57>/DataStoreMemory - x' */
  real_T M[8];
  real_T Sy[4];
  real_T b_dHdx[4];
  real_T K[4];
  real_T A[4];
  real_T b_R[4];
  real_T b_tau[2];
  real_T work[2];
  real_T M_m[8];
  real_T b_R_c[4];
  real_T b_tau_k[2];
  real_T work_c[2];
  real_T epsilon;
  real_T rtb_DataTypeConversion2_idx_1;
  real_T rtb_DataTypeConversion2_idx_0;
  real_T z_idx_1;
  real_T z_idx_0;
  real_T K_idx_0;
  real_T K_idx_2;
  real_T A_b;
  real_T A_p;
  real_T b_atmp;
  real_T xnorm;
  real_T A_c;
  real_T A_f;
  real_T S;
  real_T S_g;
  real_T scale;
  real_T absxk;
  real_T t;
  real_T b_atmp_g;
  real_T xnorm_m;
  real_T A_n;
  real_T A_pp;
  real_T S_l;
  real_T a;
  real32_T Integrator_DSTATE;          /* '<S39>/Integrator' */
  real32_T Filter_DSTATE;              /* '<S34>/Filter' */
  real32_T Sum2;                       /* '<S1>/Sum2' */
  int8_T Integrator_PrevResetState;    /* '<S39>/Integrator' */
  int8_T Filter_PrevResetState;        /* '<S34>/Filter' */
} DW;

/* Constant parameters (default storage) */
typedef struct {
  /* Pooled Parameter (Mixed Expressions)
   * Referenced by:
   *   '<S57>/Q'
   *   '<S57>/R1'
   */
  real_T pooled1[4];
} ConstP;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real32_T motor_a_actual_speed;       /* '<Root>/motor_a_actual_speed' */
  real32_T speed_a_target;             /* '<Root>/speed_a_target' */
  uint8_T running_flag;                /* '<Root>/running_flag' */
  real_T gyro[2];                      /* '<Root>/gyro' */
  real32_T accx;                       /* '<Root>/accx' */
  real32_T accy;                       /* '<Root>/accy' */
  real32_T accz;                       /* '<Root>/accz' */
} ExtU;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  int16_T motor_a_pwm;                 /* '<Root>/motor_a_pwm' */
  real_T roll;                         /* '<Root>/roll' */
  real_T pitch;                        /* '<Root>/pitch' */
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

/* Constant parameters (default storage) */
extern const ConstP rtConstP;

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
 * Block '<S57>/checkMeasurementFcn1Signals' : Unused code path elimination
 * Block '<S57>/checkStateTransitionFcnSignals' : Unused code path elimination
 * Block '<S1>/Data Type Conversion' : Eliminate redundant data type conversion
 * Block '<S57>/DataTypeConversion_Enable1' : Eliminate redundant data type conversion
 * Block '<S57>/DataTypeConversion_Q' : Eliminate redundant data type conversion
 * Block '<S57>/DataTypeConversion_R1' : Eliminate redundant data type conversion
 * Block '<S57>/DataTypeConversion_uMeas1' : Eliminate redundant data type conversion
 * Block '<S57>/DataTypeConversion_uState' : Eliminate redundant data type conversion
 * Block '<S57>/DataTypeConversion_y1' : Eliminate redundant data type conversion
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
 * '<S3>'   : 'blinky/task_5ms/MATLAB Function3'
 * '<S4>'   : 'blinky/task_5ms/kalman_system'
 * '<S5>'   : 'blinky/task_5ms/Discrete PID Controller1/Anti-windup'
 * '<S6>'   : 'blinky/task_5ms/Discrete PID Controller1/D Gain'
 * '<S7>'   : 'blinky/task_5ms/Discrete PID Controller1/External Derivative'
 * '<S8>'   : 'blinky/task_5ms/Discrete PID Controller1/Filter'
 * '<S9>'   : 'blinky/task_5ms/Discrete PID Controller1/Filter ICs'
 * '<S10>'  : 'blinky/task_5ms/Discrete PID Controller1/I Gain'
 * '<S11>'  : 'blinky/task_5ms/Discrete PID Controller1/Ideal P Gain'
 * '<S12>'  : 'blinky/task_5ms/Discrete PID Controller1/Ideal P Gain Fdbk'
 * '<S13>'  : 'blinky/task_5ms/Discrete PID Controller1/Integrator'
 * '<S14>'  : 'blinky/task_5ms/Discrete PID Controller1/Integrator ICs'
 * '<S15>'  : 'blinky/task_5ms/Discrete PID Controller1/N Copy'
 * '<S16>'  : 'blinky/task_5ms/Discrete PID Controller1/N Gain'
 * '<S17>'  : 'blinky/task_5ms/Discrete PID Controller1/P Copy'
 * '<S18>'  : 'blinky/task_5ms/Discrete PID Controller1/Parallel P Gain'
 * '<S19>'  : 'blinky/task_5ms/Discrete PID Controller1/Reset Signal'
 * '<S20>'  : 'blinky/task_5ms/Discrete PID Controller1/Saturation'
 * '<S21>'  : 'blinky/task_5ms/Discrete PID Controller1/Saturation Fdbk'
 * '<S22>'  : 'blinky/task_5ms/Discrete PID Controller1/Sum'
 * '<S23>'  : 'blinky/task_5ms/Discrete PID Controller1/Sum Fdbk'
 * '<S24>'  : 'blinky/task_5ms/Discrete PID Controller1/Tracking Mode'
 * '<S25>'  : 'blinky/task_5ms/Discrete PID Controller1/Tracking Mode Sum'
 * '<S26>'  : 'blinky/task_5ms/Discrete PID Controller1/Tsamp - Integral'
 * '<S27>'  : 'blinky/task_5ms/Discrete PID Controller1/Tsamp - Ngain'
 * '<S28>'  : 'blinky/task_5ms/Discrete PID Controller1/postSat Signal'
 * '<S29>'  : 'blinky/task_5ms/Discrete PID Controller1/preInt Signal'
 * '<S30>'  : 'blinky/task_5ms/Discrete PID Controller1/preSat Signal'
 * '<S31>'  : 'blinky/task_5ms/Discrete PID Controller1/Anti-windup/Passthrough'
 * '<S32>'  : 'blinky/task_5ms/Discrete PID Controller1/D Gain/Internal Parameters'
 * '<S33>'  : 'blinky/task_5ms/Discrete PID Controller1/External Derivative/Error'
 * '<S34>'  : 'blinky/task_5ms/Discrete PID Controller1/Filter/Disc. Forward Euler Filter'
 * '<S35>'  : 'blinky/task_5ms/Discrete PID Controller1/Filter ICs/Internal IC - Filter'
 * '<S36>'  : 'blinky/task_5ms/Discrete PID Controller1/I Gain/Internal Parameters'
 * '<S37>'  : 'blinky/task_5ms/Discrete PID Controller1/Ideal P Gain/Passthrough'
 * '<S38>'  : 'blinky/task_5ms/Discrete PID Controller1/Ideal P Gain Fdbk/Disabled'
 * '<S39>'  : 'blinky/task_5ms/Discrete PID Controller1/Integrator/Discrete'
 * '<S40>'  : 'blinky/task_5ms/Discrete PID Controller1/Integrator ICs/Internal IC'
 * '<S41>'  : 'blinky/task_5ms/Discrete PID Controller1/N Copy/Disabled'
 * '<S42>'  : 'blinky/task_5ms/Discrete PID Controller1/N Gain/Internal Parameters'
 * '<S43>'  : 'blinky/task_5ms/Discrete PID Controller1/P Copy/Disabled'
 * '<S44>'  : 'blinky/task_5ms/Discrete PID Controller1/Parallel P Gain/Internal Parameters'
 * '<S45>'  : 'blinky/task_5ms/Discrete PID Controller1/Reset Signal/External Reset'
 * '<S46>'  : 'blinky/task_5ms/Discrete PID Controller1/Saturation/Passthrough'
 * '<S47>'  : 'blinky/task_5ms/Discrete PID Controller1/Saturation Fdbk/Disabled'
 * '<S48>'  : 'blinky/task_5ms/Discrete PID Controller1/Sum/Sum_PID'
 * '<S49>'  : 'blinky/task_5ms/Discrete PID Controller1/Sum Fdbk/Disabled'
 * '<S50>'  : 'blinky/task_5ms/Discrete PID Controller1/Tracking Mode/Disabled'
 * '<S51>'  : 'blinky/task_5ms/Discrete PID Controller1/Tracking Mode Sum/Passthrough'
 * '<S52>'  : 'blinky/task_5ms/Discrete PID Controller1/Tsamp - Integral/TsSignalSpecification'
 * '<S53>'  : 'blinky/task_5ms/Discrete PID Controller1/Tsamp - Ngain/Passthrough'
 * '<S54>'  : 'blinky/task_5ms/Discrete PID Controller1/postSat Signal/Forward_Path'
 * '<S55>'  : 'blinky/task_5ms/Discrete PID Controller1/preInt Signal/Internal PreInt'
 * '<S56>'  : 'blinky/task_5ms/Discrete PID Controller1/preSat Signal/Forward_Path'
 * '<S57>'  : 'blinky/task_5ms/kalman_system/Extended Kalman Filter2'
 * '<S58>'  : 'blinky/task_5ms/kalman_system/Extended Kalman Filter2/Correct1'
 * '<S59>'  : 'blinky/task_5ms/kalman_system/Extended Kalman Filter2/Output'
 * '<S60>'  : 'blinky/task_5ms/kalman_system/Extended Kalman Filter2/Predict'
 * '<S61>'  : 'blinky/task_5ms/kalman_system/Extended Kalman Filter2/Output/MATLAB Function'
 */
#endif                                 /* blinky_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
