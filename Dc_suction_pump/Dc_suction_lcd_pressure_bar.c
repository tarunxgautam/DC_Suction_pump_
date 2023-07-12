#include "Dc_suction_declarations.h"

void lcd_pressure_bar_init(void)
{
	bar_draw_filled_rectangle (0,0,starting_bar_width,max_bar_height);
	//draw blank
}


// void lcd_pressure_bar_main(void)
// {
// //	calculating_bar_L_mmhg();
// // 	if (updated_bar_len_diff > 0)
// // 	{
// // 		draw_filled_rectangle(previous_x1, 0, (updated_bar_len_diff+previous_x1), max_bar_height);
// // 		previous_x1 += updated_bar_len_diff;															//to keep the value of the starting point of draw rectangle function
// // 	}
// // 	else if (updated_bar_len_diff < 0)
// // 	{
// // 		draw_delete_rectangle((previous_x1+updated_bar_len_diff), 0, previous_x1, max_bar_height);		//(previous_x1+difference_pixels) if difference_pixels is -ve the (previous_x1-difference_pixels) will make it +ve and insted of subtraction addition will happen.
// // 		previous_x1 += updated_bar_len_diff;															//to keep the value of the starting point of draw rectangle function
// // 	}
//
// 	difference_pressure = current_pressure_mmhg - previous_pressure_bar_mmhg;
// 	if ( (difference_pressure > (max_mmhg_value/max_bar_length)) || (difference_pressure < ((max_mmhg_value/max_bar_length)*(-1))) )
// 	{
// 		float pixel_per_pressure = (max_bar_length/max_mmhg_value);
// 		if (difference_pressure < 0)
// 		{
//
// 			updated_bar_len_diff = (uint8_t)((pixel_per_pressure * difference_pressure)*(-1.0));			//amount of pixel in length to be drawn or deleted from the bar
// 				USART1_sendString("updated");
// 				USART1_sendInt(updated_bar_len_diff);
//
// 			previous_pressure_bar_mmhg = current_pressure_mmhg;
// 			bar_draw_delete_rectangle((previous_x1-updated_bar_len_diff), 0, previous_x1, max_bar_height);
// 			previous_x1 -= updated_bar_len_diff;
// 		}
// 		else if (difference_pressure > 0)
// 		{
//
// 			updated_bar_len_diff = (uint8_t)((pixel_per_pressure * difference_pressure));			//amount of pixel in length to be drawn or deleted from the bar
// 				USART1_sendString("updated");
// 				USART1_sendInt(updated_bar_len_diff);
//
// 			previous_pressure_bar_mmhg = (current_pressure_mmhg - (current_pressure_mmhg % (max_mmhg_value/max_bar_length)));
// 			bar_draw_filled_rectangle(previous_x1, 0, (previous_x1+updated_bar_len_diff), max_bar_height);
// 			previous_x1 += updated_bar_len_diff;
// 		}
//
// // 		if (current_pressure_mmhg <= (max_mmhg_value/max_bar_length))
// // 		{
// // 			bar_draw_delete_rectangle(0, 0, 77, max_bar_height);
// //			previous_x1 = 3;
// //		}
// 	}
//
// 	USART1_sendString("x1");
// 	USART1_sendInt(previous_x1);
// }


void lcd_pressure_bar_main(void)
{
	static const float pixel_per_pressure = (max_bar_length/max_mmhg_value);
	static uint16_t previous_x2 = 3;
	uint16_t x2 = 0;
	x2 = (uint16_t)(current_pressure_mmhg * pixel_per_pressure);
	
	// 	if (current_pressure_mmhg > 9)
	//	{
	//Now we are filling the whole bar.
	if (x2 > previous_x2)
	{
		if (x2 > 74)					// for bar to not exceed the length 77 pixels. even if pressure is very high
		{
			#ifdef _DEBUG
			USART1_sendString("lcd_pressure_bar_main : pressure out of range");
			#endif
			x2 = 77;
		}
		bar_draw_filled_rectangle((uint8_t)(previous_x2+2), 2, (uint8_t)(x2+2), max_bar_height);
		previous_x2 = x2;
	}
	else if (x2 < previous_x2)
	{
		bar_draw_delete_rectangle((uint8_t)(x2+2), 2, (uint8_t)(previous_x2+2), max_bar_height);
		previous_x2 = x2;
	}
}

void calculating_bar_L_mmhg(void)		//to calculate the length of the need to be updated according to the unit mode selected i.e cmh2o, mpa, mmhg
{
	difference_pressure = current_pressure_mmhg - previous_pressure_bar_mmhg;
	updated_bar_len_diff = (int)((max_bar_length/max_mmhg_value) * difference_pressure);			//amount of pixel in length to be drawn or deleted from the bar
	previous_pressure_bar_mmhg = current_pressure_mmhg;
}


void bar_draw_delete_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
	uint8_t no_rows = y2-y1, no_col = (x2-x1);
	for (uint8_t i=0; i<no_rows; i++)
	{
		window_program_start_end_colm_row_addr(x1, (y1+i), x2, (y1+i));
		for (uint8_t j=0; j<no_col; j++)
		{
			UC1698U_SEND_BYTE_DATA(0x00);
			UC1698U_SEND_BYTE_DATA(0x00);
		}
	}
}

void bar_draw_filled_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
	uint8_t no_rows = y2-y1, no_col = (x2-x1);
	for (uint8_t i=0; i<no_rows; i++)
	{
		window_program_start_end_colm_row_addr(x1, (y1+i), x2, (y1+i));
		for (uint8_t j=0; j<no_col; j++)
		{
			UC1698U_SEND_BYTE_DATA(0xff);
			UC1698U_SEND_BYTE_DATA(0xff);
		}
	}
}
