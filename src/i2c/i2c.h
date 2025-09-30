#ifndef _I2C_H
#define _I2C_H

#include <avr/io.h>

// MASTER TRANSMITTER MODE CODES
#define I2C_START (0x08)	    // start condition has been sent
#define I2C_REP_START (0x10)	    // repeated start has been sent
#define I2C_MT_SLA_ACK (0x18)	    // sla+w tx; ack received
#define I2C_MT_SLA_NACK (0x20)	    // sla+w tx; not ack recieved
#define I2C_MT_DATA_ACK (0x28)	    // data tx; ack received
#define I2C_MT_DATA_NACK (0x30)	    // data tx; not ack received
#define I2C_ARBITRATION_LOST (0x38) // arbitration lost in sla+w

// MASTER RECEIVE MODE CODES
#define I2C_MR_SLA_ACK (0x40)	// sla+r rx; ack received
#define I2C_MR_SLA_NACK (0x48)	// sla+r rx; not ack received
#define I2C_MR_DATA_ACK (0x50)	// data rx; ack received
#define I2C_MR_DATA_NACK (0x50) // data rx; not ack received

#define I2C_SLA_W (0)
#define I2C_SLA_R (1)

#define I2C_OK (0)
#define I2C_ERR_BAD_SLA (0xd0)
#define I2C_ERR_BAD_RW (0xd1)

// initializes the spi interface
void i2c_init(void);
// send a start condition over i2c bus
void i2c_start(void);
// send a stop condition over i2c bus
void i2c_stop(void);
// checks the TWI status register for the status by masking the prescaler bits
// returns the status of an action with prescaler bits masked
uint8_t i2c_get_status(void);
// reads a byte of data from the bus expecting an ack
uint8_t i2c_read_ack();
// reads a byte of data from the bus without expecting an ack
uint8_t i2c_read_nack();
// send data to a previously selected address
void i2c_send_data(uint8_t data);
// addresses a slave at address "sla" with a r/w bit from "rw"
// returns 0 for success and non-zero for error
uint8_t i2c_tx(uint8_t sla, uint8_t rw);
// waits on the TWINT to be set;
// useful for waiting on data transfer to complete or ack has been received
void i2c_wait_twint(void);

#endif // _I2C_H
