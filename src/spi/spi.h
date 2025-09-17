#ifndef _SPI_H
#define _SPI_H

#include <avr/io.h>
#include <stdint.h>

#define SPI_SS (PB0)
#define SPI_SCK (PB1)
#define SPI_MOSI (PB2)
#define SPI_MISO (PB3)

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

#define MODE_COUNT (4)
#define DIVS_COUNT (4)

#define SPI_ERR_SPEED_MISMATCH (0xd0)
#define SPI_ERR_INVALID_MODE (0xd1)
#define SPI_ERR_NULL_CONFIG (0xd2)

#define SPI_MODE_0 (0)
#define SPI_MODE_1 (1)
#define SPI_MODE_2 (2)
#define SPI_MODE_3 (3)

#define DOUBLE_SPEED_MASK                                                      \
	(SPI_SCK_DIV2 | SPI_SCK_DIV8 | SPI_SCK_DIV32 | SPI_SCK_DIV64)
#define ALL_DIVS                                                               \
	(SPI_SCK_DIV2 | SPI_SCK_DIV4 | SPI_SCK_DIV8 | SPI_SCK_DIV16 |          \
	 SPI_SCK_DIV32 | SPI_SCK_DIV64 | SPI_SCK_DIV128)

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
	// set data order (lsb or msb)
	uint8_t lsb;
};

uint8_t spi_init(const struct spi_cfg_s *config);
uint8_t spi_tx(const uint8_t data);
void spi_send(const uint8_t data);
void spi_deselect(const uint8_t ss);
void spi_select(const uint8_t ss);


#endif // _SPI_H
