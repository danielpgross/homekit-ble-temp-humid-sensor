/**
 * Copyright Gabor Paller.
 * Code taken from http://mylifewithandroid.blogspot.ca/2016/02/thermometer-application-with-nrf51822.html
 */
#include	<nrf.h>

void dht22init(int pin );
int dht22read(int pin, int data[]);
int dht22decode( int pin, unsigned char data_bits[] );
