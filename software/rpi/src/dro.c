#include "dro.h"

void configure_default_dro(dro_t *dro, int num_axes) {
	dro->num_axes = num_axes;

	if (num_axes >= 1) {
		dro->axis_configs[0].name = "X";
		dro->axis_configs[0].function = DRO_AXIS_FUNCTION_X;
		dro->axis_configs[0].type = DRO_AXIS_TYPE_LINEAR;
		dro->axis_configs[0].divider = 100;
		dro->axes[0].curr_pos = 0;
		dro->axes[0].ref = 0;
	}

	if (num_axes >= 2) {
		dro->axis_configs[1].name = "Y";
		dro->axis_configs[1].function = DRO_AXIS_FUNCTION_Y;
		dro->axis_configs[1].type = DRO_AXIS_TYPE_LINEAR;
		dro->axis_configs[1].divider = 100;
		dro->axes[1].curr_pos = 0;
		dro->axes[1].ref = 0;
	}

	if (num_axes >= 3) {
		dro->axis_configs[2].name = "Z";
		dro->axis_configs[2].function = DRO_AXIS_FUNCTION_Z;
		dro->axis_configs[2].type = DRO_AXIS_TYPE_LINEAR;
		dro->axis_configs[2].divider = 100;
		dro->axes[2].curr_pos = 0;
		dro->axes[2].ref = 0;
	}
}
