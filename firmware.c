#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include "keycode.h"

#define SLAVE_ADDRESS 0x0F
#define I2C_FREQ 400000

#define TWCR_ACK	TWCR = (1<<TWINT) | (1<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (0<<TWWC) | (1<<TWEN) | (1<<TWIE)
#define TWCR_NACK	TWCR = (1<<TWINT) | (0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (0<<TWWC) | (1<<TWEN) | (1<<TWIE)
#define TWCR_RESET	TWCR = (1<<TWINT) | (1<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (0<<TWWC) | (1<<TWEN) | (1<<TWIE)
#define TWCR_STA	TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWSTA) | (0<<TWSTO) | (0<<TWWC) | (1<<TWEN) | (1<<TWIE)
#define TWCR_STO	TWCR = (1<<TWINT) | (1<<TWEA) | (0<<TWSTA) | (1<<TWSTO) | (0<<TWWC) | (1<<TWEN) | (1<<TWIE)
#define TWCR_DISABLE	TWCR &= ~( (1<<TWEN) | (1<<TWIE) | (1<<TWEA) )
#define TWCR_RELEASE_BUS TWCR = (1<<TWEN) | (1<<TWIE) | (1<<TWEA) | (1<<TWINT)

#define USART_BAUDRATE 115200
#define BAUD_PRESCALE ((F_CPU/16)/USART_BAUDRATE - 1)

unsigned char addr_present = 1;
unsigned char buffAddr = 0xFF;
unsigned char data;
volatile unsigned char Buff[256];

ISR(TWI_vect){
  switch(TW_STATUS)
  {
    case TW_START:
    case TW_REP_START:
      TWCR_ACK;
      break;

      //*********** SLAVE RECIEVED **********
    case TW_SR_SLA_ACK: 		//0x60 **
    case TW_SR_ARB_LOST_SLA_ACK:	//0x68
    case TW_SR_GCALL_ACK:		//0x70
    case TW_SR_ARB_LOST_GCALL_ACK:	//0x78
      TWCR_ACK;
      break;

    case TW_SR_DATA_ACK:		//0x80 **
    case TW_SR_GCALL_DATA_ACK: 	//0x90
      data = TWDR;
      if( addr_present )
      {
        buffAddr = data;	//Get address from bus
        addr_present = 0;
      }else
      {
        addr_present = 1;
      }
      TWCR_ACK;
      break;

    case TW_SR_DATA_NACK:		//0x88
    case TW_SR_GCALL_DATA_NACK: 	//0x98
      TWCR_NACK;
      break;

    case TW_SR_STOP: 		//0xA0 **
      TWCR_RELEASE_BUS;
      break;

      //*********** SLAVE TRANSMITTED **********

    case TW_ST_SLA_ACK:		//0xA8 **
    case TW_ST_ARB_LOST_SLA_ACK:	//0xB0
    case TW_ST_DATA_ACK:		//0xB8
      TWDR = Buff[buffAddr];
      addr_present = 1;
      TWCR_ACK;
      break;

    case TW_ST_DATA_NACK:		//0xC0 **
    case TW_ST_LAST_DATA:		//0xC8
      TWCR_ACK;
      break;

    case TW_NO_INFO:		//0xF8
      break;
    case TW_BUS_ERROR:		//0x00
      TWCR_STO;
      break;

    default:
      TWCR_RELEASE_BUS;
      break;
  };
}

void I2C_Slave_Init(uint8_t addr){
  TWAR = addr << 1;
  TWBR = ((F_CPU / I2C_FREQ) - 16) / 2;
  TWCR &= ~( (1<<TWSTA)|(1<<TWSTO) );
  TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);
  sei();
}

int main(void)
{
  DDRA = 0xFF; //ROW Output PIN
  DDRC = 0x03; //Column Input PIN + PC0 (SCL) + PC1 (SDA)
  DDRD = 0xFF; //Layer LED
  PORTC = 0xFF;
  I2C_Slave_Init(SLAVE_ADDRESS);

  for(;;){
    keyboard_task(Buff);
  }

  return 0;
}
