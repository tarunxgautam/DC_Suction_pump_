#include "Dc_suction_declarations.h"

void foot_switch_gpio_init(void)
{
	foot_switch_sens_port.DIR &= ~foot_switch_sens_pin;		foot_switch_sens_port.PIN7CTRL	|= (0x01);
	foot_switch_detc_port.DIR &= ~foot_switch_detc_pin;		foot_switch_detc_port.PIN3CTRL	|= (0x01);
	
	#ifdef _DEBUG
	USART1_sendString("foot_switch_gpio_init: foot switch gpio initialize");
	#endif
}

void foot_switch_init (void)
{
	foot_switch_gpio_init();
	if (foot_switch_detc_port.IN & (foot_switch_detc_pin))	//checking initially if the foot switch is connected
	{
		foot_switch_connected_flag = true;
		
		#ifdef _DEBUG_FOOT
		USART1_sendString("foot switch detected.");
		#endif
	}
	else
	{
		foot_switch_connected_flag = false;
		
		#ifdef _DEBUG_FOOT
		USART1_sendString("foot switch not found.");
		#endif
	}
}

void foot_switch_detect_check(void)
{
	if (foot_switch_detect_isr_flag)
	{
		if (foot_switch_detc_port.IN & (foot_switch_detc_pin))	//checking initially if the foot switch is connected
		{
			foot_switch_connected_flag = true;
//			FOOT_SW_LED_PORT.OUT |= FOOt_SW_LED_PIN;
            PORTB.OUT |= (1<<0);
			#ifdef _DEBUG
			USART1_sendString("foot switch detected.");
			#endif
		}
		else
		{
			foot_switch_connected_flag = false;
//			FOOT_SW_LED_PORT.OUT &= ~FOOt_SW_LED_PIN;
            PORTB.OUT &= ~(1<<0);
			#ifdef _DEBUG
			USART1_sendString("foot switch not found.");
			#endif
		}
		
		foot_switch_detect_isr_flag = false;
	}
	
	if (foot_switch_connected_flag)
	{
		PORTB.OUT |= (1<<0);
		#ifdef _DEBUG
		USART1_sendString("foot switch detected.");
		#endif
	}
}

/************************************************************************/
/*    Power save mode will not work until smart switch is pressed       */
/************************************************************************/

void foot_switch_main (void)
{
	if (smart_switch_mode_flag)
	{
		smart_sensing();
	}
	else if (!smart_switch_mode_flag)
	{
		foot_switch_sensing();
	}
	foot_switch_detect_check();
}




void smart_sensing (void)
{
	//to check if the double tap is occured
	if ( (foot_switch_press_flag) && (!smart_switch_cont_motor_on_flag) && (!smart_switch_normal_motor_on_flag) && (foot_switch_release_flag) && (foot_switch_press_count > 1) && ((millis-smart_switch_timmer) < smart_switch_press_time) )
	{
		#ifdef _DEBUG
		USART1_sendString("smart_sensing: smart switch motor on");
		#endif
		
		MOTOR_ON_OFF(true);
		smart_switch_cont_motor_on_flag = true;
		foot_switch_press_flag = false;
		foot_switch_release_flag = false;
		smart_switch_timmer = 0;
		foot_switch_press_count = 0;
	}
	//to turn off motor on single tap after double tap
	else if ( (foot_switch_press_flag) && (foot_switch_release_flag) && (smart_switch_cont_motor_on_flag))
	{
		#ifdef _DEBUG
		USART1_sendString("smart_sensing: smart switch motor off");
		#endif

		MOTOR_ON_OFF(false);
		smart_switch_cont_motor_on_flag = false;
		foot_switch_press_flag = false;
		foot_switch_release_flag = false;
		smart_switch_timmer = 0;
		foot_switch_press_count = 0;
		
		if (power_save_timer_on_flag)
		{
			#ifdef _DEBUG
			USART1_sendString("smart_sensing: POWER SAVE TIMMER OFF");
			#endif

			power_save_timer_on_flag = false;
			power_save_motor_on_timmer = 0;
		}
	}
	//to normally turn on the motor if tap is cont. pressed
	else if ( (foot_switch_press_flag) && (!smart_switch_cont_motor_on_flag) && (!smart_switch_normal_motor_on_flag) && (!foot_switch_press_count) && (!foot_switch_release_flag) && ((millis-smart_switch_timmer) > smart_switch_press_time) )
	{
		#ifdef _DEBUG
		USART1_sendString("smart_sensing: normal motor on");
		#endif
		
		MOTOR_ON_OFF(true);
		smart_switch_normal_motor_on_flag = true;
		power_save_icon_blink_flag = false;
		
	}
	//to turn off the motor on the cont. tap release
	else if ( (foot_switch_release_flag)  && (smart_switch_normal_motor_on_flag) && (foot_switch_press_count) && (foot_switch_press_flag) )
	{
		#ifdef _DEBUG
		USART1_sendString("smart_sensing: normal motor off");
		#endif
		
		MOTOR_ON_OFF(false);
		smart_switch_normal_motor_on_flag = false;
		foot_switch_release_flag = false;
		foot_switch_press_flag = false;
		foot_switch_press_count = 0;
		smart_switch_timmer = 0;
	}

	// to detect a delay b/w tap 1 and 2
	// if 2 tap is done b/w the time frame but is continuously pressed then the motor will not turn on in normal mode instead a delay is detected and nothing is done
	else if ( ((foot_switch_press_flag) || (foot_switch_release_flag) || (foot_switch_press_count)) && (!smart_switch_cont_motor_on_flag) && (!smart_switch_normal_motor_on_flag) && ((millis-smart_switch_timmer) > smart_switch_press_time) )
	{
		#ifdef _DEBUG
		USART1_sendString("smart_sensing: delay in double tap");
		#endif
		smart_switch_cont_motor_on_flag = false;
		smart_switch_timmer = 0;
		foot_switch_press_flag = false;
		foot_switch_release_flag = false;
		foot_switch_press_count = 0;
	}
}

void power_save_protocol (void) //if smart switch mode and power save mode are both enabled



{
	if (power_save_mode_on_flag)
	{
		if ( ((smart_switch_cont_motor_on_flag) || (button_motor_on_off_flag)) && (!power_save_timer_on_flag))
		{
			#ifdef _DEBUG
			USART1_sendString("power_save_protocol: power save timer on");
			#endif
			power_save_timer_on_flag = true;
			power_save_motor_on_timmer = millis;
			power_save_icon_blink_flag = false;
		}
		else if ( (power_save_timer_on_flag) && ((millis-power_save_motor_on_timmer) > power_save_motor_off_time))
		{
			#ifdef _DEBUG
			USART1_sendString("power_save_protocol: power save timer UP");
			#endif
			
			MOTOR_ON_OFF(false);
			power_save_timer_on_flag = false;
			power_save_motor_on_timmer = 0;
			
			//resetting smart switch flags
			smart_switch_cont_motor_on_flag = false;
			smart_switch_timmer = 0;
			foot_switch_press_flag = false;
			foot_switch_release_flag = false;
			foot_switch_press_count = 0;
			power_save_icon_blink_flag = true;
			
			//resetting play pause button
			button_motor_on_off_flag = false;
		}
				
		else if ((power_save_timer_on_flag) && ((millis-power_save_motor_on_timmer) < power_save_motor_off_time))
		{
			/***********************************************************************************************************************************************************************&********/
			/* In this 'if' condition, power_save_motor_off_time macro is commented because we want to check the pressure variation immediately. To check it after some delay uncomment it.*/
			/********************************************************************************************************************************************************************************/
			if ((abs(previous_pressure - currentPressure) > allowedPressureDifference) && (millis - power_save_motor_on_timmer > /*power_save_motor_off_time - */check_before_time))
			{
				//power_save_motor_on_timmer = millis - (power_save_motor_off_time - check_before_time);
				power_save_motor_on_timmer = millis - check_before_time; // check-before_time(reserved time) reseted to '0' after fluctuation in pressure within 5 sec.
			}
			if (millis - currentMillis > 1000)    //reading the current
			{
				previous_pressure = currentPressure;
			}
		}
	}
}

void foot_switch_sensing (void)
{
	if ( (foot_switch_press_flag)/* && (foot_switch_sens_port.IN & foot_switch_sens_pin) */)
	{
		MOTOR_ON_OFF(true);
		foot_switch_press_flag = false;
		power_save_icon_blink_flag = false;
	}
	else if ( (foot_switch_release_flag)/* && (!(foot_switch_sens_port.IN & foot_switch_sens_pin)) */)
	{
		MOTOR_ON_OFF(false);
		foot_switch_release_flag = false;
		//		foot_switch_press_flag = false;
	}
}
