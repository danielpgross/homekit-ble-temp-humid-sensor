#include	"dht22.h"
#include	<nrf_delay.h>

#define DHT22_WAKEUP_MS 1  // MCU->DHT: 1 msec
#define DHT22_PHCG_TIMEOUT	200	// After this timeout, DHT22 is considered unresponsive

void dht22init(int pin ) {
	int pinmask = 1 << pin;
// output on pin
	NRF_GPIO->PIN_CNF[pin] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
					| (GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos)
					| (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
					| (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
					| (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
	NRF_GPIO->OUTSET = pinmask;	// pin 0->1
}

int dht22read(int pin,int data[]) {
	int toctr,dptr;
	int pinmask = 1 << pin;
// Output on pin
	NRF_GPIO->PIN_CNF[pin] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
					| (GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos)
					| (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
					| (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
					| (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
	NRF_GPIO->OUTCLR = pinmask;	// pin 1->0
	nrf_delay_ms(DHT22_WAKEUP_MS);
	NRF_GPIO->OUTSET = pinmask;	// pin 0->1
// Pin to input
	NRF_GPIO->PIN_CNF[pin] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
					| (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
					| (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
					| (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
					| (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);
	toctr = DHT22_PHCG_TIMEOUT;
// From now on, don't interrupt us as timings are very tight
	__disable_irq();
// DHT22 must respond by pulling its pin to 0
	do {
		if( ( NRF_GPIO->IN & pinmask ) == 0 )
			break;
		nrf_delay_us(10);
		toctr -= 10;
	} while( toctr > 0 );
	if( toctr <= 0 ) {
		dht22init(pin);
		__enable_irq();
		return -2;
	}
// DHT22 must now pull its pin to 1
	do {
		if( ( NRF_GPIO->IN & pinmask ) != 0 )
			break;
		nrf_delay_us(10);
		toctr -= 10;
	} while( toctr > 0 );
	if( toctr <= 0 ) {
		dht22init(pin);
		__enable_irq();
		return -1;
	}
// Now the 40 data bits
	for( dptr = 0 ; dptr < 40 ; ++dptr ) {
// First the pin goes to 0
		toctr = DHT22_PHCG_TIMEOUT;
		do {
			if( ( NRF_GPIO->IN & pinmask ) == 0 )
				break;
			nrf_delay_us(10);
			toctr -= 10;
		} while( toctr > 0 );
		if( toctr <= 0 ) {
			dht22init(pin);
			__enable_irq();
			return dptr;
		}
// Then it goes to 1 and the lenght of the pulse is to be measured
		toctr = DHT22_PHCG_TIMEOUT;
		do {
			if( ( NRF_GPIO->IN & pinmask ) != 0 )
				break;
			nrf_delay_us(10);
			toctr -= 10;
		} while( toctr > 0 );
		if( toctr <= 0 ) {
			dht22init(pin);
			__enable_irq();
			return dptr;
		}
		toctr = 0;
		do {
			if( ( NRF_GPIO->IN & pinmask ) == 0 )
				break;
			nrf_delay_us(10);
			toctr += 10;
		} while( toctr < DHT22_PHCG_TIMEOUT );
		if( toctr >= DHT22_PHCG_TIMEOUT ) {
			dht22init(pin);
			__enable_irq();
			return dptr;
		}
		data[dptr] = toctr;
	}
	dht22init(pin);
	__enable_irq();
	return dptr;
}

int dht22decode( int pin, unsigned char data_bits[] ) {
	int rsp,i,byte_counter;
	int bits[40];
	unsigned char current_byte,mask,chksum;

	rsp = dht22read(pin,bits);
	if( rsp != 40 )
		return 0;
	byte_counter = 0;
	current_byte = 0;
	mask = 0x80;
	for( i = 0 ; i < 40 ; ++i ) {
		if( bits[i] > 50 )	// Real measurements are either 30 or 70 usecs. 50 is the average.
			current_byte |= mask;
		mask >>= 1;
		if( mask == 0 ) {
			mask = 0x80;
			data_bits[byte_counter] = current_byte;
			++byte_counter;
			current_byte = 0;
		}
	}
	chksum = data_bits[0] + data_bits[1] + data_bits[2] + data_bits[3];
	if( chksum != data_bits[4] )
		return 0;
	return 1;
}
