#ifndef OPENDRO_SOFTWARE_RPI_DRO_H
#define OPENDRO_SOFTWARE_RPI_DRO_H

#include <stdint.h>

#define MAX_NUM_AXES 3

enum {
	DRO_AXIS_TYPE_LINEAR,
	DRO_AXIS_TYPE_ROTARY,
};

enum {
	DRO_AXIS_FUNCTION_X,
	DRO_AXIS_FUNCTION_Y,
	DRO_AXIS_FUNCTION_Z,
	DRO_AXIS_FUNCTION_U,
	DRO_AXIS_FUNCTION_V,
	DRO_AXIS_FUNCTION_W,
	DRO_AXIS_FUNCTION_A,
	DRO_AXIS_FUNCTION_B,
	DRO_AXIS_FUNCTION_C,
};

typedef struct dro_axis_config_t {
	uint32_t divider;
	int32_t type;
	int32_t function;
	const char *name;
} dro_axis_config_t;

typedef struct dro_axis_t {
	int32_t curr_pos;
	int32_t ref;
} dro_axis_t;

typedef struct dro_t {
	int num_axes;
	dro_axis_config_t axis_configs[MAX_NUM_AXES];
	dro_axis_t axes[MAX_NUM_AXES];
} dro_t;

void configure_default_dro(dro_t *dro, int num_axes);

#endif
