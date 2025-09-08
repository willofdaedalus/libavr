#include "spi.h"
#include <avr/io.h>

uint8_t spi_init(void)
{
#ifdef MASTER
	SPCR |= _BV(MSTR);
#endif

	return 0;
}
