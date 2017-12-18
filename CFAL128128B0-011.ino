//===========================================================================
//
//  Code written for Seeeduino v4.2 runnung at 3.3v
//
//  CRYSTALFONTZ CFAL128128B0-011 128x128 Round OLED in SPI mode
//
//  ref: https://www.crystalfontz.com/product/cfal128128B0-011
//
//  2017 - 10 - 04 Brent A. Crosby
//===========================================================================
//This is free and unencumbered software released into the public domain.
//
//Anyone is free to copy, modify, publish, use, compile, sell, or
//distribute this software, either in source code form or as a compiled
//binary, for any purpose, commercial or non-commercial, and by any
//means.
//
//In jurisdictions that recognize copyright laws, the author or authors
//of this software dedicate any and all copyright interest in the
//software to the public domain. We make this dedication for the benefit
//of the public at large and to the detriment of our heirs and
//successors. We intend this dedication to be an overt act of
//relinquishment in perpetuity of all present and future rights to this
//software under copyright law.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
//OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//OTHER DEALINGS IN THE SOFTWARE.
//
//For more information, please refer to <http://unlicense.org/>
//===========================================================================
#define SSD1327_SET_COLUMN_ADDRESS_15       (0x15)
#define SSD1327_SET_ROW_ADDRESS_75          (0x75)
#define SSD1327_SET_CONTRAST_CURRENT_81     (0x81)
#define SSD1327_SET_REMAP_A0                (0xA0)
#define SSD1327_SET_DISPLAY_START_LINE_A1   (0xA1)
#define SSD1327_SET_OFFSET_A2               (0xA2)
#define SSD1327_SET_DISPLAY_MODE_NORMAL_A4  (0xA4)
#define SSD1327_SET_DISPLAY_MODE_ALL_ON_A5  (0xA5)
#define SSD1327_SET_DISPLAY_MODE_ALL_OFF_A6 (0xA6)
#define SSD1327_SET_DISPLAY_MODE_INVERT_A7  (0xA7)
#define SSD1327_SET_MULTIPLEX_RATIO_A8      (0xA8)
#define SSD1327_FUNCTION_SELECT_A_AB        (0xAB)
#define SSD1327_SET_DISPLAY_OFF_AE          (0xAE)
#define SSD1327_SET_DISPLAY_ON_AF           (0xAF)
#define SSD1327_SET_PHASE_LENGTH_B1         (0xB1)
#define SSD1327_SET_FRONT_CLOCK_B3          (0xB3)
#define SSD1327_SET_GPIO_B5                 (0xB5)
#define SSD1327_SET_SECOND_PRECHARGE_B6     (0xB6)
#define SSD1327_SET_GRAY_SCALE_TABLE_B8     (0xB8)
#define SSD1327_SET_LINEAR_GRAY_SCALE_B9    (0xB9)
#define SSD1327_SET_PRECHARGE_VOLTAGE_BC    (0xBC)
#define SSD1327_SET_VCOMH_VOLTAGE_BE        (0xBE)
#define SSD1327_FUNCTION_SELECTION_D5       (0xD5)
#define SSD1327_COMMAND_LOCK_FD             (0xFD)
#define SSD1327_HORIZONTAL_SCROLL_RIGHT_26  (0x26)
#define SSD1327_HORIZONTAL_SCROLL_LEFT_27   (0x27)
#define SSD1327_DEACTIVATE_SCROLL_2E        (0x2E)
#define SSD1327_ACTIVATE_SCROLL_2F          (0x2F)

#include <SPI.h>
// C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\SPI\src\SPI.cpp
// C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\SPI\src\SPI.h

#include <SD.h>
// C:\Program Files (x86)\Arduino\libraries\SD\src\SD.cpp
// C:\Program Files (x86)\Arduino\libraries\SD\src\SD.h
//============================================================================
/*
The CFAL128128B0-011 is a 3.3v device. You need a 3.3v Arduino to operate this
code properly. We used a seeedunio v4.2 set to 3.3v:
http://www.seeedstudio.com/item_detail.html?p_id=2517

Connections:
    ==================
    SD Card => Arduino
    ==================
        GND => GND
       MISO => Analog 12
        CLK => Analog 13
       MOSI => Analog 11
         CS => Analog 7
        3V3 => 3V3

  ====================================
  OLED => Connection
  ------------------------------------
   1=VSS:      GND
   2=VCC:      14.5v (bench supply)
   3=VCOMH:    1uF to GND
   4=VCI:      3.3v
   5=VDD:      3.3v
   6=BS1:      GND (select 4-wire SPI)
   7=BS2:      GND (select 4-wire SPI)
   8=VSS:      GND
   9=IREF:     1MΩ to GND
  10=CS_NOT:   Arduino #10
  11=RES_NOT:  Arduino #9
  12=D/C:      Arduino #8
  13=R/W:      nc
  14=E:        GND
  15=D0(SCK):  Arduino #13
  16=D1(MOSI): Arduino #11
  17=D2:       GND
  18=D3:       GND
  19=D4:       GND
  20=D5:       GND
  21=D6:       GND
  22=D7:       GND
  23=VCC:      14.5v (bench supply)
  24=VSS:      GND
  ====================================
*/
//============================================================================
// LCD SPI & control lines
//   ARD      | Port | LCD                          | wire
// -----------+------+------------------------------+--------
//  #6/D6     |  PD6 | TIME                         | 'scope
//  #7/D7     |  PD7 | SD_CS                        | Purple
//  #8/D8     |  PB0 | LCD_RS                       | Yellow
//  #9/D9     |  PB1 | LCD_RESET                    | Purple
// #10/D10    |  PB2 | LCD_CS_NOT   (or SPI SS)     | Grey
// #11/D11    |  PB3 | LCD_MOSI/DB7 (hardware SPI)  | Green
// #12/D12    |  PB4 | not used     (would be MISO) |
// #13/D13    |  PB5 | LCD_SCK/DB6  (hardware SPI)  | White
//
//Set the pins on the Atmel ATmga328P microcontroller on the Arduino/Seeduino
#define CLR_RS    (PORTB &= ~(0x01))
#define SET_RS    (PORTB |=  (0x01))
#define CLR_RESET (PORTB &= ~(0x02))
#define SET_RESET (PORTB |=  (0x02))
#define CLR_CS    (PORTB &= ~(0x04))
#define SET_CS    (PORTB |=  (0x04))
#define CLR_MOSI  (PORTB &= ~(0x08))
#define SET_MOSI  (PORTB |=  (0x08))
#define CLR_SCK   (PORTB &= ~(0x20))
#define SET_SCK   (PORTB |=  (0x20))

#define SD_CS (7)

#define TIME  (6)
#define CLR_TIME  (PORTD &= ~(0x40))
#define SET_TIME  (PORTD |=  (0x40))


//============================================================================
void sendCommand(uint8_t command)
{
  // Select the LCD's command register
  CLR_RS;
  // Select the LCD controller
  CLR_CS;
  //Send the command via SPI:
  SPI.transfer(command);
  // Deselect the LCD controller
  SET_CS;
}
//============================================================================
void sendData(uint8_t data)
  {
  // Select the LCD's data register
  SET_RS;
  // Select the LCD controller
  CLR_CS;
  //Send the data via SPI:
  SPI.transfer(data);
  // Deselect the LCD controller
  SET_CS;
  }
//============================================================================
//Updated code, 2017-03-27
#if (ADDR_MODE==2)
void setAddr(unsigned char page,unsigned char lCol,unsigned char hCol)
{
  sendCommand(page);//Set Page Start Address
  sendCommand(lCol);//Set Lower Column Start Address
  sendCommand(hCol);//Set Higher Column Start Address
}
#else
void setPageAddr(unsigned char startAddr,unsigned char endAddr)
{
  sendCommand(startAddr);
  sendCommand(endAddr);
}

void setColAddr(unsigned char startAddr,unsigned char endAddr)
{
  sendCommand(startAddr);
  sendCommand(endAddr);
}
#endif
//============================================================================
void Home_Addr(void)
{
  #if (ADDR_MODE<2)
    sendCommand(0x22);//Set Page Address
    setPageAddr(0,7);
    sendCommand(0x21);//Set Column Address
    setColAddr(0,127);
  #else
    setAddr(0xb0,0,0x10);
  #endif
}
//============================================================================
void initDisplay(void)
  {
  //Thump the reset.
  _delay_ms(1);
  CLR_RESET;
  _delay_ms(1);
  SET_RESET;
  _delay_ms(1);

  sendCommand(SSD1327_SET_COLUMN_ADDRESS_15);
    sendCommand(0x00);  //Start Column Address
    sendCommand(0x3F);  //End Column Address

  sendCommand(SSD1327_SET_ROW_ADDRESS_75);
    sendCommand(0x00);  //Start Row Address
    sendCommand(0x7F);  //End Row Address

  sendCommand(SSD1327_SET_CONTRAST_CURRENT_81);
    sendCommand(0x9b);  //Current/Brightness

  sendCommand(SSD1327_SET_REMAP_A0);
    sendCommand(0x51);  //Default Setting

  sendCommand(SSD1327_SET_DISPLAY_START_LINE_A1);
    sendCommand(0x00);

  sendCommand(SSD1327_SET_OFFSET_A2);
    sendCommand(0x00);

  sendCommand(SSD1327_SET_DISPLAY_MODE_NORMAL_A4);

  sendCommand(SSD1327_SET_MULTIPLEX_RATIO_A8);
    sendCommand(0x7F);  //Multiplex

  sendCommand(SSD1327_FUNCTION_SELECT_A_AB);
    sendCommand(0x01);

  sendCommand(SSD1327_SET_PHASE_LENGTH_B1);
    sendCommand(0xf1);

  sendCommand(SSD1327_SET_FRONT_CLOCK_B3);
    sendCommand(0x00);

  sendCommand(SSD1327_SET_PRECHARGE_VOLTAGE_BC);
    sendCommand(0x07);

  sendCommand(SSD1327_SET_VCOMH_VOLTAGE_BE);
    sendCommand(0x07);

  sendCommand(SSD1327_SET_SECOND_PRECHARGE_B6);
    sendCommand(0x0F);

  sendCommand(SSD1327_FUNCTION_SELECTION_D5);
    sendCommand(0x62);

  //Default linear Gray Scale Table
  sendCommand(SSD1327_SET_LINEAR_GRAY_SCALE_B9);
/*
  //Set Gray Scale Table
  sendCommand(SSD1327_SET_GRAY_SCALE_TABLE_B8);
    sendCommand( 1);  //A01[5:0]
    sendCommand( 2);  //A02[5:0]
    sendCommand( 4);  //A03[5:0]
    sendCommand( 7);  //A04[5:0]
    sendCommand(11);  //A05[5:0]
    sendCommand(15);  //A06[5:0]
    sendCommand(19);  //A07[5:0]
    sendCommand(23);  //A08[5:0]
    sendCommand(27);  //A09[5:0]
    sendCommand(31);  //A10[5:0]
    sendCommand(35);  //A11[5:0]
    sendCommand(39);  //A12[5:0]
    sendCommand(43);  //A13[5:0]
    sendCommand(47);  //A14[5:0]
    sendCommand(51);  //A15[5:0]
*/
  sendCommand(SSD1327_SET_DISPLAY_ON_AF);
  }

//============================================================================
//set all the pixels to black
void clean(void)
{
	unsigned char i, j;

	sendCommand(SSD1327_SET_COLUMN_ADDRESS_15);
	sendCommand(0x00);  //Start Column Address
	sendCommand(0x3F);  //End Column Address

	sendCommand(SSD1327_SET_ROW_ADDRESS_75);
	sendCommand(0x00);  //Start Row Address
	sendCommand(0x7F);  //End Row Address

	for (j = 0; j < 128; j++)
	{
		for (i = 0; i < (128 / 8); i++)
		{
			sendData(0x00);
			sendData(0x00);
			sendData(0x00);
			sendData(0x00);
		}
	}
}

//============================================================================
//create a checkerboard display across the screen
void checkerboard(void)
{
	unsigned char i, j;

	sendCommand(SSD1327_SET_COLUMN_ADDRESS_15);
	sendCommand(0x00);  //Start Column Address
	sendCommand(0x3F);  //End Column Address

	sendCommand(SSD1327_SET_ROW_ADDRESS_75);
	sendCommand(0x00);  //Start Row Address
	sendCommand(0x7F);  //End Row Address

	for (j = 0; j<128; j++)
	{
		for (i = 0; i<(128/2); i++)
		{
			if(j%2 == 0)
				//even rows (no remainder) will run through this function
				sendData(0x0F);
			else
				//odd rows will run through this inverted function
				sendData(0xF0);
		}
	}
	//Delay long enough to see the output
	delay(1000);
}

//============================================================================
//set the color, or in this case the grayscale.
void setColor(uint8_t color)
{
	unsigned char i, j;

	sendCommand(SSD1327_SET_COLUMN_ADDRESS_15);
	sendCommand(0x00);  //Start Column Address
	sendCommand(0x3F);  //End Column Address

	sendCommand(SSD1327_SET_ROW_ADDRESS_75);
	sendCommand(0x00);  //Start Row Address
	sendCommand(0x7F);  //End Row Address

	for (j = 0; j<128; j++)
	{
		for (i = 0; i<(128 / 8); i++)
		{
			sendData(color);
			sendData(color);
			sendData(color);
			sendData(color);
		}
	}
}



//============================================================================
//From: http://rosettacode.org/wiki/Bitmap/Bresenham's_line_algorithm#C
void OLED_Line(uint16_t x0, uint16_t y0,
	uint16_t x1, uint16_t y1,
	uint8_t g)
{
	int16_t
		dx;
	int16_t
		sx;
	int16_t
		dy;
	int16_t
		sy;
	int16_t
		err;
	int16_t
		e2;
	uint8_t
		p = 1;

	
	
		dx = abs((int16_t)x1 - (int16_t)x0);
		sx = x0 < x1 ? 1 : -1;
		dy = abs((int16_t)y1 - (int16_t)y0);
		sy = y0 < y1 ? 1 : -1;
		err = (dx > dy ? dx : -dy) / 2;
		for (;;)
		{
			Put_Pixel(x0, y0, p, g);
			if ((x0 == x1) && (y0 == y1))
				break;
			e2 = err;
			if (e2 > -dx)
			{
				err -= dy;
				x0 = (uint16_t)((int16_t)x0 + sx);
			}
			if (e2 < dy)
			{
				err += dx;
				y0 = (uint16_t)((int16_t)y0 + sy);
			}
		}
	
}

//============================================================================
//this function will place a square in an x,y (x and y) coordination based on a specified length/width (p)
//in a specified grayscale (g)
void Put_Pixel(uint8_t x, uint8_t y, uint8_t p, uint8_t g)
{
	//startCol is the starting point on the x-axis
	uint8_t startCol = x;
	//endCol is the ending popint on the x-axis
	uint8_t endCol = (x + p) - 1;
	//startRow is the starting point on the y-axis
	uint8_t startRow = y * 2;
	//endRow is the ending point on the y-axis
	uint8_t endRow = (y + p) * 2 - 1;
	//dr is the differential used for the loop to populate the rows
	uint8_t dr = endRow - startRow + 2;
	//dc is the differential used for the inner loop which populates columns
	uint8_t dc = endCol - startCol + 1;
	
	sendCommand(SSD1327_SET_COLUMN_ADDRESS_15);
	sendCommand(startCol);  //Start Column Address
	sendCommand(endCol);  //End Column Address

	sendCommand(SSD1327_SET_ROW_ADDRESS_75);
	sendCommand(startRow);  //Start Row Address
	sendCommand(endRow);  //End Row Address

	int i = 0;
	int j = 0;

	//looping - outerloop is for rows inner loop is for columns
	for (i = 0; i < dr; i++) {
		for (j = 0; j < dc; j++) {
			sendData(g);
		}
	}
}

//example function that creates a square in the center of the screen and expands outward
void expandingSquare() {
	uint8_t g;
	uint8_t x;
	clean();
	
	//loop through the example a few times
	for (int i = 0; i < 5; i++) {
		//g is to set grayscale, x is for controlling the loop. 
		g = 0x00;
		x = 0;
		//Since Put_pixel set to 1 pixel (3rd parameter) is 2x2 pixels
		//this loop will loop 32 which will bring the edge of the square
		//to the extents of the screen
		while (x < 33) {
			//first loop expands the top of the square by two pixels
			for (int i = 0; i < 2 * x; i++) {
				Put_Pixel(31 - x + i, 31 - x, 1, g);
			}
			//second loop expands the right side of the square by two pixels
			for (int i = 0; i < 2 * x; i++) {
				Put_Pixel(31 + x, 31 - x + i, 1, g);
			}
			//third loop expands the bottom of the square by two pixels
			for (int i = 0; i < 2 * x + 1; i++) {
				Put_Pixel(31 - x + i, 31 + x, 1, g);
			}
			//fourth loop xpands the left side of the square by one size
			for (int i = 0; i < 2 * x; i++) {
				Put_Pixel(31 - x, 31 - x + i, 1, g);
			}
			x++;
		}
		//the following loop does the same as the above but with a different grayscale
		g = 0xFF;
		x = 0;
		while (x < 32) {
			for (int i = 0; i < 2 * x; i++) {
				Put_Pixel(31 - x + i, 31 - x, 1, g);
			}
			for (int i = 0; i < 2 * x; i++) {
				Put_Pixel(31 + x, 31 - x + i, 1, g);
			}
			for (int i = 0; i < 2 * x + 1; i++) {
				Put_Pixel(31 - x + i, 31 + x, 1, g);
			}
			for (int i = 0; i < 2 * x; i++) {
				Put_Pixel(31 - x, 31 - x + i, 1, g);
			}
			x++;
		}
	}
}

//============================================================================
//This demo spans lines across the screen starting at a fixed point in the top left corner
//and the lines pivot counterclockwise to the starting point
//
//once the screen is completely filled with the lines, the starting point will change
//a total of six times
void spanningLinesDemo() {
	uint8_t g = 0x01;
	uint8_t x;
	uint8_t y;
	clean();
	x = 0;
	y = 64;
	
	//start of span is (0,0)
	//the first loop changes the x-axis of one end of the line drawn by the function OLED_Line()
	//The end point of the line spans from one edge of the screen to the other. 
	while( x < 64)
	{
		OLED_Line(0, 0, x, 63, g);
		x++;
	}
	//the second loop does the same but it spans along the y-axis instead of the x-axis
	while(y != 0)
	{
		y--;
		OLED_Line(0, 0, 63, (y), g);
	}
	g = !g;
		
	//start point of span is (31,0);
	while (y != 64)
	{
		OLED_Line(31, 0, 0, y, g);
		y++;
	}
	x = 0;
	while (x < 64)
	{

		OLED_Line(31, 0, x, 63, g);
		x++;
	}
	while (y != 0)
	{
		y--;
		OLED_Line(31, 0, 63, (y), g);
	}
	g = !g;

	//start point of span is (63,0)
	while (y != 64)
	{
		OLED_Line(63, 0, 0, y, g);
		y++;
	}
	x = 0;
	while (x < 64)
	{

		OLED_Line(63, 0, x, 63, g);
		x++;
	}
	g = !g;

	
	//start point of span is (63,63)
	while (x != 0)
	{
		x--;
		OLED_Line(63, 63, x, 0, g);
	}
	y = 0;
	while (y < 64)
	{
		OLED_Line(63, 63, 0, y, g);
		y++;
	}
	//delay(5000);
	g = !g;


	//start point of span is (31,63);
	while (y != 0)
	{
		y--;
		OLED_Line(31, 63, 63, y, g);
	}
	x = 64;
	while (x != 0)
	{
		x--;
		OLED_Line(31, 63, x, 0, g);
	}
	while (y < 64)
	{
		OLED_Line(31, 63, 0, y, g);
		y++;
	}
	g = !g;


	//start point of span is (0,63)
	while (y != 0)
	{
		y--;
		OLED_Line(0, 63, 63, y, g);
	}
	x = 64;
	while (x != 0)
	{
		x--;
		OLED_Line(0, 63, x, 0, g);
			
	}
}



//============================================================================
// This function transfers data, in one stream. Slightly
// optimized to do index operations during SPI transfers.
// ~161uS
//
// Instead of storing a standard format BMP file on the uSD card, we could store
// a byte-for-byte image of the display memory. The BMP file is 49K, a display
// memory image would be 128/2 * 128 = 8192 bytes, and so would display much
// more quickly, perhaps ~6x faster.
void SPI_send_pixels(uint8_t pixel_count, uint8_t *data_ptr)
{
	uint8_t
		r;
	uint8_t
		g;
	uint8_t
		b;
	register uint8_t
		k;

	uint8_t
		oled_byte;

	// Select the LCD's data register
	SET_RS;
	// Select the LCD controller
	CLR_CS;

	//  SPI_sendCommand(LD7138_0x0C_DATA_WRITE_READ);
	while (pixel_count)
	{
		//Load the first pixel. BMPs BGR format
		b = *data_ptr;
		data_ptr++;
		g = *data_ptr;
		data_ptr++;
		r = *data_ptr;
		data_ptr++;

		// A grayscale conversion that is aware of the actual display
		// grey levels (which depend on the gamma table) could be
		// implemented here. The current code assumes linear, which
		// is not a bad approximation, and is also not accurate.
		//
		// Using only high 4 bits of green as an approximation
		// reduces execution time from 161uS to 39uS.
		// Spelling the shifts out gives to 117uS.
		//
		//Convert this pixel to grayscale.
		//Ideal conversion:  0.30 red, 0.59 green 0.11 blue
		//We will use: 0.25*red, 0.625*green, 0.125*blue
		//  k=(r>>2) +
		//    (g>>1) + (g>>3) +
		//    (b>>3);
		//Stuff that into the high nibble      
		//  oled_byte=k&0xF0;
		//Spelling it out for the compiler speeds it up some.
		r >>= 2;
		k = r;
		g >>= 1;
		k += g;
		g >>= 2;
		k += g;
		b >>= 3;
		k += b;
		k &= 0xF0;
		oled_byte = k;
		//Done with this pixel
		pixel_count--;

		//Load the second pixel. BMPs BGR format
		b = *data_ptr;
		data_ptr++;
		g = *data_ptr;
		data_ptr++;
		r = *data_ptr;
		data_ptr++;

		//Convert this pixel to grayscale.
		//Ideal conversion:  0.30 red, 0.59 green 0.11 blue
		//We will use: 0.25*red, 0.625*green, 0.125*blue
		//  k=(r>>2) +
		//    (g>>1) + (g>>3) +
		//    (b>>3);
		//Stuff that into the low nibble      
		//  oled_byte|=(k>>4);
		//Spelling it out for the compiler speeds it up some.
		r >>= 2;
		k = r;
		g >>= 1;
		k += g;
		g >>= 2;
		k += g;
		b >>= 3;
		k += b;
		k >>= 4;
		oled_byte |= k;

		//Done with this pixel
		pixel_count--;

		//Write it to the OLED
		//Slower:
		SPI.transfer(oled_byte); //222uS

		//Put the byte in the outout SPI shift register.
		//SPDR = oled_byte; //161uS
		//There is so much calculation that the spi transfer is
		//done well before we get back here.
	}
	//Wait for the last byte to finish, and clear the flags.
	//while (!(SPSR & _BV(SPIF))) ; // wait  
	//Needs some non-trivial delay here. Not sure why.
	//delayMicroseconds(2);

	// Deselect the LCD controller
	SET_CS;
}

//============================================================================
// BMP should be normal 24-bit RGB windows bitmap.
// Converted to 4-bit grey on the fly.
// Weighted conversion:  0.30 red, 0.59 green 0.11 blue
// Close, and easier: /4 red, /2 green /4 blue
void show_BMPs_in_root(void)
{
	File
		root_dir;
	root_dir = SD.open("/");
	if (0 == root_dir)
	{
		Serial.println("show_BMPs_in_root: Can't open \"root\"");
		return;
	}
	File
		bmp_file;

	while (1)
	{
		bmp_file = root_dir.openNextFile();
		if (0 == bmp_file)
		{
			// no more files, break out of while()
			// root_dir will be closed below.
			break;
		}
		//Skip directories
		if (0 == bmp_file.isDirectory())
		{
			//The file name must include ".BMP"
			if (0 != strstr(bmp_file.name(), ".BMP"))
			{
				//The BMP must be exactly 147,624 long
				//(this is correct for 128x128 24-bit)
				if (49208 == bmp_file.size())
				{
					//This core bit of displaying a bitmap takes
					//~242mS         
					//Jump over BMP header. BMP must be 128x128 24-bit
					bmp_file.seek(54);

					//Since we are limited in memory, break the line up from
					// 128*3 = 384 bytes into two chunks of 64 pixels
					// each 64*3 = 192 bytes.
					//Making this static speeds it up slightly (10ms)
					//Reduces flash size by 114 bytes, and uses 192 bytes.
					static uint8_t
						half_of_a_line[64 * 3];
					for (uint8_t line = 0; line < 128; line++)
					{
						//Set the OLED to the left of this line
						//Set_OLED_for_write_at_X_Y(0,35-line);
						sendCommand(SSD1327_SET_COLUMN_ADDRESS_15);
						sendCommand(0x00);  //Start Column Address
						sendCommand(0x3F);  //End Column Address

						sendCommand(SSD1327_SET_ROW_ADDRESS_75);
						sendCommand(0x7F - line);  //Start Row Address
						sendCommand(0x80);  //End Row Address


						for (uint8_t line_section = 0; line_section < 2; line_section++)
						{
							//Get half of the line:
							// ~136uS for buffer hit
							// ~1.8mS for a miss
							bmp_file.read(half_of_a_line, 64 * 3);
							//Now write this half to the TFT, doing the BGR -> 565
							//color fixup interlaced with the SPI transfers.
							SPI_send_pixels(64, half_of_a_line);
						}
					}
				}
			}
		}
		//Release the BMP file handle
		bmp_file.close();
		delay(2000);
	}
	//Release the root directory file handle
	root_dir.close();
}
//============================================================================
bool SDIsOpen = false;
void setup(void)
{
	// LCD SPI & control lines
	//   ARD      | Port | LCD                          | wire
	// -----------+------+------------------------------+--------
	//  #7/D7     |  PD7 | SD_CS                        | Purple
	//  #8/D8     |  PB0 | LCD_RS                       | Yellow
	//  #9/D9     |  PB1 | LCD_RESET                    | Purple
	// #10/D10    |  PB2 | LCD_CS_NOT   (or SPI SS)     | Grey
	// #11/D11    |  PB3 | LCD_MOSI/DB7 (hardware SPI)  | Green
	// #12/D12    |  PB4 | not used     (would be MISO) |
	// #13/D13    |  PB5 | LCD_SCK/DB6  (hardware SPI)  | White

	DDRB |= 0x2F;

	//Pin 7 is used for the SD card CS.
	pinMode(SD_CS, OUTPUT);

	//Pin 7 is used for timing
	pinMode(TIME, OUTPUT);

	//Drive the ports to a reasonable starting state.
	CLR_RESET;
	CLR_RS;
	SET_CS;
	CLR_MOSI;
	CLR_SCK;

	//debug console
	Serial.begin(9600);
	Serial.println("setup()");

	// Initialize SPI. By default the clock is 4MHz.
	SPI.begin();
	//Bump the clock to 8MHz. Appears to be the maximum.
	SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));

	//Fire up the I2C OLED
	Serial.println("initDisplay()");
	initDisplay();
	_delay_ms(500);
	clean();

	// For the Seeduino I am using, the default speed of SPI_HALF_SPEED
	// set in C:\Program Files (x86)\Arduino\libraries\SD\src\SD.cpp
	// results in a 4MHz clock.
	//
	// If you change this function call in SDClass::begin() of SD.cpp
	// from:
	//
	//  return card.init(SPI_HALF_SPEED, csPin) &&
	//         volume.init(card) &&
	//         root.openRoot(volume);
	//
	// to:
	//
	//  return card.init(SPI_FULL_SPEED, csPin) &&
	//         volume.init(card) &&
	//         root.openRoot(volume);
	//
	// That appears to make the SD library talk at 8MHz.
	//
	if (!SD.begin(SD_CS))
	{
		Serial.println("SD Card failed to initialize, or not present");
		//Reset the SPI clock to fast. SD card library does not clean up well.
		//Bump the clock to 8MHz. Appears to be the maximum.
		SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
	}
	else
	{
		Serial.println("SD Card initialized.");
		SDIsOpen = true;
	}

}
//============================================================================
uint8_t graycolor = 0x00;
void loop(void)
{
	//All the demo functions are called out in the loop function
	spanningLinesDemo();
	checkerboard();
	expandingSquare();	
	uint8_t gScale = 0x00;
	//loop through the setColor function changing grayscale from pixels off -> full on
	for (int i = 0; i < 15; i++) {
		setColor(gScale);
		gScale += 0x11;
		delay(50);
	}
	//loop through the setColor function changing grayscale from pixels full on -> off
	for (int i = 0; i < 15; i++) {
		setColor(gScale);
		gScale -= 0x11;
		delay(50);
	}
	//check to see if the SD card was initialized and if it was, show pictures loaded on the SD card
	if(SDIsOpen) show_BMPs_in_root();	
}
//============================================================================

