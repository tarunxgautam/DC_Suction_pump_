#include "Dc_suction_declarations.h"

void lcd_uc1698u_init(void)
{
 	lcd_all_icon_update();
 	// 	if (!previous_uc_ams_reset_flag)
 	// 	{
 	// 		USART1_sendString("???????????????????????????????????");
 	// 		UC1698U_init();
 	// 	}
 	// 	else if (!previous_soft_hard_reset_flag)
 	// 	{
 	// 		UC1698U_gpio_init();
 	// 		USART1_sendString("**************************************");
 	// 		UC1698U_init();
 	// 	}

 	if  ((!previous_uc_ams_reset_flag) || (!previous_soft_hard_reset_flag))
 	{
	 	UC1698U_gpio_init();
	 	UC1698U_init();
 	}
}

void lcd_all_icon_update (void)
{
	update_pressure_flag = true;
	unit_icon_update_flag = true;
}

void lcd_icon_update (void)
{
	if (previous_unit_mode != unit_mode)	//unit icon will be updated only once when the unit button is pressured, and will be printed inside the pressure updating function.
	{
		update_pressure_flag = true;
		update_bar_flag = true;
		unit_icon_update_flag = true;
		previous_unit_mode = unit_mode;
	}
	if (previous_power_save_mode_flag != power_save_mode_on_flag)				//to check the working state of device
	{
		previous_power_save_mode_flag = power_save_mode_on_flag;
		update_power_save_icon_flag = true;
	}
	if (_1sec)
	{
		update_pressure_flag = true;
		if ((smart_switch_mode_flag) || (!smart_switch_delete_flag))
		{
			smart_switch_icon_update_flag = true;
		}
		_1sec = false;
	}
	
	if (_300sec)
	{
		update_bar_flag = true;
		_300sec = false;
	}
}

void power_on_screen (void)
{
	//	bar_draw_delete_rectangle(0,0,3,30);
	delete_rectangle(0,0,4,10);
	print_icon(30, 50 ,(uint8_t*) &AgVa_logo_56x30, 30,56);
	print_icon(31, 100,(uint8_t*) &POWER_ON_48X8, 8, 48);
}

void lcd_loading_page (void)
{
	delete_rectangle(0,0,70,150);
	lcd_pressure_bar_init();
	EDGE();
	partitions();
 	current_pressure_mmhg = 0.0; //  if not 0 then disply prints fault screen at pressure values
 	show_mmgh_4digit (6,42);
	Duty_cycle = LPM_30;
    lpm30_flag = true;
	print_icon(53, 42,(uint8_t*) & arial_numerical_temp_3_49x40,49, 40);								// 3
	print_icon(66, 40,(uint8_t*) & jersey_numerical_temp_0_49x40, 49, 40);							// 0
	print_icon(66,95,(uint8_t*) & temp_lpm_15x36,15,40);                // lpm
//	print_icon(66, 129, &battery_SOC4_15x32, 15, 33);		 // battery icon
 	//	print_icon(12,129, & temp_right_arrow_15x40, 15, 42);	 // right jar icon
 	_delay_ms(100);
}

void lcd_uc1698u_main_screen (void)
{
	lcd_icon_update();
	if (update_pressure_flag)
	{
		switch (unit_mode)
		{
			case mode_mmhg:
							#ifdef	_DEBUG
							USART1_sendString("LCD MAIN SCREEN: printing mmgh pressure.");
							#endif
							show_mmgh_4digit (6,42);
							break;
			
			case mode_cmh2o:
							#ifdef	_DEBUG
							USART1_sendString("LCD MAIN SCREEN: printing cmh2o pressure");
							#endif
							
							cmH20_4digit(6,42);
							break;
			
			case mode_mpa:
							#ifdef	_DEBUG
							USART1_sendString("LCD MAIN SCREEN: printing mpa pressure");
							#endif
							MPa_4digit(6,42);
							break;
			
			default:
							show_mmgh_4digit (6,42);
							#ifdef	_DEBUG
							USART1_sendString("LCD MAIN SCREEN: invalid unit mode.");
							#endif
		}
		update_pressure_flag = false;
	}
	if (update_bar_flag)
	{
		lcd_pressure_bar_main();
		update_bar_flag = false;
	}
	
	if (update_power_save_icon_flag)
	{
		if (power_save_mode_on_flag)
		{
			#ifdef	_DEBUG_Keypad
			USART1_sendString("Power save button pressed");
			#endif
			POWER_SAVE_LED.OUT |= (POWER_SAVE_LED_PIN);
			print_icon(60,120,(uint8_t*) & temp_powersave_27x40, 27, 42);      // power_save icon
		}
		else
		{
			#ifdef	_DEBUG_Keypad
			USART1_sendString("Power save button released");
			#endif
			POWER_SAVE_LED.OUT &= ~(POWER_SAVE_LED_PIN);
			delete_rectangle(60, 120, 77, 150);
		}
		update_power_save_icon_flag = false;
	}
	/*             added by Tarun              */
	
	if ((power_save_mode_on_flag) && (power_save_icon_blink_flag))  // blinking of power save icon
	{
		if (abs(g_foot_sw_blink_millis - millis) > 1000)
		{
			BOOOOOOL_TEMP1 ^= 1;
			if (BOOOOOOL_TEMP1)
			{
				POWER_SAVE_LED.OUT &= ~(POWER_SAVE_LED_PIN);
				delete_rectangle(60, 120, 77, 150);
			}
			else
			{
				POWER_SAVE_LED.OUT |= (POWER_SAVE_LED_PIN);
				print_icon(60,120,(uint8_t*) & temp_powersave_27x40, 27, 42);      // power_save icon
			}
			g_foot_sw_blink_millis = millis;
		}
	}
	if (smart_switch_icon_update_flag)
	{
		if (smart_switch_mode_flag)
		{
		    SMART_FOOT_LED.OUT |= (SMART_FOOT_LED_PIN);
			static int smart_icon_serial = 0;
			if ( ++smart_icon_serial > 2)
			{
				smart_icon_serial = 0;
			}
			switch (smart_icon_serial)
			{
				case 0:	print_icon(7,122,(uint8_t*) & animation_1_foot_24x33,24, 40);
				break;
				
				case 1:	print_icon(7,122,(uint8_t*) & animation_2_foot_24x33,24, 40);
				break;
				
				case 2:	print_icon(7,122,(uint8_t*) & animation_3_foot_24x33,24, 40);
				break;
				
				default:
				
				#ifdef _DEBUG
				USART1_sendString("LCD MAIN: invalid smart switch icon traversal value, resetting to 0");
				#endif
				smart_icon_serial = 0;
				break;
				
			}
			smart_switch_delete_flag = false;
		}
		else
		{
			SMART_FOOT_LED.OUT &= ~(SMART_FOOT_LED_PIN);			
			delete_rectangle(7,120,30,150);
			smart_switch_delete_flag = true;
		}
		smart_switch_icon_update_flag = false;
	}
	
	if (Service_alarm_flag || Service_alarm_flag_run)
	{
			 print_icon(36 , 124 ,(uint8_t*) &service_required_26x32,26 ,32);
	}
}

void partitions(void)
{
	draw_filled_rectangle(0,0,6,30);       // for bar
	print_line (0,30,79,30,true);			 // line below bar
	print_line(0,29,79,29,true);
//	print_double_line(51,31,51,114,false);	  //  // middle vertical line
	print_line(51,31,51,114,false);				// middle verticle line
	print_line (0,114,79,114,true);          // icons partition line
	print_line(0,115,79,115,true );			// icons partition line for highlight
}

void EDGE(void)
{
	print_double_line (0,0,0,158,false);      // left verticle line
	
	print_line (0,0,79,0,true);              // top horizontal line
	print_line (0,1,79,1,true);				// to make it thick
	
	print_double_line (79,0,79,158,false);    // right verticle line
	
	print_line (0,159,79,159,true);          // bottom horizontal line
	print_line (0,158,79,158,true);			// to make it thick
}

void show_mmgh_4digit (uint8_t x1, uint8_t y1)
{
	int __buff_ = current_pressure_mmhg;
	int digit_a = 0, digit_b = 0, digit_c = 0;/*digit_d =0*/
	if (__buff_ > 750)
	{
		#ifdef _DEBUG
		USART1_sendString("show_mmgh_4digit: pressure digit overflow!!");
		#endif
		
		current_pressure_mmhg = 0.0;
	}

	//digit_d = __buff_%10;
	//__buff_ = __buff_/10;
	digit_c = __buff_%10;
	__buff_ = __buff_/ 10;
	digit_b = __buff_%10;
	digit_a = __buff_/10;
	__buff_ = 0;

	print_number((x1), y1,(uint8_t*) &arial_numerical_font_49x40, 49, 40, digit_a);
	print_number((x1+13), y1,(uint8_t*) &arial_numerical_font_49x40, 49, 40, digit_b);
	print_number((x1+26), y1,(uint8_t*) &arial_numerical_font_49x40, 49, 40, digit_c);
	//	print_number((x1+18), y1, &arial_numerical_font_49x40, 49, 40, digit_d);
	if (unit_icon_update_flag)
	{
		print_icon(31,95 ,(uint8_t*) &temp_2_mmhg_13x48, 13, 50);
		unit_icon_update_flag = false;
	}
	
}

void cmH20_4digit (uint8_t x1, uint8_t y1)
{
	int buff = current_pressure_cmh2O;
	int digit_a = 0, digit_b = 0, digit_c = 0;/*digit_d =0;*/
	if (buff > 750)
	{
		#ifdef _DEBUG
		USART1_sendString("cmH20_4digit: pressure digit overflow!!");
		#endif
		
		current_pressure_cmh2O = 0.0;
	}
	
	//digit_d = buff%10;
	//buff = buff/10;
	digit_c = buff%10;
	buff = buff/ 10;
	digit_b = buff%10;
	digit_a = buff/10;
	buff = 0;

	print_number((x1), y1,(uint8_t*) &arial_numerical_font_49x40, 49, 40, digit_a);
	print_number((x1+13), y1,(uint8_t*) &arial_numerical_font_49x40, 49, 40, digit_b);
	print_number((x1+26), y1,(uint8_t*) &arial_numerical_font_49x40, 49, 40, digit_c);
	//print_number((x1+18), y1, &jersey_numerical_font_24x24, 24, 24, digit_d);
	if (unit_icon_update_flag)
	{
		print_icon(31, 95,(uint8_t*) &temp_cmh2o_13x54, 13,57);
		unit_icon_update_flag = false;
	}
}

void MPa_4digit (uint8_t x1, uint8_t y1)
{
	int buff = current_pressure_mpa;                    // convert mmhg to Mpa  (in pressure)
	int digit_a = 0, digit_b = 0, digit_c = 0;/*digit_d =0;*/
	if (buff > 9.99)
	{
		#ifdef _DEBUG
		USART1_sendString("MPa_4digit: pressure digit overflow!!");
		#endif
		current_pressure_mpa = 0.0;
	}

	// 	digit_d = buff%10;
	// 	buff = buff/10;
	digit_c = buff%10;
	buff = buff/ 10;
	digit_b = buff;
	digit_a = 0;
	buff = 0;
	
	print_number((x1), y1,(uint8_t*) &arial_numerical_font_49x40, 49, 40, digit_a);
	print_icon(16,85,(uint8_t*) &temp_decimal_1_6x8,6,9);
	print_number((x1+13), y1,(uint8_t*) &arial_numerical_font_49x40, 49, 40, digit_b);
	print_number((x1+26), y1,(uint8_t*) &arial_numerical_font_49x40, 49, 40, digit_c);
	//	print_number((x1+23), y1, &jersey_numerical_font_24x24, 24, 24, digit_d);
	if (unit_icon_update_flag)
	{
		print_icon(31, 95,(uint8_t*) &temp_mpa_14x54, 14,57);
		unit_icon_update_flag = false;
	}
}


void display_runTime_serviceTime(void)
{
	delete_rectangle(0,0,70,150);
	print_icon( 1, 43,(uint8_t*) &Service_font_30x64, 30, 64);   //printing service and total time icons
	print_icon( 23, 43,(uint8_t*) &Hours_font_30x48, 30, 48);
	print_icon( 1, 103,(uint8_t*) &Total_font_30x48, 30, 48);
	print_icon( 17, 103,(uint8_t*) &Run_font_30x32, 30, 32);
	print_icon( 28, 103,(uint8_t*) &Hours_font_30x48, 30,  48);
	display_print_function_numerics();
	reset_time();
	delete_rectangle(0,0,80,150);
}

int numDigits(long num)
{
	int count = 0;
	while(num / 10)
	{
		num = num/10;
		count++;
	}
	count++;
	return count;
}


void printNumScreen_LeftJustified(uint8_t x_last, uint8_t y_start, long num, uint8_t digits)
{
	int temp_x = x_last;
	int tempVar = numDigits(num);
	int *tempPtr = (int *)calloc(tempVar, sizeof(int));
	int loopVal = 0;
	
	for (loopVal = 0; loopVal < tempVar - 1; loopVal++)
	{
		tempPtr[loopVal] = num % 10;
		num = num / 10;
	}
	tempPtr[tempVar - 1] = num;
	
	if (digits > tempVar)
	{
		tempPtr = (int *)realloc(tempPtr, (tempVar + (digits - tempVar)) * sizeof(int));
		for (loopVal = tempVar; loopVal < digits; loopVal++)
		{
			tempPtr[loopVal] = 0;
		}
		for (loopVal = digits - 1; loopVal >= 0; loopVal--)
		{
			print_number(temp_x, y_start, (uint8_t*) &jersey_numerical_font_24x24_po, 24, 24, tempPtr[loopVal]);
			temp_x += 6;
		}
	}
	
	else
	{
		for (loopVal = tempVar - 1; loopVal >= 0 ; loopVal--)
		{
			print_number(temp_x, y_start,(uint8_t*) &jersey_numerical_font_24x24_po, 24, 24, tempPtr[loopVal]);
			temp_x += 6;
		}
	}
	free(tempPtr);
}

void printNumScreen_RightJustified(uint8_t x_last, uint8_t y_start, long num, uint8_t digits)
{
	int temp_x = x_last;
	int tempVar = numDigits(num);
	int *tempPtr = (int *)calloc(tempVar, sizeof(int));
	int loopVal = 0;
	
	for (loopVal = 0; loopVal < tempVar - 1; loopVal++)
	{
		tempPtr[loopVal] = num % 10;
		num = num / 10;
	}
	tempPtr[tempVar - 1] = num;
	
	if (digits > tempVar)
	{
		tempPtr = (int *)realloc(tempPtr, (tempVar + (digits - tempVar)) * sizeof(int));
		for (loopVal = tempVar; loopVal < digits; loopVal++)
		{
			tempPtr[loopVal] = 0;
		}
		temp_x = temp_x - (6*(digits - 1));
		for (loopVal = digits - 1; loopVal >= 0; loopVal--)
		{
			print_number(temp_x, y_start,(uint8_t*) &jersey_numerical_font_24x24_po, 24, 24, tempPtr[loopVal]);
			temp_x += 6;
		}
	}
	
	else
	{
		temp_x = temp_x - (6*(tempVar - 1));
		for (loopVal = tempVar - 1; loopVal >= 0 ; loopVal--)
		{
			print_number(temp_x, y_start,(uint8_t*) &jersey_numerical_font_24x24_po, 24, 24, tempPtr[loopVal]);
			temp_x += 6;
		}
	}
	free(tempPtr);
}

 void display_print_function_numerics(void)                                 // printing service hours
 {
	static uint32_t value_1 = 0;
	static uint32_t value_2 = 0;
	
	value_1 =  read_long_data_in_eeprom_SPM(service_time_addr);	
	if (value_1 >= allowed_service_duration )
	{
		Service_alarm_flag = true;
	}
	else
	{
		Service_alarm_flag = false;
	}
	
	value_1 = value_1 / 3600000;
	printNumScreen_RightJustified(70, 43, value_1, 4);
	
	value_2 =  read_long_data_in_eeprom_SPM(total_run_time_addr);
	value_2 = value_2 / 3600000;
	printNumScreen_RightJustified(70, 103, value_2, 4);
	
 }

