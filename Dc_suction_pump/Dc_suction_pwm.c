#include "Dc_suction_declarations.h"

void TCA0_PWM_init(void)
{
	PORTA.DIR |= (1 << 0);			// pump pin
	PORTA.DIR |= (1 << 1);			// for proportional valve
	PORTA.PIN0CTRL |= (1 << 3);
	//PORTA.PIN1CTRL |= (1 << 3);
	//PORTA.OUTSET |= (1 << 0);
	//TCA0.SINGLE.PER = 120;		// calcu lation for 40us
	TCA0.SINGLE.PER = 3200;			// calculation for 1ms
	TCA0.SINGLE.CMP0 = 0;			//pump pwm
	// 	TCA0.SINGLE.CMP1 = 0;		// proportional valve pwm
	
	TCA0.SINGLE.CNT = 0;
	TCA0.SINGLE.CTRLB |= (1 << 4);				// compare 0 enable
	//TCA0.SINGLE.CTRLB |= (1 << 5);			// compare 1 enable
	TCA0.SINGLE.CTRLB |= (0x3 << 0);			// Single slope PWM	
	TCA0.SINGLE.CTRLA |= (1<<0);				//fclk_per 1 and  Enable pwm
	//TCA0.SINGLE.CTRLA |= (0x3 << 1);			//fclk_per 8 and  Enable pwm
	TCA0.SINGLE.CTRLA |= (0x1 << 1);			//fclk_per 4 and  Enable pwm
}
/*
void TCA0_PWM_init(void)
{
	
	PORTA.DIR |= (1 << 0);
	PORTA.PIN0CTRL |= (1 << 3);
	TCA0.SINGLE.PER = 4000;						// calculation for 1ms
	TCA0.SINGLE.CNT = 0;
	TCA0.SINGLE.CTRLA |= (0x0 << 1);			//Ftca = fclk_per
	TCA0.SINGLE.CTRLB |= (1 << 4);				// compare 0 enable
	TCA0.SINGLE.CTRLB |= (0x3 << 0);			// Single slope PWM
	TCA0.SINGLE.CTRLA |= (1 << 0);				// Enable PWM
}*/

/************************************************************************/
/* TCA0 as timer interrupt, duration = 500ms                                                                     */
/************************************************************************/
/*void TCA_0_init(void)
{
	TCA0_SINGLE_CNT = 0;
	TCA0.SINGLE.PER = 31250;
	
	TCA0.SINGLE.CTRLA |= (1 << 7)|(0x5 << 1);    // Prescaler = 64, RunInStandby mode
	TCA0.SINGLE.CTRLA |= (1 << 0);               // Enable TCA0;
}*/
