#include "Dc_suction_declarations.h"


void NTC_init (void)
{
	ADC0_init();
}

float read_ntc (void)
{
	
	//USART1_sendString(__FUNCTION__);
	uint8_t i = 0;
	for (i = 0; i < 20; i++)
	{
		ntc_raw_adc += ADC0_read(NTC_ADC_channel);
		//USART1_sendInt(ntc_raw_adc);
	}

	ntc_raw_adc = ntc_raw_adc / i;

	voltage = ((NTC_ADC_REF_VOLT * ntc_raw_adc) / 40950);

// 	current = voltage / REF_RESISTANCE;
// 	
// 	R_ntc = (ADC_REF_VOLT - voltage) / current;
// 	
// 	temperature = (1.0 / (NOMINAL_TEMPERATURE + 273.15)) + ((1.0 / BETA_VALUE) * log (R_ntc / 10000.0));
// 	
// 	temperature = (1.0 / temperature) - 273.15;
	
// 	USART1_sendString("******temp******");
// 	USART1_sendFloat(temperature, 2);
/*	return (temperature);*/
	//return (show_temp_4digit (35, 125));
	return 0;
}
