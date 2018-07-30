//#define DEBUG


#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "nokia5110.h"

const unsigned char ascii [] PROGMEM= {
0x00, 0x00, 0x80, 0x40, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x40, 0x00,
0x00, 0x00, 0x40, 0x30, 0x08, 0x06, 0x01, 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00, 0x40, 0x42,
0x7F, 0x40, 0x40, 0x00, 0x62, 0x51, 0x51, 0x51, 0x4E, 0x00, 0x22, 0x41, 0x49, 0x49, 0x3E, 0x00,
0x18, 0x14, 0x12, 0x7F, 0x10, 0x00, 0x4F, 0x49, 0x49, 0x49, 0x31, 0x00, 0x3E, 0x49, 0x49, 0x49,
0x32, 0x00, 0x01, 0x01, 0x71, 0x09, 0x07, 0x00, 0x36, 0x49, 0x49, 0x49, 0x36, 0x00, 0x26, 0x49,
0x49, 0x49, 0x3E, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x00, 0x80, 0x68, 0x00, 0x00, 0x00,
0x00, 0x08, 0x14, 0x22, 0x00, 0x00, 0x00, 0x14, 0x14, 0x14, 0x00, 0x00, 0x00, 0x22, 0x14, 0x08,
0x00, 0x02, 0x01, 0xB1, 0x09, 0x06, 0x00, 0x3E, 0x4D, 0x53, 0x55, 0x2E, 0x00, 0x78, 0x16, 0x11,
0x16, 0x78, 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36, 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22, 0x00, 0x7F,
0x41, 0x41, 0x41, 0x3E, 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41, 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01,
0x00, 0x3E, 0x41, 0x41, 0x49, 0x3A, 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00, 0x41, 0x41, 0x7F,
0x41, 0x41, 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01, 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41, 0x00, 0x7F,
0x40, 0x40, 0x40, 0x40, 0x00, 0x7F, 0x02, 0x04, 0x02, 0x7F, 0x00, 0x7F, 0x06, 0x08, 0x30, 0x7F,
0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06, 0x00, 0x3E, 0x41, 0x41,
0x61, 0x5E, 0x00, 0x7F, 0x09, 0x09, 0x19, 0x66, 0x00, 0x26, 0x49, 0x49, 0x49, 0x32, 0x00, 0x01,
0x01, 0x7F, 0x01, 0x01, 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00, 0x07, 0x18, 0x60, 0x18, 0x07,
0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F, 0x00, 0x63, 0x14, 0x08, 0x14, 0x63, 0x00, 0x01, 0x02, 0x7C,
0x02, 0x01, 0x00, 0x61, 0x51, 0x49, 0x45, 0x43, 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00, 0x00, 0x01,
0x02, 0x0C, 0x18, 0x60, 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00, 0x00, 0x04, 0x02, 0x01, 0x02, 0x04,
0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x01, 0x02, 0x04, 0x00, 0x00, 0x38, 0x44, 0x44,
0x24, 0x78, 0x00, 0x7F, 0x48, 0x48, 0x30, 0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x28, 0x00, 0x00,
0x30, 0x48, 0x48, 0x7F, 0x00, 0x3C, 0x4A, 0x4A, 0x4A, 0x2C, 0x00, 0x00, 0x08, 0x7F, 0x09, 0x02,
0x00, 0x00, 0x5C, 0x94, 0x9C, 0x78, 0x00, 0x00, 0x7F, 0x08, 0x70, 0x00, 0x00, 0x00, 0x00, 0x7A,
0x00, 0x00, 0x00, 0x40, 0x80, 0x80, 0x74, 0x00, 0x00, 0x7F, 0x10, 0x68, 0x00, 0x00, 0x00, 0x00,
0x00, 0x7F, 0x00, 0x00, 0x00, 0x78, 0x10, 0x20, 0x10, 0x60, 0x00, 0x00, 0x78, 0x10, 0x10, 0x60,
0x00, 0x30, 0x48, 0x48, 0x30, 0x00, 0x00, 0xF8, 0x28, 0x38, 0x00, 0x00, 0x00, 0x38, 0x28, 0xF8,
0x08, 0x00, 0x00, 0x7C, 0x08, 0x08, 0x10, 0x00, 0x00, 0x24, 0x4A, 0x4A, 0x32, 0x00, 0x00, 0x00,
0x04, 0x7E, 0x04, 0x00, 0x00, 0x00, 0x38, 0x40, 0x40, 0x38, 0x00, 0x00, 0x18, 0x60, 0x18, 0x00,
0x00, 0x38, 0x40, 0x20, 0x40, 0x38, 0x00, 0x00, 0x50, 0x20, 0x50, 0x00, 0x00, 0x00, 0x80, 0x50,
0x20, 0x10, 0x00, 0x00, 0x64, 0x54, 0x4C, 0x4C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 
}; /**<ASCII table used for displaying characters. \image html ascii.bmp*/


void nokia5110_init(void){
	// Plan to use 2Mhz SPI Clock. 8Mhz System Clock, Mode 0
	/* Enable SPI, Master, set clock rate fck/4 */
	SPCR = (1<<SPE)|(1<<MSTR)|(3<<SPR0);
	//Nokia DC Pin: PB6
	//Nokia CE Pin: PB7
	//Nokia Din Pin: PB2
	//Nokia CLK Pin: PB1
	DDRB |= 0b11100111;
	PORTB &= ~(0b00100000);
	_delay_ms(10);
	PORTB |= 0b00100000;
	nokia5110_control(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION);
	nokia5110_control(PCD8544_SETBIAS | 4);
	nokia5110_control(PCD8544_SETVOP | 0x30);
	nokia5110_control(PCD8544_FUNCTIONSET);
	nokia5110_control(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);
}


uint8_t spi_byte(uint8_t data){
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}

void nokia5110_control(char a_char){
	PORTB &= ~(0b11000000);
	spi_byte((uint8_t) a_char);
	PORTB |= 0b10000000;
}

void nokia5110_char2lcd(char a_char){
	PORTB |= 0b01000000;
	PORTB &= ~(0b10000000);
	uint8_t j = 0;

	if (a_char == ' ')
		for (j=0;j<6;j++)
			spi_byte((uint8_t) 0x00);
	else	
		for (j=0;j<6;j++){
			spi_byte((uint8_t)  pgm_read_byte(&(ascii[((a_char-44)*6)+j-1])));
			}
	PORTB |= 0b10000000;

}


void nokia5110_string2lcd(char *lcd_str){
	uint8_t length, i, j, xpos;

length = strlen(lcd_str);

for (i=0;i<length;i++){
	nokia5110_char2lcd(lcd_str[i]);
	}
}


void nokia5110_line(uint8_t line){
	nokia5110_control(0b01000000  | (line & 0b00000111));
	nokia5110_control(0b10000000);
}


void nokia5110_clear(void){
	uint16_t i = 0;
	
	nokia5110_control(0b01000000);
	nokia5110_control(0b10000000);
	PORTB |= 0b01000000;
	PORTB &= ~(0b10000000);
	for (i = 0; i<(6*84); i++){
		spi_byte((uint8_t) 0x00);
		_delay_us(100);
	}
	PORTB |= 0b10000000;
}





