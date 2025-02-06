#ifndef OPENDRO_SOFTWARE_RPI_CONFIG_H
#define OPENDRO_SOFTWARE_RPI_CONFIG_H

#include <stdint.h>
#include <stdbool.h>

typedef struct opendro_config_t {
	bool display_readout;
} opendro_config_t;

void populate_default_config(opendro_config_t *config);

#endif // OPENDRO_SOFTWARE_RPI_CONFIG_H
