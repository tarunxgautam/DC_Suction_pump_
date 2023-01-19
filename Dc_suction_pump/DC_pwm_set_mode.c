#include "Dc_suction_declarations.h"

void mode_1(void)
{
	if ((mode_button_flag) && ((millis - long_press_pwms_set) > button_normal_press))
	{
		USART1_sendString("Entered PWM mode");
		print_icon(60,130,(uint8_t*) &s_alphabet_17X17,17, 25);
		settings();
		long_press_pwms_set = 0;
		mode_button_flag = false;
	}
}

int settings(void)
{
	value_ok_flag = false;	
	USART1_sendString("ENTERING IN SETTING MENU.............................");
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
		if ((value_increment_flag) && (lpm20_flag))
		{
			USART1_sendString("mode for 30lpm increment :");
			if (++pwm_value > 5)
			{
				pwm_value = 1;
			}
			switch (pwm_value)
			{
				case 1:
				TCA0.SINGLE.CTRLA |= (1 << 0);
				TCA0.SINGLE.CMP0 = 1550;
				LPM_20 = 1550;
				break;
				
				case 2:
				TCA0.SINGLE.CTRLA |= (1 << 0);
				TCA0.SINGLE.CMP0 = 1600;
				LPM_20 = 1600;
				break;
				
				case 3:
				TCA0.SINGLE.CTRLA |= (1 << 0);
				TCA0.SINGLE.CMP0 = 1650;
				LPM_20 = 1650;
				break;
				
				case 4:
				TCA0.SINGLE.CTRLA |= (1 << 0);
				TCA0.SINGLE.CMP0 = 1700;
				LPM_20 = 1700;
				break;
				
				default:
				LPM_20 = 1550;
				break;
			}
			value_increment_flag = false;
			return_val = LPM_20;
		}
		
		else if ((value_decrement_flag) && (lpm20_flag))
		{
			USART1_sendString("mode for 30lpm decrement :");
			if (++pwm_value > 5)
			{
				pwm_value = 1;
			}
			switch (pwm_value)
			{
				case 1:
				TCA0.SINGLE.CTRLA |= (1 << 0);
				TCA0.SINGLE.CMP0 = 1550;
				LPM_20 = 1550;
				break;
				
				case 2:
				TCA0.SINGLE.CTRLA |= (1 << 0);
				TCA0.SINGLE.CMP0 = 1450;
				LPM_20 = 1450;
				break;
				
				case 3:
				TCA0.SINGLE.CTRLA |= (1 << 0);
				TCA0.SINGLE.CMP0 = 1400;
				LPM_20 = 1400;
				break;
				
				case 4:
				TCA0.SINGLE.CTRLA |= (1 << 0);
				TCA0.SINGLE.CMP0 = 1350;
				LPM_20 = 1350;
				break;
				
				default:
				LPM_20 = 1550;
				break;
				
			}
			value_decrement_flag = false;
			return_val = LPM_20;
		}
		if ((value_increment_flag) && (lpm30_flag))
		{
			USART1_sendString("mode for 30lpm increment :");
			if (++pwm_value > 5)
			{
				pwm_value = 1;
			}
			switch (pwm_value)
			{
				case 1:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 1800;
						LPM_30 = 1800;
						break;
			
				case 2:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 1850;
						LPM_30 = 1850;
						break;
				
				case 3: 	
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 1900;				
						LPM_30 = 1900;
						break;
				
				case 4:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 1950;
				        LPM_30 = 1950;
						break;
						
				default:
				        LPM_30 = 1800;
				        break;	
			}
			value_increment_flag = false;
			return_val = LPM_30;
		}
	
		else if ((value_decrement_flag) && (lpm30_flag))
		{
			USART1_sendString("mode for 30lpm decrement :");
			if (++pwm_value > 5)
			{
				pwm_value = 1;
			}
			switch (pwm_value)
			{
				case 1:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 1800;
						LPM_30 = 1800;
						break;
				
				case 2:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 1750;
						LPM_30 = 1750;
						break;
				
				case 3:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 1700;
						LPM_30 = 1700;
						break;
				
				case 4:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 1650;
						LPM_30 = 1650;
						break;
							
				default:
						LPM_30 = 1800;
						break;
				
			}
			value_decrement_flag = false;
			return_val = LPM_30;
		}
		
		else if ((value_increment_flag) && (lpm40_flag))
		{
			USART1_sendString("mode for 40 lpm increment :");		
			if (++pwm_value > 5)
			{
				pwm_value = 1;
			}
			switch (pwm_value)
			{
				case 1:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 2000;
						LPM_40 = 2000;
						break;
				
				case 2:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 2050;
						LPM_40 = 2050;
						break;
				
				case 3:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 2100;
						LPM_40 = 2100;
						break;
				
				case 4:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 2250;
						LPM_40 = 2250;
						break;
				
				default:
						LPM_40 = 2000;
						break;
				
			}
			value_increment_flag = false;
			return_val = LPM_40;
		}
		
		else if ((value_decrement_flag) && (lpm40_flag))
		{
			USART1_sendString("mode for 40 lpm decrement :");
			if (++pwm_value > 5)
			{
				pwm_value = 1;
			}
			switch (pwm_value)
			{
				case 1:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 2000;
						LPM_40 = 2000;
						break;
				
				case 2:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 1950;
						LPM_40 = 1950;
						break;
				
				case 3:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 1900;
						LPM_40 = 1900;
						break;
						
				case 4:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 1850;
						LPM_40 = 1850;
						break;
				
				default:
						LPM_40 = 2000;
						break;
				
			}
			value_decrement_flag = false;
			return_val = LPM_40;
		}
		
		else if ((value_increment_flag) && (lpm50_flag))
		{
			USART1_sendString("mode for 50 lpm increment :");
			if (++pwm_value > 5)
			{
				pwm_value = 1;
			}
			switch (pwm_value)
			{
				case 1:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 2400;
						LPM_50 = 2400;
						break;
				
				case 2:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 2450;
						LPM_50 = 2450;
						break;
				
				case 3:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 2500;
						LPM_50 = 2500;
						break;
				
				case 4:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 2550;
						LPM_50 = 2550;
						break;
				
				default:
						LPM_50 = 2400;
						break;
				
			}
			value_increment_flag = false;
			return_val = LPM_50;
		}
		
		else if ((value_decrement_flag) && (lpm50_flag))
		{
			USART1_sendString("mode for 50 lpm decrement :");
			if (++pwm_value > 5)
			{
				pwm_value = 1;
			}
			switch (pwm_value)
			{
				case 1:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 2400;
						LPM_50 = 2400;
						break;
				
				case 2:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 2350;
						LPM_50 = 2350;
						break;
						
				case 3:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 2300;
						LPM_50 = 2300;
						break;
				
				case 4:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 2250;
						LPM_50 = 2250;
						break;
				
				default:
						LPM_50 = 2400;
						break;
				
			}
			value_decrement_flag = false;
			return_val = LPM_50;
		}
		
		else if ((value_increment_flag) && (lpm60_flag))
		{
			USART1_sendString("mode for 60 lpm increment :");
			if (++pwm_value > 5)
			{
				pwm_value = 1;
			}
			switch (pwm_value)
			{
				case 1:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 2850;
						LPM_60 = 2850;
						break;
				
				case 2:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 2900;
						LPM_60 = 2900;
						break;
				
				case 3:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 2950;
						LPM_60 = 2950;
						break;
				
				case 4:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 3000;
						LPM_60 = 3000;
						break;
				
				default:
						LPM_60 = 3000;
						break;
				
			}
			value_increment_flag = false;
			return_val = LPM_60;
		}
		
		else if ((value_decrement_flag) && (lpm60_flag))
		{
			USART1_sendString("mode for 60 lpm decrement :");
			if (++pwm_value > 5)
			{
				pwm_value = 1;
			}
			switch (pwm_value)
			{
				case 1:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 3000;
						LPM_60 = 3000;
						break;
				
				case 2:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 2950;
						LPM_60 = 2950;
						break;
				
				case 3:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 2900;
						LPM_60 = 2900;
						break;
				
				case 4:
						TCA0.SINGLE.CTRLA |= (1 << 0);
						TCA0.SINGLE.CMP0 = 2850;
						LPM_60 = 2850;
						break;
				
				default:
						LPM_60 = 3000;
						break;
				
			}
			value_decrement_flag = false;
			return_val = LPM_60;
		}
	}
 
	value_ok_flag = false;
	delete_rectangle(55,122,75,155);
	USART1_sendInt(return_val);
	mode_button_flag = false;
	lpm20_flag = false;		
	lpm30_flag = false;
	lpm40_flag = false;
	lpm50_flag = false;
	lpm60_flag = false;
	USART1_sendString("EXIT FROM SETTINS MENU");
	return return_val;
 }
