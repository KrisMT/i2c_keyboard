#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <avr/interrupt.h>

#define SLAVE_ADDRESS 0x10

#define TWCR_ACK	TWCR = (1<<TWINT) | (1<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (0<<TWWC) | (1<<TWEN) | (1<<TWIE)
#define TWCR_NACK	TWCR = (1<<TWINT) | (0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (0<<TWWC) | (1<<TWEN) | (1<<TWIE)
#define TWCR_RESET	TWCR = (1<<TWINT) | (1<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (0<<TWWC) | (1<<TWEN) | (1<<TWIE)

#define USART_BAUDRATE 115200
#define BAUD_PRESCALE ((F_CPU/16)/USART_BAUDRATE - 1)

unsigned char addr_present = 1;
unsigned char buffAddr = 0xFF;
unsigned char data;
unsigned char Buff[256];

ISR(TWI_vect){
	switch(TWSR & 0xF8)
	{
	//*********** SLAVE RECIEVED **********
	case TW_SR_SLA_ACK: 		//0x60
		TWCR_ACK;
		break;
/*
	case TW_SR_ARB_LOST_SLA_ACK:	//0x68
		break;

	case TW_SR_GCALL_ACK:		//0x70
		break;

	case TW_SR_ARB_LOST_GCALL_ACK:	//0x78
		break;
*/
	case TW_SR_DATA_ACK:		//0x80
		data = TWDR;
		if( addr_present )
		{
			buffAddr = data;	//Get address from bus
			addr_present = 0;
		}else
		{
			Buff[buffAddr] = data;
			addr_present = 1;
		}
		TWCR_ACK;
		break;
/*
	case TW_SR_DATA_NACK:		//0x88
		break;

	case TW_SR_GCALL_DATA_ACK: 	//0x90
		break;

	case TW_SR_GCALL_DATA_NACK: 	//0x98
		break;

	case TW_SR_STOP: 		//0xA0
		break;
*/
	//*********** SLAVE TRANSMITTED **********

	case TW_ST_SLA_ACK:		//0xA8
		TWDR = Buff[buffAddr];
		addr_present = 1;
		TWCR_ACK;
		break;
/*
	case TW_ST_ARB_LOST_SLA_ACK:	//0xB0
		TWDR = 0xB0;
		TWCR_ACK;
		break;

	case TW_ST_DATA_ACK:		//0xB8
		TWDR = 0xB0;
		TWCR_ACK;
		break;

	case TW_ST_DATA_NACK:		//0xC0
		TWDR = 0xC0;
		TWCR_ACK;
		break;

	case TW_ST_LAST_DATA:		//0xC8
		TWDR = 0xC8;
		TWCR_NACK;
		break;
*/
	default:
		TWCR_RESET;
		break;

	};
}


void I2C_Slave_Init(uint8_t addr){
	TWAR = addr << 1;
	TWCR &= ~( (1<<TWSTA)|(1<<TWSTO) );
	TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);
	sei();
}


void USART_Init(void)
{
	UCSRC = (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1);
	UBRRL = BAUD_PRESCALE;
	UBRRH = (BAUD_PRESCALE >> 8);
	UCSRB = ((1<<TXEN) | (1<<RXEN));
}


int main(void)
{
	int i;
	USART_Init();
	DDRC = 0xFF;
	DDRD |= 0x03;
	I2C_Slave_Init(SLAVE_ADDRESS);
	for(i=0; i<256; i++) Buff[i] = i;
	Buff[0] = 'K';
	Buff[1] = 'r';
	Buff[2] = 'i';
	Buff[3] = 's';
	Buff[4] = 'M';
	Buff[5] = 'T';
	Buff[6] = '-';
	Buff[7] = 'I';
	Buff[8] = '2';
	Buff[9] = 'C';

	for(;;){
		_delay_ms(500);
	}
	
	return 0;
}
