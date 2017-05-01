#include	<nrf.h>

void dht22init(int pin );
int dht22read(int pin, int data[]);
int dht22decode( int pin, unsigned char data_bits[] );
