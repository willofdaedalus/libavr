#include "spi.h"
#include <avr/io.h>

#define SPR0_BIT (1 << SPR0)
#define SPR1_BIT (1 << SPR1)
#define SPR0_SPR1 (1 << SPR0 | 1 << SPR1)
#define CPHA_BIT (1 << CPHA)
#define CPOL_BIT (1 << CPOL)
#define CPOL_CPHA (1 << CPOL | 1 << CPHA)

// instead of building SPCR bits at runtime, we precompute
// divider/mode combinations in spcr_table[] for speed and clarity.
// MSTR, DORD, and SPI2X are applied separately.

// each entry is structured like so
// {mode 0, mode 1, mode 2, mode 3}
static const uint8_t spcr_table[5][4] = {
	// div4 and div2
	{0, CPOL_BIT, CPHA_BIT, CPOL_CPHA},

	// div16 and div8
	{SPR0_BIT,
	 SPR0_BIT | CPOL_BIT,
	 SPR0_BIT | CPHA_BIT,
	 SPR0_BIT | CPOL_CPHA},

	// div32 and div64 (normal)
	{SPR1_BIT,
	 SPR1_BIT | CPOL_BIT,
	 SPR1_BIT | CPHA_BIT,
	 SPR1_BIT | CPOL_BIT | CPHA_BIT},

	// div128 and div64 (fast version)
	{SPR0_SPR1,
	 SPR0_SPR1 | CPOL_BIT,
	 SPR0_SPR1 | CPHA_BIT,
	 SPR0_SPR1 | CPHA_BIT | CPOL_BIT},
};

// ensures the speeds match their clock divisions
// returns -1 on mismatch or 0 on legal match
uint8_t validate_matches(uint8_t speed, uint8_t sck_div)
{
	if (speed) {
		// 2x mode only supports: DIV2, DIV8, DIV32, DIV64
		switch (sck_div) {
		case SPI_SCK_DIV2:
		case SPI_SCK_DIV8:
		case SPI_SCK_DIV32:
		case SPI_SCK_DIV64:
			break; // Valid combinations
		default:
			return -SPI_ERR_SPEED_MISMATCH;
		}
	} else {
		// Normal mode only supports: DIV4, DIV16, DIV64, DIV128
		switch (sck_div) {
		case SPI_SCK_DIV4:
		case SPI_SCK_DIV16:
		case SPI_SCK_DIV64:
		case SPI_SCK_DIV128:
			break; // Valid combinations
		default:
			return -SPI_ERR_SPEED_MISMATCH;
		}
	}

	return SPI_OK;
}

// configures spi mode based on user's cfg
// returns 0 for successful or non-zero on failure
uint8_t spi_init(struct spi_config_s *cfg)
{
	uint8_t div_idx, res;
	if (!cfg)
		return -1;

	if ((res = validate_matches(cfg->speed_mode, cfg->sck_div)) != SPI_OK)
		return res;

	switch (cfg->sck_div) {
	case SPI_SCK_DIV2:
	case SPI_SCK_DIV4:
		div_idx = 0;
		break;

	case SPI_SCK_DIV8:
	case SPI_SCK_DIV16:
		div_idx = 1;
		break;

	case SPI_SCK_DIV32:
		div_idx = 2;
		break;

	case SPI_SCK_DIV64:
		div_idx = 3;
		break;

	case SPI_SCK_DIV128:
		div_idx = 3;
		break;
	default:
		return -SPI_ERR_SPEED_MISMATCH;
	}

	SPCR = spcr_table[div_idx][cfg->mode];

	if (cfg->speed_mode)
		SPSR = _BV(SPI2X);

	if (cfg->master)
		SPCR |= _BV(MSTR);

	if (cfg->lsb)
		SPCR |= _BV(DORD);

	SPCR |= _BV(SPE) | _BV(SPIE);

	return SPI_OK;
}

// sends a byte, reads and returns another byte from the other peripheral
uint8_t spi_send(uint8_t data)
// configures spi mode based on user's cfg
{
	SPDR = data;
	loop_until_bit_is_set(SPSR, SPIF);

	// below clears the SPIF flag in the SPI status register
	return SPDR;
}
