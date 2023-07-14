#include "ADC_setup.h"

//
// ConfigureADC - Write ADC configurations and power up the ADC for both
//                ADC A and ADC B
//
void ConfigureADC(void) {
  /* Configure ADCB */
  EALLOW;

  //
  // write configurations
  //
  AdcaRegs.ADCCTL2.bit.PRESCALE = 6;  // set ADCCLK divider to /4
  AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

  //
  // Set pulse positions to late
  //
  AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;

  //
  // power up the ADC
  //
  AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;

  //
  // delay for 1ms to allow ADC time to power up
  //
  DELAY_US(1000);

  EDIS;

  /* Configure ADCB */
  EALLOW;

  //
  // write configurations
  //
  AdcbRegs.ADCCTL2.bit.PRESCALE = 6;  // set ADCCLK divider to /4
  AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

  //
  // Set pulse positions to late
  //
  AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;

  //
  // power up the ADC
  //
  AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;

  //
  // delay for 1ms to allow ADC time to power up
  //
  DELAY_US(1000);

  EDIS;
}

//
// SetupADCEpwm - Setup ADC EPWM acquisition window
//
void SetupADCEpwm(void) {
  Uint16 acqps;

  //
  // Determine minimum acquisition window (in SYSCLKS) based on resolution
  //
  if (ADC_RESOLUTION_12BIT == AdcaRegs.ADCCTL2.bit.RESOLUTION) {
    acqps = 14;  // 75ns
  } else         // resolution is 16-bit
  {
    acqps = 63;  // 320ns
  }

  //
  // Select the channels to convert and end of conversion flag
  //
  EALLOW;
  AdcaRegs.ADCSOC0CTL.bit.CHSEL = ADC_CHANNEL_2;  // SOC0 will convert pin A0
  AdcaRegs.ADCSOC0CTL.bit.ACQPS = acqps;          // sample window is 100 SYSCLK cycles
  AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 5;            // trigger on ePWM1 SOCA/C
  AdcbRegs.ADCSOC0CTL.bit.CHSEL = ADC_CHANNEL_2;
  AdcbRegs.ADCSOC0CTL.bit.ACQPS = acqps;
  AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = 5;

  AdcaRegs.ADCSOC1CTL.bit.CHSEL = ADC_CHANNEL_3;
  AdcaRegs.ADCSOC1CTL.bit.ACQPS = acqps;
  AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = 5;
  AdcbRegs.ADCSOC1CTL.bit.CHSEL = ADC_CHANNEL_3;
  AdcbRegs.ADCSOC1CTL.bit.ACQPS = acqps;
  AdcbRegs.ADCSOC1CTL.bit.TRIGSEL = 5;

  AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 1;  // end of SOC1 will set ADCA's INT1 flag
  AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;    // enable INT1 flag
  AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;  // make sure INT1 flag is cleared

  AdcbRegs.ADCINTSEL1N2.bit.INT1SEL = 1;  // end of SOC1 will set ADCB's INT1 flag
  AdcbRegs.ADCINTSEL1N2.bit.INT1E = 1;    // enable INT1 flag
  AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
  EDIS;
}
