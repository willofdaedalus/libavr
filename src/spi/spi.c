#include "spi.h"

// pulls low the ss pin for communication with a slave
void spi_select(const uint8_t ss)
{
	PORTB &= ~_BV(ss);
}

// pulls high the ss pin for slave deselect
void spi_deselect(const uint8_t ss)
{
	PORTB |= _BV(ss);
}

// configures spi mode based on user's cfg
// returns 0 for successful or non-zero on failure
uint8_t spi_init(const struct spi_cfg_s *cfg)
{
	uint8_t spi_cfg;

	spi_cfg = 0;
	if (!cfg || cfg->mode > SPI_MODE_3)
		return SPI_ERR_INVALID_MODE;

	if ((cfg->sck_div & ALL_DIVS) == 0)
		return SPI_ERR_SPEED_MISMATCH;

	// toggle spi mode
	if (cfg->mode & 0x01)
		spi_cfg |= CPHA_BIT;

	if (cfg->mode & 0x02)
		spi_cfg |= CPOL_BIT;

	switch (cfg->sck_div) {
	case SPI_SCK_DIV2:
	case SPI_SCK_DIV4:
		// these values don't enable or disable any of the SPRx bits so
		// they're zero
		break;

	case SPI_SCK_DIV8:
	case SPI_SCK_DIV16:
		spi_cfg |= SPR0_BIT;
		break;

	case SPI_SCK_DIV32:
		spi_cfg |= SPR1_BIT;
		break;

	case SPI_SCK_DIV64:
		// this is separate for future reference but regardless of 2x
		// spr, both div64 are the same; they both set the frequency to
		// 250kHz
	case SPI_SCK_DIV128:
		spi_cfg |= SPR0_SPR1;
		break;

	default:
		// should never get here since check_sck makes sure of that but
		// some defensive programming never hurt anyone
		return SPI_ERR_SPEED_MISMATCH;
	}

	// we set the speed mode based the clock division
	// SPI_SCK_DIV64 has same speed for 1x or 2x mode; SPI2X setting ignored
	if (cfg->sck_div & DOUBLE_SPEED_MASK)
		SPSR = _BV(SPI2X);

	if (cfg->master) {
		// set all lines except MISO as output
		DDRB |= _BV(SPI_MOSI) | _BV(SPI_SCK) | _BV(SPI_SS);
		PORTB |= _BV(SPI_SS) | _BV(SPI_MISO);

		// configure master mode
		spi_cfg |= _BV(MSTR);
	} else {
		// miso is output on slave mode
		DDRB |= _BV(SPI_MISO);
	}

	if (cfg->lsb)
		spi_cfg |= _BV(DORD);

	if (cfg->spie)
		spi_cfg |= _BV(SPIE);

	// enable spi mode
	spi_cfg |= _BV(SPE);

	// assign the final config to the spi config register
	SPCR = spi_cfg;

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
