#ifndef udr2_h
#define udr2_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int bool;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;

typedef union
{
	struct
	{
		uint8 low_low, low_high, high_low, high_high;
	} byte;
	uint32 word;
} address_t;

typedef struct
{
	uint8 byte[65536];
} buffer_t;

void digirule_update( const char *device, buffer_t *buffer, bool program );

void hexfile_load( buffer_t *buffer );

void port_open( const char *device );
void port_read( void *buffer, size_t size );
void port_write( const void *buffer, size_t size );
void port_close( void );

#endif	// udr2_h
