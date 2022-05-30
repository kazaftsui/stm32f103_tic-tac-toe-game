#include "xpt2046.h"
#include "lcdtp.h"
#include <stdio.h> 
#include <string.h>



#define READ_X 0xD0
#define READ_Y 0x90




#define XPT2046_SCALE_X 240
#define XPT2046_SCALE_Y 320



#define XPT2046_MIN_RAW_X 3400
#define XPT2046_MAX_RAW_X 29000
#define XPT2046_MIN_RAW_Y 3300
#define XPT2046_MAX_RAW_Y 30000

static void spi_write_byte(uint8_t data)
{
	for(size_t i = 0; i < 8; i++)
	{
		if (data & 0x80)
		{
			macXPT2046_MOSI_1();
		}
		else
		{
			macXPT2046_MOSI_0();
		}
		data = data << 1;
		macXPT2046_CLK_LOW();
		macXPT2046_CLK_HIGH();
	}
}

static uint8_t spi_read_byte(void)
{
	uint8_t i, ret , value;
	ret = 0;
	i = 8;

	do {
		i--;
		macXPT2046_CLK_LOW();
		value = macXPT2046_MISO();
		if (value)
		{
			//set the bit to 0 no matter what
			ret |= (1 << i);
		}

		macXPT2046_CLK_HIGH();
	} while (i > 0);

	return ret;
}



static void XPT2046_TouchSelect()
{
    macXPT2046_CS_DISABLE();
}

static void XPT2046_TouchUnselect()
{
    macXPT2046_CS_ENABLE();
}

int XPT2046_TouchPressed(void)
{
    return macXPT2046_EXTI_Read() == GPIO_PIN_RESET;
}

int XPT2046_TouchGetCoordinates(uint16_t* x, uint16_t* y)
{


	static const uint8_t cmd_read_x[] = { READ_X };
    static const uint8_t cmd_read_y[] = { READ_Y };
    static const uint8_t zeroes_tx[] = { 0x00, 0x00 };



    XPT2046_TouchSelect();

    uint32_t avg_x = 0;
    uint32_t avg_y = 0;
    uint8_t nsamples = 0;

    for(uint8_t i = 0; i < 16; i++)
    {
        if(!XPT2046_TouchPressed())
            break;

        nsamples++;

        uint8_t y_raw[2];
        uint8_t x_raw[2];



       spi_write_byte(READ_Y);

	   y_raw[0] = spi_read_byte();
	   y_raw[1] = spi_read_byte();

	   spi_write_byte(READ_X);

	   x_raw[0] = spi_read_byte();
	   x_raw[1] = spi_read_byte();






        avg_x += (((uint16_t)x_raw[0]) << 8) | ((uint16_t)x_raw[1]);
        avg_y += (((uint16_t)y_raw[0]) << 8) | ((uint16_t)y_raw[1]);
    }

    XPT2046_TouchUnselect();

    if(nsamples < 16)
        return 0;

    uint32_t raw_x = (avg_x / 16);
    if(raw_x < XPT2046_MIN_RAW_X) raw_x = XPT2046_MIN_RAW_X;
    if(raw_x > XPT2046_MAX_RAW_X) raw_x = XPT2046_MAX_RAW_X;

    uint32_t raw_y = (avg_y / 16);
    if(raw_y < XPT2046_MIN_RAW_Y) raw_y = XPT2046_MIN_RAW_Y;
    if(raw_y > XPT2046_MAX_RAW_Y) raw_y = XPT2046_MAX_RAW_Y;

    // Uncomment this line to calibrate touchscreen:
    //printf("raw_x = %d, raw_y = %d\r\n", (int) raw_x, (int) raw_y);

    *x = (raw_x - XPT2046_MIN_RAW_X) * XPT2046_SCALE_X / (XPT2046_MAX_RAW_X - XPT2046_MIN_RAW_X);
    *y = (raw_y - XPT2046_MIN_RAW_Y) * XPT2046_SCALE_Y / (XPT2046_MAX_RAW_Y - XPT2046_MIN_RAW_Y);

    return 1;
}
