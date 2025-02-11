#include <segdisplay/segdisplay.h>

#ifdef OPENDRO_PLATFORM_RPI
#include <stdio.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <sys/stat.h>
#include <linux/types.h>
#include <pigpio.h>
#elif defined(OPENDRO_PLATFORM_PICO)
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#endif

bool segdisplay_init(segdisplay_t *display, int spi_num, int chip_select) {
    display->chip_select = chip_select;

#ifdef OPENDRO_PLATFORM_PICO
    spi_inst_t *spi;
    switch (spi_num) {
    case SEGDISPLAY_SPI_0: {
        gpio_set_function(19, GPIO_FUNC_SPI);
        gpio_set_function(18, GPIO_FUNC_SPI);
        gpio_set_function(16, GPIO_FUNC_SPI);
        spi = spi0;
    } break;
    case SEGDISPLAY_SPI_1: {
        gpio_set_function(15, GPIO_FUNC_SPI);
        gpio_set_function(14, GPIO_FUNC_SPI);
        gpio_set_function(13, GPIO_FUNC_SPI);
        spi = spi1;
    } break;
    }

    gpio_init(chip_select);
    gpio_set_dir(chip_select, GPIO_OUT);
    gpio_put(chip_select, true);

    spi_init(spi, 10000000);
    spi_set_format(spi, 16, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

    display->_internal = spi;
#else
    gpioInitialise();
    gpioSetMode(display->chip_select, PI_OUTPUT);
    gpioWrite(display->chip_select, 1);

    int fd = -1;
    switch (spi_num) {
        case SEGDISPLAY_SPI_0: {
            fd = open( "/dev/spidev0.0", O_RDWR);
        } break;
        case SEGDISPLAY_SPI_1: {
            fd = open("/dev/spidev1.0", O_RDWR);
        } break;
    }

    if (fd < 0) {
        return false;
    }
    
    display->_internal = (segdisplay_o*)(uint64_t)fd;

    int ret = 0;

    const uint32_t mode = 0;
    uint32_t request = mode;
    ret = ioctl(fd, SPI_IOC_WR_MODE32, &request);
    if (ret == -1) {
        return false;
    }

    ret = ioctl(fd, SPI_IOC_RD_MODE32, &request);
    if (ret == -1) {
        return false;
    }

    if (request != mode) {
        return false;
    }

    const uint32_t bits = 8;
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1) {
        return false;
    }

	const uint32_t speed = 10000000;
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1) {
        return false;
    }
	
    const uint32_t lsb_first = 0;
	ret = ioctl(fd, SPI_IOC_WR_LSB_FIRST, &lsb_first);
	if (ret == -1) {
        return false;
    }
#endif

    bool success = true;
    success = success && segdisplay_write_command(display, SEGDISPLAY_SHUT_DOWN, 0x01);
    success = success && segdisplay_write_command(display, SEGDISPLAY_TEST_DISPLAY, 0x00);
    success = success && segdisplay_write_command(display, SEGDISPLAY_DECODE_MODE, 0xFF);
    success = success && segdisplay_write_command(display, SEGDISPLAY_SCAN_LIMIT, 0x07);
    success = success && segdisplay_write_command(display, SEGDISPLAY_INTENSITY, 0x0F);
    return success;
}

bool segdisplay_write_command(segdisplay_t *display, uint8_t cmd, uint8_t arg) {
#ifdef OPENDRO_PLATFORM_PICO
    gpio_put(display->chip_select, 0);

    const uint16_t data = (cmd << 8) | arg;
    spi_write16_blocking((spi_inst_t*)display->_internal, &data, 1);

    gpio_put(display->chip_select, 1);
#else
    gpioWrite(display->chip_select, 0);
    static uint16_t data;
    data = (arg << 8) | cmd;
	int ret;
	int out_fd;
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)&data,
		.rx_buf = (unsigned long)0,
		.len = sizeof(data),
		.speed_hz = 0,
		.delay_usecs = 0,
		.bits_per_word = 0,
        .cs_change = false,
        .tx_nbits = 0,
        .rx_nbits = 0,
        .word_delay_usecs = 0,
        .pad = 0
	};

    int fd = (int)(uint64_t)display->_internal;
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1) {
        perror("Failed to send message through SPI");
        return false;
    }
    gpioWrite(display->chip_select, 1);
#endif
    return true;
}

void segdisplay_shutdown(segdisplay_t *display) {
#ifdef OPENDRO_PLATFORM_PICO
#else
    int fd = (int)(uint64_t)display->_internal;
    close(fd);
#endif
}
