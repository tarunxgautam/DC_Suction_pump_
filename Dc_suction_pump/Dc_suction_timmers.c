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
	{
		if((PORTC.IN &= (1 << 6)) && (PORTE.IN &= (1 << 2)))
		{
			_delay_ms(1000);
			delete_rectangle(0, 0, 80, 150);
			print_icon(5, 73,(uint8_t*) &Press_font_20x48, 20,48);
			print_icon(22, 73,(uint8_t*) &Button_font_30x64, 30,64);
			print_icon(42, 73,(uint8_t*) &In_font_30x24, 30,24);
			print_icon(50, 73,(uint8_t*) &Special_font_30x64, 30,64);
			print_icon(6, 93,(uint8_t*) &Pattern_font_30x64, 30,64);
			clear_all_keypad_flags();
			while((!unit_button_pressed_flag) && (!power_save_mode_on_flag) && (!play_pause_button_press_flag) && (!smart_switch_mode_flag) && (!speed_button_press_flag ));	
						
				if(smart_switch_mode_flag)
				{
					smart_switch_mode_flag = false;
					while ((!unit_button_pressed_flag) && (!power_save_mode_on_flag) && (!play_pause_button_press_flag) && (!smart_switch_mode_flag) && (!speed_button_press_flag ));
					
					if(play_pause_button_press_flag)
					{
						play_pause_button_press_flag = false ;							// reseting the service time in at service_time_addr in EEprom
						write_data_in_eeprom_SPM_32bits(service_time_addr, 0x00);
						Service_alarm_flag = false;
						delete_rectangle(0,0,80,150);
						display_runTime_serviceTime();
						clear_all_keypad_flags();
					}
				}
		}
	}
		clear_all_keypad_flags();
		return; 
}

void calcutale_run_service_time(void)
 {
	
		unsigned long var = 0;
				
		if(((millis - g_currentMillis_runTime) > 600000 ) && countRunTime ) 							// waiting for 10 minutes.
		{
 			var =  (read_long_data_in_eeprom_SPM(total_run_time_addr));	
 			var += 600000 ;
			write_data_in_eeprom_SPM_32bits(total_run_time_addr, var);
			
 			var = (read_long_data_in_eeprom_SPM(service_time_addr));									//for service time
 			var += 600000 ;
 			write_data_in_eeprom_SPM_32bits(service_time_addr, var);
			  if ( var >= allowed_service_duration)
			  {
				  Service_alarm_flag_run = true;
			  }
			  else
			  {
				   Service_alarm_flag_run = false;
			  }

 			//printNumScreen_RightJustified(uint8_t x_last, uint8_t y_start, long num, uint8_t digits);
 				
				g_currentMillis_runTime = millis ;
			/*	less_than_ten_minute_flag = false;*/
				//_delay_ms(10000);
			
		}

		
}

void clear_all_keypad_flags(void)
{
	unit_button_pressed_flag		= false; 
	power_save_mode_on_flag			= false;
	play_pause_button_press_flag	= false; 
	smart_switch_mode_flag			= false;
	speed_button_press_flag			= false;
}

void clear_all_lpm_flags(void)
{
	lpm20_flag = lpm30_flag = lpm40_flag = lpm50_flag = lpm60_flag = false;
}