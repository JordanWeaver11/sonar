#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include "cp210x.h"

void cp210x_init(uint16_t baud){
	DDRD |= (1<<3);
	DDRD &= ~(1<<2);
	
	UCSR1B |= (1<<RXEN1) | (1<<TXEN1);
	UCSR1C |= (3<<UCSZ10);
	UBRR1 = baud; 
}

void cp210x_putc(char data){
	while ((UCSR1A & (1<<UDRE1)) != (1<<UDRE1));    // Wait for previous transmissions
	UDR1 = data;    // Send data byte
}

void cp210x_puts(char *data){
	int i = 0;
	while(data[i] != '\0') { // Loop through string, sending each character
		cp210x_putc(data[i]);
		i++;
	}
}

uint8_t cp210x_getc_timeout(char *data, uint8_t timeout){
	while (!(UCSR1A & (1<<RXC1))){
		if (timeout == 0) {//Has Timeout elapsed?
			return (1); // If so return and error
		}
		_delay_ms(1); // Wait a tick
		timeout --; // Decrement the Timeout number
	} 
	*data = UDR1;
	return (0);
}

uint8_t cp210x_geti_timeout(uint8_t *data, uint8_t timeout){
	while (!(UCSR1A & (1<<RXC1))){
		if (timeout == 0) {//Has Timeout elapsed?
			return (1); // If so return and error
		}
		_delay_ms(1); // Wait a tick
		timeout --; // Decrement the Timeout number
	} 
	*data = UDR1;
	return (0);
}

char cp210x_getc(){
	while (!(UCSR1A & (1<<RXC1))); // Wait for byte to arrive
	return UDR1;
}

