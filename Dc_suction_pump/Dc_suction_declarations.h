#ifndef AC_suction_include_and_variables
#define AC_suction_include_and_variables

#define PCB_VERSION 15 // PCB version multiplied by 10

#include <util/delay.h>
#include <avr/io.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/eeprom.h>
#include <avr/xmega.h>
#include <avr/cpufunc.h>

#include "LIBRARIES/UART_1_AVR128DA64.h"
#include "LIBRARIES/SPI_0_AVR128DA64.h"
#include "LIBRARIES/I2C_0_AVR128DA64.h"
#include "LIBRARIES/icons_and_numerics.h"
#include "LIBRARIES/ac_suction_icons.h"
#include "LIBRARIES/ADC_AVR128DA64.h"
#include "LIBRARIES/AMS_5812.h"
#include "LIBRARIES/_UC1698U.h"
#include "LIBRARIES/UC1698U.c"

/*              KEYPAD							*/
#if PCB_VERSION == 13
	#define NTC_CHANNEL						channel_2
	#define AMS_5812_CHANNEL				channel_2
	
	#define SMART_FOOT_LED					PORTC
	#define SMART_FOOT_LED_PIN				(1<<2)
	#define SMART_SW_PORT                   PORTF
	#define SMART_SW_PIN                    (1<<2)
	#define UNIT_PORT						PORTC
	#define UNIT_PIN						(1<<6)
	#define SUCTION_LED_PORT				PORTE
	#define SUCTION_LED_PIN					(1<<3)
	#define MOTOR_ON_OFF_PORT				PORTE
	#define MOTOR_ON_OFF_PIN                (1<<1)
	#define POWER_SAVE_PORT                 PORTE
	#define POWER_SAVE_PIN                  (1<<2)
	#define POWER_SAVE_LED					PORTB
	#define POWER_SAVE_LED_PIN				(1<<5)
	#define FOOT_SW_LED_PORT				PORTA
	#define FOOt_SW_LED_PIN			        (1<<5)

	#define SPEED_PORT						PORTE
	#define SPEED_PIN						(1<<0)

	#define AMS_PORT						PORTC
	#define AMS_PIN							(1<<4)
	
#elif PCB_VERSION == 14
	#define NTC_CHANNEL						channel_2
	#define AMS_5812_CHANNEL				channel_2
	
	#define SMART_FOOT_LED					PORTC
	#define SMART_FOOT_LED_PIN				(1<<2)
	#define SMART_SW_PORT                   PORTF
	#define SMART_SW_PIN                    (1<<2)
	#define UNIT_PORT						PORTC
	#define UNIT_PIN						(1<<6)
	#define SUCTION_LED_PORT				PORTE
	#define SUCTION_LED_PIN					(1<<3)
	#define MOTOR_ON_OFF_PORT				PORTE
	#define MOTOR_ON_OFF_PIN                (1<<1)
	#define POWER_SAVE_PORT                 PORTE
	#define POWER_SAVE_PIN                  (1<<2)
	#define POWER_SAVE_LED					PORTB
	#define POWER_SAVE_LED_PIN				(1<<5)
	#define FOOT_SW_LED_PORT				PORTB
	#define FOOt_SW_LED_PIN			        (1<<0)

	#define SPEED_PORT						PORTE
	#define SPEED_PIN						(1<<0)

	#define AMS_PORT						PORTC
	#define AMS_PIN							(1<<4)
	//FAN//
	#define FAN_PORT						PORTF
	#define FAN_PIN							(1<<5)

#elif PCB_VERSION == 15

	#define NTC_CHANNEL						channel_2
	#define AMS_5812_CHANNEL				channel_21
	
	#define SMART_FOOT_LED					PORTC
	#define SMART_FOOT_LED_PIN				(1<<2)
	#define SMART_SW_PORT                   PORTF
	#define SMART_SW_PIN                    (1<<2)
	#define UNIT_PORT						PORTF
	#define UNIT_PIN						(1<<3)
	#define SUCTION_LED_PORT				PORTE
	#define SUCTION_LED_PIN					(1<<3)
	#define MOTOR_ON_OFF_PORT				PORTE
	#define MOTOR_ON_OFF_PIN                (1<<1)
	#define POWER_SAVE_PORT                 PORTE
	#define POWER_SAVE_PIN                  (1<<2)
	#define POWER_SAVE_LED					PORTB
	#define POWER_SAVE_LED_PIN				(1<<5)
	#define FOOT_SW_LED_PORT				PORTB
	#define FOOt_SW_LED_PIN			        (1<<0)
	
	#define SPEED_PORT						PORTE
	#define SPEED_PIN						(1<<0)
	
	//AMS_DIGITAL_PIN//
	#define AMS_PORT						PORTC
	#define AMS_PIN							(1<<4)
	//AMS_ANALOG_PIN//
	#define AMS_ANALOG_PORT					PORTF
	#define AMS_ANALOG_PIN					(1<<5)
	
	//FAN//
	#define FAN_PORT						PORTC
	#define FAN_PIN							(1<<6)
	
#endif

/*				motor sol GPIO					*/
// #define POWER_SAVE_MOTOR_ON_OFF_TIME    60000

/*********************** ICON'S LCD CO-ORDINATES *******************************/
#define icon_valveBlock_x	36
#define icon_valveBlock_y	130
#define icon_service_x		20
#define icon_service_y		124
#define icon_power_save_x	60
#define icon_power_save_y	123
#define icon_foot_x			7
#define icon_foot_y			126
#define icon_callibrate_x	50
#define icon_callibrate_y	130

/*				foot_switch						*/
#define foot_switch_sens_port			PORTC
#define foot_switch_sens_pin			(1<<7)
#define foot_switch_detc_port			PORTC
#define foot_switch_detc_pin			(1<<3)
#define smart_switch_press_time			1000
#define check_before_time				0
#define power_save_motor_off_time		1800000											// milliseconds in 30 minutes
#define allowed_pressure_diff_mmHg		40.0 
#define allowed_pressure_diff_cmH2O		30.0
#define allowed_pressure_diff_MPa		2.0//0.2										// difference value of current pressure and previous pressure

/*********************** NTC *******************************/
#define NTC_ADC_channel			NTC_CHANNEL
#define supply_voltage			3.3
#define ntc_series_resistance	10000
#define beta_value				3435
#define ntc_R_at_25				10000
#define warning_temp_value		35.0
#define alarm_temp_value		100.0
#define high_temp_timer			30000	//0.5 minute


/*		timmers		*/
#define clock_values		  23999
#define reset_time_threshold  10000

/*				lcd_bar						*/
#define max_mmhg_value		750.0			//max value of mmhg the bar will represent
#define max_cmh2o_value     (1.3595 * max_mmhg_value)
#define max_mpa_value       (0.013332 * max_mmhg_value)
#define max_bar_height		29
#define max_bar_length		77.0			//237.0
#define starting_bar_width	2			//pressure bar starting updation point |___-----------|

/*				Alarms          */
#define allowed_service_duration		3600000000	//1000 hours into microseconds //720000000		// time for 200 hours   //21600000  // for 6 hrs

/*		AC suction protocol				*/
#define mode_mmhg	1
#define mode_cmh2o	2
#define mode_mpa	3

/*				stp						*/
#define max_sens_error_val		520		//sensor giving max value cont. after reset as error
#define min_sens_error_val		0		//sensor giving min value cont. after reset as error
#define ams_switch_port			PORTC
#define ams_switch_pin			(1<<4)

/************************************************************************/
/* EEPROM addresses                                                                     */
/************************************************************************/
/*				stp_variables						*/
#define uc_ams_reset_count_addr					0x01
#define uc_ams_reset_flag_addr					0x03
#define previous_soft_hard_reset_flag_addr		0x05
#define flowrate_addr							0x04
#define Smartswitch_status_addr					0x02
#define Powersave_status_addr					0x06

///////  added by tarun/////////
#define total_run_time_addr						0x11 // four bites for storing seconds
#define service_time_addr						0x15 // four bites for storing seconds
#define lpm20_duty_addr							0x19 // 2 bytes
#define lpm30_duty_addr							0x1B // 2 bytes
#define lpm40_duty_addr							0x1D // 2 bytes
#define lpm50_duty_addr							0x1F // 2 bytes
#define lpm60_duty_addr							0x21 // 2 bytes

int timeout_count = 0;


/*		DC suction protocol				*/
//uint8_t jar_select = JAR1;
bool button_motor_on_off_status    = false;	//status of motor through button
bool previous_soft_hard_reset_flag = false;	//true is software reset 
bool previous_uc_ams_reset_flag    = false;

/*		pressure sensor AMS5812			*/
float current_pressure_mmhg  = 0.0;
float current_pressure_cmh2O = 0.0;
float current_pressure_mpa   = 0.0;
float previous_pressure = 0.0, currentPressure = 0.0;
float allowedPressureDifference = 0.0;	// Will be used in smart foot mode.
uint16_t d_pressure = 0;

/***********TIMMER VARIABLES**************/
unsigned long currentMillis = 0, millis = 0, millis1 = 0, millis2 = 0, millis3 = 0,  g_foot_sw_blink_millis = 0;
bool _1sec = false, _300sec = false, blink_flag = false;
bool set_ten_minutes_flag = false;
unsigned long g_currentMillis_runTime = 0;



/*		keypad variables				*/
uint8_t unit_mode = 1;
bool smart_switch_mode_flag = false, unit_button_pressed_flag = false, Speed_change_flag = false, speed_button_press_flag = false, button_motor_on_off_flag = false;
bool selection1_button_press_flag = false, selection2_button_press_flag = false;
bool powersave_icon_flag = false, play_pause_button_press_flag = false;
bool smart_switch_icon_flag= false;

uint8_t unit_power_mode = 1;

/*		NTC		*/
float motor_temp = 0.0;
long motor_high_temp_timer = 0;
bool warning_motor_temp_flag = false, alarm_motor_temp_flag;
bool tempo_alarm_motor_temp_flag = false, tempo_warning_motor_temp_flag = false;
float voltage = 0.0, temperature = 0.0, R_ntc = 0.0, current = 0.0;
unsigned long ntc_raw_adc = 0;

/************************************************************************/
/*        NtC Check                                                     */
/************************************************************************/

#define NOMINAL_TEMPERATURE 25.0
#define BETA_VALUE			3435.0
#define REF_RESISTANCE		10000.0
#define NTC_ADC_REF_VOLT	3.3
#define TEMP_ALARM_RANGE1   70
#define TEMP_ALARM_RANGE2   71.0

/*				lcd_back_led						*/
bool backlit_on_flag = false;


/*		alarms and hazard		*/
bool Service_alarm_flag = false;
bool Service_alarm_flag_run = false;
volatile int pressure_blockage	=  0;

/*		ac suction lcd uc1698u		*/
bool  update_pressure_flag = false, unit_icon_update_flag = false, update_bar_flag = false;
uint8_t previous_unit_mode = 1;
bool previous_power_save_mode_flag = false, update_power_save_icon_flag = true;
bool smart_switch_icon_update_flag = false, smart_switch_delete_flag = false;

/*		foot switch variables				*/
bool foot_switch_connected_flag = false, foot_switch_press_flag = false, foot_switch_release_flag = false;
bool foot_switch_detect_isr_flag = false, smart_switch_normal_motor_on_flag = false, smart_switch_cont_motor_on_flag = false;
uint8_t foot_switch_press_count = 0;
bool power_save_timer_on_flag = false, power_save_mode_on_flag = false, power_save_icon_blink_flag = false;
unsigned long power_save_motor_on_timmer = 0, smart_switch_timmer = 0;

/*				lcd_pressure_bar					*/
float previous_pressure_bar_mmhg = 0.0, difference_pressure = 0.0;
uint8_t updated_bar_len_diff = 0/*, previous_x1 = 3*/;

bool temp_ = false;

/************************************************************************/
/*                 mode settings                                         */
/************************************************************************/
int return_val = 0;
#define button_normal_press   3000

volatile bool value_increment_flag = false;
volatile bool value_decrement_flag = false;
volatile bool value_ok_flag = true;
volatile bool mode_button_flag = false;
bool countRunTime = false;
 
bool value_increment = false;
bool value_decrement = false;
bool value_ok = false;
uint8_t pwm_value = 0;

unsigned long long_press_pwms_set = 0;

void mode_1(void);
int settings(void);
void FAN_opt(void);

/*          ams functions					*/   //  by tarun
void ams_reset (void);
void ams_check (void);
void sensor_reset(void);

/*			keypad functions				*/
void keypad_gpio_init(void);
void power_save_button (void);
void keypad_main (void);
void unit_change (void);
void button_motor_on_off (void);
void clear_all_keypad_flags(void);
void clear_all_lpm_flags(void);


typedef uint16_t eeprom_addr_t;		//FOR EEPROM
void eeprom_init (void);
void set_NVM_command_SPM(int);
void write_8t_data_in_eeprom_SPM(uint16_t, uint8_t);
void write_16t_data_in_eeprom_SPM(uint16_t, uint16_t);
void write_32t__data_in_eeprom_SPM(uint16_t, uint32_t);
uint8_t  read_8t_data_in_eeprom_SPM (eeprom_addr_t);
uint16_t read_16t_data_in_eeprom_SPM(eeprom_addr_t);
uint32_t read_long_data_in_eeprom_SPM(eeprom_addr_t);
uint32_t read_data_in_eeprom_SPM_32bits(uint16_t index);
void write_data_in_eeprom_SPM_32bits(uint16_t index, uint32_t data);



/*		AC_suction protocol				*/
void motor_sol_gpio_init (void);
void jar_selection(void);
void MOTOR_ON_OFF(bool);
void dc_suction_protocol_main(void);
void dc_suction_pressure_main (uint8_t);
void powersave_icon (void);
void smart_switch_icon (void);

/*		foot switch				*/
void foot_switch_gpio_init(void);
void foot_switch_init (void);
void foot_switch_sensing (void);
void foot_switch_main (void);
void foot_switch_check	(void);
void foot_switch_detect_isr_code(void);
void smart_sensing (void);
void power_save_protocol (void);

/*		stp protocol	*/
void check_pressure_sensor (void);
void ams_check (void);		// new added
//void AMS_5812_checkup (void);
void STP (void);

/*		lcd uc1698u		*/
void lcd_uc1698u_init(void);
void lcd_all_icon_update (void);
void lcd_icon_update (void);
void lcd_loading_page (void);
void lcd_uc1698u_main_screen (void);
void power_on_screen (void);
void pressure_bar (void);
void EDGE(void);
void partitions(void);
void show_mmgh_4digit (uint8_t,uint8_t);
void cmH20_4digit (uint8_t , uint8_t );
void MPa_4digit (uint8_t , uint8_t);
/////////////fuctions added by tarun///////////////
void display_runTime_serviceTime(void);
void display_print_runservice_time(void);
void calcutale_run_service_time(void);
void lcd_pressure_bar_init(void);
void bar_draw_delete_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void bar_draw_filled_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void display_print_function_numerics(void);

/*		NTC		*/
void NTC_init(void);
float read_ntc (void);
void temp_hazard_check (float);
float adc_to_temp (long);


/*		alarms and hazard		*/
void hazard_icon_conditions_check(void);	// all alarm icons will be turned on or off according to the conditions of different flags
void alarms_main(void);
void NTC_check(float);


/*		timers		*/
void TCB1_init(void);
void timmer_init (void);
////////fuctions added by tarun //////////////
void reset_time(void);
void calcutale_run_service_time(void);

/*				lcd_back_led						*/
void LCD_backlit_gpio_init (void);
void LCD_backlit_main (void);

/*				lcd_pressure_bar					*/
void lcd_pressure_bar_main(void);
void calculating_bar_L_mmhg(void);		//to calculate the length of the need to be updated according to the unit mode selected i.e cmh2o, mpa, mmhg


/*****  PWM ***************/
void pwm_change (void);

volatile int Duty_cycle = 0;

bool lpm20_flag = false;
bool lpm30_flag = false;
bool lpm40_flag = false;
bool lpm50_flag = false;
bool lpm60_flag = false;

volatile int LPM_20 = 1500;
volatile int LPM_30 = 1670;
volatile int LPM_40 = 1975;
volatile int LPM_50 = 2400;
volatile int LPM_60 = 3200;

uint8_t unit_mode_pwm = 1;

#endif