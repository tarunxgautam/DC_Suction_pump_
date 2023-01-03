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
	if (TCB1_INTFLAGS & (1<<0))
	{
		millis++;
		TCB1_INTFLAGS |= (1<<0);
	}
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

/////////////////////////////reset and calculation of data stored in eeprom in every 10 minutes //////////////////////////////////////////////////////////////////////////

void reset_time(void)
{

	unsigned long curr_millisecond = 0 ;
	curr_millisecond =  millis ;
	while (abs(curr_millisecond - millis) < 4000)
	{USART1_sendString("Reset mode");
		if((PORTF.IN &= (1 << 2)) && (PORTE.IN &= (1 << 2)))
		{
			delete_rectangle(0, 0, 80, 150);
			
			print_icon(5, 73, &Press_font_20x48, 20,48);
			print_icon(22, 73, &Button_font_30x64, 30,64);
			print_icon(42, 73, &In_font_30x24, 30,24);
			print_icon(50, 73, 
			
			&Special_font_30x64, 30,64);
			print_icon(6, 93, &Pattern_font_30x64, 30,64);
			clear_all_keypad_flags();
			while((!unit_button_pressed_flag) && (!power_save_mode_on_flag) && (!play_pause_button_press_flag) && (!smart_switch_mode_flag) && (!speed_button_press_flag ));
			if(unit_button_pressed_flag)
			{
				unit_button_pressed_flag = false;
				while ((!unit_button_pressed_flag) && (!power_save_mode_on_flag) && (!play_pause_button_press_flag) && (!smart_switch_mode_flag) && (!speed_button_press_flag ));
				if(play_pause_button_press_flag)
				{
					play_pause_button_press_flag = false ;
					
					// reseting the service time in at service_time_addr in EEprom
					write_data_in_eeprom_SPM_32bits(service_time_addr, 0x00);
					USART1_sendString("debugging- eeprom reset");
					delete_rectangle(0,0,75,150);
					print_icon( 1, 43, &Service_font_30x64, 30, 64);   //printing service and total time icons
					print_icon( 23, 43, &Hours_font_30x48, 30, 48);
					print_icon( 1, 103, &Total_font_30x48, 30, 48);
					print_icon( 17, 103, &Run_font_30x32, 30, 32);
					print_icon( 28, 103, &Hours_font_30x48, 30,  48);
					display_print_function_numerics();
				}
				else
				{
					display_runTime_serviceTime();
				}
			}
			else
			{
				display_runTime_serviceTime();
			}
		}
	}
	return; 
}

void calcutale_run_service_time(void)
 {
	
		unsigned long var = 0;

		USART1_sendInt(millis - g_currentMillis_runTime);
		
		if(((millis - g_currentMillis_runTime) > 600000 ) && countRunTime ) 		// waiting for 10 minutes.
		{
				
				USART1_sendString("??????????????????pt-2 CHAL GAYA //////////////////////////") ;
 			var =  (read_long_data_in_eeprom_SPM(total_run_time_addr));	
			 USART1_sendString("print the value of time");					//For Total Run Time
			 USART1_sendInt(var );
 			var += 600000 ;
			 USART1_sendInt(var );
 			 write_data_in_eeprom_SPM_32bits(total_run_time_addr, var);
			 USART1_sendInt(read_long_data_in_eeprom_SPM(total_run_time_addr));


 			var = (read_long_data_in_eeprom_SPM(service_time_addr));									//for service time
 			var += 600000 ;
			 USART1_sendInt(var);
 			write_data_in_eeprom_SPM_32bits(service_time_addr, var);

 			//printNumScreen_RightJustified(uint8_t x_last, uint8_t y_start, long num, uint8_t digits);
 				
				g_currentMillis_runTime = millis ;
			/*	less_than_ten_minute_flag = false;*/
				//_delay_ms(10000);
			
		}
// 		else if (less_than_ten_minute_flag && (millis - g_currentMillis_runTime) < 600000)
// 			{
// 				USART1_sendString("**************** bhiya chll giya**************************");
// 				//  				var = read_data_in_eeprom_SPM_32bits(total_run_time_addr);									//For Total Run Time
// 				//  				var += (millis - g_currentMillis_runTime) ;
// 				//  				write_data_in_eeprom_SPM_32bits(total_run_time_addr, var);
			
			
// 				//  				var = read_data_in_eeprom_SPM_32bits(service_time_addr);									//for service time
// 				//  				var += (millis - g_currentMillis_runTime0) ;
// 				//  				write_data_in_eeprom_SPM_32bits(service_time_addr, var);
// 				less_than_ten_minute_flag = false;
// 			}
		
}

void clear_all_keypad_flags(void)
{
	unit_button_pressed_flag = false; 
	power_save_mode_on_flag = false;
	play_pause_button_press_flag = false; 
	smart_switch_mode_flag = false ;
	speed_button_press_flag = false;
}