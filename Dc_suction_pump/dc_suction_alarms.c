#include "Dc_suction_declarations.h"

void alarms_main (void)
{
	NTC_check(temperature);
}

void NTC_check(float temp)
{
	if ((int)temp > TEMP_ALARM_RANGE1)
	{
	  print_icon(26,122, &high_temp_26x32,26,33);
		//	PORTF.OUT |= (1<<5);
		//MOTOR_ON_OFF(false);
	}
	else if ((int)temp < TEMP_ALARM_RANGE1)
	{
		delete_rectangle(26,122,50,140);
		//	  PORTF.OUT &= ~(1<<5);
	}
}