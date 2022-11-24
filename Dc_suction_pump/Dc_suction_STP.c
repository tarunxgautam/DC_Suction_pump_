#include "Dc_suction_declarations.h"

void stp_foot_switch_check	(void)
{
	if (foot_switch_detc_port.IN & (foot_switch_detc_pin))	//checking initially if the foot switch is connected
	{
		foot_switch_connected_flag = true;
		#ifdef _DEBUG
		USART1_sendString("foot switch detected.");
		#endif
	}
	else
	{
		#ifdef _DEBUG
		USART1_sendString("foot switch not found.");
		#endif
	}
}

// void AMS_5812_checkup (void)
// {
// 	//int timeout_count = 0;
// 	for (int i = 0; i < 10; i++)
// 	{
// 		current_pressure = AMS_mmhg_average(1);
// 		if ((i2c_timeout_flag) || (max_sens_error_val))
// 		{
// 			timeout_count++;
// 			i2c_timeout_flag = false;
// 		}
// 	}
//
// 	if (timeout_count > 3)
// 	{
// 		ccp_write_io((uint8_t*)0x0041, RSTCTRL_SWRST_bm);
// 	}
//
// }

void check_pressure_sensor (void)
{
	
}

void STP (void)
{
	MOTOR_ON_OFF(false);
	stp_foot_switch_check();
	power_on_screen();
	ams_reset();
	ams_check();
}

void ams_reset (void)
{
	ams_switch_port.OUT |= ams_switch_pin;
	_delay_ms(500);
	ams_switch_port.OUT &= ~ams_switch_pin;
	_delay_ms(500);
	AMS_5812_init();
}

void ams_check (void)
{
	uint8_t uc_reset_count = read_8t_data_in_eeprom_SPM (uc_ams_reset_count_addr);
	previous_uc_ams_reset_flag = read_8t_data_in_eeprom_SPM (uc_ams_reset_flag_addr);
	bool sensor_all_right = false;
	float old_current_pressure = 0.0;
	uint8_t ams_reset_count = 0, same_pressure_read_count = 0;
	
	// 	if(uc_reset_count < 3)
	// 	{
	while ( (sensor_all_right == false) && (ams_reset_count < 3) )
	{
		for(int i = 0; i<10; i++)
		{
			current_pressure_mmhg = AMS_mmhg_average(1);
			//	USART1_sendFloat(current_pressure,3);
			if (i2c_timeout_flag)/* || (current_pressure_mmhg > max_sens_error_val) || (current_pressure_mmhg <= min_sens_error_val))*/
			{
				timeout_count++;
				i2c_timeout_flag = false;
				USART1_sendString("timeout_count++");
			}
			if(old_current_pressure == current_pressure_mmhg)
			{
				USART1_sendString("same_pressure_read_count++");
				same_pressure_read_count++;
			}
			else
			{
				USART1_sendString("same_pressure_read_count = 0");
				same_pressure_read_count = 0;
			}
			old_current_pressure = current_pressure_mmhg;
		}
		if ((timeout_count > 3) || (same_pressure_read_count > 5))		//sensor not working
		{
			#ifdef stp_debug
			USART1_sendString("-----AMS Reset-----");
			#endif
			
			ams_reset();
			ams_reset_count++;
			same_pressure_read_count = 0;
			timeout_count = 0;
			sensor_all_right = false;
		}
		else 															//sensor starts working
		{
			uc_reset_count = 0;
			ams_reset_count = 0;
			same_pressure_read_count = 0;
			timeout_count = 0;
			sensor_all_right = true;
			previous_uc_ams_reset_flag = false;
			write_8t_data_in_eeprom_SPM(uc_ams_reset_count_addr, uc_reset_count);
			write_8t_data_in_eeprom_SPM(uc_ams_reset_flag_addr, previous_uc_ams_reset_flag);
		}
	}

	if (ams_reset_count >= 3)
	{
		uc_reset_count++;
		previous_uc_ams_reset_flag = true;
		write_8t_data_in_eeprom_SPM(uc_ams_reset_count_addr, uc_reset_count);
		write_8t_data_in_eeprom_SPM(uc_ams_reset_flag_addr, previous_uc_ams_reset_flag);
		
		if (uc_reset_count > 3)
		{
			#ifdef stp_debug
			USART1_sendString("------PRINTING SENSOR FAIL SCREEN-----");
			#endif

			//				print_error_display();
		}
		
		_delay_ms(5);
		#ifdef stp_debug
		USART1_sendString("------CONTROLLER RESETING-----");
		#endif
		
		// UPDATE EEPROM ADDR FOR SMARTSWITCH AND OTHER
		write_8t_data_in_eeprom_SPM(Smartswitch_status_addr,smart_switch_mode_flag);
		write_8t_data_in_eeprom_SPM(Powersave_status_addr,power_save_mode_on_flag);
		write_8t_data_in_eeprom_SPM(previous_soft_hard_reset_flag_addr, 1);
		_delay_ms(5);
		ccp_write_io((uint8_t*)0x0041, RSTCTRL_SWRST_bm);    // controller reset
	}
	
	//}
	
}