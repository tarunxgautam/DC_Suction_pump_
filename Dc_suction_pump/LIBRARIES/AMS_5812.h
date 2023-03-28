#ifndef AMS5812_H_
#define AMS5812_H_

// \Dc_suction_declarations.h
// \LIBRARIES\AMS_5812.h

#define AMS_ADDR				0x78  //address of sensor
#define AMS_5812_MODE_ANALOG

/*********************** AMS-5812 *******************************/
#define AMS_5812_ADC_CHANNEL			channel_2								// ADC channel for AMS-5812
#define AMS_5812_ADC_REF_VOLT			3.3												// ADC ref voltage
#define AMS_5812_R1						3300											// Value of resister in ohms connected next to the sensor
#define AMS_5812_R2						10000											// Value of resister in ohms connected AMS_5812_R1 and ground
#define AMS_5812_P_MIN					0.0												// Minimum pressure in PSI
#define AMS_5812_P_MAX					15.0											// Maximum pressure in PSI
#define AMS_5812_C_MIN					3277											// Minimum digital count
#define AMS_5812_C_MAX					29491											// Maximum digital count
#define AMS_5812_V_MIN					0.5												// Minimum voltage in volts on analog output pin of AMS-5812
#define AMS_5812_V_MAX					4.5												// Maximum voltage in volts on analog output pin of AMS-5812
#define AMS_5812_VDR					0.752//(AMS_5812_R2 / (AMS_5812_R1 + AMS_5812_R2))		// Voltage Divider Ratio: Divides the voltage by this fraction

void AMS_5812_analog_init(void);
void AMS_5812_init(void);
float AMS_raw(void);
float AMS_5812_psi_read (void);
float AMS_psi_average (uint8_t);
float AMS_mmhg_average (uint8_t);
float AMS_5812_temp_read(void);

void AMS_5812_analog_init(void)
{
	ADC0_init();
}

void AMS_5812_init(void)			// to initialize i2c, UART, and other peripherals for communication with sensor
{
	// 	PORTA.PIN2CTRL |= (1 << 3);   // SDA pull up //PA2
	// 	PORTA.PIN3CTRL |= (1 << 3);   // SCL pull up //PA3
	USART1_init(9600);
	I2C_0_initialization();
	I2C_0_master_initialization();
		I2C_0_sendAddress(0x78,1);  //1 for read
// 		_delay_ms(10);

}


float AMS_raw(void)
{
	uint16_t pressure_raw = 0;
	
	#ifdef AMS_5812_MODE_DIGITAL
	
			uint16_t temp_raw = 0;

			pressure_raw = I2C_0_recieveData();	// Read first byte of pressure raw data
			pressure_raw <<= 8;					// Shifting for placing LSB bits of pressure data
			TWI0.MCTRLB &= ~(1 << 4);			// Send ACK
			
			pressure_raw |=I2C_0_recieveData();	// Read second byte of pressure raw data
			TWI0.MCTRLB &= ~(1 << 4);			// Send ACK
			
			temp_raw = I2C_0_recieveData();		// Read first byte of temperature raw data
			temp_raw <<= 8;						// Shifting for placing LSB bits of temperature data
			TWI0.MCTRLB &= ~(1 << 4);			// Send ACK
			
			temp_raw |= I2C_0_recieveData();	// Read second byte of temperature raw data
			I2C_0_stop_transmission();			// Stop the transmission
						
	#endif
	
	#ifdef AMS_5812_MODE_ANALOG
		
		USART1_sendString_without_newline("VDR raw data is:\t");
		USART1_sendFloat(AMS_5812_VDR, 2);
		
		float voltage = ((AMS_5812_ADC_REF_VOLT * (float)ADC0_read(AMS_5812_ADC_CHANNEL)) / 40950.0); // Voltage after dividing
		USART1_sendString_without_newline("OLD Voltage raw data is:\t");
		USART1_sendFloat(voltage, 2);
		voltage = voltage / AMS_5812_VDR;	
		USART1_sendString_without_newline("NEW Voltage raw data is:\t");
		USART1_sendFloat(voltage, 2);
		
		pressure_raw = ((voltage - AMS_5812_V_MIN)/((AMS_5812_V_MAX - AMS_5812_V_MIN)/(AMS_5812_P_MAX - AMS_5812_P_MIN))) + AMS_5812_P_MIN;


	#endif
	
	USART1_sendString_without_newline("Pressure raw data is:\t");
	USART1_sendInt(pressure_raw);
	return ((float)(pressure_raw));		// Return pressure raw data
}

////////////////////////////////////////////////////////////////////
// float AMS_raw_temp(void)
// {
// 	uint32_t pressure_raw = 0, temp_raw = 0;
// 	
// 	pressure_raw = I2C_0_recieveData();			
// 	pressure_raw <<= 8;
// 	TWI0.MCTRLB &= ~(1 << 4);						// Send ACK
// 	
// 	pressure_raw |=I2C_0_recieveData();//   	
// 	TWI0.MCTRLB &= ~(1 << 4);						// Send ACK
// 	
// 	temp_raw = I2C_0_recieveData();				
// 	temp_raw <<= 8;
// 	TWI0.MCTRLB &= ~(1 << 4);						// Send ACK
// 	
// 	temp_raw |= I2C_0_recieveData();	
// 	I2C_0_stop_transmission();
// 
// 	return (float)(temp_raw);
// }
// 
// float AMS_5812_temp_read(void)
// {
// // 	 	uint16_t _data2;
// // 		uint16_t _data3 = 0;										
// // 	 	for (int i = 0; i<100; i++)											//
// // 	 	{
// // 	 		_data2 = AMS_raw_temp();
// // 			_data2 &= 0xFF;
// // 			_data3 += _data2 ;	 
// // 			_data3 /= 2;	
// // 		 }		 
// 		//USART1_sendInt(_data3);
// 		return ((((AMS_raw_temp() - 3277) / 26214) - 110) - 25);				// returning the temperature value from AMS5812-0150-D
// }


float AMS_5812_psi_read (void)
{
	#ifdef AMS_5812_MODE_DIGITAL
			return (((AMS_raw() - AMS_5812_C_MIN) / ((AMS_5812_C_MAX - AMS_5812_C_MIN)/(AMS_5812_P_MAX - AMS_5812_P_MIN))) + AMS_5812_P_MIN);         // AMS 5812-0150-D is used instead of AMS 5812-1000-D
	#endif
	
	#ifdef AMS_5812_MODE_ANALOG
			return AMS_raw();
	#endif
}

float AMS_psi_average (uint8_t average_no)
{
	float AMS_current_psi = 0.0;
	for (int i = 0; i<average_no; i++)
	{
		AMS_current_psi += AMS_5812_psi_read();
		AMS_current_psi /= 2.0;
	}
	return AMS_current_psi;
}

float AMS_mmhg_average (uint8_t average_no)
{
	
	float AMS_current_psi = AMS_5812_psi_read();
	for (int i = 0; i<average_no - 1; i++)
	{
		AMS_current_psi += AMS_5812_psi_read();
		AMS_current_psi /= 2.0;
	}
	return (AMS_current_psi*51.7149);
}

#endif