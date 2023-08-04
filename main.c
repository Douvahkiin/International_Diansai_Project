//
// Included Files
//
#include "ADC_setup.h"
#include "DAC_setup.h"
#include "EPWM_setup.h"
#include "F28x_Project.h"
#include "MACRO.h"
#include "OLED.h"
#include "filters.h"
#include "keys.h"
#include "math.h"
#include "pid.h"
#include "pll.h"
#include "pr.h"
#include "string.h"
#include "utils.h"

//
// Function Prototypes
//
interrupt void adca1_isr(void);
interrupt void xint1_isr(void);
interrupt void xint2_isr(void);
interrupt void xint3_isr(void);
interrupt void xint4_isr(void);
interrupt void xint5_isr(void);

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

Uint16 ADCCResults3[BUFFER_SIZE];
float32 ADCCResults3_converted[BUFFER_SIZE];

float32 ADCAResult0_mean = 0;
float32 ADCAResult2_mean = 0;
float32 ADCAResult3_mean = 0;
float32 ADCAResult14_mean = 0;
float32 ADCAResult15_mean = 0;

float32 ADCBResult2_mean = 0;
float32 ADCBResult3_mean = 0;

float32 ADCCResult3_mean = 0;

float32 wt = 0;
float32 wt1 = 0;
float32 wt2 = 0;
float32 wt3 = 0;

Uint16 frameIndex;
Uint16 largeIndex;

float32 Uref_u2 = 1.047;
float32 K_u2 = 69.2;
float32 Uref_u22 = 1.035;
float32 K_u22 = 35.7;
float32 Uref_i = 1.777;
float32 K_i = 3.195;
float32 Uref_i2 = 1.777;
float32 K_i2 = 3.5;
float32 Uref_udc = 1.044;
float32 K_udc = 140;
float32 Uref_udc2 = 1.021;
float32 K_udc2 = 70;
float32 Udc;

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
float32 outputPre_C3 = 0;

float32 rampInterval = 10;  // 10s
float32 inverter_std_I = 2.828427;
// float32 inverter_std_I = 2;
// float32 inverter_std_I = 1.41421356;
// float32 inverter_std_I = 1;
float32 inverter_std_U2 = 33.941125;
// float32 inverter_std_U2 = 21.2132034;
// float32 inverter_std_U2 = 14.1421356;  // 10*sqrt(2)
// float32 inverter_std_U2 = 7.0711;
// float32 inverter_std_U2 = 2.828;
// float32 rectifier_std_I = 2;
// float32 rectifier_std_Udc = 30;
// float32 rectifier_std_Udc = 20;
// float32 rectifier_std_Udc = 10;

// float32 triggerV = 180;
// float32 triggerV = 50;
// float32 triggerV = 30;
float32 triggerV = 18;
// float32 triggerV = 16.9705627;  // 12*sqrt(2)
// float32 triggerV = 10;
// float32 triggerV = 7.0711;  // 5*sqrt(2)

int intcount = 0;

float32 pid_n1_limit = 1;
float32 pid_n2_limit = 0.5;

float32 DAADCAL_receiver = 0;

int inverter_std_I_numArray[4];

float32 U2_q = 0;

/* 启动判断的相关变量 */
bool b1 = 0;
bool b2 = 0;
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
  GpioCtrlRegs.GPAPUD.bit.GPIO22 = 0;    // Enable pullup on GPIO22
  GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0;   // GPIO22 = GPIO22
  GpioCtrlRegs.GPADIR.bit.GPIO22 = 1;    // GPIO22 = output
  GpioDataRegs.GPACLEAR.bit.GPIO22 = 1;  // Load output latch

  GpioCtrlRegs.GPAPUD.bit.GPIO0 = 0;   // Enable pullup on GPIO0
  GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;  // GPIO0 = GPIO0
  GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;   // GPIO0 = output
  GpioDataRegs.GPACLEAR.bit.GPIO0 = 1;  // Load output latch

  GpioCtrlRegs.GPAPUD.bit.GPIO2 = 0;   // Enable pullup on GPIO2
  GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;  // GPIO2 = GPIO2
  GpioCtrlRegs.GPADIR.bit.GPIO2 = 1;   // GPIO2 = output
  GpioDataRegs.GPACLEAR.bit.GPIO2 = 1;  // Load output latch

  GpioCtrlRegs.GPAPUD.bit.GPIO4 = 0;    // Enable pullup on GPIO4
  GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;   // GPIO4 = GPIO4
  GpioCtrlRegs.GPADIR.bit.GPIO4 = 1;    // GPIO4 = output
  GpioDataRegs.GPACLEAR.bit.GPIO4 = 1;  // Load output latch

  GpioCtrlRegs.GPAPUD.bit.GPIO6 = 0;    // Enable pullup on GPIO6
  GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 0;   // GPIO6 = GPIO6
  GpioCtrlRegs.GPADIR.bit.GPIO6 = 1;    // GPIO6 = output
  GpioDataRegs.GPACLEAR.bit.GPIO6 = 1;  // Load output latch
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
  PieVectTable.XINT3_INT = &xint3_isr;
  PieVectTable.XINT4_INT = &xint4_isr;
  PieVectTable.XINT5_INT = &xint5_isr;
  EDIS;

  // Configure the ADC and power it up
  ConfigureADC();

  // Configure the ePWM
  ConfigureEPWM();

  // Setup the ADC for ePWM triggered conversions
  SetupADCEpwm();

  // Enable global Interrupts and higher priority real-time debug events:
  IER |= M_INT1;  // Enable group 1 interrupts
  IER |= M_INT12;  // Enable group 12 interrupts
  EINT;           // Enable Global interrupt INTM
  ERTM;           // Enable Global realtime interrupt DBGM

  frameIndex = 0;
  largeIndex = 0;

  // enable PIE interrupt
  PieCtrlRegs.PIEIER1.bit.INTx1 = 1;
  PieCtrlRegs.PIEIER1.bit.INTx4 = 1;  // Enable PIE Group 1 INT4
  PieCtrlRegs.PIEIER1.bit.INTx5 = 1;  // Enable PIE Group 1 INT5
  PieCtrlRegs.PIEIER12.bit.INTx1 = 1;  // XINT3
  PieCtrlRegs.PIEIER12.bit.INTx2 = 1;  // XINT4
  PieCtrlRegs.PIEIER12.bit.INTx3 = 1;  // XINT5

  // sync ePWM
  EALLOW;
  CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
  EDIS;

  // configure keys
  configure_keys();

  // init OLED
  OLED_Init();
  OLED_ShowString(0, 0, "1919810", 16, 1);
  OLED_ShowString(0, 16, "114514", 16, 1);
  OLED_Refresh();

  // pll, pid init
  pll_Init(2 * PI * 50, 2);  // 50Hz
  pid_nx_Init(0.15, 0, 0, pid_n1_limit, -pid_n1_limit, &pid_n1);
  pid_nx_Init(0.01, 7, 0, pid_n2_limit / 7, -pid_n2_limit / 7, &pid_n2);

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
  pr_init(1, -1.9966, 0.99686, 0.065682, -0.099831, 0.034161, &pr1);  // p=0.05, r=5
  // pr_init(1, -1.9966, 0.99686, 0.032841, -0.049915, 0.01708, &pr1);  // p=0.025, r=5
  // pr_init(1, -1.9966, 0.99686, 0.11568, -0.19966, 0.084004, &pr1);  // p=0.1, r=10
  // pr_init(1, -1.9966, 0.99686, 0.50784, -0.99831, 0.49059, &pr1);  // p=0.5, r=5
  // pr_init(1, -1.9966, 0.99686, 0.20784, -0.39932, 0.19153, &pr1);  // p=0.2, r=5
  // pr_init(1, -1.9966, 0.99686, 0.20157, -0.39932, 0.1978, &pr1);  // p=0.2, r=1

  //
  // pr2 init
  //
  // pr_init(1, -1.9966, 0.99686, 1.0016, -1.9966, 0.99530, &pr2);  // p=1, r=1
  // pr_init(1, -1.9966, 0.99686, 1.0078, -1.9966, 0.98902, &pr2);  // p=1, r=5
  // pr_init(1, -1.9966, 0.99686, 1.0157, -1.9966, 0.98118, &pr2);  // p=1, r=10
  // pr_init(1, -1.9966, 0.99686, 1.0314, -1.9966, 0.96550, &pr2);  // p=1, r=20
  // pr_init(1, -1.9966, 0.99686, 0.53136, -0.99831, 0.46707, &pr2);  // p=0.5, r=20
  // pr_init(1, -1.9966, 0.99686, 0.13136, -0.19966, 0.068322, &pr2);  // p=0.1, r=20
  pr_init(1, -1.9966, 0.99686, 0.10784, -0.19966, 0.091845, &pr2);  // p=0.1, r=5
  // pr_init(1, -1.9966, 0.99686, 0.065682, -0.099831, 0.034161, &pr2);  // p=0.05, r=5
  // pr_init(1, -1.9966, 0.99686, 0.11568, -0.19966, 0.084004, &pr2);  // p=0.1, r=10
  // pr_init(1, -1.9966, 0.99686, 0.50784, -0.99831, 0.49059, &pr2);  // p=0.5, r=5
  // pr_init(1, -1.9966, 0.99686, 0.20784, -0.39932, 0.19153, &pr2);  // p=0.2, r=5
  // pr_init(1, -1.9966, 0.99686, 0.30784, -0.59899, 0.29122, &pr2);  // p=0.3, r=5
  // pr_init(1, -1.9966, 0.99686, 0.20157, -0.39932, 0.1978, &pr2);  // p=0.2, r=1

  //
  // pr3 init
  //
  // pr_init(1, -1.9966, 0.99686, 1.0016, -1.9966, 0.99530, &pr3);  // p=1, r=1
  // pr_init(1, -1.9966, 0.99686, 1.0078, -1.9966, 0.98902, &pr3);  // p=1, r=5
  // pr_init(1, -1.9966, 0.99686, 1.0157, -1.9966, 0.98118, &pr3);  // p=1, r=10
  // pr_init(1, -1.9966, 0.99686, 1.0314, -1.9966, 0.96550, &pr3);  // p=1, r=20
  // pr_init(1, -1.9966, 0.99686, 0.53136, -0.99831, 0.46707, &pr3);  // p=0.5, r=20
  // pr_init(1, -1.9966, 0.99686, 0.13136, -0.19966, 0.068322, &pr3);  // p=0.1, r=20
  pr_init(1, -1.9966, 0.99686, 0.10784, -0.19966, 0.091845, &pr3);  // p=0.1, r=5
  // pr_init(1, -1.9966, 0.99686, 0.065682, -0.099831, 0.034161, &pr3);  // p=0.05, r=5
  // pr_init(1, -1.9966, 0.99686, 0.11568, -0.19966, 0.084004, &pr3);  // p=0.1, r=10
  // pr_init(1, -1.9966, 0.99686, 0.50784, -0.99831, 0.49059, &pr3);  // p=0.5, r=5
  // pr_init(1, -1.9966, 0.99686, 0.20784, -0.39932, 0.19153, &pr3);  // p=0.2, r=5
  // pr_init(1, -1.9966, 0.99686, 0.20157, -0.39932, 0.1978, &pr3);  // p=0.2, r=1

  wt1 = 0;
  wt2 = -2 * PI / 3;
  wt3 = 2 * PI / 3;

  b1 = 0;
  b2 = 0;
  b3 = 0;
  b4 = 0;

  unsigned char s1[16] = {0};
  unsigned char s2[16] = {0};
  // char const* s_stdI = "std I = ";
  char const* s_U2_q = "U2 q:";

  // take conversions indefinitely in loop
  EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  // unfreeze, and enter updown count mode
  EPwm1Regs.ETSEL.bit.SOCAEN = 1;                 // enable SOCA
  EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  // unfreeze, and enter updown count mode
  EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  // unfreeze, and enter updown count mode
  EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  // unfreeze, and enter updown count mode
  do {
    OLED_ClearGRAM();
    int len = strlen(s_U2_q);
    int i = 0;
    for (; i < len; i++) {
      s1[i] = s_U2_q[i];
    }
    float2numarray(U2_q, inverter_std_I_numArray);
    numarray2str(s2, inverter_std_I_numArray);

    OLED_ShowString(0, 0, s1, 16, 1);
    OLED_ShowString(0, 16, s2, 16, 1);
    OLED_Refresh();

    DELAY_US(100000);

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

  GpioDataRegs.GPATOGGLE.bit.GPIO22 = 1;

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

  ADCCResults3[frameIndex] = AdccResultRegs.ADCRESULT0;
  ADCCResults3_converted[frameIndex] = ADCCResults3[frameIndex] * 3.0 / 4096.0;

  ADCAResult0_mean = low_pass_filter(ADCAResults0_converted[frameIndex], &outputPre_A0, alpha_for_avg);
  ADCAResult2_mean = low_pass_filter(ADCAResults2_converted[frameIndex], &outputPre_A2, alpha_for_avg);
  ADCAResult3_mean = low_pass_filter(ADCAResults3_converted[frameIndex], &outputPre_A3, alpha_for_avg);
  ADCAResult14_mean = low_pass_filter(ADCAResults14_converted[frameIndex], &outputPre_A14, alpha_for_avg);
  ADCAResult15_mean = low_pass_filter(ADCAResults15_converted[frameIndex], &outputPre_A15, alpha_for_avg);

  ADCBResult2_mean = low_pass_filter(ADCBResults2_converted[frameIndex], &outputPre_B2, alpha_for_avg);
  ADCBResult3_mean = low_pass_filter(ADCBResults3_converted[frameIndex], &outputPre_B3, alpha_for_avg);

  ADCCResult3_mean = low_pass_filter(ADCCResults3_converted[frameIndex], &outputPre_C3, alpha_for_avg);

  // ADCAResults2_converted[frameIndex] = low_pass_filter(ADCAResults2_converted[frameIndex], &outputPre1, alpha1);
  // ADCAResults3_converted[frameIndex] = low_pass_filter(ADCAResults3_converted[frameIndex], &outputPre2, alpha2);
  // ADCBResults2_converted[frameIndex] = low_pass_filter(ADCBResults2_converted[frameIndex], &outputPre3, alpha3);
  // ADCBResults3_converted[frameIndex] = low_pass_filter(ADCBResults3_converted[frameIndex], &outputPre4, alpha4);

  DAADCAL_receiver = ADCAResults2_converted[frameIndex];

  /* 这是周期为50Hz的正弦波表示 */
  wt = wt + PI / 100 / 2 * SW_FREQ;
  if (wt > PI * 2) wt -= PI * 2;

  U2_result[frameIndex] = (ADCAResults14_converted[frameIndex] - Uref_u2) * K_u2;
  U22_result[frameIndex] = (ADCAResults2_converted[frameIndex] - Uref_u22) * K_u22;
  // U22_result[frameIndex] = U2_result[frameIndex];
  ig_result[frameIndex] = -(ADCBResults3_converted[frameIndex] - Uref_i) * K_i;
  ig2_result[frameIndex] = (ADCAResults15_converted[frameIndex] - Uref_i2) * K_i2;
  Udc_result[frameIndex] = (ADCCResults3_converted[frameIndex] - Uref_udc) * K_udc;
  Udc2_result[frameIndex] = (ADCAResults0_converted[frameIndex] - Uref_udc2) * K_udc2;

  // U2 pll
  float32 pll_input = U2_result[frameIndex];
  // float32 pll_input = inverter_std_U2 * sin(wt);
  // pll 的结果
  pll_result = pll_Run(pll_input);
  // 用正弦便于判断正确
  pll_result = cos(pll_result);
  // changeDACBVal(2048 + 2000.0 * pll_result);
  // changeDACAVal(ADCBResults3[frameIndex]);
  changeDACBVal(ADCAResults14[frameIndex]);

  //
  // (逆变侧)交流电压环
  //
  // err1 = sin(wt) * inverter_std_U2 - U2_result[frameIndex];
  // float32 pr1_input;
  // if (b2) {
  //   pr1_input = err1;
  // } else {
  //   pr1_input = 0;
  // }
  // pr1_out = pr_run(pr1_input, &pr1);

  float32 err_U2_q = inverter_std_U2 - U2_q;
  float32 pid_n2_input = b2 ? err_U2_q : 0;
  float32 pid_n2_out = pid_nx_Run(pid_n2_input, &pid_n2);
  pid_n2_out = saturation(pid_n2_out, pid_n2_limit, -pid_n2_limit);
  float32 pwm_sig = (pid_n2_out + 0.5) * sin(wt);
  changeCMP_value(pwm_sig);
  // changeCMP_value(sin(wt));

  //
  // (逆变侧)交流电流环
  //
  // err2 = sin(wt) * inverter_std_I - ig_result[frameIndex];
  // err2 = pll_result * inverter_std_I - ig_result[frameIndex];
  // err2 = pr1_out - ig_result[frameIndex];
  // float32 pr2_input;
  // float32 pid_n1_input;
  // if (b2) {
  //   pr2_input = err2;
  //   pid_n1_input = err2;
  // } else {
  //   pr2_input = 0;
  //   pid_n1_input = 0;
  // }
  // pr2_out = pr_run(pr2_input, &pr2);
  // pid_n1_out = pid_nx_Run(pid_n1_input, &pid_n1);
  // pid_n1_out = saturation(pid_n1_out, pid_n1_limit, -pid_n1_limit);
  // changeDACAVal(2048 + 2000.0 * err1 / inverter_std_U2);

  // changeCMP_value(pr1_out);
  // changeCMP_value(pr2_out);
  // changeCMP_value(pid_n1_out);

  if (b2) {
    GpioDataRegs.GPASET.bit.GPIO0 = 1;
    GpioDataRegs.GPASET.bit.GPIO2 = 1;
  } else {
    GpioDataRegs.GPACLEAR.bit.GPIO0 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO2 = 1;
  }

  frameIndex++;
  largeIndex++;
  largeIndex %= LARGE_BUFFER;
  frameIndex %= BUFFER_SIZE;
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
  Uref_u2 = ADCAResult14_mean;
  Uref_u22 = ADCAResult2_mean;
  Uref_i = ADCBResult3_mean;
  Uref_i2 = ADCAResult15_mean;
  Uref_udc = ADCCResult3_mean;
  Uref_udc2 = ADCAResult0_mean;

  // 差点忘了这个! 没有这个的话, 这个以及其它同组的中断都不会再被触发了
  PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

interrupt void xint2_isr(void) {
  b2 = !b2;
  pid_n2.integral = 0;

  PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

interrupt void xint3_isr(void) {
  intcount++;

  PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;
}
//
// End of file
//

interrupt void xint4_isr(void) {
  intcount++;

  PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;
}
interrupt void xint5_isr(void) {
  intcount++;

  PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;
}