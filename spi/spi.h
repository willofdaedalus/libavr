#ifndef _SPI_H
#define _SPI_H

#include <stdint.h>

#define SPI_SCK_DIV2 (2)
#define SPI_SCK_DIV4 (4)
#define SPI_SCK_DIV8 (8)
#define SPI_SCK_DIV16 (16)
#define SPI_SCK_DIV32 (32)
#define SPI_SCK_DIV64 (64)
#define SPI_SCK_DIV128 (128)

#define SPI_OK (0)

#define SPI_ERR_SPEED_MISMATCH (0x01)

#define SPI_MODE_0 (0)
#define SPI_MODE_1 (1)
#define SPI_MODE_2 (2)
#define SPI_MODE_3 (3)

struct spi_config_s {
	uint8_t sck_div;
	uint8_t master;
	uint8_t mode;
	uint8_t speed_mode;
	uint8_t lsb;
};

uint8_t spi_init(struct spi_config_s *config);
uint8_t spi_send(uint8_t data);


#endif //
