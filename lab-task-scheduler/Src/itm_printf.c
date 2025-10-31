#include "itm_printf.h"
#include <stdio.h>
#include <stdint.h>


void ITM_SendChar(uint8_t ch)
{
	//Enable TRCENA
	DEMCR |= ( 1 << 24);
	//enable stimulus port 0
	ITM_TRACE_EN |= ( 1 << 0);
	// read FIFO status in bit [0]:
	while(!(ITM_STIMULUS_PORT0 & 1));
	//Write to ITM stimulus port0
	ITM_STIMULUS_PORT0 = ch;
}


//__attribute__((weak)) int _write(int file, char *ptr, int len)
int _write(int file, char *ptr, int len)
{
  (void)file;
  int DataIdx;

  for (DataIdx = 0; DataIdx < len; DataIdx++)
  {
	  /*CORTEX MS COURSE*/
    //__io_putchar(*ptr++);
    ITM_SendChar(*ptr++);
  }
  return len;
}

