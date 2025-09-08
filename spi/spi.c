#include "spi.h"
#include <avr/io.h>

// configures spi mode as slave device unless MASTER is defined
uint8_t spi_init(struct spi_s *config)
{
	if (!config)
		return -1;

	if (config->master)
		SPCR |= _BV(MSTR);

	if (config->spie)
		SPCR |= _BV(SPIE);

	// enable 2x speed based on what the user set
	if (config->speed_mode) {
		switch (config->c_div) {
		case SPI_SCK_DIV2:
		case SPI_SCK_DIV8:
		case SPI_SCK_DIV32:
		case SPI_SCK_DIV64:
			SPCR |= _BV(SPI2X);
			break;

		default:
			// reset the SPCR on error
			SPCR = 0x0;
			return ESPI_SPEED_MISMATCH;
		}
	} else {
		switch (config->c_div) {
		case SPI_SCK_DIV4:
		case SPI_SCK_DIV16:
		case SPI_SCK_DIV64:
		case SPI_SCK_DIV128:
			SPCR &= ~_BV(SPI2X);
			break;

		default:
			SPCR = 0x0;
			return ESPI_SPEED_MISMATCH;
		}
	}

	// intentional resetting in case user reinitializes their spi
	// mode again
	switch (config->c_div) {
	case SPI_SCK_DIV2:
	case SPI_SCK_DIV4:
		SPCR &= ~_BV(SPR1);
		SPCR &= ~_BV(SPR0);
		break;

	case SPI_SCK_DIV16:
	case SPI_SCK_DIV8:
		SPCR &= ~_BV(SPR1);
		SPCR |= _BV(SPR0);
		break;

	case SPI_SCK_DIV32:
		SPCR |= _BV(SPR1);
		SPCR &= ~_BV(SPR0);
		break;

	case SPI_SCK_DIV64:
		if (config->speed_mode) {
			SPCR |= _BV(SPR1) | _BV(SPR0);
		} else {
			SPCR |= _BV(SPR1);
			SPCR &= ~_BV(SPR0);
		}
		break;

	case SPI_SCK_DIV128:
		SPCR |= _BV(SPR1) | _BV(SPR0);
		break;
	}

	// cpol cpha
	switch (config->mode) {
	case SPI_MODE_0: // 0 0
		SPCR &= ~_BV(CPOL);
		SPCR &= ~_BV(CPHA);
		break;

	case SPI_MODE_1: // 0 1
		SPCR &= ~_BV(CPOL);
		SPCR |= _BV(CPHA);
		break;

	case SPI_MODE_2: // 1 0
		SPCR |= _BV(CPOL);
		SPCR &= ~_BV(CPHA);
		break;

	case SPI_MODE_3: // 1 1
		SPCR |= _BV(CPOL);
		SPCR |= _BV(CPHA);
		break;
	}

	if (config->lsb) {
		SPCR |= _BV(DORD);
	} else {
		SPCR &= ~_BV(DORD);
	}


	// finally enable spi mode
	SPCR |= _BV(SPE);

	return SPI_OK;
}
