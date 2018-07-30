//#define DEBUG

//#include "cp210x.c"

#define TXD 3
#define RXD 2

/************************************************************
cp210x_init(uint16_t baud)
  This function initializes the Teensy 2.0 system to utilize the on-board UART1 hardware module. The baudrate of the UART is set via the passed in value. The number can be pre calculated using the equations found in the ATMEGA32U4 datasheet. 
************************************************************/
void cp210x_init(uint16_t baud); 

/************************************************************
void cp210x_putc(char data)
  This function will copy the value passed in via 'data' to the UDR1 register. It will ensure that any previous transmission has already completed. 
************************************************************/
void cp210x_putc(char data);

/************************************************************
cp210x_puts(char *data)
  This function will transmit eaach byte in a passed in array until the '\0' (null) character is found. The null character is not transmitted.
************************************************************/
void cp210x_puts(char *data);

/************************************************************
uint8_t cp210x_getc_timeout(char *data, uint8_t timeout)
  This function will read one new byte from the UDR1 register. A time out value must be set indicating how long at maximum the function will block waiting for a serial byte. The function returns a 1 on time out and a 0 on successful read of the UDR1 register.
************************************************************/
uint8_t cp210x_getc_timeout(char *data, uint8_t timeout);

uint8_t cp210x_geti_timeout(uint8_t *data, uint8_t timeout);

/************************************************************
char cp210x_getc()
  This function will return one new byte from the UDR1 register. If no new byte is available, the function will block indefinately.
************************************************************/
char cp210x_getc();

