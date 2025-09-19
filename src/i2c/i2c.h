#ifndef _I2C_H
#define _I2C_H

#include <avr/io.h>

// MASTER TRANSMITTER MODE CODES
#define I2C_START (0x08)      // start condition has been sent
#define I2C_REP_START (0x10)  // repeated start has been sent
#define I2C_MT_SLA_ACK (0x18) // sla+w tx; ack received


void i2c_init(void);

#endif // _I2C_H
