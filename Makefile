# AVR Static Library Makefile
# Builds individual static libraries for SPI, UART, and I2C modules
# Usage: make all (builds all libraries)
#        make spi (builds libavr_spi.a)

# Library version
VERSION = 1.0.0

# Default configuration - users can override
MCU ?= atmega32u4
F_CPU ?= 16000000UL

# Compiler settings
CC = avr-gcc
AR = avr-ar
RANLIB = avr-ranlib

# Compiler flags for library building
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -Wextra -std=c99
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -ffunction-sections -fdata-sections

# Directory structure
SRC_DIR = src
BUILD_DIR = build
LIB_DIR = lib

# Library names
LIB_PREFIX = libavr_
SPI_LIB = $(LIB_DIR)/$(LIB_PREFIX)spi.a
UART_LIB = $(LIB_DIR)/$(LIB_PREFIX)uart.a
I2C_LIB = $(LIB_DIR)/$(LIB_PREFIX)i2c.a

# Source files for each module
SPI_SOURCES = $(SRC_DIR)/spi/spi.c
UART_SOURCES = $(SRC_DIR)/uart/uart.c
I2C_SOURCES = $(SRC_DIR)/i2c/i2c.c

# Object files (fixed variable names!)
SPI_OBJECTS = $(BUILD_DIR)/spi/spi.o
UART_OBJECTS = $(BUILD_DIR)/uart/uart.o
I2C_OBJECTS = $(BUILD_DIR)/i2c/i2c.o

# Header files
SPI_HEADERS = $(SRC_DIR)/spi/spi.h
UART_HEADERS = $(SRC_DIR)/uart/uart.h
I2C_HEADERS = $(SRC_DIR)/i2c/i2c.h

# Include paths for each module
CFLAGS += -I$(SRC_DIR)/spi -I$(SRC_DIR)/uart -I$(SRC_DIR)/i2c

# Default target - build all libraries
all: $(SPI_LIB) $(UART_LIB) $(I2C_LIB)

# Create directories (including subdirectories)
$(BUILD_DIR) $(LIB_DIR):
	mkdir -p $@

$(BUILD_DIR)/spi $(BUILD_DIR)/uart $(BUILD_DIR)/i2c:
	mkdir -p $@

# Compile object files (fixed paths and dependencies)
$(BUILD_DIR)/spi/spi.o: $(SPI_SOURCES) $(SPI_HEADERS) | $(BUILD_DIR) $(BUILD_DIR)/spi
	$(CC) $(CFLAGS) -DAVR_SPI_LIBRARY -c $(SPI_SOURCES) -o $@

$(BUILD_DIR)/uart/uart.o: $(UART_SOURCES) $(UART_HEADERS) | $(BUILD_DIR) $(BUILD_DIR)/uart
	$(CC) $(CFLAGS) -DAVR_UART_LIBRARY -c $(UART_SOURCES) -o $@

$(BUILD_DIR)/i2c/i2c.o: $(I2C_SOURCES) $(I2C_HEADERS) | $(BUILD_DIR) $(BUILD_DIR)/i2c
	$(CC) $(CFLAGS) -DAVR_I2C_LIBRARY -c $(I2C_SOURCES) -o $@

# Build static libraries (fixed variable references)
$(SPI_LIB): $(SPI_OBJECTS) | $(LIB_DIR)
	$(AR) rcs $@ $^
	$(RANLIB) $@
	@echo "Built SPI library: $@"

$(UART_LIB): $(UART_OBJECTS) | $(LIB_DIR)
	$(AR) rcs $@ $^
	$(RANLIB) $@
	@echo "Built UART library: $@"

$(I2C_LIB): $(I2C_OBJECTS) | $(LIB_DIR)
	$(AR) rcs $@ $^
	$(RANLIB) $@
	@echo "Built I2C library: $@"

# Individual module targets
spi: $(SPI_LIB)
uart: $(UART_LIB)
i2c: $(I2C_LIB)

# Development helpers
check-tools:
	@echo "Checking for required tools..."
	@which $(CC) > /dev/null || (echo "ERROR: $(CC) not found" && exit 1)
	@which $(AR) > /dev/null || (echo "ERROR: $(AR) not found" && exit 1)
	@which $(RANLIB) > /dev/null || (echo "ERROR: $(RANLIB) not found" && exit 1)
	@echo "All tools found!"

# Show library information
info:
	@echo "AVR Library Build System"
	@echo "========================"
	@echo "Version: $(VERSION)"
	@echo "MCU: $(MCU)"
	@echo "F_CPU: $(F_CPU)"
	@echo ""
	@echo "Libraries that will be built:"
	@echo "  $(SPI_LIB) - SPI communication"
	@echo "  $(UART_LIB) - UART/Serial communication"
	@echo "  $(I2C_LIB) - I2C/TWI communication"
	@echo ""
	@echo "To use in your projects:"
	@echo "  LDFLAGS += -L$(shell pwd)/$(LIB_DIR) -lavr_spi"
	@echo "  or: LDFLAGS += $(shell pwd)/$(SPI_LIB)"

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR)
	@echo "Cleaned build artifacts"

# Clean everything including any editor backups
distclean: clean
	find . -name "*~" -delete
	find . -name "*.bak" -delete

# Help
help:
	@echo "AVR Static Library Build System"
	@echo ""
	@echo "Targets:"
	@echo "  all        - Build all libraries (default)"
	@echo "  spi        - Build SPI library only"
	@echo "  uart       - Build UART library only"
	@echo "  i2c        - Build I2C library only"
	@echo "  clean      - Remove build files"
	@echo "  info       - Show build information"
	@echo "  help       - Show this help"
	@echo ""
	@echo "Variables:"
	@echo "  MCU=$(MCU)       - Target microcontroller"
	@echo "  F_CPU=$(F_CPU)    - Clock frequency"

.PHONY: all spi uart i2c check-tools info clean distclean help
