Simple GPIO for 74HC595D project 

Use simple loop to display on 4 digits Led Display (74HC595D) via GPIO

---
# Back Pill (STM32F401 !!!)
https://www.newinnovations.nl/black-pill-stm32-stm32f411-stm32f411ceu6/
https://www.st.com/resource/en/datasheet/stm32f401ce.pdf
https://www.st.com/resource/en/reference_manual/dm00096844-stm32f401xbc-and-stm32f401xde-advanced-armbased-32bit-mcus-stmicroelectronics.pdf


![[_attachments/black-pill-f401-pinout.png]]



---
# 7-Segment Digital Tube Led Display on 74HC595D


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



---
# Temperature, Humidity and Pressure sensor BME280

GY-BME280/GY-BMP280
Humidity Atmospheric Pressure Sensor Module IIC I2C SPI Smart Home

 [BME280](https://www.aliexpress.com/item/32862421810.html?spm=a2g0o.order_detail.order_detail_item.5.abf1f19cmd0yx1)
 
- Digital interface type: IIC (slave mode 3.4MHz) or SPI (3-wire or 4-wire slave mode 10MHz)
- CSB: Chip select pin, connected to the high level (default) to the IIC communication interface, connected to the low level for the SPI communication interface