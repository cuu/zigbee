#ifndef GUU_SERIAL_H
#define GUU_SERIAL_H

#define SER_PORT 0x00
#define SER_PORT2 0x01

void guu_serial_init(void );
void guu_serial2_init(void);

uint8 guu_usart( uint8*,  uint8 ); // only for router and enddev
void  guu_usart2(uint8*, uint8 );

void guu_serial_callback(uint8 port, uint8 event);

#endif