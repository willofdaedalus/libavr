#include "spi.h"
#include <avr/io.h>

// instead of building SPCR bits at runtime, we precompute
// divider/mode combinations in spcr_table[] for speed and clarity.
// MSTR, DORD, and SPI2X are applied separately.

// each entry is structured like so
// {mode 0, mode 1, mode 2, mode 3}
// this is basically a look up table of table 17-5 in the atmega32u4 datasheet
static const uint8_t spcr_table[4][4] = {
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
	// note; div64 is in both so that we don't trigger on a false "negative"
	if (speed) {
		// 2x mode only supports: div2, div8, div32, div64
		switch (sck_div) {
		case SPI_SCK_DIV2:
		case SPI_SCK_DIV8:
		case SPI_SCK_DIV32:
		case SPI_SCK_DIV64:
			break; // valid combinations
		default:
			return -SPI_ERR_SPEED_MISMATCH;
		}
	} else {
		// normal mode only supports: div4, div16, div64, div128
		switch (sck_div) {
		case SPI_SCK_DIV4:
		case SPI_SCK_DIV16:
		case SPI_SCK_DIV64:
		case SPI_SCK_DIV128:
			break; // valid combinations
		default:
			return -SPI_ERR_SPEED_MISMATCH;
		}
	}

	return SPI_OK;
}

// pulls low the ss pin for communication with a slave
void spi_select(void)
{
	// TODO; in the future add a slave id so we can pull low on that
	PORTB &= ~_BV(SPI_SS);
}

// pulls high the ss pin for slave deselect
void spi_deselect(void)
{
	// TODO; in the future add a slave id so we can pull high on that
	PORTB |= _BV(SPI_SS);
}

// configures spi mode based on user's cfg
// returns 0 for successful or non-zero on failure
uint8_t spi_init(struct spi_config_s *cfg)
{
	uint8_t div_idx, res;
	if (!cfg)
		return -1;

	if (cfg->mode > 3)
		return -SPI_ERR_INVALID_MODE;

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

	if (cfg->master) {
		DDRB |= _BV(SPI_MOSI) | _BV(SPI_SCK) | _BV(SPI_SS);
		PORTB |= _BV(SPI_SS);
		SPCR |= _BV(MSTR);
	} else {
		DDRB |= _BV(SPI_MISO);
	}

	if (cfg->lsb)
		SPCR |= _BV(DORD);

	if (cfg->spie)
		SPCR |= _BV(SPIE);

	SPCR |= _BV(SPE);

	return SPI_OK;
}

// sends a byte over the serial line
// returns the byte read from the other peripheral
uint8_t spi_tx(uint8_t data)
{
	SPDR = data;
	loop_until_bit_is_set(SPSR, SPIF);

	// below clears the SPIF flag in the SPI status register
	return SPDR;
}

// sends a single byte of data but discards any data read
void spi_send(uint8_t data)
{
	SPDR = data;
	loop_until_bit_is_set(SPSR, SPIF);
	(void) SPDR;
}
