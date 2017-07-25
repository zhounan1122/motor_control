#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "control_algorithm.h"

__interrupt void adcint_isr(void);

extern void Init_F28335_Adc(void);

PIDREG3 IntensityPID = PIDREG3_DEFAULTS;
PIDREG3 SpeedPID = PIDREG3_DEFAULTS;

Uint16 loopCnt = 0;
Uint16 Intensity = 0;
Uint32 deltaCnt = 0;


void main(void)
{
// Step 1. Initialize System Control:PLL, WatchDog, enable Peripheral Clocks
   InitSysCtrl();

// Step 3. Clear all interrupts and initialize PIE vector table:
   DINT;

// Initialize PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags are cleared.
   InitPieCtrl();

// Disable CPU interrupts and clear all CPU interrupt flags:
   IER = 0x0000;
   IFR = 0x0000;

// Initialize the PIE vector table with pointers to the shell Interrupt Service Routines (ISR).
// This will populate the entire table, even if the interrupt is not used in this example.  This is useful for debug purposes.
   InitPieVectTable();

// Interrupts that are used in this example are re-mapped to ISR functions found within this file.
   EALLOW;	// This is needed to write to EALLOW protected registers
   PieVectTable.ADCINT = &adcint_isr;
   EDIS;   // This is needed to disable write to EALLOW protected registers

   InitEPwm();	//完成 PWM模块的初始化

   InitAdc();	//完成ADC模块的初始化
   Init_F28335_Adc();	//感觉可以写到一个函数内

   InitEQep();//完成eQEP模块的初始化

// Enable WAKEINT in the PIE: Group 1 interrupt 8
// Enable INT1 which is connected to WAKEINT:
   EALLOW;
   PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
   PieCtrlRegs.PIEACK.all = 0xFFFF;
   PieCtrlRegs.PIEIER1.bit.INTx6 = 1;   // Enable PIE Group 1 INT6
   IER |= M_INT1;                       // Enable CPU int1
   EINT;                                // Enable Global Interrupts
   EDIS;


   SpeedPID.Ref = 3000;	//设定速度值

   for(;;)
   {
   }
}


__interrupt void adcint_isr(void)
{
	//在此中断函数中进行电流环计算，同时适时进行速度环计算
	++loopCnt;
	Intensity = AdcRegs.ADCRESULT0>>4;	//读取电流值

	//有符号数和无符号数之间运算，有错误！！！
	if(loopCnt==5)
	{
		loopCnt = 0;
		deltaCnt = EQep1Regs.QPOSCNT;
		SpeedPID.Fdb = (int32)deltaCnt;
		SpeedPID.calc(&SpeedPID);
	}
	IntensityPID.Ref = SpeedPID.Out;
	IntensityPID.Fdb = Intensity;
	IntensityPID.calc(&IntensityPID);

	EPwm1Regs.CMPA.half.CMPA = IntensityPID.Out;
	EPwm2Regs.CMPA.half.CMPA = 3750-IntensityPID.Out;

	// Acknowledge this interrupt to get more from group 1
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;	//重新开始序列1的转换
	AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;	//把序列1的中断标志清除掉


}
