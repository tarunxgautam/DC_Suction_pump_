#include "Dc_suction_declarations.h"

void mode_1(void)
{
	if (((mode_button_flag) && ((millis - long_press_pwms_set) > button_normal_press)) && (i >= 2))
	{
		mode_button_flag = false;
		long_press_pwms_set = 0;
		i = 0;
		USART1_sendString("Entered PWM mode");
		print_icon(icon_callibrate_x,icon_callibrate_y,(uint8_t*) &s_alphabet_17X17,17, 25);
		settings();
	}
}

int get_int_limited(int startVal, int lowerVal, int upperVal, int changeBy)
{
	
	int tempVal = startVal;
	
	if (POWER_SAVE_PORT.IN & (POWER_SAVE_PIN))
	{
		//USART1_sendString(__FUNCTION__);
		tempVal += changeBy;
		if (tempVal > upperVal)
		{
			tempVal = upperVal;
		}
		USART1_sendInt(tempVal);
	}
	else if (SMART_SW_PORT.IN & (SMART_SW_PIN))
	{
		//USART1_sendString(__FUNCTION__);
		tempVal -= changeBy;
		if (tempVal < lowerVal)
		{
			tempVal = lowerVal;
		}
		USART1_sendInt(tempVal);
	}
	_delay_ms(250);
	return tempVal;
}

int settings(void)
{
	uint16_t l_address = 0x00;
	value_ok_flag = false;	
	USART1_sendString("ENTERING IN SETTING MENU.............................");
	USART1_sendString_without_newline("20 LPM STATUS: ");
	USART1_sendInt(lpm20_flag);
	USART1_sendString_without_newline("30 LPM STATUS: ");
	USART1_sendInt(lpm30_flag);
	USART1_sendString_without_newline("40 LPM STATUS: ");
	USART1_sendInt(lpm40_flag);
	USART1_sendString_without_newline("50 LPM STATUS: ");
	USART1_sendInt(lpm50_flag);
	USART1_sendString_without_newline("60 LPM STATUS: ");
	USART1_sendInt(lpm60_flag);
	
	
	while(!value_ok_flag)
	{
		//USART1_sendString(__FUNCTION__);
		if (lpm20_flag)
		{USART1_sendString("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
			TCA0.SINGLE.CMP0 = LPM_20 = get_int_limited(LPM_20, LPM_20 - 100, TCA0.SINGLE.PER, 5);
			l_address = lpm20_duty_addr;
			return_val = LPM_20;
		}
		
		else if (lpm30_flag)
		{
			TCA0.SINGLE.CMP0 = LPM_30 = get_int_limited(LPM_30, LPM_30 - 100, TCA0.SINGLE.PER, 5);
			l_address = lpm30_duty_addr;
			return_val = LPM_30;
		}
		
		else if (lpm40_flag)
		{
			TCA0.SINGLE.CMP0 = LPM_40 = get_int_limited(LPM_40, LPM_40 - 100, TCA0.SINGLE.PER, 5);
			l_address = lpm40_duty_addr;
			return_val = LPM_40;
		}
		
		else if (lpm50_flag)
		{
			TCA0.SINGLE.CMP0 = LPM_50 = get_int_limited(LPM_50, LPM_50 - 100, TCA0.SINGLE.PER, 5);
			l_address = lpm50_duty_addr;
			return_val = LPM_50;
		}
		
		else if (lpm60_flag)
		{
			TCA0.SINGLE.CMP0 = LPM_60 = get_int_limited(LPM_60, LPM_60 - 100, TCA0.SINGLE.PER, 5);
			l_address = lpm60_duty_addr;
			return_val = LPM_60;
		}
	}
	write_16t_data_in_eeprom_SPM(l_address, return_val);
	USART1_sendString("EEPROM UPDATED. Maja aaaaaaa gayaaaaaaaaa..........");
 
	value_ok_flag = false;
	delete_rectangle(50,130,60,147);
	USART1_sendInt(return_val);
	mode_button_flag = false;
	lpm20_flag = false;		
	lpm30_flag = false;
	lpm40_flag = false;
	lpm50_flag = false;
	lpm60_flag = false;
	play_pause_button_press_flag = false;
	USART1_sendString("EXIT FROM SETTINS MENU");
	return return_val;
 }
