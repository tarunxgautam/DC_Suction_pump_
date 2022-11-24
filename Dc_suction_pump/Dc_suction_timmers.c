#include "Dc_suction_declarations.h"

void TCB1_init(void)
{
	TCB1_CCMP = clock_values;						// Write a TOP value to the Compare/Capture (TCBn.CCMP) register

	TCB1_CTRLB |= (0x0 << 0);
	TCB1_INTCTRL |= (1<<0);

	TCB1_CTRLA |= (1<<0)|(0x0 <<1);		// ENABLE bit in the Control A (TCBn.CTRLA) register,
}

void TCB2_init(void)
{
	TCB2_CCMP = clock_values;						// Write a TOP value to the Compare/Capture (TCBn.CCMP) register

	TCB2_CTRLB |= (0x0 << 0);
	TCB2_INTCTRL |= (1<<0);

	TCB2_CTRLA |= (1<<0)|(0x0 <<1);		// ENABLE bit in the Control A (TCBn.CTRLA) register,
}


void timmer_init (void)
{
	TCB1_init();
	TCB2_init();
}

ISR (TCB1_INT_vect)
{
	millis++;
	TCB1_INTFLAGS |= (1<<0);
}

ISR (TCB2_INT_vect)
{
	millis1++;
	millis2++;
	millis3++;
	if ( millis1 > 1000)
	{
		_1sec = true;
		millis1 = 0;
	}
	if ( millis2 > 200)
	{
		_300sec = true;
		millis2 = 0;
	}
	TCB2_INTFLAGS |= (1<<0);
}
