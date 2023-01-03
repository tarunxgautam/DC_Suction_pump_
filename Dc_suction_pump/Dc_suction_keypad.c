#include "Dc_suction_declarations.h"

void keypad_gpio_init(void)
{
	#ifdef _DEBUG
	USART1_sendString("keypad gpio init start");
	#endif

	UNIT_PORT.DIR			&= ~UNIT_PIN;				UNIT_PORT.PIN6CTRL			|= 0x1;	
	MOTOR_ON_OFF_PORT.DIR	&= ~MOTOR_ON_OFF_PIN;		MOTOR_ON_OFF_PORT.PIN1CTRL	|= 0x2;
	POWER_SAVE_PORT.DIR		&= ~POWER_SAVE_PIN;			POWER_SAVE_PORT.PIN2CTRL	|= 0x2;
	SMART_SW_PORT.DIR		&= ~SMART_SW_PIN;			SMART_SW_PORT.PIN2CTRL		|= 0x2;
	SPEED_PORT.DIR          &= ~SPEED_PIN;				SPEED_PORT.PIN0CTRL         |= 0X2;
    SMART_FOOT_LED.DIR |= (SMART_FOOT_LED_PIN);
    POWER_SAVE_LED.DIR |= (POWER_SAVE_LED_PIN);

	#ifdef _DEBUG
	USART1_sendString("keypad_gpio_init: keypad gpio init stop");
	#endif
}

void unit_change (void)
{
	if(unit_button_pressed_flag)
	{
		backlit_on_flag = true;
		if (++unit_mode > 3)
		{
			unit_mode = 1;
		}

		switch (unit_mode)
		{
			case mode_mmhg:
							#ifdef _DEBUG_Keypad
							USART1_sendString("unit_change:   MMHG selected.   ");
							#endif
							break;
			
			case mode_cmh2o:
							#ifdef _DEBUG_Keypad
							USART1_sendString("unit_change:   CMH2O selected.    ");
							#endif
							break;
			
			case mode_mpa:
							#ifdef _DEBUG_Keypad
							USART1_sendString("unit_change:   MPa selected.   ");
							#endif
							break;
							
			default:
							unit_mode = mode_mmhg;
							#ifdef _DEBUG_Keypad
							USART1_sendString("UNIT MAIN: invalid unit mode, resetting to mmhg.");
							#endif
							break;
		}
		unit_button_pressed_flag = false;
	}
}

void play_pause_button_main(void)
{
	if (play_pause_button_press_flag)
	{
		USART1_sendString("Play_ Pause_button is pressed");
		
		if ( (button_motor_on_off_flag) || (!(TCA0.SINGLE.CMP0 = Duty_cycle)) )
		{

			button_motor_on_off_flag= false;
			
			smart_switch_cont_motor_on_flag = false;
			foot_switch_press_flag = false;
			foot_switch_release_flag = false;
			smart_switch_timmer = 0;
			foot_switch_press_count = 0;
			power_save_timer_on_flag = false;
			power_save_motor_on_timmer = 0;
			MOTOR_ON_OFF(false);
		}
		else
		{
			button_motor_on_off_flag= true;
			MOTOR_ON_OFF(true);
			
		}
		play_pause_button_press_flag = false;
		
	}
	
	// 	if(previous_on_off_button_flag != Speed_change_flag)
	// 	{
	// 		previous_on_off_button_flag = Speed_change_flag;
	// 		update_motor_on_off_flag = true;
	// 	}
	// 	if (update_motor_on_off_flag)
	// 	{
	// 		if (Speed_change_flag)
	// 		{
	// 			#ifdef _DEBUG_KEYPAD
	// 			USART1_sendString("");
	// 			USART1_sendString("ISR: on off button pressed.");
	// 			#endif
	// 			MOTOR_ON_OFF(true);
	// 		}
	// 		else
	// 		{
	// 			#ifdef _DEBUG_KEYPAD
	// 			USART1_sendString("");
	// 			USART1_sendString("ISR: on off button released");
	// 			#endif
	// 			MOTOR_ON_OFF(false);
	// 		}
	// 		update_motor_on_off_flag = false;
	// 	}
}

void keypad_main (void)
{
	unit_change();
	play_pause_button_main();
	pwm_change();
}



// as UNIT ISR belong to PORTC which is also called in Foot_switch section we are gonna call UNIT ISR in FOOT_SWITCH file.//

void pwm_change (void)
{
//	if((speed_button_press_flag) && (!button_motor_on_off_flag) && (!power_save_mode_on_flag) && (!smart_switch_mode_flag))   // removing this part as for now we want speed to be changed while motor is on and this part will work only when motor is off.
	if(speed_button_press_flag)
	{
		if (++unit_mode_pwm > 4)
		{
			unit_mode_pwm = 1;
		}
	switch (unit_mode_pwm) 
	{
		case 1:
				#ifdef _DEBUG_Keypad
				//USART1_sendString("unit_mode_pwm :   1800 ");
				#endif
                if (button_motor_on_off_flag)
                {
				  TCA0.SINGLE.CTRLA |= (1 << 0);				//these two newly added for speed to change when speed button is pressed.
				  TCA0.SINGLE.CMP0 = LPM_30;					//these two newly added for speed to change when speed button is pressed.
                }
//				TCA0.SINGLE.CTRLA |= (1 << 0);				//these two newly added for speed to change when speed button is pressed.
//				TCA0.SINGLE.CMP0 = LPM_30;					//these two newly added for speed to change when speed button is pressed.
				Duty_cycle = LPM_30;				// just commented because the updated value was not coming to speed
				if (!value_ok_flag)
				{
					Duty_cycle = return_val;
					TCA0.SINGLE.CTRLA |= (1 << 0);
					TCA0.SINGLE.CMP0 = return_val;
				}
				lpm30_flag = true;
				print_icon(53, 42, & arial_numerical_temp_3_49x40,49, 40);								// 3
				print_icon(66, 40, & jersey_numerical_temp_0_49x40, 49, 40);							// 0
				print_icon(66,95, & temp_lpm_15x36,15,40);                // lpm
				break;
		
		case 2:
				#ifdef _DEBUG_Keypad
				//USART1_sendString("unit_mode_pwm :   2000 ");
				#endif
	//			TCA0.SINGLE.CTRLA |= (1 << 0);
//				TCA0.SINGLE.CTRLA |= (1 << 0);				//these two newly added for speed to change when speed button is pressed.
//				TCA0.SINGLE.CMP0 = LPM_40;					//these two newly added for speed to change when speed button is pressed.
                
				if (button_motor_on_off_flag)
				{
					TCA0.SINGLE.CTRLA |= (1 << 0);				//these two newly added for speed to change when speed button is pressed.
					TCA0.SINGLE.CMP0 = LPM_40;					//these two newly added for speed to change when speed button is pressed.
				}

				Duty_cycle = LPM_40;
				if (!value_ok_flag)
				{
					Duty_cycle = return_val;
				TCA0.SINGLE.CTRLA |= (1 << 0);
				TCA0.SINGLE.CMP0 = return_val;
				}
				lpm40_flag = true;
				print_icon(53, 42, & arial_numerical_temp_4_49x40,49, 40);								// 4
				print_icon(66, 40, & jersey_numerical_temp_0_49x40, 49, 40);							// 0
				print_icon(66,95, & temp_lpm_15x36,15,40);                // lpm
				break;
		
		case 3:
				#ifdef _DEBUG_Keypad
		//		USART1_sendString("unit_mode_pwm :   2400 ");
				#endif
		//		TCA0.SINGLE.CTRLA |= (1 << 0);
//				TCA0.SINGLE.CTRLA |= (1 << 0);				//these two newly added for speed to change when speed button is pressed.
//				TCA0.SINGLE.CMP0 = LPM_50;			        //these two newly added for speed to change when speed button is pressed.

                if (button_motor_on_off_flag)
                {
	                TCA0.SINGLE.CTRLA |= (1 << 0);				//these two newly added for speed to change when speed button is pressed.
	                TCA0.SINGLE.CMP0 = LPM_50;					//these two newly added for speed to change when speed button is pressed.
                }
				
				Duty_cycle = LPM_50;
				if (!value_ok_flag)
				{
					Duty_cycle = return_val;
					TCA0.SINGLE.CTRLA |= (1 << 0);
					TCA0.SINGLE.CMP0 = return_val;
				}
				lpm50_flag = true;
				print_icon(53, 42, & arial_numerical_temp_5_49x40,49, 40);								// 5
				print_icon(66, 40, & jersey_numerical_temp_0_49x40, 49, 40);							// 0
				print_icon(66,95, & temp_lpm_15x36,15,40);                // lpm
				break;
		
		case 4:
				#ifdef _DEBUG_Keypad
		//		USART1_sendString("unit_mode_pwm :   3000");
				#endif
	    	//	TCA0.SINGLE.CTRLA |= (1 << 0);
//				TCA0.SINGLE.CTRLA |= (1 << 0);					//these two newly added for speed to change when speed button is pressed.
//				TCA0.SINGLE.CMP0 = LPM_60;						//these two newly added for speed to change when speed button is pressed.
                if (button_motor_on_off_flag)
                {
	                TCA0.SINGLE.CTRLA |= (1 << 0);				//these two newly added for speed to change when speed button is pressed.
	                TCA0.SINGLE.CMP0 = LPM_60;					//these two newly added for speed to change when speed button is pressed.
                }
				Duty_cycle = LPM_60;
				if (!value_ok_flag)
				{
					Duty_cycle = return_val;
					TCA0.SINGLE.CTRLA |= (1 << 0);
					TCA0.SINGLE.CMP0 = return_val;
				}
				lpm60_flag = true;
				print_icon(53, 42, & arial_numerical_temp_6_49x40,49, 40);								// 6
				print_icon(66, 40, & jersey_numerical_temp_0_49x40, 49, 40);							// 0
				print_icon(66,95, & temp_lpm_15x36,15,40);                // lpm
				break;
		
		default:
				Duty_cycle = LPM_30;
				lpm30_flag = true;
				print_icon(53, 42, & arial_numerical_temp_3_49x40,49, 40);								// 3
				print_icon(66, 40, & jersey_numerical_temp_0_49x40, 49, 40);							// 0
				print_icon(66,95, & temp_lpm_15x36,15,40);                // lpm
				break;
	}
	speed_button_press_flag = false;
 }
 // just adding for speed change when motor is on
else if ((speed_button_press_flag) && ((button_motor_on_off_flag) || (power_save_mode_on_flag) || (smart_switch_mode_flag)))
  {
	if (++unit_mode_pwm > 4)
	  {
		  unit_mode_pwm = 1;
	  }
	  switch (unit_mode_pwm)
	  {
		  case 1:
				  //TCA0.SINGLE.CTRLA |= (1 << 0);
				  TCA0.SINGLE.CTRLA |= (1 << 0);				//these two newly added for speed to change when speed button is pressed.
				  TCA0.SINGLE.CMP0 = LPM_30;					//these two newly added for speed to change when speed button is pressed.
				  //Duty_cycle = LPM_30;
				  print_icon(53, 42, & arial_numerical_temp_3_49x40,49, 40);							// 3
				  print_icon(66, 40, & jersey_numerical_temp_0_49x40, 49, 40);							// 0
				  print_icon(66,95, & temp_lpm_15x36,15,40);                // lpm
				  break;
		  
		  case 2:
				  //TCA0.SINGLE.CTRLA |= (1 << 0);
				  TCA0.SINGLE.CTRLA |= (1 << 0);				//these two newly added for speed to change when speed button is pressed.
				  TCA0.SINGLE.CMP0 = LPM_40;					//these two newly added for speed to change when speed button is pressed.
				  //				Duty_cycle = LPM_40;
				  print_icon(53, 42, & arial_numerical_temp_4_49x40,49, 40);								// 4
				  print_icon(66, 40, & jersey_numerical_temp_0_49x40, 49, 40);							// 0
				  print_icon(66,95, & temp_lpm_15x36,15,40);                // lpm
				  break;
		  
		  case 3:
				  //		TCA0.SINGLE.CTRLA |= (1 << 0);
				  TCA0.SINGLE.CTRLA |= (1 << 0);				//these two newly added for speed to change when speed button is pressed.
				  TCA0.SINGLE.CMP0 = LPM_50;			        //these two newly added for speed to change when speed button is pressed.
				  //				Duty_cycle = LPM_50;
				  print_icon(53, 42, & arial_numerical_temp_5_49x40,49, 40);								// 5
				  print_icon(66, 40, & jersey_numerical_temp_0_49x40, 49, 40);							// 0
				  print_icon(66,95, & temp_lpm_15x36,15,40);                // lpm
				  break;
		  
		  case 4:
				  //	TCA0.SINGLE.CTRLA |= (1 << 0);
				  TCA0.SINGLE.CTRLA |= (1 << 0);					//these two newly added for speed to change when speed button is pressed.
				  TCA0.SINGLE.CMP0 = LPM_60;						//these two newly added for speed to change when speed button is pressed.
				  //				Duty_cycle = LPM_60;
				  print_icon(53, 42, & arial_numerical_temp_6_49x40,49, 40);								// 6
				  print_icon(66, 40, & jersey_numerical_temp_0_49x40, 49, 40);							// 0
				  print_icon(66,95, & temp_lpm_15x36,15,40);                // lpm
				  break;
		  
		  default:
				  Duty_cycle = LPM_30;
				  lpm30_flag = true;
				  print_icon(53, 42, & arial_numerical_temp_3_49x40,49, 40);								// 3
				  print_icon(66, 40, & jersey_numerical_temp_0_49x40, 49, 40);							// 0
				  print_icon(66,95, & temp_lpm_15x36,15,40);                // lpm
				  break;
	  }
	  speed_button_press_flag = false; 
  }
}
		