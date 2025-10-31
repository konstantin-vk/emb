#ifndef TIM_PRINTF_H
#define TIM_PRINTF_H

//Debug Exception and Monitor Control Register base address
#define DEMCR        			*((volatile uint32_t*) 0xE000EDFCU )
/* ITM register addresses */
#define ITM_STIMULUS_PORT0   	*((volatile uint32_t*) 0xE0000000 )
#define ITM_TRACE_EN          	*((volatile uint32_t*) 0xE0000E00 )

//redefine : __attribute__((weak)) int _write(int file, char *ptr, int len)
int _write(int file, char *ptr, int len);

#endif
