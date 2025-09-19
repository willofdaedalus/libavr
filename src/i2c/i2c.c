#include "i2c.h"

void i2c_init(void)
{
	TWCR = _BV(TWEN);
}

// send a start condition
void i2c_start_tx(void)
{
	// send a start condition
	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);

	// wait for the TWINT flag to be set
	while (!(TWCR & _BV(TWINT)))
		;
}

// send a stop condition
void i2c_stop(void)
{
	TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
}


// void i2c_master_tx(uint8_t data)
// {
// }
