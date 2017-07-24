//###########################################################################
//
//!  \addtogroup f2833x_example_list
//!  <h1>Watchdog interrupt Test (watchdog)</h1>
//!
//!  This program exercises the watchdog.
//!
//!  First the watchdog is connected to the WAKEINT interrupt of the
//!  PIE block.  The code is then put into an infinite loop.
//!
//!  The user can select to feed the watchdog key register or not
//!  by commenting the following line of code in the infinite loop:
//!  \b ServiceDog(); \n
//!
//!  If the watchdog key register is fed by the ServiceDog function
//!  then the WAKEINT interrupt is not taken.  If the key register
//!  is not fed by the ServiceDog function then WAKEINT will be taken.
//!
//!  \b Watch \b Variables \n
//!  - \b LoopCount - Number of times through the infinite loop
//!  - \b WakeCount - Number of times through WAKEINT
//
//###########################################################################
// $TI Release: F2833x/F2823x Header Files and Peripheral Examples V140 $
// $Release Date: March  4, 2015 $
// $Copyright: Copyright (C) 2007-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

// add a new comment by QiuZhikang

#include "DSP2833x_Device.h"     // Headerfile Include File
#include "DSP2833x_Examples.h"   // Examples Include File


// Prototype statements for functions found within this file.
__interrupt void adcint_isr(void);

int LoopCount = 0;



// Global variables for this example


void main(void)
{
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2833x_SysCtrl.c file.
   InitSysCtrl();

// Step 2. Initialize GPIO:
// This example function is found in the DSP2833x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// InitGpio();  // Skipped for this example

// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
   DINT;

// Initialize PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the DSP2833x_PieCtrl.c file.
   InitPieCtrl();

// Disable CPU interrupts and clear all CPU interrupt flags:
   IER = 0x0000;
   IFR = 0x0000;

// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in DSP2833x_DefaultIsr.c.
// This function is found in DSP2833x_PieVect.c.
   InitPieVectTable();

// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
   EALLOW;	// This is needed to write to EALLOW protected registers
   PieVectTable.ADCINT = &adcint_isr;
   EDIS;   // This is needed to disable write to EALLOW protected registers

// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP2833x_InitPeripherals.c
// InitPeripherals(); // Not required for this example

   InitEPwm();
   InitAdc();
   Init_F28335_Adc();



// Enable WAKEINT in the PIE: Group 1 interrupt 8
// Enable INT1 which is connected to WAKEINT:
   PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
   PieCtrlRegs.PIEACK.all = 0xFFFF;
   PieCtrlRegs.PIEIER1.bit.INTx6 = 1;   // Enable PIE Group 1 INT8
   IER |= M_INT1;                       // Enable CPU int1
   EINT;                                // Enable Global Interrupts




   EDIS;

// Step 6. IDLE loop. Just sit and loop forever (optional):
   for(;;)
   {
      LoopCount++;

      // Uncomment ServiceDog to just loop here
      // Comment ServiceDog to take the WAKEINT instead
      // ServiceDog();
   }
}







__interrupt void adcint_isr(void)
{


	// Acknowledge this interrupt to get more from group 1
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;
	AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;


}

//===========================================================================
// No more.
//===========================================================================
