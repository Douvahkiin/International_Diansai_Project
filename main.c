//
// Included Files
//
#include "ADC_setup.h"
#include "DAC_setup.h"
#include "EPWM_setup.h"
#include "F28x_Project.h"
#include "MACRO.h"
#include "filters.h"
#include "keys.h"
#include "math.h"
#include "pid.h"
#include "pll.h"
#include "pr.h"

//
// Function Prototypes
//
interrupt void adca1_isr(void);
interrupt void xint1_isr(void);
interrupt void xint2_isr(void);

//
// externs
//
extern struct _pr pr1;
extern struct _pr pr2;
extern struct _pr pr3;
extern struct _pr pr4;
extern struct _pid pid_n1;
extern struct _pid pid_n2;
extern struct _pll pll;

//
// Globals
//
Uint16 ADCAResults0[BUFFER_SIZE];
float32 ADCAResults0_converted[BUFFER_SIZE];
Uint16 ADCAResults2[BUFFER_SIZE];
float32 ADCAResults2_converted[BUFFER_SIZE];
Uint16 ADCAResults3[BUFFER_SIZE];
float32 ADCAResults3_converted[BUFFER_SIZE];
Uint16 ADCAResults14[BUFFER_SIZE];
float32 ADCAResults14_converted[BUFFER_SIZE];
Uint16 ADCAResults15[BUFFER_SIZE];
float32 ADCAResults15_converted[BUFFER_SIZE];

Uint16 ADCBResults2[BUFFER_SIZE];
float32 ADCBResults2_converted[BUFFER_SIZE];
Uint16 ADCBResults3[BUFFER_SIZE];
float32 ADCBResults3_converted[BUFFER_SIZE];

Uint16 ADCCResults2[BUFFER_SIZE];
float32 ADCCResults2_converted[BUFFER_SIZE];
Uint16 ADCCResults3[BUFFER_SIZE];
float32 ADCCResults3_converted[BUFFER_SIZE];

float32 ADCAResult0_mean = 0;
float32 ADCAResult2_mean = 0;
float32 ADCAResult3_mean = 0;
float32 ADCAResult14_mean = 0;
float32 ADCAResult15_mean = 0;

float32 ADCBResult2_mean = 0;
float32 ADCBResult3_mean = 0;

float32 ADCCResult2_mean = 0;
float32 ADCCResult3_mean = 0;

float32 wt = 0;
float32 wt1 = 0;
float32 wt2 = 0;
float32 wt3 = 0;

Uint16 frameIndex;
Uint16 largeIndex;

volatile Uint16 bufferFull;

float32 Uref_u2 = 0.4167;
float32 K_u2 = 30;
float32 Uref_u22 = 1.035;
float32 K_u22 = 35.7;
float32 Uref_i = 1.5;
float32 K_i = 4.175;
float32 Uref_i2 = 1.777;
float32 K_i2 = 3.5;
float32 Uref_udc = 0;
float32 K_udc = 10;
float32 Uref_udc2 = 1.021;
float32 K_udc2 = 70;
float32 std_ig;
float32 Udc;
float32 std_Udc = 10;

float32 U2_result[BUFFER_SIZE];
float32 U22_result[BUFFER_SIZE];
float32 Udc_result[BUFFER_SIZE];
float32 Udc2_result[BUFFER_SIZE];
float32 ig_result[BUFFER_SIZE];
float32 ig2_result[BUFFER_SIZE];
float32 pll_result;
float32 pid_n1_out;
float32 err1;
float32 err2;
float32 pr1_out;
float32 pr2_out;

float32 alpha1 = 1;
float32 alpha2 = 1;
float32 alpha3 = 1;
float32 alpha4 = 1;
float32 alpha_for_avg = 0.1;

float32 outputPre1 = 0;
float32 outputPre2 = 0;
float32 outputPre3 = 0;
float32 outputPre4 = 0;
float32 outputPre_A0 = 0;
float32 outputPre_A2 = 0;
float32 outputPre_A3 = 0;
float32 outputPre_A14 = 0;
float32 outputPre_A15 = 0;
float32 outputPre_B2 = 0;
float32 outputPre_B3 = 0;
float32 outputPre_C2 = 0;
float32 outputPre_C3 = 0;

float32 inverter_std_I = 1;
// float32 inverter_std_U2 = 21.2132;
float32 inverter_std_U2 = 7.0711;
// float32 inverter_std_U2 = 2.828;
float32 rectifier_std_I = 2;
// float32 rectifier_std_Udc = 30;
float32 rectifier_std_Udc = 10;

/* 启动判断的相关变量 */
bool b1;
bool b2;
bool b3 = 0;
bool b4 = 0;

void main(void) {
  // Initialize System Control: PLL, WatchDog, enable Peripheral Clocks
  InitSysCtrl();

  // Initialize GPIO:
  InitGpio();
  InitEPwm1Gpio();
  InitEPwm2Gpio();
  InitEPwm3Gpio();
  InitEPwm4Gpio();
  ConfigureDAC();
  //
  // Enable an GPIO output on GPIO22, set it high/low
  //
  EALLOW;
  GpioCtrlRegs.GPAPUD.bit.GPIO22 = 0;   // Enable pullup on GPIO22
  GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0;  // GPIO22 = GPIO22
  GpioCtrlRegs.GPADIR.bit.GPIO22 = 1;   // GPIO22 = output
  GpioDataRegs.GPASET.bit.GPIO22 = 1;   // Load output latch
  EDIS;

  // Clear all interrupts and initialize PIE vector table: Disable CPU interrupts
  DINT;

  // Initialize the PIE control registers to their default state.
  // The default state is all PIE interrupts disabled and flags are cleared.
  InitPieCtrl();

  // Disable CPU interrupts and clear all CPU interrupt flags:
  IER = 0x0000;
  IFR = 0x0000;

  // Initialize the PIE vector table with pointers to the shell Interrupt Service Routines (ISR).
  InitPieVectTable();

  // Map ISR functions
  EALLOW;
  PieVectTable.ADCA1_INT = &adca1_isr;  // function for ADCA interrupt 1
  PieVectTable.XINT1_INT = &xint1_isr;
  PieVectTable.XINT2_INT = &xint2_isr;
  EDIS;

  // Configure the ADC and power it up
  ConfigureADC();

  // Configure the ePWM
  ConfigureEPWM();

  // Setup the ADC for ePWM triggered conversions
  SetupADCEpwm();

  // Enable global Interrupts and higher priority real-time debug events:
  IER |= M_INT1;  // Enable group 1 interrupts
  EINT;           // Enable Global interrupt INTM
  ERTM;           // Enable Global realtime interrupt DBGM

  frameIndex = 0;
  largeIndex = 0;
  bufferFull = 0;

  // enable PIE interrupt
  PieCtrlRegs.PIEIER1.bit.INTx1 = 1;
  PieCtrlRegs.PIEIER1.bit.INTx4 = 1;  // Enable PIE Group 1 INT4
  PieCtrlRegs.PIEIER1.bit.INTx5 = 1;  // Enable PIE Group 1 INT5

  // sync ePWM
  EALLOW;
  CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
  EDIS;

  // configure keys
  configure_keys();

  // pll, pid init
  pll_Init(2 * PI * 50, 2);  // 50Hz
  pid_nx_Init(0.1, 1, 0, 3, -3, &pid_n1);

  //
  // pr1 init
  //
  /* pr init, Ts = 0.0001*/
  // pr_init(1, -1.9928, 0.99374, 1.3131, -1.9928, 0.68064, &pr1);  // p=1, r=100
  // pr_init(1, -1.9928, 0.99374, 1.1565, -1.9928, 0.83719, &pr1);  // p=1, r=50
  // pr_init(1, -1.9928, 0.99374, 1.0313, -1.9928, 0.96243, &pr1);  // p=1, r=10
  // pr_init(1, -1.9928, 0.99374, 1.0157, -1.9928, 0.97808, &pr1);  // p=1, r=5

  /* pr_init, Ts = 0.00005*/
  // pr_init(1, -1.9966, 0.99686, 1.0016, -1.9966, 0.99530, &pr1);  // p=1, r=1
  // pr_init(1, -1.9966, 0.99686, 1.0078, -1.9966, 0.98902, &pr1);  // p=1, r=5
  // pr_init(1, -1.9966, 0.99686, 1.0157, -1.9966, 0.98118, &pr1);  // p=1, r=10
  // pr_init(1, -1.9966, 0.99686, 1.0314, -1.9966, 0.96550, &pr1);  // p=1, r=20
  // pr_init(1, -1.9966, 0.99686, 0.53136, -0.99831, 0.46707, &pr1);  // p=0.5, r=20
  // pr_init(1, -1.9966, 0.99686, 0.13136, -0.19966, 0.068322, &pr1);  // p=0.1, r=20
  // pr_init(1, -1.9966, 0.99686, 0.10784, -0.19966, 0.091845, &pr1);  // p=0.1, r=5
  // pr_init(1, -1.9966, 0.99686, 0.50784, -0.99831, 0.49059, &pr1);  // p=0.5, r=5
  pr_init(1, -1.9966, 0.99686, 0.20784, -0.39932, 0.19153, &pr1);  // p=0.2, r=5

  //
  // pr2 init
  //
  // pr_init(1, -1.9966, 0.99686, 1.0016, -1.9966, 0.99530, &pr2);  // p=1, r=1
  // pr_init(1, -1.9966, 0.99686, 1.0078, -1.9966, 0.98902, &pr2);  // p=1, r=5
  // pr_init(1, -1.9966, 0.99686, 1.0157, -1.9966, 0.98118, &pr2);  // p=1, r=10
  // pr_init(1, -1.9966, 0.99686, 1.0314, -1.9966, 0.96550, &pr2);  // p=1, r=20
  // pr_init(1, -1.9966, 0.99686, 0.53136, -0.99831, 0.46707, &pr2);  // p=0.5, r=20
  // pr_init(1, -1.9966, 0.99686, 0.13136, -0.19966, 0.068322, &pr2);  // p=0.1, r=20
  // pr_init(1, -1.9966, 0.99686, 0.10784, -0.19966, 0.091845, &pr2);  // p=0.1, r=5
  // pr_init(1, -1.9966, 0.99686, 0.50784, -0.99831, 0.49059, &pr2);  // p=0.5, r=5
  pr_init(1, -1.9966, 0.99686, 0.20784, -0.39932, 0.19153, &pr2);  // p=0.2, r=5

  //
  // pr3 init
  //
  // pr_init(1, -1.9966, 0.99686, 1.0016, -1.9966, 0.99530, &pr3);  // p=1, r=1
  // pr_init(1, -1.9966, 0.99686, 1.0078, -1.9966, 0.98902, &pr3);  // p=1, r=5
  // pr_init(1, -1.9966, 0.99686, 1.0157, -1.9966, 0.98118, &pr3);  // p=1, r=10
  // pr_init(1, -1.9966, 0.99686, 1.0314, -1.9966, 0.96550, &pr3);  // p=1, r=20
  // pr_init(1, -1.9966, 0.99686, 0.53136, -0.99831, 0.46707, &pr3);  // p=0.5, r=20
  // pr_init(1, -1.9966, 0.99686, 0.13136, -0.19966, 0.068322, &pr3);  // p=0.1, r=20
  // pr_init(1, -1.9966, 0.99686, 0.10784, -0.19966, 0.091845, &pr3);  // p=0.1, r=5
  // pr_init(1, -1.9966, 0.99686, 0.50784, -0.99831, 0.49059, &pr3);  // p=0.5, r=5
  pr_init(1, -1.9966, 0.99686, 0.20784, -0.39932, 0.19153, &pr3);  // p=0.2, r=5

  wt1 = 0;
  wt2 = -2 * PI / 3;
  wt3 = 2 * PI / 3;

  // take conversions indefinitely in loop
  EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  // unfreeze, and enter updown count mode
  EPwm1Regs.ETSEL.bit.SOCAEN = 1;                 // enable SOCA
  EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  // unfreeze, and enter updown count mode
  EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  // unfreeze, and enter updown count mode
  EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  // unfreeze, and enter updown count mode
  do {
    // wait while ePWM causes ADC conversions, which then cause interrupts,
    // which fill the results buffer, eventually setting the bufferFull flag
    while (!bufferFull) {
    }
    bufferFull = 0;  // clear the buffer full flag

    // software breakpoint, hit run again to get updated conversions
    // asm("   ESTOP0");
  } while (1);
}

//
// adca1_isr - Read ADC Buffer in ISR
//
interrupt void adca1_isr(void) {
  while (AdcaRegs.ADCINTFLG.bit.ADCINT1 == 0 || AdcbRegs.ADCINTFLG.bit.ADCINT1 == 0 || AdccRegs.ADCINTFLG.bit.ADCINT1 == 0) {
  }
  AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
  AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
  AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;

  ADCAResults0[frameIndex] = AdcaResultRegs.ADCRESULT2;
  ADCAResults0_converted[frameIndex] = ADCAResults0[frameIndex] * 3.0 / 4096.0;
  ADCAResults2[frameIndex] = AdcaResultRegs.ADCRESULT0;
  ADCAResults2_converted[frameIndex] = ADCAResults2[frameIndex] * 3.0 / 4096.0;
  ADCAResults3[frameIndex] = AdcaResultRegs.ADCRESULT1;
  ADCAResults3_converted[frameIndex] = ADCAResults3[frameIndex] * 3.0 / 4096.0;
  ADCAResults14[frameIndex] = AdcaResultRegs.ADCRESULT14;
  ADCAResults14_converted[frameIndex] = ADCAResults14[frameIndex] * 3.0 / 4096.0;
  ADCAResults15[frameIndex] = AdcaResultRegs.ADCRESULT15;
  ADCAResults15_converted[frameIndex] = ADCAResults15[frameIndex] * 3.0 / 4096.0;
  // changeDACAVal(ADCAResults14[frameIndex]);

  ADCBResults2[frameIndex] = AdcbResultRegs.ADCRESULT0;
  ADCBResults2_converted[frameIndex] = ADCBResults2[frameIndex] * 3.0 / 4096.0;
  ADCBResults3[frameIndex] = AdcbResultRegs.ADCRESULT1;
  ADCBResults3_converted[frameIndex] = ADCBResults3[frameIndex] * 3.0 / 4096.0;

  ADCCResults2[frameIndex] = AdccResultRegs.ADCRESULT1;
  ADCCResults2_converted[frameIndex] = ADCCResults2[frameIndex] * 3.0 / 4096.0;
  ADCCResults3[frameIndex] = AdccResultRegs.ADCRESULT0;
  ADCCResults3_converted[frameIndex] = ADCCResults3[frameIndex] * 3.0 / 4096.0;

  ADCAResult0_mean = low_pass_filter(ADCAResults0_converted[frameIndex], &outputPre_A0, alpha_for_avg);
  ADCAResult2_mean = low_pass_filter(ADCAResults2_converted[frameIndex], &outputPre_A2, alpha_for_avg);
  ADCAResult3_mean = low_pass_filter(ADCAResults3_converted[frameIndex], &outputPre_A3, alpha_for_avg);
  ADCAResult14_mean = low_pass_filter(ADCAResults14_converted[frameIndex], &outputPre_A14, alpha_for_avg);
  ADCAResult15_mean = low_pass_filter(ADCAResults15_converted[frameIndex], &outputPre_A15, alpha_for_avg);

  ADCBResult2_mean = low_pass_filter(ADCBResults2_converted[frameIndex], &outputPre_B2, alpha_for_avg);
  ADCBResult3_mean = low_pass_filter(ADCBResults3_converted[frameIndex], &outputPre_B3, alpha_for_avg);

  ADCCResult2_mean = low_pass_filter(ADCCResults2_converted[frameIndex], &outputPre_C2, alpha_for_avg);
  ADCCResult3_mean = low_pass_filter(ADCCResults3_converted[frameIndex], &outputPre_C3, alpha_for_avg);

  // ADCAResults2_converted[frameIndex] = low_pass_filter(ADCAResults2_converted[frameIndex], &outputPre1, alpha1);
  // ADCAResults3_converted[frameIndex] = low_pass_filter(ADCAResults3_converted[frameIndex], &outputPre2, alpha2);
  // ADCBResults2_converted[frameIndex] = low_pass_filter(ADCBResults2_converted[frameIndex], &outputPre3, alpha3);
  // ADCBResults3_converted[frameIndex] = low_pass_filter(ADCBResults3_converted[frameIndex], &outputPre4, alpha4);

  /* 这是周期为50Hz的正弦波表示 */
  wt = wt + PI / 100 / 2 * SW_FREQ;
  if (wt > PI * 2) wt -= PI * 2;

  U2_result[frameIndex] = (ADCAResults3_converted[frameIndex] - Uref_u2) * K_u2;
  U22_result[frameIndex] = (ADCAResults2_converted[frameIndex] - Uref_u22) * K_u22;
  ig_result[frameIndex] = -(ADCBResults2_converted[frameIndex] - Uref_i) * K_i;
  ig2_result[frameIndex] = (ADCAResults15_converted[frameIndex] - Uref_i2) * K_i2;
  Udc_result[frameIndex] = (ADCCResults2_converted[frameIndex] - Uref_udc) * K_udc;
  Udc2_result[frameIndex] = (ADCAResults0_converted[frameIndex] - Uref_udc2) * K_udc2;

  // // pll input 为交流侧电压
  // float32 pll_input = U2_result[frameIndex];
  // // float32 pll_input = inverter_std_U2 * sin(wt);
  // // pll 的结果
  // pll_result = pll_Run(pll_input);
  // // 用正弦便于判断正确
  // pll_result = cos(pll_result);
  // changeDACBVal(ADCAResults14[frameIndex]);
  // // changeDACAVal(2048 + 2000.0 * pll_result);

  //
  // (逆变侧)交流电压环
  //
  err1 = sin(wt) * inverter_std_U2 - U2_result[frameIndex];
  float32 pr1_input = err1;
  pr1_out = pr_run(pr1_input, &pr1);

  //
  // (逆变侧)交流电流环
  //
  // err2 = sin(wt) * inverter_std_I - ig_result[frameIndex];
  err2 = pr1_out - ig_result[frameIndex];
  float32 pr2_input = err2;
  pr2_out = pr_run(pr2_input, &pr2);

  // U22 pll
  float32 pll_input = U22_result[frameIndex];
  // float32 pll_input = inverter_std_U2 * sin(wt);
  // pll 的结果
  pll_result = pll_Run(pll_input);
  // 用正弦便于判断正确
  pll_result = cos(pll_result);
  changeDACBVal(2048 + 2000.0 * pll_result);
  // changeDACBVal(ADCAResults2[frameIndex]);

  /* PR控制器启动判断, 启动后变量 b2 自锁 */
  b1 = fabsf(U22_result[frameIndex]) >= 3;
  b2 = b1 || b3;
  b3 = b2;

  //
  // (整流侧)直流电压环
  //
  float32 err_Udc;
  if (b2) {
    err_Udc = rectifier_std_Udc - Udc2_result[frameIndex];
  } else {
    err_Udc = 0;
  }
  float32 pid_n1_input = err_Udc;
  pid_n1_out = pid_nx_Run(pid_n1_input, &pid_n1);
  pid_n1_out = saturation(pid_n1_out, 4.5, -4.5);

  //
  // (整流侧)交流电流环
  //
  static float32 err_i22;
  if (b2) {
    err_i22 = pll_result * pid_n1_out - ig2_result[frameIndex];
  } else {
    err_i22 = 0;
  }
  float32 pr3_input = -err_i22;
  float32 pr3_out = pr_run(pr3_input, &pr3);

  //
  // change PWM duty
  //
  // changeCMP_value(pr1_out);
  // changeCMP_value(pr2_out);
  // changeCMP_value(pid_n1_out);
  // changeCMP_value_brige2(1 * err_i22);
  changeCMP_phase(wt);
  // changeCMP_value_brige2(sin(wt));
  changeCMP_value_brige2(pr3_out);
  // changeCMP_value_brige2(-err_i22);
  // changeCMP_value(0.8);

  frameIndex++;
  largeIndex++;
  largeIndex %= LARGE_BUFFER;
  if (BUFFER_SIZE <= frameIndex) {
    frameIndex = 0;
    bufferFull = 1;
  }
  //
  // Check if overflow has occurred
  //
  if (1 == AdcaRegs.ADCINTOVF.bit.ADCINT1) {
    AdcaRegs.ADCINTOVFCLR.bit.ADCINT1 = 1;  // clear INT1 overflow flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;  // clear INT1 flag
  }
  if (1 == AdcbRegs.ADCINTOVF.bit.ADCINT1) {
    AdcbRegs.ADCINTOVFCLR.bit.ADCINT1 = 1;  // clear INT1 overflow flag
    AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;  // clear INT1 flag
  }

  PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

interrupt void xint1_isr(void) {
  Uref_u2 = ADCAResult3_mean;
  Uref_u22 = ADCAResult2_mean;
  Uref_i = ADCBResult2_mean;
  Uref_i2 = ADCAResult15_mean;
  Uref_udc = ADCCResult2_mean;
  Uref_udc2 = ADCAResult0_mean;

  // 差点忘了这个! 没有这个的话, 这个以及其它同组的中断都不会再被触发了
  PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

interrupt void xint2_isr(void) {
  PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
//
// End of file
//
