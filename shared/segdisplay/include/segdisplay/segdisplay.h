/*
 * Library for communicating with MAX7219-based segment displays
 *
 * Displays are put in "Code B" mode, so `segdisplay_write_command` with SEGDISPLAY_DIGIT_n
 * expects a Code B font codepoint rather than a segment bitmask. 
 * (see https://www.analog.com/media/en/technical-documentation/data-sheets/max7219-max7221.pdf)
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct segdisplay_o segdisplay_o;

enum {
  SEGDISPLAY_NO_OP = 0x00,
  SEGDISPLAY_DIGIT_0 = 0x01,
  SEGDISPLAY_DIGIT_1 = 0x02,
  SEGDISPLAY_DIGIT_2 = 0x03,
  SEGDISPLAY_DIGIT_3 = 0x04,
  SEGDISPLAY_DIGIT_4 = 0x05,
  SEGDISPLAY_DIGIT_5 = 0x06,
  SEGDISPLAY_DIGIT_6 = 0x07,
  SEGDISPLAY_DIGIT_7 = 0x08,
  SEGDISPLAY_DECODE_MODE = 0x09,
  SEGDISPLAY_INTENSITY = 0x0A,
  SEGDISPLAY_SCAN_LIMIT = 0x0B,
  SEGDISPLAY_SHUT_DOWN = 0x0C,
  SEGDISPLAY_TEST_DISPLAY = 0x0F,
};

enum {
    SEGDISPLAY_SPI_0,
    SEGDISPLAY_SPI_1,
};

typedef struct segdisplay_t {
    segdisplay_o *_internal;
    int chip_select;
} segdisplay_t;

/**
 * \brief Initializes a segdisplay_t struct and the MAX7219 chip located on \ref spi_num (\sa DISPLAY_SPI_0),
 * and uses software-controlled chip select on GPI \ref chip_select
 * \returns true on success, false on failure
 */
bool segdisplay_init(segdisplay_t *display, int spi_num, int chip_select);

/**
 * \brief Sends a command (one of SEGDISPLAY_COMMAND_*) with arg \ref arg
 * \note Commands SEGDISPLAY_DIGIT_n expect a Code B font codepoint. See MAX7219 documentation.
 * \warning It is the caller's responsibility to ensure there is enough time for the MAX7219 to process
 *   the given command, usually by sleeping between command writes.
 * \returns true on success, false on failure
 */
bool segdisplay_write_command(segdisplay_t *display, uint8_t cmd, uint8_t arg);

/// \brief Shuts down \ref display
void segdisplay_shutdown(segdisplay_t *display);
