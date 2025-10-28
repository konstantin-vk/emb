Simple GPIO for 74HC595D project 

Use simple loop to display on 4 digits Led Display (74HC595D) via GPIO

https://www.newinnovations.nl/black-pill-stm32-stm32f411-stm32f411ceu6/
https://www.st.com/resource/en/datasheet/stm32f401ce.pdf
https://www.st.com/resource/en/reference_manual/dm00096844-stm32f401xbc-and-stm32f401xde-advanced-armbased-32bit-mcus-stmicroelectronics.pdf


![[Pasted image 20251028165530.png]]






OUTPUT 
PA0  0x4002 0000   10    RightBottom 6
PA1                               11   RB7
PA2                               12   RB8
= AHB1 (100MHz)



PINS 
Led Display	74HC595D									STM32F401
LOAD		STCP       12  storage register clock input	PA0
SCLK		SHCP       11  shift register clock input	PA1
SDO			DS         14  serial data input			PA2
GND
VCC

