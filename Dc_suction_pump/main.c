
#define F_CPU 24000000UL
//#define _DEBUG 
#define _DEBUG_Keypad

#include "Dc_suction_declarations.h"
#include "Dc_suction_keypad.c"
#include "Dc_suction_protocol.c"
#include "Dc_suction_STP.c"
#include "Dc_suction_foot_switch.c"
#include "Dc_suction_NTC_read.c"
#include "Dc_suction_timmers.c"
#include "DC_suction_lcd_uc1698.c"
#include "Dc_suction_lcd_pressure_bar.c"
#include "Dc_suction_pwm.c"
#include "DC_suction_eeprom.c"
#include "DC_suction_ISR.c"
#include "dc_suction_alarms.c"
#include "DC_pwm_set_mode.c"

int main(void)
{
	//_delay_ms(500);
	cli();
	_PROTECTED_WRITE (CLKCTRL.OSCHFCTRLA, ((CLKCTRL_FRQSEL_24M_gc)|(CLKCTRL_AUTOTUNE_bm)));
 	timmer_init();
 	USART1_init(9600);
 	AMS_PORT.DIR |= AMS_PIN;
 	AMS_PORT.OUT &= ~(AMS_PIN);									//ams
 	PORTB.DIR |= (1<<0);									    // foot switch detection led pin
 	TCA0_PWM_init();
 	SPI_0_init();
 	FOOT_SW_LED_PORT.DIR |= FOOt_SW_LED_PIN;
 	FOOT_SW_LED_PORT.OUT &= ~(FOOt_SW_LED_PIN);
 	SUCTION_LED_PORT.DIR |= SUCTION_LED_PIN;
 	AMS_5812_init();
 	eeprom_init_read();
 	//motor_sol_gpio_init();
 	NTC_init();
 	keypad_gpio_init();
 	lcd_uc1698u_init();
 	foot_switch_init();
 	//PORTD.DIR |= (1 << 1); 
	//PORTF.DIR |= (1 << 1);									 //Setting FAN pin as Output
 	STP();
 	_delay_ms(100);
 	sei();
 	display_runTime_serviceTime();								 //service time and total run time display
 	lcd_loading_page();
 
 	while (1)
 	{
 			dc_suction_pressure_main (50);						//argument given to read n times and find average, where n is the argument provided
 			keypad_main();
 			foot_switch_main();
 			read_ntc();
			dc_suction_protocol_main();
			lcd_uc1698u_main_screen();
			power_save_protocol();
			mode_1();											// just added for PWM mode \to dynamically calibrate the PWM of the motor.
			calcutale_run_service_time();
			//FAN_opt();										// FAN Operation using NTC.
 	}
	return 0;
}
