/*
 * File: blinky.h
 *
 * Code generated for Simulink model 'blinky'.
 *
 * Model version                  : 1.75
 * Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
 * C/C++ source code generated on : Mon Aug 24 15:14:20 2026
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
#define ANGLE_INTERGRAL_MAX            10.0                      /* Referenced by: '<S41>/Integrator' */
#define ANGLE_INTERGRAL_MIN            -10.0                     /* Referenced by: '<S41>/Integrator' */
#define ANGLE_MAX                      15.0                      /* Referenced by: '<S48>/Saturation' */
#define ANGLE_MIN                      -15.0                     /* Referenced by: '<S48>/Saturation' */
#define BALANCE_OFFSET                 0.0                       /* Referenced by: '<S114>/BALANCE_OFFSET' */
#define PWM_MAX                        800                       /* Referenced by: '<S114>/Saturation' */
#define PWM_MIN                        -800                      /* Referenced by: '<S114>/Saturation' */
#define pitch_max                      60.0                      /* Referenced by: '<Root>/output_flag' */
#define pitch_min                      -55.0                     /* Referenced by: '<Root>/output_flag' */

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  real_T P_i[4];                       /* '<S170>/DataStoreMemory - P' */
  real_T x[2];                         /* '<S170>/DataStoreMemory - x' */
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
  real_T UnitDelay2;                   /* '<Root>/Unit Delay2' */
  real_T UnitDelay2_DSTATE;            /* '<Root>/Unit Delay2' */
  real_T UD_DSTATE;                    /* '<S116>/UD' */
  real_T Integrator_DSTATE;            /* '<S152>/Integrator' */
  real_T Filter_DSTATE;                /* '<S36>/Filter' */
  real_T Integrator_DSTATE_n;          /* '<S41>/Integrator' */
  real_T epsilon;
  real_T Gain1;                        /* '<S5>/Gain1' */
  real_T TSamp;                        /* '<S116>/TSamp' */
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
  real32_T Filter_DSTATE_i;            /* '<S147>/Filter' */
  real32_T Filter_DSTATE_j;            /* '<S89>/Filter' */
  int16_T UnitDelay;                   /* '<Root>/Unit Delay' */
  int16_T UnitDelay1;                  /* '<Root>/Unit Delay1' */
  int16_T DataTypeConversion;          /* '<S4>/Data Type Conversion' */
  int16_T UnitDelay_DSTATE;            /* '<Root>/Unit Delay' */
  int16_T UnitDelay1_DSTATE;           /* '<Root>/Unit Delay1' */
  int8_T Filter_PrevResetState;        /* '<S89>/Filter' */
  int8_T Filter_PrevResetState_a;      /* '<S36>/Filter' */
  int8_T Integrator_PrevResetState;    /* '<S41>/Integrator' */
  uint8_T UnitDelay5;                  /* '<Root>/Unit Delay5' */
  uint8_T running_success_flag;        /* '<Root>/output_flag' */
  uint8_T UnitDelay3_DSTATE;           /* '<Root>/Unit Delay3' */
  uint8_T UnitDelay5_DSTATE;           /* '<Root>/Unit Delay5' */
  uint8_T UnitDelay4_DSTATE;           /* '<Root>/Unit Delay4' */
} DW;

/* Constant parameters (default storage) */
typedef struct {
  /* Expression: p.R{1}
   * Referenced by: '<S170>/R1'
   */
  real_T R1_Value[4];

  /* Expression: p.Q
   * Referenced by: '<S170>/Q'
   */
  real_T Q_Value[4];
} ConstP;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real32_T gyroy;                      /* '<Root>/gyroy' */
  real32_T accx;                       /* '<Root>/accx' */
  real32_T accy;                       /* '<Root>/accy' */
  real32_T accz;                       /* '<Root>/accz' */
  real_T motor_a_speed;                /* '<Root>/motor_a_speed' */
  real_T motor_b_speed;                /* '<Root>/motor_b_speed' */
  real_T target_speed;                 /* '<Root>/target_speed' */
  real32_T gyrox;                      /* '<Root>/gyrox' */
  real32_T gyroz;                      /* '<Root>/gyroz' */
  real32_T turn_target;                /* '<Root>/turn_target' */
  uint8_T running_flag;                /* '<Root>/running_flag' */
} ExtU;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T roll;                         /* '<Root>/roll' */
  real_T pitch;                        /* '<Root>/pitch' */
  int16_T motor_a_pwm;                 /* '<Root>/motor_a_pwm' */
  int16_T motor_b_pwm;                 /* '<Root>/motor_b_pwm' */
  real_T angle_target;                 /* '<Root>/angle_target' */
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
      uint8_T TID[4];
      uint8_T cLimit[4];
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
extern real_T BALANCE_KP;              /* Variable: BALANCE_KP
                                        * Referenced by: '<S157>/Proportional Gain'
                                        */
extern real_T BAL_KFF;                 /* Variable: BAL_KFF
                                        * Referenced by: '<S114>/Gain'
                                        */
extern real_T SPD_KI;                  /* Variable: SPD_KI
                                        * Referenced by: '<S38>/Integral Gain'
                                        */
extern real_T SPD_KP;                  /* Variable: SPD_KP
                                        * Referenced by: '<S46>/Proportional Gain'
                                        */
extern real32_T BALANCE_KD;            /* Variable: BALANCE_KD
                                        * Referenced by: '<S145>/Derivative Gain'
                                        */
extern real32_T TURN_KD;               /* Variable: TURN_KD
                                        * Referenced by: '<S87>/Derivative Gain'
                                        */
extern real32_T TURN_KP;               /* Variable: TURN_KP
                                        * Referenced by: '<S99>/Proportional Gain'
                                        */

/* Model entry point functions */
extern void blinky_initialize(void);
extern void blinky_step0(void);        /* Sample time: [0.001s, 0.0s] */
extern void blinky_step1(void);        /* Sample time: [0.005s, 0.0s] */
extern void blinky_step2(void);        /* Sample time: [0.01s, 0.0s] */
extern void blinky_step3(void);        /* Sample time: [0.02s, 0.0s] */
extern void blinky_terminate(void);

/* Real-time Model object */
extern RT_MODEL *const rtM;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S116>/Data Type Duplicate' : Unused code path elimination
 * Block '<S170>/checkMeasurementFcn1Signals' : Unused code path elimination
 * Block '<S170>/checkStateTransitionFcnSignals' : Unused code path elimination
 * Block '<S114>/Data Type Conversion' : Eliminate redundant data type conversion
 * Block '<S114>/Data Type Conversion1' : Eliminate redundant data type conversion
 * Block '<S170>/DataTypeConversion_Enable1' : Eliminate redundant data type conversion
 * Block '<S170>/DataTypeConversion_Q' : Eliminate redundant data type conversion
 * Block '<S170>/DataTypeConversion_R1' : Eliminate redundant data type conversion
 * Block '<S170>/DataTypeConversion_uMeas1' : Eliminate redundant data type conversion
 * Block '<S170>/DataTypeConversion_uState' : Eliminate redundant data type conversion
 * Block '<S170>/DataTypeConversion_y1' : Eliminate redundant data type conversion
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
 * '<S1>'   : 'blinky/output_flag'
 * '<S2>'   : 'blinky/task_10ms'
 * '<S3>'   : 'blinky/task_1ms'
 * '<S4>'   : 'blinky/task_20ms'
 * '<S5>'   : 'blinky/task_5ms'
 * '<S6>'   : 'blinky/task_10ms/Discrete PID Controller'
 * '<S7>'   : 'blinky/task_10ms/Discrete PID Controller/Anti-windup'
 * '<S8>'   : 'blinky/task_10ms/Discrete PID Controller/D Gain'
 * '<S9>'   : 'blinky/task_10ms/Discrete PID Controller/External Derivative'
 * '<S10>'  : 'blinky/task_10ms/Discrete PID Controller/Filter'
 * '<S11>'  : 'blinky/task_10ms/Discrete PID Controller/Filter ICs'
 * '<S12>'  : 'blinky/task_10ms/Discrete PID Controller/I Gain'
 * '<S13>'  : 'blinky/task_10ms/Discrete PID Controller/Ideal P Gain'
 * '<S14>'  : 'blinky/task_10ms/Discrete PID Controller/Ideal P Gain Fdbk'
 * '<S15>'  : 'blinky/task_10ms/Discrete PID Controller/Integrator'
 * '<S16>'  : 'blinky/task_10ms/Discrete PID Controller/Integrator ICs'
 * '<S17>'  : 'blinky/task_10ms/Discrete PID Controller/N Copy'
 * '<S18>'  : 'blinky/task_10ms/Discrete PID Controller/N Gain'
 * '<S19>'  : 'blinky/task_10ms/Discrete PID Controller/P Copy'
 * '<S20>'  : 'blinky/task_10ms/Discrete PID Controller/Parallel P Gain'
 * '<S21>'  : 'blinky/task_10ms/Discrete PID Controller/Reset Signal'
 * '<S22>'  : 'blinky/task_10ms/Discrete PID Controller/Saturation'
 * '<S23>'  : 'blinky/task_10ms/Discrete PID Controller/Saturation Fdbk'
 * '<S24>'  : 'blinky/task_10ms/Discrete PID Controller/Sum'
 * '<S25>'  : 'blinky/task_10ms/Discrete PID Controller/Sum Fdbk'
 * '<S26>'  : 'blinky/task_10ms/Discrete PID Controller/Tracking Mode'
 * '<S27>'  : 'blinky/task_10ms/Discrete PID Controller/Tracking Mode Sum'
 * '<S28>'  : 'blinky/task_10ms/Discrete PID Controller/Tsamp - Integral'
 * '<S29>'  : 'blinky/task_10ms/Discrete PID Controller/Tsamp - Ngain'
 * '<S30>'  : 'blinky/task_10ms/Discrete PID Controller/postSat Signal'
 * '<S31>'  : 'blinky/task_10ms/Discrete PID Controller/preInt Signal'
 * '<S32>'  : 'blinky/task_10ms/Discrete PID Controller/preSat Signal'
 * '<S33>'  : 'blinky/task_10ms/Discrete PID Controller/Anti-windup/Passthrough'
 * '<S34>'  : 'blinky/task_10ms/Discrete PID Controller/D Gain/Internal Parameters'
 * '<S35>'  : 'blinky/task_10ms/Discrete PID Controller/External Derivative/Error'
 * '<S36>'  : 'blinky/task_10ms/Discrete PID Controller/Filter/Disc. Forward Euler Filter'
 * '<S37>'  : 'blinky/task_10ms/Discrete PID Controller/Filter ICs/Internal IC - Filter'
 * '<S38>'  : 'blinky/task_10ms/Discrete PID Controller/I Gain/Internal Parameters'
 * '<S39>'  : 'blinky/task_10ms/Discrete PID Controller/Ideal P Gain/Passthrough'
 * '<S40>'  : 'blinky/task_10ms/Discrete PID Controller/Ideal P Gain Fdbk/Disabled'
 * '<S41>'  : 'blinky/task_10ms/Discrete PID Controller/Integrator/Discrete'
 * '<S42>'  : 'blinky/task_10ms/Discrete PID Controller/Integrator ICs/Internal IC'
 * '<S43>'  : 'blinky/task_10ms/Discrete PID Controller/N Copy/Disabled'
 * '<S44>'  : 'blinky/task_10ms/Discrete PID Controller/N Gain/Internal Parameters'
 * '<S45>'  : 'blinky/task_10ms/Discrete PID Controller/P Copy/Disabled'
 * '<S46>'  : 'blinky/task_10ms/Discrete PID Controller/Parallel P Gain/Internal Parameters'
 * '<S47>'  : 'blinky/task_10ms/Discrete PID Controller/Reset Signal/External Reset'
 * '<S48>'  : 'blinky/task_10ms/Discrete PID Controller/Saturation/Enabled'
 * '<S49>'  : 'blinky/task_10ms/Discrete PID Controller/Saturation Fdbk/Disabled'
 * '<S50>'  : 'blinky/task_10ms/Discrete PID Controller/Sum/Sum_PID'
 * '<S51>'  : 'blinky/task_10ms/Discrete PID Controller/Sum Fdbk/Disabled'
 * '<S52>'  : 'blinky/task_10ms/Discrete PID Controller/Tracking Mode/Disabled'
 * '<S53>'  : 'blinky/task_10ms/Discrete PID Controller/Tracking Mode Sum/Passthrough'
 * '<S54>'  : 'blinky/task_10ms/Discrete PID Controller/Tsamp - Integral/TsSignalSpecification'
 * '<S55>'  : 'blinky/task_10ms/Discrete PID Controller/Tsamp - Ngain/Passthrough'
 * '<S56>'  : 'blinky/task_10ms/Discrete PID Controller/postSat Signal/Forward_Path'
 * '<S57>'  : 'blinky/task_10ms/Discrete PID Controller/preInt Signal/Internal PreInt'
 * '<S58>'  : 'blinky/task_10ms/Discrete PID Controller/preSat Signal/Forward_Path'
 * '<S59>'  : 'blinky/task_20ms/PID Controller'
 * '<S60>'  : 'blinky/task_20ms/PID Controller/Anti-windup'
 * '<S61>'  : 'blinky/task_20ms/PID Controller/D Gain'
 * '<S62>'  : 'blinky/task_20ms/PID Controller/External Derivative'
 * '<S63>'  : 'blinky/task_20ms/PID Controller/Filter'
 * '<S64>'  : 'blinky/task_20ms/PID Controller/Filter ICs'
 * '<S65>'  : 'blinky/task_20ms/PID Controller/I Gain'
 * '<S66>'  : 'blinky/task_20ms/PID Controller/Ideal P Gain'
 * '<S67>'  : 'blinky/task_20ms/PID Controller/Ideal P Gain Fdbk'
 * '<S68>'  : 'blinky/task_20ms/PID Controller/Integrator'
 * '<S69>'  : 'blinky/task_20ms/PID Controller/Integrator ICs'
 * '<S70>'  : 'blinky/task_20ms/PID Controller/N Copy'
 * '<S71>'  : 'blinky/task_20ms/PID Controller/N Gain'
 * '<S72>'  : 'blinky/task_20ms/PID Controller/P Copy'
 * '<S73>'  : 'blinky/task_20ms/PID Controller/Parallel P Gain'
 * '<S74>'  : 'blinky/task_20ms/PID Controller/Reset Signal'
 * '<S75>'  : 'blinky/task_20ms/PID Controller/Saturation'
 * '<S76>'  : 'blinky/task_20ms/PID Controller/Saturation Fdbk'
 * '<S77>'  : 'blinky/task_20ms/PID Controller/Sum'
 * '<S78>'  : 'blinky/task_20ms/PID Controller/Sum Fdbk'
 * '<S79>'  : 'blinky/task_20ms/PID Controller/Tracking Mode'
 * '<S80>'  : 'blinky/task_20ms/PID Controller/Tracking Mode Sum'
 * '<S81>'  : 'blinky/task_20ms/PID Controller/Tsamp - Integral'
 * '<S82>'  : 'blinky/task_20ms/PID Controller/Tsamp - Ngain'
 * '<S83>'  : 'blinky/task_20ms/PID Controller/postSat Signal'
 * '<S84>'  : 'blinky/task_20ms/PID Controller/preInt Signal'
 * '<S85>'  : 'blinky/task_20ms/PID Controller/preSat Signal'
 * '<S86>'  : 'blinky/task_20ms/PID Controller/Anti-windup/Disabled'
 * '<S87>'  : 'blinky/task_20ms/PID Controller/D Gain/Internal Parameters'
 * '<S88>'  : 'blinky/task_20ms/PID Controller/External Derivative/External Ydot'
 * '<S89>'  : 'blinky/task_20ms/PID Controller/Filter/Disc. Forward Euler Filter Only'
 * '<S90>'  : 'blinky/task_20ms/PID Controller/Filter ICs/Internal IC - Filter'
 * '<S91>'  : 'blinky/task_20ms/PID Controller/I Gain/Disabled'
 * '<S92>'  : 'blinky/task_20ms/PID Controller/Ideal P Gain/Passthrough'
 * '<S93>'  : 'blinky/task_20ms/PID Controller/Ideal P Gain Fdbk/Disabled'
 * '<S94>'  : 'blinky/task_20ms/PID Controller/Integrator/Disabled'
 * '<S95>'  : 'blinky/task_20ms/PID Controller/Integrator ICs/Disabled'
 * '<S96>'  : 'blinky/task_20ms/PID Controller/N Copy/Disabled'
 * '<S97>'  : 'blinky/task_20ms/PID Controller/N Gain/Internal Parameters'
 * '<S98>'  : 'blinky/task_20ms/PID Controller/P Copy/Disabled'
 * '<S99>'  : 'blinky/task_20ms/PID Controller/Parallel P Gain/Internal Parameters'
 * '<S100>' : 'blinky/task_20ms/PID Controller/Reset Signal/External Reset'
 * '<S101>' : 'blinky/task_20ms/PID Controller/Saturation/Enabled'
 * '<S102>' : 'blinky/task_20ms/PID Controller/Saturation Fdbk/Disabled'
 * '<S103>' : 'blinky/task_20ms/PID Controller/Sum/Sum_PD'
 * '<S104>' : 'blinky/task_20ms/PID Controller/Sum Fdbk/Disabled'
 * '<S105>' : 'blinky/task_20ms/PID Controller/Tracking Mode/Disabled'
 * '<S106>' : 'blinky/task_20ms/PID Controller/Tracking Mode Sum/Passthrough'
 * '<S107>' : 'blinky/task_20ms/PID Controller/Tsamp - Integral/TsSignalSpecification'
 * '<S108>' : 'blinky/task_20ms/PID Controller/Tsamp - Ngain/Passthrough'
 * '<S109>' : 'blinky/task_20ms/PID Controller/postSat Signal/Forward_Path'
 * '<S110>' : 'blinky/task_20ms/PID Controller/preInt Signal/Internal PreInt'
 * '<S111>' : 'blinky/task_20ms/PID Controller/preSat Signal/Forward_Path'
 * '<S112>' : 'blinky/task_5ms/MATLAB Function'
 * '<S113>' : 'blinky/task_5ms/MATLAB Function3'
 * '<S114>' : 'blinky/task_5ms/Subsystem'
 * '<S115>' : 'blinky/task_5ms/kalman_system'
 * '<S116>' : 'blinky/task_5ms/Subsystem/Discrete Derivative'
 * '<S117>' : 'blinky/task_5ms/Subsystem/PID Controller'
 * '<S118>' : 'blinky/task_5ms/Subsystem/PID Controller/Anti-windup'
 * '<S119>' : 'blinky/task_5ms/Subsystem/PID Controller/D Gain'
 * '<S120>' : 'blinky/task_5ms/Subsystem/PID Controller/External Derivative'
 * '<S121>' : 'blinky/task_5ms/Subsystem/PID Controller/Filter'
 * '<S122>' : 'blinky/task_5ms/Subsystem/PID Controller/Filter ICs'
 * '<S123>' : 'blinky/task_5ms/Subsystem/PID Controller/I Gain'
 * '<S124>' : 'blinky/task_5ms/Subsystem/PID Controller/Ideal P Gain'
 * '<S125>' : 'blinky/task_5ms/Subsystem/PID Controller/Ideal P Gain Fdbk'
 * '<S126>' : 'blinky/task_5ms/Subsystem/PID Controller/Integrator'
 * '<S127>' : 'blinky/task_5ms/Subsystem/PID Controller/Integrator ICs'
 * '<S128>' : 'blinky/task_5ms/Subsystem/PID Controller/N Copy'
 * '<S129>' : 'blinky/task_5ms/Subsystem/PID Controller/N Gain'
 * '<S130>' : 'blinky/task_5ms/Subsystem/PID Controller/P Copy'
 * '<S131>' : 'blinky/task_5ms/Subsystem/PID Controller/Parallel P Gain'
 * '<S132>' : 'blinky/task_5ms/Subsystem/PID Controller/Reset Signal'
 * '<S133>' : 'blinky/task_5ms/Subsystem/PID Controller/Saturation'
 * '<S134>' : 'blinky/task_5ms/Subsystem/PID Controller/Saturation Fdbk'
 * '<S135>' : 'blinky/task_5ms/Subsystem/PID Controller/Sum'
 * '<S136>' : 'blinky/task_5ms/Subsystem/PID Controller/Sum Fdbk'
 * '<S137>' : 'blinky/task_5ms/Subsystem/PID Controller/Tracking Mode'
 * '<S138>' : 'blinky/task_5ms/Subsystem/PID Controller/Tracking Mode Sum'
 * '<S139>' : 'blinky/task_5ms/Subsystem/PID Controller/Tsamp - Integral'
 * '<S140>' : 'blinky/task_5ms/Subsystem/PID Controller/Tsamp - Ngain'
 * '<S141>' : 'blinky/task_5ms/Subsystem/PID Controller/postSat Signal'
 * '<S142>' : 'blinky/task_5ms/Subsystem/PID Controller/preInt Signal'
 * '<S143>' : 'blinky/task_5ms/Subsystem/PID Controller/preSat Signal'
 * '<S144>' : 'blinky/task_5ms/Subsystem/PID Controller/Anti-windup/Passthrough'
 * '<S145>' : 'blinky/task_5ms/Subsystem/PID Controller/D Gain/Internal Parameters'
 * '<S146>' : 'blinky/task_5ms/Subsystem/PID Controller/External Derivative/External Ydot'
 * '<S147>' : 'blinky/task_5ms/Subsystem/PID Controller/Filter/Disc. Forward Euler Filter Only'
 * '<S148>' : 'blinky/task_5ms/Subsystem/PID Controller/Filter ICs/Internal IC - Filter'
 * '<S149>' : 'blinky/task_5ms/Subsystem/PID Controller/I Gain/Internal Parameters'
 * '<S150>' : 'blinky/task_5ms/Subsystem/PID Controller/Ideal P Gain/Passthrough'
 * '<S151>' : 'blinky/task_5ms/Subsystem/PID Controller/Ideal P Gain Fdbk/Disabled'
 * '<S152>' : 'blinky/task_5ms/Subsystem/PID Controller/Integrator/Discrete'
 * '<S153>' : 'blinky/task_5ms/Subsystem/PID Controller/Integrator ICs/Internal IC'
 * '<S154>' : 'blinky/task_5ms/Subsystem/PID Controller/N Copy/Disabled'
 * '<S155>' : 'blinky/task_5ms/Subsystem/PID Controller/N Gain/Internal Parameters'
 * '<S156>' : 'blinky/task_5ms/Subsystem/PID Controller/P Copy/Disabled'
 * '<S157>' : 'blinky/task_5ms/Subsystem/PID Controller/Parallel P Gain/Internal Parameters'
 * '<S158>' : 'blinky/task_5ms/Subsystem/PID Controller/Reset Signal/Disabled'
 * '<S159>' : 'blinky/task_5ms/Subsystem/PID Controller/Saturation/Enabled'
 * '<S160>' : 'blinky/task_5ms/Subsystem/PID Controller/Saturation Fdbk/Disabled'
 * '<S161>' : 'blinky/task_5ms/Subsystem/PID Controller/Sum/Sum_PID'
 * '<S162>' : 'blinky/task_5ms/Subsystem/PID Controller/Sum Fdbk/Disabled'
 * '<S163>' : 'blinky/task_5ms/Subsystem/PID Controller/Tracking Mode/Disabled'
 * '<S164>' : 'blinky/task_5ms/Subsystem/PID Controller/Tracking Mode Sum/Passthrough'
 * '<S165>' : 'blinky/task_5ms/Subsystem/PID Controller/Tsamp - Integral/TsSignalSpecification'
 * '<S166>' : 'blinky/task_5ms/Subsystem/PID Controller/Tsamp - Ngain/Passthrough'
 * '<S167>' : 'blinky/task_5ms/Subsystem/PID Controller/postSat Signal/Forward_Path'
 * '<S168>' : 'blinky/task_5ms/Subsystem/PID Controller/preInt Signal/Internal PreInt'
 * '<S169>' : 'blinky/task_5ms/Subsystem/PID Controller/preSat Signal/Forward_Path'
 * '<S170>' : 'blinky/task_5ms/kalman_system/Extended Kalman Filter2'
 * '<S171>' : 'blinky/task_5ms/kalman_system/Extended Kalman Filter2/Correct1'
 * '<S172>' : 'blinky/task_5ms/kalman_system/Extended Kalman Filter2/Output'
 * '<S173>' : 'blinky/task_5ms/kalman_system/Extended Kalman Filter2/Predict'
 * '<S174>' : 'blinky/task_5ms/kalman_system/Extended Kalman Filter2/Output/MATLAB Function'
 */
#endif                                 /* blinky_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
