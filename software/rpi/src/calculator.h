#ifndef OPENDRO_SOFTWARE_RPI_CALCULATOR_H
#define OPENDRO_SOFTWARE_RPI_CALCULATOR_H

#include "keypad.h"

typedef struct dro_t dro_t;
typedef struct ui_t ui_t;

typedef struct calculator_t {
	dro_t *dro;
	keypad_grid_t keypad;
	float operand;
	float scratch;
} calculator_t;

void init_calculator(calculator_t *calculator, dro_t *dro);
void draw_calculator(calculator_t *calculator, ui_t *ui, int offset_x, int offset_y);

#endif // OPENDRO_SOFTWARE_RPI_CALCULATOR_H
