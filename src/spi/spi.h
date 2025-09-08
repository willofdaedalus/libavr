#ifndef _SPI_H
#define _SPI_H

#include <stdint.h>

#define SPI_SS (PB0)
#define SPI_MISO (PB1)
#define SPI_MOSI (PB2)
#define SPI_SCK (PB3)

#define SPR0_BIT (1 << SPR0)
#define SPR1_BIT (1 << SPR1)
#define SPR0_SPR1 (1 << SPR0 | 1 << SPR1)
#define CPHA_BIT (1 << CPHA)
#define CPOL_BIT (1 << CPOL)
#define CPOL_CPHA (1 << CPOL | 1 << CPHA)

#define SPI_SCK_DIV2 (2)
#define SPI_SCK_DIV4 (4)
#define SPI_SCK_DIV8 (8)
#define SPI_SCK_DIV16 (16)
#define SPI_SCK_DIV32 (32)
#define SPI_SCK_DIV64 (64)
#define SPI_SCK_DIV128 (128)

#define SPI_OK (0)

#define SPI_ERR_SPEED_MISMATCH (0x01)
#define SPI_ERR_INVALID_MODE (0x02)

#define SPI_MODE_0 (0)
#define SPI_MODE_1 (1)
#define SPI_MODE_2 (2)
#define SPI_MODE_3 (3)

// struct to configure spi
struct spi_cfg_s {
	// freq division value
	uint8_t sck_div;
	// should the spi device be a master
	uint8_t master;
	// enable interrupts?
	uint8_t spie;
	// set mode
	uint8_t mode;
	// set 2x mode
	uint8_t speed_mode;
	// set data order (lsb or msb)
	uint8_t lsb;
};

uint8_t spi_init(struct spi_cfg_s *config);
uint8_t spi_tx(uint8_t data);
void spi_send(uint8_t data);
void spi_deselect(void);
void spi_select(void);


#endif // _SPI_H
