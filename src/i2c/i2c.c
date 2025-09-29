#include "i2c.h"

// initialize the i2c interface
void i2c_init(void)
{
	// set scl frequency
	// FOR 100KHZ I2C WITH F_CPU = 16MHz:
	// 100000 = 16000000 / (16 + 2 * TWBR * 1)
	// twbr = 72

	TWSR = 0; // Prescaler = 1 (bits 0-1 are prescaler, clear them)
	TWBR = ((F_CPU / 100000UL) - 16) / 2; // For 100kHz

	TWCR = _BV(TWEN); // Enable TWI
}

// send a start condition
void i2c_start(void)
{
	// send a start condition
	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);

	// wait for the TWINT flag to be set
	i2c_wait_twint();
}

// send a stop condition
void i2c_stop(void)
{
	TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
}

uint8_t i2c_read_ack(void)
{
	TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
	i2c_wait_twint();

	return TWDR;
}

uint8_t i2c_read_nack(void)
{
	TWCR = _BV(TWINT) | _BV(TWEN);
	i2c_wait_twint();

	return TWDR;
}

// i2c_wait_twint waits on the TWINT flag to be set;
// useful for checking if an ack has been received or data has been transmitted
void i2c_wait_twint(void)
{
	while (!(TWCR & _BV(TWINT)))
		;
}

uint8_t i2c_sla_tx(uint8_t sla, uint8_t rw)
{
	// i2c slave addresses are 7-bits; any longer and that's an issue
	if (sla > 0x7f || sla == 0)
		return I2C_ERR_BAD_SLA;

	if (rw > 1)
		return I2C_ERR_BAD_RW;

	// shift the bits to the left by 1 and append the rw
	sla = (sla << 1) | rw;
	TWDR = sla;
	TWCR = _BV(TWINT) | _BV(TWEN);

	i2c_wait_twint();

	return I2C_OK;
}

// send data to the peripheral in address
void i2c_send_data(uint8_t data)
{
	TWDR = data;
	TWCR = _BV(TWINT) | _BV(TWEN);

	i2c_wait_twint();
}
