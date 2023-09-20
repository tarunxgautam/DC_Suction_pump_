 #include "Dc_suction_declarations.h"


ISR(PORTF_PORT_vect)
{
	if (SMART_SW_PORT.INTFLAGS & (SMART_SW_PIN))
	{
		//	USART1_sendString("smart_swicth_button_pressed");

		(smart_switch_mode_flag == true)  ? (smart_switch_mode_flag = false) : (smart_switch_mode_flag = true);                ////////////////////
		value_decrement_flag = true;
		#ifdef _DEBUG_KEYPAD
		USART1_sendString("");
		USART1_sendString("ISR: smart_swicth_button_pressed");
		if (smart_switch_mode_flag)
		{
			USART1_sendString("ISR: smart switch enabled.");
		}
		else
		{
			USART1_sendString("ISR: smart switch disabled.");
		}
		#endif // _DEBUG
		//smart_switch_icon_flag = true;                   //////////// this flag should be true somewhere else
		SMART_SW_PORT.INTFLAGS |= (SMART_SW_PIN);
	}
	
	
	
}

ISR(PORTC_PORT_vect)
{
	if (foot_switch_sens_port.INTFLAGS & foot_switch_sens_pin)
	{
		// 		USART1_sendString("ISR: ")
		if (smart_switch_mode_flag)
		{
			if ( (!foot_switch_press_flag) && (foot_switch_sens_port.IN & foot_switch_sens_pin) )
			{
				#ifdef _DEBUG_KEYPAD
				USART1_sendString("");
				USART1_sendString("FOOT SWITCH SENSING ISR PRESSED:");
				#endif
				MOTOR_ON_OFF(true);
				smart_switch_timmer = millis;
				foot_switch_press_flag = true;
			}
			
			else if ( (foot_switch_press_flag) && (!(foot_switch_sens_port.IN & foot_switch_sens_pin)) )
			{
				#ifdef _DEBUG_KEYPAD
				USART1_sendString("");
				USART1_sendString("FOOT SWITCH SENSING ISR RELEASED:");
				#endif
				MOTOR_ON_OFF(false);
				foot_switch_press_count++;
				foot_switch_release_flag = true;
				
			}
		}
		else
		{
			if ( (!foot_switch_press_flag) && (foot_switch_sens_port.IN & foot_switch_sens_pin) )
			{
				#ifdef _DEBUG_KEYPAD
				USART1_sendString("");
				USART1_sendString("FOOT SWITCH SENSING ISR PRESSED:");
				#endif

				foot_switch_press_flag = true;
			}
			else if (!(foot_switch_sens_port.IN & foot_switch_sens_pin))
			{
				#ifdef _DEBUG_KEYPAD
				USART1_sendString("");
				USART1_sendString("FOOT SWITCH SENSING ISR RELEASED:");
				#endif
				foot_switch_release_flag = true;
			}
		}
		foot_switch_sens_port.INTFLAGS |= foot_switch_sens_pin;
	}
	 
	if (foot_switch_detc_port.INTFLAGS & foot_switch_detc_pin) 
	{
		#ifdef _DEBUG_KEYPAD
		USART1_sendString("");
		USART1_sendString("ISR: foot switch isr triggered.");
		#endif
		
		foot_switch_detect_isr_flag = true;
		foot_switch_detc_port.INTFLAGS |= foot_switch_detc_pin;
		
	}
	if (UNIT_PORT.INTFLAGS & UNIT_PIN)
	{
		#ifdef _DEBUG_KEYPAD
		USART1_sendString("");
		USART1_sendString("UNIT BUTTON PRESSED.");
		#endif
		
		if ((!unit_button_pressed_flag) && ((millis - long_press_pwms_set) > 250))
		{
			unit_button_pressed_flag = true;
		}
		if ((UNIT_PORT.IN & UNIT_PIN) && countRunTime)
		{
			mode_button_flag = true;
			long_press_pwms_set = millis;
		}
		else if (!(UNIT_PORT.IN & UNIT_PIN))
		{
			long_press_pwms_set = 0;
			mode_button_flag = false;
		}
		
		UNIT_PORT.INTFLAGS |= UNIT_PIN;
	}
}

ISR(PORTE_PORT_vect)
{
	if (MOTOR_ON_OFF_PORT.INTFLAGS & MOTOR_ON_OFF_PIN)
	{
		//		(button_motor_on_off_flag) ? (button_motor_on_off_flag = false) : (button_motor_on_off_flag = true);
		play_pause_button_press_flag = true;
		
		// 		 		if ((button_motor_on_off_flag) || (!(RELAY_MOTOR_PORT.OUT | RELAY_MOTOR_PIN)) )
		// 		 		{
		// 					 button_motor_on_off_flag= false;
		//
		// 			 		smart_switch_cont_motor_on_flag = false;
		// 			 		foot_switch_press_flag = false;
		// 			 		foot_switch_release_flag = false;
		// 			 		smart_switch_timmer = 0;
		// 			 		foot_switch_press_count = 0;
		// 			 		power_save_timer_on_flag = false;
		// 			 		power_save_motor_on_timmer = 0;
		// 		 		}
		// 				else
		// 				{
		// 					button_motor_on_off_flag = true;
		// 				}
		//
		value_ok_flag = true;

		MOTOR_ON_OFF_PORT.INTFLAGS |= MOTOR_ON_OFF_PIN;
	}

	if (POWER_SAVE_PORT.INTFLAGS & (POWER_SAVE_PIN))
	{
		(power_save_mode_on_flag) ? (power_save_mode_on_flag = false) : (power_save_mode_on_flag = true);
		value_increment_flag = true;
		
		#ifdef _DEBUG_KEYPAD
		if (power_save_mode_on_flag)
		{
			USART1_sendString("");  
			USART1_sendString("ISR: power save mode on");
		}
		else
		{
			USART1_sendString("");
			USART1_sendString("ISR: power save mode off");
		}
		#endif
		//powersave_icon_flag = true;
		POWER_SAVE_PORT.INTFLAGS |= (POWER_SAVE_PIN);
	}

	//as FOOT SWITCH DETECT ISR belong to porte which is also called in keypad section we are gonna call FOOT SWITCH DETECT ISR in keypad file.//
	
		if (SPEED_PORT.INTFLAGS & (SPEED_PIN))
		{
			#ifdef _DEBUG_Keypad
			USART1_sendString("Speed Change button is pressed");
			#endif
//			Speed_change_flag = true;
			speed_button_press_flag = true;
			SPEED_PORT.INTFLAGS |= SPEED_PIN;
		} 
}
