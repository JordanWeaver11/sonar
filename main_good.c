/*************************************************************************
***************************************************************************/

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz       0x00
#define CPU_8MHz        0x01
#define CPU_4MHz        0x02
#define CPU_2MHz        0x03
#define CPU_1MHz        0x04
#define CPU_500kHz      0x05
#define CPU_250kHz      0x06
#define CPU_125kHz      0x07
#define CPU_62kHz       0x08
#define DIVISOR 1

#define delay_count 4000  //uP clock in MHz divided by 4000

#define F_CPU 8000000UL
#define BAUD_RATE 38400
#define MY_UBRR F_CPU/16/BAUD_RATE-1
//#define DEBUG

//#include <stdio.h>
#include <avr/io.h>

#include <stdlib.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>
#include "nokia5110.h"
#include "cp210x.h"

void init(void);

char line_str[14];

init(void){
    DDRB=0b11111111;  // Setup PORTB to be all output pins
	DDRD=0b01000000; // Setup the LED on PORTD to be an output
	DDRD |= 1<<5; // Setup HC-SR04 Trigger as an output
	DDRD &= ~(1<<4); // Setup HC-SR04 Echo a an input
}

uint8_t hcsr04_simple(){
		uint8_t count = 0; // Initialize a count variable
		
		PORTD |= (1<<5); // Set Trigger Pin connected to PORTD pin 5 to logic 1
		_delay_us(15); // Leave on for 15uS for require pulse length
		PORTD &= ~(1<<5); // Set Trigger Pin connected to PORTD pin 5 to logic 0 end 15uS pulse
		while ((PIND & (1<<4)) != (1<<4)); // While Echo Pulse is still high
		while (1){ // Do forever
			if (count == 255) // If I counted too long (object very far away) 
				return(count); // return maximum distance.
			if ((PIND & (1<<4)) != (1<<4)) // Echo pulse on PORTD, Pin 4 is high (detected)
				return(count); // Return current count
			_delay_us(39); // Wait 39us before counting and looping again
			count ++; // Increment Count
		}
}

void clear_line_str() {
	uint8_t i = 0;
	for(i = 0; i < 14; i++) {
		line_str[i] = ' ';
	}
}

int main (void){
	uint8_t i = 0;
	uint8_t temp;
	float temp_float;
	char tmp_str[6];

	CPU_PRESCALE(CPU_8MHz); // 3.3 volt compatible

	init();	//Initialize 
	nokia5110_init(); //Initialize the Nokia LCD including SPI, and LCD specific setup
	
	while(1){
		
		temp = hcsr04_simple(); // Send a string out over the hardware UART

		
		
		
		nokia5110_clear(); // Clear the screen
		/*
		nokia5110_line(2); // Adjust the text starting posistion to the 3rd line (out of 6)
		clear_line_str();
		strcpy(line_str, "Num:");
		strcat(line_str, itoa(temp, tmp_str, 10));
		//line_str[strlen(line_str)] = ' ';
		nokia5110_string2lcd(line_str); // Print the string 'Welcome', MAX 14 vchar/line
		*/
		
		clear_line_str();
		nokia5110_line(3); // Adjust the text starting posistion to the 3rd line (out of 6)

		temp_float = (float)(temp) * 40; //Convert the count to uS
		temp_float = temp_float / 58; //Convert uS to cm
		sprintf(tmp_str, "%.3f", temp_float);
		
		strcpy(line_str, "Dist:");
		strcat(line_str, tmp_str);
		strcat(line_str, "cm");
		//line_str[strlen(line_str)] = ' ';
		nokia5110_string2lcd(line_str); // Print the string 'Welcome', MAX 14 vchar/line

		PORTD ^= 0b01000000; // Turn on the Teensy 2.0 LED to ensure we know the program is running.
		//while (1); // Stop here forever!
		_delay_ms(1000);
	}


}//main
