#include "Dc_suction_declarations.h"

void alarms_main (void)
{
	NTC_check(temperature);
}

void NTC_check(float temp)
{
	if ((int)temp > TEMP_ALARM_RANGE1)
	{
	  print_icon(26,122,(uint8_t*) &high_temp_26x32,26,33);
		//	PORTF.OUT |= (1<<5);
		//MOTOR_ON_OFF(false);
	}
	else if ((int)temp < TEMP_ALARM_RANGE1)
	{
		delete_rectangle(26,122,50,140);
		//	  PORTF.OUT &= ~(1<<5);
	}
}

// void FAN_opt(void)
// {
// 	if (temperature > TEMP_ALARM_RANGE1)							//temperature range is too much it should be reduced around (25-35'C).
// 	{
//		PORTF.OUT |= (1 << 5);										//FAN ON
//	}
//	else if ( temperature > (TEMP_ALARM_RANGE2))					// Threshold temperature will be according to the max. temperature of battery optimal(10-30'C) / opt(-20-60'C).
//	{
//		MOTOR_ON_OFF(false);										//Safety Precaution
//	}
//	else
//	{
//		PORTF.OUT &= (0 << 5);										//FAN OFF
//	}
//}