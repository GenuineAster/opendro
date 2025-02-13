#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "hardware/spi.h"
#include "hardware/timer.h"
#include "encoder.pio.h"

#ifdef OPENDRO_PICO_FIRMWARE_SEGDISPLAY
#include <segdisplay/segdisplay.h>
#endif

enum {
  DIRECTION_NONE = 0,
  DIRECTION_0 = -1,
  DIRECTION_1 = 1,
  INVALID = 0
};

const int8_t lookup[16] = {
  DIRECTION_NONE, // 0000
  DIRECTION_0, // 0001
  DIRECTION_1, // 0010
  INVALID, // 0011
  DIRECTION_1, // 0100
  DIRECTION_NONE, // 0101
  INVALID, // 0110
  DIRECTION_0, // 0111
  DIRECTION_0, // 1000
  INVALID, // 1001
  DIRECTION_NONE, // 1010
  DIRECTION_1, // 1011
  INVALID, // 1100
  DIRECTION_1, // 1101
  DIRECTION_0, // 1110
  DIRECTION_NONE, // 1111
};

volatile int32_t pulses1 = 0, pulses2 = 0;
volatile int prev1 = 0, prev2 = 0;

#define AXIS_1_CLOCK_PIN 6
#define AXIS_1_A_PIN 7
#define AXIS_1_B_PIN 8
// 1e6 / um per div
#define AXIS_1_DIVISOR (1000000 / 20)
#define AXIS_2_CLOCK_PIN 10
#define AXIS_2_A_PIN 11
#define AXIS_2_B_PIN 12
// 1e6 / um per div
#define AXIS_2_DIVISOR (1000000 / 10)

const uint sm = 0;

#ifdef OPENDRO_PICO_FIRMWARE_SEGDISPLAY
segdisplay_t disp0, disp1;
#endif

void setup() {
  stdio_init_all();

  gpio_set_function(19, GPIO_FUNC_SPI);
  gpio_set_function(18, GPIO_FUNC_SPI);
  gpio_set_function(16, GPIO_FUNC_SPI);
  gpio_init(17);
  gpio_set_dir(17, GPIO_OUT);
  gpio_put(17, true);
  gpio_init(20);
  gpio_set_dir(20, GPIO_OUT);
  gpio_put(20, true);
  spi_init(spi0, 10000000);
  spi_set_format(spi0, 16, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

#ifdef OPENDRO_PICO_FIRMWARE_SEGDISPLAY
  if ( ! segdisplay_init(&disp0, 0, 17)) {
    while (true) {
        // spin forever
        sleep_ms(100);
    }
  }

  if ( ! segdisplay_init(&disp1, 0, 20)) {
    while (true) {
        // spin forever
        sleep_ms(100);
    }
  }

  // set displays to some known state
  {
    segdisplay_write_command(&disp0, SEGDISPLAY_DIGIT_0, 0x0);
    segdisplay_write_command(&disp0, SEGDISPLAY_DIGIT_1, 0x0);
    segdisplay_write_command(&disp0, SEGDISPLAY_DIGIT_2, 0x0);
    segdisplay_write_command(&disp0, SEGDISPLAY_DIGIT_3, 0x0);
    segdisplay_write_command(&disp0, SEGDISPLAY_DIGIT_4, 0xD);
    segdisplay_write_command(&disp0, SEGDISPLAY_DIGIT_5, 0xD);
    segdisplay_write_command(&disp0, SEGDISPLAY_DIGIT_6, 0xB);
    segdisplay_write_command(&disp0, SEGDISPLAY_DIGIT_7, 0xC);
    segdisplay_write_command(&disp1, SEGDISPLAY_DIGIT_0, 0x0);
    segdisplay_write_command(&disp1, SEGDISPLAY_DIGIT_1, 0x0);
    segdisplay_write_command(&disp1, SEGDISPLAY_DIGIT_2, 0x0);
    segdisplay_write_command(&disp1, SEGDISPLAY_DIGIT_3, 0x0);
    segdisplay_write_command(&disp1, SEGDISPLAY_DIGIT_4, 0xD);
    segdisplay_write_command(&disp1, SEGDISPLAY_DIGIT_5, 0xD);
    segdisplay_write_command(&disp1, SEGDISPLAY_DIGIT_6, 0xB);
    segdisplay_write_command(&disp1, SEGDISPLAY_DIGIT_7, 0xC);
  }
#endif

  // give the encoder lamp time to warm up
  sleep_ms(200);

  // Base pin to connect the A phase of the encoder.
  // The B phase must be connected to the next pin
  const uint AXIS_1_PIN_AB = AXIS_1_A_PIN;
  const uint AXIS_2_PIN_AB = AXIS_2_A_PIN;

  // we don't really need to keep the offset, as this program must be loaded
  // at offset 0
  pio_add_program(pio0, &encoder_program);
  encoder_program_init(pio0, sm, AXIS_2_PIN_AB, 0);

  pio_add_program(pio1, &encoder_program);
  encoder_program_init(pio1, sm, AXIS_1_PIN_AB, 0);
}

#define TEXTBUF_SIZE 12
char buf1[TEXTBUF_SIZE];
char buf2[TEXTBUF_SIZE];

void loop() {
  pulses1 = encoder_get_count(pio1, sm);
  int32_t millimeters1 = pulses1 / (AXIS_1_DIVISOR / 1000);
  int32_t micrometers1 = pulses1 % (AXIS_1_DIVISOR / 1000);
  bool sign1 = true;
  if (micrometers1 < 0) {
    micrometers1 = -micrometers1;
    millimeters1 = -millimeters1;
    sign1 = false;
  }
  int res1 = snprintf(buf1, sizeof(buf1), "%c%04ld", sign1 ? ' ' : '-', millimeters1);
  buf1[res1] = '.';
  snprintf(buf1+res1+1, sizeof(buf1)-res1-1, "%02ld", micrometers1);
  
  pulses2 = encoder_get_count(pio0, sm);
  int32_t millimeters2 = pulses2 / (AXIS_1_DIVISOR / 1000);
  int32_t micrometers2 = pulses2 % (AXIS_1_DIVISOR / 1000);
  bool sign2 = true;
  if (micrometers2 < 0) {
    micrometers2 = -micrometers2;
    millimeters2 = -millimeters2;
    sign2 = false;
  }
  int res2 = snprintf(buf2, sizeof(buf2), "%c%04ld", sign2 ? ' ' : '-', millimeters2);
  buf2[res2] = '.';
  snprintf(buf2+res2+1, sizeof(buf2)-res2-1, "%02ld", micrometers2);

  printf("%s\n", buf1);
  printf("%s\n", buf2);

#ifdef OPENDRO_PICO_FIRMWARE_SEGDISPLAY
  segdisplay_write_command(&disp0, SEGDISPLAY_DIGIT_0, (micrometers2 / 1) % 10);
  sleep_us(1);
  segdisplay_write_command(&disp0, SEGDISPLAY_DIGIT_1, (micrometers2 / 10) % 10);
  sleep_us(1);
  segdisplay_write_command(&disp0, SEGDISPLAY_DIGIT_2, ((millimeters2 / 1) % 10) | 0x80);
  sleep_us(1);
  segdisplay_write_command(&disp0, SEGDISPLAY_DIGIT_3, (millimeters2 / 10) % 10);
  sleep_us(1);
  segdisplay_write_command(&disp0, SEGDISPLAY_DIGIT_4, (millimeters2 / 100) % 10);
  sleep_us(1);
  segdisplay_write_command(&disp0, SEGDISPLAY_DIGIT_5, (millimeters2 / 1000) % 10);
  sleep_us(1);
  segdisplay_write_command(&disp0, SEGDISPLAY_DIGIT_6, (millimeters2 / 10000) % 10);
  sleep_us(1);
  segdisplay_write_command(&disp0, SEGDISPLAY_DIGIT_7, sign2 ? 0x0F : 0x0A);
  sleep_us(1);

  segdisplay_write_command(&disp1, SEGDISPLAY_DIGIT_0, (micrometers1 / 1) % 10);
  sleep_us(1);
  segdisplay_write_command(&disp1, SEGDISPLAY_DIGIT_1, (micrometers1 / 10) % 10);
  sleep_us(1);
  segdisplay_write_command(&disp1, SEGDISPLAY_DIGIT_2, ((millimeters1 / 1) % 10) | 0x80);
  sleep_us(1);
  segdisplay_write_command(&disp1, SEGDISPLAY_DIGIT_3, (millimeters1 / 10) % 10);
  sleep_us(1);
  segdisplay_write_command(&disp1, SEGDISPLAY_DIGIT_4, (millimeters1 / 100) % 10);
  sleep_us(1);
  segdisplay_write_command(&disp1, SEGDISPLAY_DIGIT_5, (millimeters1 / 1000) % 10);
  sleep_us(1);
  segdisplay_write_command(&disp1, SEGDISPLAY_DIGIT_6, (millimeters1 / 10000) % 10);
  sleep_us(1);
  segdisplay_write_command(&disp1, SEGDISPLAY_DIGIT_7, sign1 ? 0x0F : 0x0A);
  sleep_us(1);
#endif

  sleep_ms(1);
}

int main() {
  setup();
  for (;;) {
    loop();
  }
  return 0;
}
