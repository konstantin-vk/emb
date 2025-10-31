/*
 * FourDigitsLedDisplay.c
 *
 *  Created on: Oct 29, 2025
 *      Author: konstantin
 */


#include <stdint.h>
#include <stdlib.h>
#include "FourDigitsLedDisplay.h"


/*
 * **************************
 * Global static = Singleton
 * address for output register and port numbers
 * **************************
 */
static GPIOToLedRegisterDefinition_t *g_gpio_output_register;


// supress flickering by supressing update scrin if displayed value hasnt changed
static uint8_t displayed_bitfield[4];


/*
 * *********************************
 *
 * Character map
 * converts ASCII to Led Display Bitmap
 * TODO -> Private
 * *********************************
 */

const uint8_t char_map_base = 48; 						// ASCII number for 0 (Zero)
const uint8_t char_map_bitfield_err    = 0b11111111UL ; 	// indicate error as all leds are highlited
const uint8_t char_map_bitfield_clear  = 0b00000000UL ;
const uint8_t char_map_bitfield_bright = 0b01111111UL ;
const uint8_t char_map_dotmask = 0b10000000UL ;

// Convert Numbers to Led Display pin signal level
const uint8_t char_map[10] =
{
	/////////////////////
	// PINS :
	//   __1__
	//  6| 7 |2
	//   |---|
	//  5|___|3   . 8
	//     4
	/////////////////////
    // 87654321 = pins
    0b00111111UL,         /*0*/
    0b00000110UL,         /*1*/
    0b01011011UL,         /*2*/
    0b01001111UL,         /*3*/
    0b01100110UL,         /*4*/
    0b01101101UL,         /*5*/
    0b01111101UL,         /*6*/
    0b00000111UL,         /*7*/
    0b01111111UL,         /*8*/
    0b01101111UL          /*9*/
} ;



/*
 * ***********************************
 * Private bitmap
 * ***********************************
 */
uint8_t char_map_translate_ascii___(uint8_t numeric_ascii)
{
	if ( (numeric_ascii < char_map_base) || (numeric_ascii > char_map_base+11) ) {return char_map_bitfield_err;}
	else
	{
		uint8_t tmp = char_map[numeric_ascii - char_map_base];
		return tmp;
	}
}

uint8_t char_map_translate_uint___(uint8_t num)
{
	if ( (num < 0) || (num > 0+9) ) {return char_map_bitfield_err;}
	else
	{
		uint8_t tmp = char_map[num];
		return tmp;
	}
}


/*
 * ****************************
 * Init
 * ***************************
 */

GPIOToLedRegisterDefinition_t *gpiotoled_init (void)
{
    // do lock here
    if(g_gpio_output_register == NULL)
    {
    	g_gpio_output_register = malloc(sizeof(*g_gpio_output_register));
    }
	return g_gpio_output_register;
}

void gpiotoled_deinit (void)
{
    // do lock here
    if(g_gpio_output_register != NULL)
    {
    	free(g_gpio_output_register);
    }
}


/*
 * ************************************
 * Private internal
 * ***********************************
 */
void push_data_pin___(uint8_t led_on_off)
{
	if      (led_on_off==LEDOFF){ *(g_gpio_output_register->pGPIOABSRR) |= (0b1 << (16 + g_gpio_output_register->output_pin_data));} // reset (anode inverted)
	else if (led_on_off==LEDON ){ *(g_gpio_output_register->pGPIOABSRR) |= (0b1 << (0  + g_gpio_output_register->output_pin_data));} // set   (anode inverted)
}
void push_shift_clock_pin___(uint8_t bit_high_low)
{
	if      (bit_high_low==LOW ){ *(g_gpio_output_register->pGPIOABSRR) |= (0b1 << (16 + g_gpio_output_register->output_pin_shift_clock));} // reset (anode inverted)
	else if (bit_high_low==HIGH){ *(g_gpio_output_register->pGPIOABSRR) |= (0b1 << (0  + g_gpio_output_register->output_pin_shift_clock));} // set   (anode inverted)
}
void push_storage_clock_pin___(uint8_t bit_high_low)
{
	if      (bit_high_low==LOW ){ *(g_gpio_output_register->pGPIOABSRR) |= (0b1 << (16 + g_gpio_output_register->output_pin_storage_clock));} // reset (anode inverted)
	else if (bit_high_low==HIGH){ *(g_gpio_output_register->pGPIOABSRR) |= (0b1 << (0  + g_gpio_output_register->output_pin_storage_clock));} // set   (anode inverted)
}


void push_section_bitmap___(uint8_t bits )
{
	uint8_t bit ;

	push_storage_clock_pin___(LOW);
	for (int i=0;i<8;i++)
	{
		bit = 0;
		//bit = !!((bits >> i)&0b1);
		bit = !!((bits << i)&0b10000000UL); // starts with MSB
		push_shift_clock_pin___(LOW);
		push_data_pin___(bit);
		push_shift_clock_pin___(HIGH);
	}
	push_storage_clock_pin___(HIGH);
}


// handle section invertion
void push_four_section_bitmap_raw___(uint8_t bits_a , uint8_t bits_b ,uint8_t bits_c ,uint8_t bits_d)
{
		//update screen
		push_section_bitmap___(bits_d);
		push_section_bitmap___(bits_c);
		push_section_bitmap___(bits_b);
		push_section_bitmap___(bits_a);
}


//handle flickering
void push_four_section_bitmap___(uint8_t bits_a , uint8_t bits_b ,uint8_t bits_c ,uint8_t bits_d)
{
	//blast string
	// Attention = inversion !!!!!!!!!!!!!!
	// suppress flickering
	if ( displayed_bitfield[0]!=bits_a ||
			displayed_bitfield[1]!=bits_b ||
			displayed_bitfield[2]!=bits_c ||
			displayed_bitfield[3]!=bits_d )
	{
		//save
		displayed_bitfield[0] = bits_a ;
		displayed_bitfield[1] = bits_b ;
		displayed_bitfield[2] = bits_c ;
		displayed_bitfield[3] = bits_d ;
		//update screen
		push_four_section_bitmap_raw___(char_map_bitfield_clear, char_map_bitfield_clear, char_map_bitfield_clear, char_map_bitfield_clear);
		push_four_section_bitmap_raw___(bits_a, bits_b, bits_c, bits_d);

	}
}

void push_four_section_clear___(void)
{
	push_four_section_bitmap___(char_map_bitfield_clear, char_map_bitfield_clear, char_map_bitfield_clear, char_map_bitfield_clear);
}

void push_four_section_bright___(void)
{
	push_four_section_bitmap___(char_map_bitfield_bright, char_map_bitfield_bright, char_map_bitfield_bright, char_map_bitfield_bright);
}


/*
 * ****************
 * Private interface
 * ****************
 */


void blast_bitmap___(uint8_t bitmask_a , uint8_t bitmask_b ,uint8_t bitmask_c ,uint8_t bitmask_d)
{
	// clear
	//push_four_section_bitmap___(char_map_bitfield_clear, char_map_bitfield_clear, char_map_bitfield_clear, char_map_bitfield_clear);
	push_four_section_clear___();
	// blast
	push_four_section_bitmap___(bitmask_a, bitmask_b, bitmask_c, bitmask_d);
}


void blast_ascii___(uint8_t ascii_a , uint8_t ascii_b ,uint8_t ascii_c ,uint8_t ascii_d)
{
	uint8_t bits_a = char_map_translate_ascii___(ascii_a);
	uint8_t bits_b = char_map_translate_ascii___(ascii_b);
	uint8_t bits_c = char_map_translate_ascii___(ascii_c);
	uint8_t bits_d = char_map_translate_ascii___(ascii_d);
	push_four_section_bitmap___(bits_a, bits_b, bits_c, bits_d);
}

void blast_uint___(uint8_t uint_a , uint8_t uint_b ,uint8_t uint_c ,uint8_t uint_d)
{
	uint8_t bits_a = char_map_translate_uint___(uint_a);
	uint8_t bits_b = char_map_translate_uint___(uint_b);
	uint8_t bits_c = char_map_translate_uint___(uint_c);
	uint8_t bits_d = char_map_translate_uint___(uint_d);
	push_four_section_bitmap___(bits_a, bits_b, bits_c, bits_d);
}

void blast_time___(uint8_t uint_a , uint8_t uint_b ,uint8_t uint_c ,uint8_t uint_d)
{
	uint8_t bits_a = char_map_translate_uint___(uint_a);
	uint8_t bits_b = char_map_translate_uint___(uint_b);
	bits_b |= char_map_dotmask;
	uint8_t bits_c = char_map_translate_uint___(uint_c);
	uint8_t bits_d = char_map_translate_uint___(uint_d);
	push_four_section_bitmap___(bits_a, bits_b, bits_c, bits_d);
}






/*
 * ************************************
 * Public
 * ************************************
 */

void gpiotoled_clear(void)
{
//	for (int i=0; i<4;i++)  // 4 segments
//	{
//		push_to_storage_clock_pin___(LOW);
//		for (int j=0; j<8;j++){
//			push_to_shift_clock_pin___(LOW);
//			push_to_data_pin___(LEDOFF);
//			push_to_shift_clock_pin___(HIGH);
//		}
//		push_to_storage_clock_pin___(HIGH);
//	}
	push_four_section_clear___();
}


void gpiotoled_bright(void)
{
//	for (int i=0; i<4;i++)  // 4 segments
//	{
//		push_to_storage_clock_pin___(LOW);
//		for (int j=0; j<8;j++){
//			push_to_shift_clock_pin___(LOW);
//			push_to_data_pin___(LEDON);
//			push_to_shift_clock_pin___(HIGH);
//		}
//		push_to_storage_clock_pin___(HIGH);
//	}
	push_four_section_bright___();
}


void gpiotoled_blast_error(void)
{
	//gpiotoled_bright();
	push_four_section_bright___();
}


void gpiotoled_blast_uint(uint32_t num)
{
    int dummy = num;
    uint8_t a,b,c,d;
    //uint8_t arr[4];
    if (dummy>9999 || dummy<0)
    {
    	gpiotoled_blast_error();
    	return;
    }
    else
    {
        if (dummy>999)			{a = dummy/1000;dummy=dummy-(dummy/1000)*1000;}
        else a = char_map_bitfield_clear; //TODO !!!! error - nomask but number

        if (dummy>99 || num>=99)	{ b = dummy / 100; dummy=dummy-(dummy/100)*100;}
        else b = char_map_bitfield_clear; //TODO !!!! error - nomask but number

        if (dummy>9 || num>=9)	{ c= dummy/10;dummy=dummy-(dummy/10)*10;}
        else c = char_map_bitfield_clear; //TODO !!!! error - nomask but number

        if (dummy>=0)			{ d = dummy;}
        else d = char_map_bitfield_clear; //TODO !!!! error - nomask but number
    };

	//gpiotoled_clear();
	blast_uint___(a,b,c,d);


}





void gpiotoled_blast_time(uint8_t aa, uint8_t bb)
{
    int dummy;
    uint8_t a,b,c,d;
    if (aa>99 || aa<0 || bb>99 || bb<0)
    {
    	gpiotoled_blast_error();
    	return;
    }
    else
    {
    	dummy = aa;
        if (dummy>9 || aa>=9)	{ a = dummy/10;dummy=dummy-(dummy/10)*10;}
        else a = 0;
        if (dummy>=0)			{ b = dummy;}
        else b = 0;

    	dummy = bb;
        if (dummy>9 || bb>=9)	{ c = dummy/10;dummy=dummy-(dummy/10)*10;}
        else c = 0;
        if (dummy>=0)			{ d = dummy;}
        else d = 0;

    };

	blast_time___(a,b,c,d);

}
