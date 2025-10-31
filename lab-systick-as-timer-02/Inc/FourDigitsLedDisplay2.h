/*
 * FourDigitsLedDisplay.h
 *
 *  Created on: Oct 29, 2025
 *      Author: konstantin
 *
 *
 *  version 2
 *  	enable refresh by using callback function
 *  	for irq implementation
 */

#ifndef FOURDIGITSLEDDISPLAY2_H_
#define FOURDIGITSLEDDISPLAY2_H_

#include <stdint.h>



#define LEDON   0
#define LEDOFF  1

#define LOW  0
#define HIGH 1

/*
 * *********************************
 * Hardware
 * GPIO output to Led Display
 * 	GPIO must be correctly set for output
 * 	specify address for GPIOx_BSRR (set\reset register) to be able to write to output
 * 	needs numbers for 3 pins to drive Led with
 * 		Data
 * 		Shift clock
 * 		Storage clock
 * 	all pins are to be placed on the same GPIO port
 */

typedef struct
{
	uint32_t *pGPIOABSRR;
	uint8_t   output_pin_data;
	uint8_t   output_pin_shift_clock;
	uint8_t   output_pin_storage_clock;
} GPIOToLedRegisterDefinition_t;
//static GPIOToLedRegisterDefinition_t  GPIO_OutputRegister;





/*
 * *********************************
 *
 * Character map
 * converts ASCII to Led Display Bitmap
 * TODO -> Private
 * *********************************
 */

extern const uint8_t char_map_base ;
extern const uint8_t char_map_bitfield_err ; // indicate error as all leds are highlited
extern const uint8_t char_map[10] ;

/*
 * ********************************
 * Init
 * get Led output register as singleton
 * ********************************
 */
GPIOToLedRegisterDefinition_t *gpiotoled2_init  (void);
void gpiotoled2_deinit (void);




/*
 * *******************
 * Public
 * Write Led Display methods
 * *******************
 */
void gpiotoled2_set_uint(uint32_t n);
void gpiotoled2_set_time(uint8_t aa, uint8_t bb);
void gpiotoled2_set_millis(uint32_t millis);
void gpiotoled2_update_screen(void);





#endif /* FOURDIGITSLEDDISPLAY2_H_ */
