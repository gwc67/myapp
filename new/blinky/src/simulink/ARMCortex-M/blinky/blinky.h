/*
 * File: blinky.h
 *
 * Code generated for Simulink model 'blinky'.
 *
 * Model version                  : 1.63
 * Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
 * C/C++ source code generated on : Sat Aug 22 20:14:05 2026
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
#define BALANCE_OFFSET                 0.0                       /* Referenced by: '<S4>/BALANCE_OFFSET' */
#define PWM_MAX                        700                       /* Referenced by: '<S4>/Saturation' */
#define PWM_MIN                        -700                      /* Referenced by: '<S4>/Saturation' */

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  real_T P_i[4];                       /* '<S59>/DataStoreMemory - P' */
  real_T x[2];                         /* '<S59>/DataStoreMemory - x' */
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
  real_T Filter_DSTATE;                /* '<S36>/Filter' */
  real_T Integrator_DSTATE;            /* '<S41>/Integrator' */
  real_T epsilon;
  real_T Gain1;                        /* '<S2>/Gain1' */
  real_T FilterCoefficient;            /* '<S44>/Filter Coefficient' */
  real_T rtb_DataTypeConversion2_idx_0;
  real_T z_idx_1;
  real_T z_idx_0;
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
  int16_T UnitDelay;                   /* '<Root>/Unit Delay' */
  int16_T UnitDelay1;                  /* '<Root>/Unit Delay1' */
  int16_T UnitDelay_DSTATE;            /* '<Root>/Unit Delay' */
  int16_T UnitDelay1_DSTATE;           /* '<Root>/Unit Delay1' */
} DW;

/* Constant parameters (default storage) */
typedef struct {
  /* Expression: p.R{1}
   * Referenced by: '<S59>/R1'
   */
  real_T R1_Value[4];

  /* Expression: p.Q
   * Referenced by: '<S59>/Q'
   */
  real_T Q_Value[4];
} ConstP;

/* External inputs (root inport signals with default storage) */
typedef struct {
  uint8_T running_flag;                /* '<Root>/running_flag' */
  real_T gyro[2];                      /* '<Root>/gyro' */
  real32_T accx;                       /* '<Root>/accx' */
  real32_T accy;                       /* '<Root>/accy' */
  real32_T accz;                       /* '<Root>/accz' */
} ExtU;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T roll;                         /* '<Root>/roll' */
  real_T pitch;                        /* '<Root>/pitch' */
  int16_T motor_a_pwm;                 /* '<Root>/motor_a_pwm' */
  int16_T motor_b_pwm;                 /* '<Root>/motor_b_pwm' */
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

    struct {
      uint8_T TID0_1;
    } RateInteraction;
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

/*
 * Exported Global Parameters
 *
 * Note: Exported global parameters are tunable parameters with an exported
 * global storage class designation.  Code generation will declare the memory for
 * these parameters and exports their symbols.
 *
 */
extern real_T BALANCE_KD;              /* Variable: BALANCE_KD
                                        * Referenced by: '<S34>/Derivative Gain'
                                        */
extern real_T BALANCE_KP;              /* Variable: BALANCE_KP
                                        * Referenced by: '<S46>/Proportional Gain'
                                        */

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
 * Block '<S59>/checkMeasurementFcn1Signals' : Unused code path elimination
 * Block '<S59>/checkStateTransitionFcnSignals' : Unused code path elimination
 * Block '<S59>/DataTypeConversion_Enable1' : Eliminate redundant data type conversion
 * Block '<S59>/DataTypeConversion_Q' : Eliminate redundant data type conversion
 * Block '<S59>/DataTypeConversion_R1' : Eliminate redundant data type conversion
 * Block '<S59>/DataTypeConversion_uMeas1' : Eliminate redundant data type conversion
 * Block '<S59>/DataTypeConversion_uState' : Eliminate redundant data type conversion
 * Block '<S59>/DataTypeConversion_y1' : Eliminate redundant data type conversion
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
 * '<S1>'   : 'blinky/task_1ms'
 * '<S2>'   : 'blinky/task_5ms'
 * '<S3>'   : 'blinky/task_5ms/MATLAB Function3'
 * '<S4>'   : 'blinky/task_5ms/Subsystem'
 * '<S5>'   : 'blinky/task_5ms/kalman_system'
 * '<S6>'   : 'blinky/task_5ms/Subsystem/PID Controller'
 * '<S7>'   : 'blinky/task_5ms/Subsystem/PID Controller/Anti-windup'
 * '<S8>'   : 'blinky/task_5ms/Subsystem/PID Controller/D Gain'
 * '<S9>'   : 'blinky/task_5ms/Subsystem/PID Controller/External Derivative'
 * '<S10>'  : 'blinky/task_5ms/Subsystem/PID Controller/Filter'
 * '<S11>'  : 'blinky/task_5ms/Subsystem/PID Controller/Filter ICs'
 * '<S12>'  : 'blinky/task_5ms/Subsystem/PID Controller/I Gain'
 * '<S13>'  : 'blinky/task_5ms/Subsystem/PID Controller/Ideal P Gain'
 * '<S14>'  : 'blinky/task_5ms/Subsystem/PID Controller/Ideal P Gain Fdbk'
 * '<S15>'  : 'blinky/task_5ms/Subsystem/PID Controller/Integrator'
 * '<S16>'  : 'blinky/task_5ms/Subsystem/PID Controller/Integrator ICs'
 * '<S17>'  : 'blinky/task_5ms/Subsystem/PID Controller/N Copy'
 * '<S18>'  : 'blinky/task_5ms/Subsystem/PID Controller/N Gain'
 * '<S19>'  : 'blinky/task_5ms/Subsystem/PID Controller/P Copy'
 * '<S20>'  : 'blinky/task_5ms/Subsystem/PID Controller/Parallel P Gain'
 * '<S21>'  : 'blinky/task_5ms/Subsystem/PID Controller/Reset Signal'
 * '<S22>'  : 'blinky/task_5ms/Subsystem/PID Controller/Saturation'
 * '<S23>'  : 'blinky/task_5ms/Subsystem/PID Controller/Saturation Fdbk'
 * '<S24>'  : 'blinky/task_5ms/Subsystem/PID Controller/Sum'
 * '<S25>'  : 'blinky/task_5ms/Subsystem/PID Controller/Sum Fdbk'
 * '<S26>'  : 'blinky/task_5ms/Subsystem/PID Controller/Tracking Mode'
 * '<S27>'  : 'blinky/task_5ms/Subsystem/PID Controller/Tracking Mode Sum'
 * '<S28>'  : 'blinky/task_5ms/Subsystem/PID Controller/Tsamp - Integral'
 * '<S29>'  : 'blinky/task_5ms/Subsystem/PID Controller/Tsamp - Ngain'
 * '<S30>'  : 'blinky/task_5ms/Subsystem/PID Controller/postSat Signal'
 * '<S31>'  : 'blinky/task_5ms/Subsystem/PID Controller/preInt Signal'
 * '<S32>'  : 'blinky/task_5ms/Subsystem/PID Controller/preSat Signal'
 * '<S33>'  : 'blinky/task_5ms/Subsystem/PID Controller/Anti-windup/Passthrough'
 * '<S34>'  : 'blinky/task_5ms/Subsystem/PID Controller/D Gain/Internal Parameters'
 * '<S35>'  : 'blinky/task_5ms/Subsystem/PID Controller/External Derivative/External Ydot'
 * '<S36>'  : 'blinky/task_5ms/Subsystem/PID Controller/Filter/Disc. Forward Euler Filter Only'
 * '<S37>'  : 'blinky/task_5ms/Subsystem/PID Controller/Filter ICs/Internal IC - Filter'
 * '<S38>'  : 'blinky/task_5ms/Subsystem/PID Controller/I Gain/Internal Parameters'
 * '<S39>'  : 'blinky/task_5ms/Subsystem/PID Controller/Ideal P Gain/Passthrough'
 * '<S40>'  : 'blinky/task_5ms/Subsystem/PID Controller/Ideal P Gain Fdbk/Disabled'
 * '<S41>'  : 'blinky/task_5ms/Subsystem/PID Controller/Integrator/Discrete'
 * '<S42>'  : 'blinky/task_5ms/Subsystem/PID Controller/Integrator ICs/Internal IC'
 * '<S43>'  : 'blinky/task_5ms/Subsystem/PID Controller/N Copy/Disabled'
 * '<S44>'  : 'blinky/task_5ms/Subsystem/PID Controller/N Gain/Internal Parameters'
 * '<S45>'  : 'blinky/task_5ms/Subsystem/PID Controller/P Copy/Disabled'
 * '<S46>'  : 'blinky/task_5ms/Subsystem/PID Controller/Parallel P Gain/Internal Parameters'
 * '<S47>'  : 'blinky/task_5ms/Subsystem/PID Controller/Reset Signal/Disabled'
 * '<S48>'  : 'blinky/task_5ms/Subsystem/PID Controller/Saturation/Enabled'
 * '<S49>'  : 'blinky/task_5ms/Subsystem/PID Controller/Saturation Fdbk/Disabled'
 * '<S50>'  : 'blinky/task_5ms/Subsystem/PID Controller/Sum/Sum_PID'
 * '<S51>'  : 'blinky/task_5ms/Subsystem/PID Controller/Sum Fdbk/Disabled'
 * '<S52>'  : 'blinky/task_5ms/Subsystem/PID Controller/Tracking Mode/Disabled'
 * '<S53>'  : 'blinky/task_5ms/Subsystem/PID Controller/Tracking Mode Sum/Passthrough'
 * '<S54>'  : 'blinky/task_5ms/Subsystem/PID Controller/Tsamp - Integral/TsSignalSpecification'
 * '<S55>'  : 'blinky/task_5ms/Subsystem/PID Controller/Tsamp - Ngain/Passthrough'
 * '<S56>'  : 'blinky/task_5ms/Subsystem/PID Controller/postSat Signal/Forward_Path'
 * '<S57>'  : 'blinky/task_5ms/Subsystem/PID Controller/preInt Signal/Internal PreInt'
 * '<S58>'  : 'blinky/task_5ms/Subsystem/PID Controller/preSat Signal/Forward_Path'
 * '<S59>'  : 'blinky/task_5ms/kalman_system/Extended Kalman Filter2'
 * '<S60>'  : 'blinky/task_5ms/kalman_system/Extended Kalman Filter2/Correct1'
 * '<S61>'  : 'blinky/task_5ms/kalman_system/Extended Kalman Filter2/Output'
 * '<S62>'  : 'blinky/task_5ms/kalman_system/Extended Kalman Filter2/Predict'
 * '<S63>'  : 'blinky/task_5ms/kalman_system/Extended Kalman Filter2/Output/MATLAB Function'
 */
#endif                                 /* blinky_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
