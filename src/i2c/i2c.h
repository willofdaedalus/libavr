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

void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
uint8_t i2c_tx(uint8_t sla, uint8_t rw);
void i2c_wait_twint(void);

#endif // _I2C_H
