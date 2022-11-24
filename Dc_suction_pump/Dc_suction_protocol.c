#include "Dc_suction_declarations.h"

void motor_sol_gpio_init (void)
{
//	RELAY_MOTOR_PORT.DIR	|= RELAY_MOTOR_PIN;		RELAY_MOTOR_PORT.OUT |= RELAY_MOTOR_PIN;
//	RELAY_SOLENOID_PORT.DIR |= RELAY_SOLENOID_PIN;  RELAY_SOLENOID_PORT.OUT |= RELAY_SOLENOID_PIN;
	
// 	#ifdef _DEBUG
// 	USART1_sendString("motor_sol_gpio_init: motor gpio initialized.");
// 	#endif
	
}
/*
void jar_selection (void)
{
	if (selection1_button_press_flag || selection2_button_press_flag)
	{
		if (jar_select == JAR1)
		{
			delete_rectangle(12,129,20,140);     // jar2 disable
			print_icon(12,129, & temp_right_arrow_15x40, 15, 42);	 // right jar icon
			RELAY_SOLENOID_PORT.OUT |= RELAY_SOLENOID_PIN;
			#ifdef _DEBUG
			USART1_sendString("jar_selection: jar 1 select");
			#endif
			jar_select = 0;			//so that RELAY_SOLENOID_PORT is not setting again and again
		}
		else if (jar_select == JAR2)
		{
			delete_rectangle(12,129,20,140);     // jar1 disable
			print_icon(12,129, & temp_left_arrow_15x40, 15, 42);		 // left jar_icon
			RELAY_SOLENOID_PORT.OUT &= ~RELAY_SOLENOID_PIN;
			#ifdef _DEBUG
			USART1_sendString("jar_selection: jar 2 select");
			#endif
			jar_select = 0;			//so that RELAY_SOLENOID_PORT is not setting again and again
			
		}
		selection2_button_press_flag = false;
		selection1_button_press_flag = false;
	}
}
*/
void MOTOR_ON_OFF(bool status)
{
	if(status)
	{
		TCA0.SINGLE.CTRLA |= (1 << 0);
        TCA0.SINGLE.CMP0 = Duty_cycle;
		//#ifdef _DEBUG
		USART1_sendString("MOTOR_ON_OFF: motor turned on");
		USART1_sendInt(Duty_cycle);

		//#endif
	}
	else if (!status)
	{
		TCA0.SINGLE.CTRLA |= (1 << 0);
		TCA0.SINGLE.CMP0 = 0;
		
 
    button_motor_on_off_flag = false;
	
		USART1_sendString("MOTOR_ON_OFF: motor turned off");

	}
}

void dc_suction_pressure_main (uint8_t avg)
{
    current_pressure_mmhg = AMS_mmhg_average(avg);
    //current_pressure_mmhg = AMS_mmhg_queue_average();
    if(current_pressure_mmhg < 0)
    {
	    current_pressure_mmhg = 0.0;
    }
    else if (current_pressure_mmhg > max_mmhg_value)
    {
	    current_pressure_mmhg = max_mmhg_value;					//so that mmhg value does not exceed max mmhg value, and does not give garbage value
    }

    //  current_pressure_mmhg = 564.412;
    // 	#ifdef _DEBUG
    //USART1_sendString("current pressure");
    //USART1_sendFloat(current_pressure_mmhg,3);
    //	#endif
    switch (unit_mode)
    {
	    case mode_mmhg:
						#ifdef _DEBUG
		//				USART1_sendString("PRESSURE MAIN: mmhg calculating.");
						#endif
						break;

	    case mode_cmh2o:
						#ifdef _DEBUG
						USART1_sendString("PRESSURE MAIN: cmh2o calculating.");
						#endif
	    
						current_pressure_cmh2O = (current_pressure_mmhg * 1.3595);
						if (current_pressure_cmh2O > max_cmh2o_value)
						{
							current_pressure_cmh2O = max_cmh2o_value;
						}
						break;
	    
	    case mode_mpa:
						//#ifdef _DEBUG
//						USART1_sendString("PRESSURE MAIN: Mpa calculating.");
						//						#endif
	    
						current_pressure_mpa = (current_pressure_mmhg * 0.13332);
						// 		if(current_pressure_mpa > max_mpa_value)
						// 		{
						// 			current_pressure_mpa = max_mpa_value;
						// 		}
						// 		USART1_sendFloat(current_pressure_mpa,3);
						break;
	    
	    default:
					#ifdef _DEBUG
					USART1_sendString("PRESSURE MAIN: invalid unit mode, resetting to mmhg");
					#endif
	    
					unit_mode = mode_mmhg;
					break;
    }
}

void dc_suction_protocol_main (void)
{
	//	jar_selection();
//	powersave_icon();
//	smart_switch_icon();
}

// void powersave_icon (void)
// {
// 	if (powersave_icon_flag)
// 	{
// 		//delete_rectangle(45,5,60,8);
// 		print_icon(35,120, & temp_powersave_27x40, 27, 42);      // power_save icon
// 		powersave_icon_flag = false;
// 	}
	//     if(powersave_icon_flag)
	//     {
	// 	    if (++unit_power_mode > 2)
	// 	    {
	// 		    unit_power_mode = 1;
	// 	    }
	//
	// 	    switch (unit_power_mode)
	// 	    {
	// 		    case 1:
	// 		    print_icon(35,120, & temp_powersave_27x40, 27, 42);      // power_save icon
	// 		    break;
	//
	// 		    case 2:
	// 		    delete_rectangle(35,120,60,150);
	// 		    break;
	//
	// 		    default:
	// 		    break;
	// 	    }
	// 	    powersave_icon_flag = false;
	//     }


// void smart_switch_icon (void)
// {
// 	if (smart_switch_icon_flag)
// 	{
// 		//delete_rectangle(45,5,60,8);
// 		print_icon(60,122, & animation_1_foot_24x33,24, 40);
//		_delay_ms(300);
// 		print_icon(60,122, & animation_2_foot_24x33,24, 40);
// 		_delay_ms(300);
//		print_icon(60,122, & animation_3_foot_24x33,24, 40);
//		_delay_ms(300);
// 		smart_switch_icon_flag = false;
// 	}
// }



// void sensor_reset (void)
// {
// 	if (Speed_change_flag)
// 	{
// 		USART1_sendString("-----Reset-----");
// 		PORTE.OUT |= (1<<0);
// 		_delay_ms(5000);
// 		PORTE.OUT &= ~(1<<0);
// 		_delay_ms(500);
// 		AMS_5812_init();
// 		Speed_change_flag = false;
// 	}
// }

