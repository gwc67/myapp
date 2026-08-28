/*
 * File: blinky.h
 *
 * Code generated for Simulink model 'blinky'.
 *
 * Model version                  : 1.79
 * Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
 * C/C++ source code generated on : Fri Aug 28 17:40:41 2026
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
#define ANGLE_INTERGRAL_MAX            10.0F                     /* Referenced by: '<S40>/Integrator' */
#define ANGLE_INTERGRAL_MIN            -10.0F                    /* Referenced by: '<S40>/Integrator' */
#define ANGLE_MAX                      15.0F                     /* Referenced by: '<S47>/Saturation' */
#define ANGLE_MIN                      -15.0F                    /* Referenced by: '<S47>/Saturation' */
#define PWM_MAX                        800                       /* Referenced by: '<S113>/Saturation1' */
#define PWM_MIN                        -800                      /* Referenced by: '<S113>/Saturation1' */
#define bal_e_max                      30.0F                     /* Referenced by: '<S113>/gain' */
#define bal_ec_max                     200.0F                    /* Referenced by: '<S113>/gain2' */
#define bal_kd_factor                  0.0F                      /* Referenced by: '<S113>/Gain2' */
#define bal_ki_factor                  0.0F                      /* Referenced by: '<S113>/Gain1' */
#define bal_kp_factor                  0.0F                      /* Referenced by: '<S113>/Gain' */
#define pitch_max                      60.0F                     /* Referenced by: '<Root>/output_flag' */
#define pitch_min                      -55.0F                    /* Referenced by: '<Root>/output_flag' */

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  real32_T P_i[4];                     /* '<S118>/DataStoreMemory - P' */
  real32_T x[2];                       /* '<S118>/DataStoreMemory - x' */
  real32_T M[8];
  real32_T Sy[4];
  real32_T b_dHdx[4];
  real32_T K[4];
  real32_T A[4];
  real32_T b_R[4];
  real32_T frac[2];
  real32_T b_tau[2];
  real32_T work[2];
  real32_T M_m[8];
  real32_T b_R_c[4];
  real32_T b_tau_k[2];
  real32_T work_c[2];
  real32_T UnitDelay2;                 /* '<Root>/Unit Delay2' */
  real32_T UnitDelay3_DSTATE;          /* '<Root>/Unit Delay3' */
  real32_T UnitDelay2_DSTATE;          /* '<Root>/Unit Delay2' */
  real32_T UD_DSTATE;                  /* '<S116>/UD' */
  real32_T DiscreteTimeIntegrator_DSTATE;/* '<S113>/Discrete-Time Integrator' */
  real32_T DiscreteFilter_states;      /* '<S113>/Discrete Filter' */
  real32_T UD_DSTATE_f;                /* '<S115>/UD' */
  real32_T Filter_DSTATE;              /* '<S88>/Filter' */
  real32_T Filter_DSTATE_n;            /* '<S35>/Filter' */
  real32_T Integrator_DSTATE;          /* '<S40>/Integrator' */
  real32_T epsilon;
  real32_T Gain1;                      /* '<S4>/Gain1' */
  real32_T e;                          /* '<S113>/Sum2' */
  real32_T gain;                       /* '<S113>/gain' */
  real32_T TSamp;                      /* '<S116>/TSamp' */
  real32_T Diff;                       /* '<S116>/Diff' */
  real32_T gain2;                      /* '<S113>/gain2' */
  real32_T TSamp_f;                    /* '<S115>/TSamp' */
  real32_T DiscreteFilter_tmp;
  real32_T z_idx_1;
  real32_T rtb_FISLookupTableData_idx_2;
  real32_T rtb_FISLookupTableData_idx_1;
  real32_T z_idx_1_tmp;
  real32_T b_atmp;
  real32_T xnorm;
  real32_T b_atmp_b;
  real32_T xnorm_p;
  int32_T i;
  int32_T b_i;
  int32_T A_tmp;
  int32_T ii;
  int32_T knt;
  int32_T lastv;
  int32_T ii_c;
  int32_T knt_f;
  uint32_T bpIndex[3];
  int16_T DataTypeConversion;          /* '<S3>/Data Type Conversion' */
  int16_T UnitDelay_DSTATE;            /* '<Root>/Unit Delay' */
  int16_T UnitDelay1_DSTATE;           /* '<Root>/Unit Delay1' */
  int8_T Filter_PrevResetState;        /* '<S88>/Filter' */
  int8_T Filter_PrevResetState_a;      /* '<S35>/Filter' */
  int8_T Integrator_PrevResetState;    /* '<S40>/Integrator' */
  uint8_T UnitDelay5;                  /* '<Root>/Unit Delay5' */
  uint8_T UnitDelay6;                  /* '<Root>/Unit Delay6' */
  uint8_T UnitDelay4_DSTATE;           /* '<Root>/Unit Delay4' */
  uint8_T UnitDelay5_DSTATE;           /* '<Root>/Unit Delay5' */
  uint8_T UnitDelay6_DSTATE;           /* '<Root>/Unit Delay6' */
} DW;

/* Constant parameters (default storage) */
typedef struct {
  /* Pooled Parameter (Mixed Expressions)
   * Referenced by:
   *   '<S117>/FISLookupTableIndex1'
   *   '<S117>/FISLookupTableIndex2'
   */
  real32_T pooled3[21];

  /* Computed Parameter: FISLookupTableData_Table
   * Referenced by: '<S117>/FISLookupTableData'
   */
  real32_T FISLookupTableData_Table[1323];

  /* Expression: p.R{1}
   * Referenced by: '<S118>/R1'
   */
  real32_T R1_Value[4];

  /* Expression: p.Q
   * Referenced by: '<S118>/Q'
   */
  real32_T Q_Value[4];
} ConstP;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real32_T gyroy;                      /* '<Root>/gyroy' */
  real32_T accx;                       /* '<Root>/accx' */
  real32_T accy;                       /* '<Root>/accy' */
  real32_T accz;                       /* '<Root>/accz' */
  real32_T motor_a_speed;              /* '<Root>/motor_a_speed' */
  real32_T motor_b_speed;              /* '<Root>/motor_b_speed' */
  real32_T target_speed;               /* '<Root>/target_speed' */
  real32_T gyrox;                      /* '<Root>/gyrox' */
  real32_T gyroz;                      /* '<Root>/gyroz' */
  real32_T turn_target;                /* '<Root>/turn_target' */
  uint8_T running_flag;                /* '<Root>/running_flag' */
} ExtU;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real32_T roll;                       /* '<Root>/roll' */
  real32_T pitch;                      /* '<Root>/pitch' */
  int16_T motor_a_pwm;                 /* '<Root>/motor_a_pwm' */
  int16_T motor_b_pwm;                 /* '<Root>/motor_b_pwm' */
  real32_T angle_target;               /* '<Root>/angle_target' */
  uint8_T running_success_flag;        /* '<Root>/running_success_flag' */
  real32_T e_factor;                   /* '<Root>/e_factor' */
  real32_T ec_factor;                  /* '<Root>/ec_factor' */
  real32_T ec_raw;                     /* '<Root>/ec_raw' */
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
extern real32_T BALANCE_KD;            /* Variable: BALANCE_KD
                                        * Referenced by: '<S113>/Constant1'
                                        */
extern real32_T BALANCE_KP;            /* Variable: BALANCE_KP
                                        * Referenced by: '<S113>/Constant3'
                                        */
extern real32_T SPD_KD;                /* Variable: SPD_KD
                                        * Referenced by: '<S33>/Derivative Gain'
                                        */
extern real32_T SPD_KI;                /* Variable: SPD_KI
                                        * Referenced by: '<S37>/Integral Gain'
                                        */
extern real32_T SPD_KP;                /* Variable: SPD_KP
                                        * Referenced by: '<S45>/Proportional Gain'
                                        */
extern real32_T TURN_KD;               /* Variable: TURN_KD
                                        * Referenced by: '<S86>/Derivative Gain'
                                        */
extern real32_T TURN_KP;               /* Variable: TURN_KP
                                        * Referenced by: '<S98>/Proportional Gain'
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
 * Block '<S115>/Data Type Duplicate' : Unused code path elimination
 * Block '<S116>/Data Type Duplicate' : Unused code path elimination
 * Block '<S113>/Scope' : Unused code path elimination
 * Block '<S113>/Scope1' : Unused code path elimination
 * Block '<S113>/Scope2' : Unused code path elimination
 * Block '<S118>/checkMeasurementFcn1Signals' : Unused code path elimination
 * Block '<S118>/checkStateTransitionFcnSignals' : Unused code path elimination
 * Block '<S117>/InputConversion' : Eliminate redundant data type conversion
 * Block '<S118>/DataTypeConversion_Enable1' : Eliminate redundant data type conversion
 * Block '<S118>/DataTypeConversion_Q' : Eliminate redundant data type conversion
 * Block '<S118>/DataTypeConversion_R1' : Eliminate redundant data type conversion
 * Block '<S118>/DataTypeConversion_uMeas1' : Eliminate redundant data type conversion
 * Block '<S118>/DataTypeConversion_uState' : Eliminate redundant data type conversion
 * Block '<S118>/DataTypeConversion_y1' : Eliminate redundant data type conversion
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
 * '<S3>'   : 'blinky/task_20ms'
 * '<S4>'   : 'blinky/task_5ms'
 * '<S5>'   : 'blinky/task_10ms/Discrete PID Controller'
 * '<S6>'   : 'blinky/task_10ms/Discrete PID Controller/Anti-windup'
 * '<S7>'   : 'blinky/task_10ms/Discrete PID Controller/D Gain'
 * '<S8>'   : 'blinky/task_10ms/Discrete PID Controller/External Derivative'
 * '<S9>'   : 'blinky/task_10ms/Discrete PID Controller/Filter'
 * '<S10>'  : 'blinky/task_10ms/Discrete PID Controller/Filter ICs'
 * '<S11>'  : 'blinky/task_10ms/Discrete PID Controller/I Gain'
 * '<S12>'  : 'blinky/task_10ms/Discrete PID Controller/Ideal P Gain'
 * '<S13>'  : 'blinky/task_10ms/Discrete PID Controller/Ideal P Gain Fdbk'
 * '<S14>'  : 'blinky/task_10ms/Discrete PID Controller/Integrator'
 * '<S15>'  : 'blinky/task_10ms/Discrete PID Controller/Integrator ICs'
 * '<S16>'  : 'blinky/task_10ms/Discrete PID Controller/N Copy'
 * '<S17>'  : 'blinky/task_10ms/Discrete PID Controller/N Gain'
 * '<S18>'  : 'blinky/task_10ms/Discrete PID Controller/P Copy'
 * '<S19>'  : 'blinky/task_10ms/Discrete PID Controller/Parallel P Gain'
 * '<S20>'  : 'blinky/task_10ms/Discrete PID Controller/Reset Signal'
 * '<S21>'  : 'blinky/task_10ms/Discrete PID Controller/Saturation'
 * '<S22>'  : 'blinky/task_10ms/Discrete PID Controller/Saturation Fdbk'
 * '<S23>'  : 'blinky/task_10ms/Discrete PID Controller/Sum'
 * '<S24>'  : 'blinky/task_10ms/Discrete PID Controller/Sum Fdbk'
 * '<S25>'  : 'blinky/task_10ms/Discrete PID Controller/Tracking Mode'
 * '<S26>'  : 'blinky/task_10ms/Discrete PID Controller/Tracking Mode Sum'
 * '<S27>'  : 'blinky/task_10ms/Discrete PID Controller/Tsamp - Integral'
 * '<S28>'  : 'blinky/task_10ms/Discrete PID Controller/Tsamp - Ngain'
 * '<S29>'  : 'blinky/task_10ms/Discrete PID Controller/postSat Signal'
 * '<S30>'  : 'blinky/task_10ms/Discrete PID Controller/preInt Signal'
 * '<S31>'  : 'blinky/task_10ms/Discrete PID Controller/preSat Signal'
 * '<S32>'  : 'blinky/task_10ms/Discrete PID Controller/Anti-windup/Passthrough'
 * '<S33>'  : 'blinky/task_10ms/Discrete PID Controller/D Gain/Internal Parameters'
 * '<S34>'  : 'blinky/task_10ms/Discrete PID Controller/External Derivative/Error'
 * '<S35>'  : 'blinky/task_10ms/Discrete PID Controller/Filter/Disc. Forward Euler Filter'
 * '<S36>'  : 'blinky/task_10ms/Discrete PID Controller/Filter ICs/Internal IC - Filter'
 * '<S37>'  : 'blinky/task_10ms/Discrete PID Controller/I Gain/Internal Parameters'
 * '<S38>'  : 'blinky/task_10ms/Discrete PID Controller/Ideal P Gain/Passthrough'
 * '<S39>'  : 'blinky/task_10ms/Discrete PID Controller/Ideal P Gain Fdbk/Disabled'
 * '<S40>'  : 'blinky/task_10ms/Discrete PID Controller/Integrator/Discrete'
 * '<S41>'  : 'blinky/task_10ms/Discrete PID Controller/Integrator ICs/Internal IC'
 * '<S42>'  : 'blinky/task_10ms/Discrete PID Controller/N Copy/Disabled'
 * '<S43>'  : 'blinky/task_10ms/Discrete PID Controller/N Gain/Internal Parameters'
 * '<S44>'  : 'blinky/task_10ms/Discrete PID Controller/P Copy/Disabled'
 * '<S45>'  : 'blinky/task_10ms/Discrete PID Controller/Parallel P Gain/Internal Parameters'
 * '<S46>'  : 'blinky/task_10ms/Discrete PID Controller/Reset Signal/External Reset'
 * '<S47>'  : 'blinky/task_10ms/Discrete PID Controller/Saturation/Enabled'
 * '<S48>'  : 'blinky/task_10ms/Discrete PID Controller/Saturation Fdbk/Disabled'
 * '<S49>'  : 'blinky/task_10ms/Discrete PID Controller/Sum/Sum_PID'
 * '<S50>'  : 'blinky/task_10ms/Discrete PID Controller/Sum Fdbk/Disabled'
 * '<S51>'  : 'blinky/task_10ms/Discrete PID Controller/Tracking Mode/Disabled'
 * '<S52>'  : 'blinky/task_10ms/Discrete PID Controller/Tracking Mode Sum/Passthrough'
 * '<S53>'  : 'blinky/task_10ms/Discrete PID Controller/Tsamp - Integral/TsSignalSpecification'
 * '<S54>'  : 'blinky/task_10ms/Discrete PID Controller/Tsamp - Ngain/Passthrough'
 * '<S55>'  : 'blinky/task_10ms/Discrete PID Controller/postSat Signal/Forward_Path'
 * '<S56>'  : 'blinky/task_10ms/Discrete PID Controller/preInt Signal/Internal PreInt'
 * '<S57>'  : 'blinky/task_10ms/Discrete PID Controller/preSat Signal/Forward_Path'
 * '<S58>'  : 'blinky/task_20ms/PID Controller'
 * '<S59>'  : 'blinky/task_20ms/PID Controller/Anti-windup'
 * '<S60>'  : 'blinky/task_20ms/PID Controller/D Gain'
 * '<S61>'  : 'blinky/task_20ms/PID Controller/External Derivative'
 * '<S62>'  : 'blinky/task_20ms/PID Controller/Filter'
 * '<S63>'  : 'blinky/task_20ms/PID Controller/Filter ICs'
 * '<S64>'  : 'blinky/task_20ms/PID Controller/I Gain'
 * '<S65>'  : 'blinky/task_20ms/PID Controller/Ideal P Gain'
 * '<S66>'  : 'blinky/task_20ms/PID Controller/Ideal P Gain Fdbk'
 * '<S67>'  : 'blinky/task_20ms/PID Controller/Integrator'
 * '<S68>'  : 'blinky/task_20ms/PID Controller/Integrator ICs'
 * '<S69>'  : 'blinky/task_20ms/PID Controller/N Copy'
 * '<S70>'  : 'blinky/task_20ms/PID Controller/N Gain'
 * '<S71>'  : 'blinky/task_20ms/PID Controller/P Copy'
 * '<S72>'  : 'blinky/task_20ms/PID Controller/Parallel P Gain'
 * '<S73>'  : 'blinky/task_20ms/PID Controller/Reset Signal'
 * '<S74>'  : 'blinky/task_20ms/PID Controller/Saturation'
 * '<S75>'  : 'blinky/task_20ms/PID Controller/Saturation Fdbk'
 * '<S76>'  : 'blinky/task_20ms/PID Controller/Sum'
 * '<S77>'  : 'blinky/task_20ms/PID Controller/Sum Fdbk'
 * '<S78>'  : 'blinky/task_20ms/PID Controller/Tracking Mode'
 * '<S79>'  : 'blinky/task_20ms/PID Controller/Tracking Mode Sum'
 * '<S80>'  : 'blinky/task_20ms/PID Controller/Tsamp - Integral'
 * '<S81>'  : 'blinky/task_20ms/PID Controller/Tsamp - Ngain'
 * '<S82>'  : 'blinky/task_20ms/PID Controller/postSat Signal'
 * '<S83>'  : 'blinky/task_20ms/PID Controller/preInt Signal'
 * '<S84>'  : 'blinky/task_20ms/PID Controller/preSat Signal'
 * '<S85>'  : 'blinky/task_20ms/PID Controller/Anti-windup/Disabled'
 * '<S86>'  : 'blinky/task_20ms/PID Controller/D Gain/Internal Parameters'
 * '<S87>'  : 'blinky/task_20ms/PID Controller/External Derivative/External Ydot'
 * '<S88>'  : 'blinky/task_20ms/PID Controller/Filter/Disc. Forward Euler Filter Only'
 * '<S89>'  : 'blinky/task_20ms/PID Controller/Filter ICs/Internal IC - Filter'
 * '<S90>'  : 'blinky/task_20ms/PID Controller/I Gain/Disabled'
 * '<S91>'  : 'blinky/task_20ms/PID Controller/Ideal P Gain/Passthrough'
 * '<S92>'  : 'blinky/task_20ms/PID Controller/Ideal P Gain Fdbk/Disabled'
 * '<S93>'  : 'blinky/task_20ms/PID Controller/Integrator/Disabled'
 * '<S94>'  : 'blinky/task_20ms/PID Controller/Integrator ICs/Disabled'
 * '<S95>'  : 'blinky/task_20ms/PID Controller/N Copy/Disabled'
 * '<S96>'  : 'blinky/task_20ms/PID Controller/N Gain/Internal Parameters'
 * '<S97>'  : 'blinky/task_20ms/PID Controller/P Copy/Disabled'
 * '<S98>'  : 'blinky/task_20ms/PID Controller/Parallel P Gain/Internal Parameters'
 * '<S99>'  : 'blinky/task_20ms/PID Controller/Reset Signal/External Reset'
 * '<S100>' : 'blinky/task_20ms/PID Controller/Saturation/Enabled'
 * '<S101>' : 'blinky/task_20ms/PID Controller/Saturation Fdbk/Disabled'
 * '<S102>' : 'blinky/task_20ms/PID Controller/Sum/Sum_PD'
 * '<S103>' : 'blinky/task_20ms/PID Controller/Sum Fdbk/Disabled'
 * '<S104>' : 'blinky/task_20ms/PID Controller/Tracking Mode/Disabled'
 * '<S105>' : 'blinky/task_20ms/PID Controller/Tracking Mode Sum/Passthrough'
 * '<S106>' : 'blinky/task_20ms/PID Controller/Tsamp - Integral/TsSignalSpecification'
 * '<S107>' : 'blinky/task_20ms/PID Controller/Tsamp - Ngain/Passthrough'
 * '<S108>' : 'blinky/task_20ms/PID Controller/postSat Signal/Forward_Path'
 * '<S109>' : 'blinky/task_20ms/PID Controller/preInt Signal/Internal PreInt'
 * '<S110>' : 'blinky/task_20ms/PID Controller/preSat Signal/Forward_Path'
 * '<S111>' : 'blinky/task_5ms/MATLAB Function'
 * '<S112>' : 'blinky/task_5ms/MATLAB Function3'
 * '<S113>' : 'blinky/task_5ms/Subsystem'
 * '<S114>' : 'blinky/task_5ms/kalman_system'
 * '<S115>' : 'blinky/task_5ms/Subsystem/Discrete Derivative'
 * '<S116>' : 'blinky/task_5ms/Subsystem/Discrete Derivative1'
 * '<S117>' : 'blinky/task_5ms/Subsystem/Fuzzy Logic  Controller'
 * '<S118>' : 'blinky/task_5ms/kalman_system/Extended Kalman Filter2'
 * '<S119>' : 'blinky/task_5ms/kalman_system/Extended Kalman Filter2/Correct1'
 * '<S120>' : 'blinky/task_5ms/kalman_system/Extended Kalman Filter2/Output'
 * '<S121>' : 'blinky/task_5ms/kalman_system/Extended Kalman Filter2/Predict'
 * '<S122>' : 'blinky/task_5ms/kalman_system/Extended Kalman Filter2/Output/MATLAB Function'
 */
#endif                                 /* blinky_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
