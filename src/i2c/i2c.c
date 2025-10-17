#include "i2c.h"

// initializes the spi interface
void i2c_init(void)
{
	TWSR = 0;
	// Try 50kHz instead of 100kHz (or even 25kHz)
	TWBR = ((F_CPU / 50000UL) - 16) / 2;
	TWCR = _BV(TWEN);
}
// void i2c_init(void)
// {
// 	// set scl frequency
// 	// FOR 100KHZ I2C WITH F_CPU = 16MHz:
// 	// 100000 = 16000000 / (16 + 2 * TWBR * 1)
// 	// twbr = 72
//
// 	TWSR = 0; // Prescaler = 1 (bits 0-1 are prescaler, clear them)
// 	TWBR = ((F_CPU / 100000UL) - 16) / 2; // For 100kHz
//
// 	TWCR = _BV(TWEN); // Enable TWI
// }

// send a start condition over i2c bus
void i2c_start(void)
{
	// send a start condition
	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);

	// wait for the TWINT flag to be set
	i2c_wait_twint();
}

// checks the TWI status register for the status by masking the prescaler bits
// returns the status of an action with prescaler bits masked
uint8_t i2c_get_status(void)
{
	return TWSR & 0xf8;
}

// send a start condition over i2c bus
void i2c_stop(void)
{
	TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
}

// reads a byte of data from the bus expecting an ack
uint8_t i2c_read_ack(void)
{
	TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
	i2c_wait_twint();

	return TWDR;
}

// reads a byte of data from the bus without expecting an ack
uint8_t i2c_read_nack(void)
{
	TWCR = _BV(TWINT) | _BV(TWEN);
	i2c_wait_twint();

	return TWDR;
}

// waits on the TWINT to be set;
// useful for waiting on data transfer to complete or ack has been received
void i2c_wait_twint(void)
{
	while (!(TWCR & _BV(TWINT)))
		;
}

// addresses a slave at address "sla" with a r/w bit from "rw"
// returns 0 for success and non-zero for error
uint8_t i2c_sla_tx(uint8_t sla, uint8_t rw)
{
	// i2c slave addresses are 7-bits; any longer and that's an issue
	// that means the max we can support is 127 leaving 1-bit for rw
	if (sla > 127)
		return I2C_ERR_BAD_SLA;

	if (rw > 1)
		return I2C_ERR_BAD_RW;

	// shift the bits to the left by 1 and append the rw
	TWDR = (sla << 1) | rw;
	TWCR = _BV(TWINT) | _BV(TWEN);

	i2c_wait_twint();

	return I2C_OK;
}

// send data to a previously selected address
void i2c_send_data(uint8_t data)
{
	TWDR = data;
	TWCR = _BV(TWINT) | _BV(TWEN);

	i2c_wait_twint();
}

// sets an address for device and returns 0 for sucess and non-zero for failure
uint8_t i2c_set_address(uint8_t addr)
{
	if (addr > 127) {
		return I2C_ERR_BAD_SLA;
	}

	// we only need the first 7 bits
	TWAR = (addr << 1);
	// setting TWEA allows the devices to be included in all actions of the
	// i2c bus
	TWCR = _BV(TWEA) | _BV(TWEN) | _BV(TWIE);

	return I2C_OK;
}
