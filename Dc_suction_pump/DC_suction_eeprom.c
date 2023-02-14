#include "DC_suction_declarations.h"

#define eeprom_erase_write		0x13

void fetch_lpm_duty_from_EEPROM(void)
{
	LPM_20 = read_16t_data_in_eeprom_SPM(lpm20_duty_addr);
	LPM_30 = read_16t_data_in_eeprom_SPM(lpm30_duty_addr);
	LPM_40 = read_16t_data_in_eeprom_SPM(lpm40_duty_addr);
	LPM_50 = read_16t_data_in_eeprom_SPM(lpm50_duty_addr);
	LPM_60 = read_16t_data_in_eeprom_SPM(lpm60_duty_addr);
}

void eeprom_init_read(void)
{
	fetch_lpm_duty_from_EEPROM();
	
	previous_soft_hard_reset_flag = read_8t_data_in_eeprom_SPM (previous_soft_hard_reset_flag_addr);			//true is software reset
	#ifdef eprom_debug
	if (previous_soft_hard_reset_flag)
	{
		USART1_sendString("eeprom_init: SOFTWARE RESET DETECTED");
	}
	else
	{
		USART1_sendString("eeprom_init: POWER ON RESET DETECTED");
	}
	#endif
	write_8t_data_in_eeprom_SPM(previous_soft_hard_reset_flag_addr, 0);		//false if power reset
	previous_uc_ams_reset_flag = read_8t_data_in_eeprom_SPM (uc_ams_reset_flag_addr);
	#ifdef eprom_debug
	if (previous_uc_ams_reset_flag)
	{
		USART1_sendString("eeprom_init: RESET DUE TO AMS FAULT");
	}
	#endif
	//	write_8t_data_in_eeprom_SPM(uc_ams_reset_flag_addr, 0);		//false if AMS IS WORKING FINE
	
	if(previous_uc_ams_reset_flag)
	{
		//READ EEPROM FOR SMART SWITCH, FLOW RATE, POWER SAVE MODE;
		// while writing in eeprom, 1 stands for true ,and 0 stands for false.
		read_8t_data_in_eeprom_SPM(Smartswitch_status_addr);
		read_8t_data_in_eeprom_SPM(Powersave_status_addr);
	}
}



void eeprom_init(void)
{
	set_NVM_command_SPM(eeprom_erase_write);
}

/************************EEPROM FUNCTIONS************************************************************/
void set_NVM_command_SPM(int command)
{
	CCP = 0x9D;   // SPM - Allow Self-Programming
	NVMCTRL_CTRLA = command;
}

void write_8t_data_in_eeprom_SPM(uint16_t index,uint8_t data)
{
	while(NVMCTRL.STATUS & (NVMCTRL_EEBUSY_bm));
	set_NVM_command_SPM(NVMCTRL_CMD_EEERWR_gc);

	//eeprom_write_word((MAPPED_EEPROM_START + index), data);
	*(uint8_t*)(eeprom_addr_t)(MAPPED_EEPROM_START + index) = data;

	set_NVM_command_SPM(NVMCTRL_CMD_NONE_gc);
}


void write_16t_data_in_eeprom_SPM(uint16_t index,uint16_t data)
{
	while(NVMCTRL.STATUS & (NVMCTRL_EEBUSY_bm));
	set_NVM_command_SPM(NVMCTRL_CMD_EEERWR_gc);

	//eeprom_write_word((MAPPED_EEPROM_START + index), data);
	*(uint16_t*)(eeprom_addr_t)(MAPPED_EEPROM_START + index) = data;

	set_NVM_command_SPM(NVMCTRL_CMD_NONE_gc);
}

void write_32t__data_in_eeprom_SPM(uint16_t index,uint32_t data)
{
	while(NVMCTRL.STATUS & (NVMCTRL_EEBUSY_bm));
	set_NVM_command_SPM(NVMCTRL_CMD_EEERWR_gc);
	//eeprom_write_word((MAPPED_EEPROM_START + index), data);
	*(uint32_t*)(MAPPED_EEPROM_START + index) = data;

	set_NVM_command_SPM(NVMCTRL_CMD_NONE_gc);
}

uint8_t read_8t_data_in_eeprom_SPM (eeprom_addr_t index)
{
	uint8_t data;
	eeprom_is_ready();
	data = eeprom_read_byte((uint8_t *)index);
	return data;
}

uint16_t read_16t_data_in_eeprom_SPM(eeprom_addr_t index)
{
	uint16_t data;
	eeprom_is_ready();
	data = eeprom_read_word((uint16_t *)index);
	return data;
}

uint32_t read_long_data_in_eeprom_SPM(eeprom_addr_t index)
{
	uint32_t data;
	eeprom_is_ready();
	data = eeprom_read_dword((uint32_t *)index);
	return data;
}
// added by Tarun

void write_data_in_eeprom_SPM_32bits(uint16_t index, uint32_t data)
{
	write_8t_data_in_eeprom_SPM(index++, data & 0x000000ff);
	write_8t_data_in_eeprom_SPM(index++, (data & 0x0000ff00) >> 8);
	write_8t_data_in_eeprom_SPM(index++, (data & 0x00ff0000) >> 16);
	write_8t_data_in_eeprom_SPM(index++, (data & 0xff000000) >> 24);
}

// uint32_t read_data_in_eeprom_SPM_32bits(uint16_t index)
// {
// 	
// 	return ((read_8t_data_in_eeprom_SPM(index))|(read_8t_data_in_eeprom_SPM(index+1) << 8)|(read_8t_data_in_eeprom_SPM(index+2) << 16)|(read_8t_data_in_eeprom_SPM(index + 3) << 24));
// }
