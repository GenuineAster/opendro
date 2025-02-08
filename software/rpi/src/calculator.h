#ifndef OPENDRO_SOFTWARE_RPI_CALCULATOR_H
#define OPENDRO_SOFTWARE_RPI_CALCULATOR_H

#include "keypad.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct dro_t dro_t;
typedef struct ui_t ui_t;
typedef struct memory_store_t memory_store_t;

enum {
	CALCULATOR_OPERATION_NONE = 0,
	CALCULATOR_OPERATION_ADD,
	CALCULATOR_OPERATION_SUB,
	CALCULATOR_OPERATION_MUL,
	CALCULATOR_OPERATION_DIV,
	CALCULATOR_OPERATION_SIN,
	CALCULATOR_OPERATION_COS,
	CALCULATOR_OPERATION_TAN,
};

enum {
	CALCULATOR_MENU_KEYPAD = 0,
	CALCULATOR_MENU_MEMORY,
};

typedef struct calculator_t {
	dro_t *dro;
	memory_store_t *memory_store;
	keypad_grid_t keypad;
	float operand;
	float result;
	uint32_t operation;
	char scratch[64];
	uint32_t scratch_index;
	bool scratch_sign;
	uint32_t calculator_menu;
} calculator_t;

void init_calculator(calculator_t *calculator, dro_t *dro, memory_store_t *memory_store);
void draw_calculator(calculator_t *calculator, ui_t *ui, int offset_x, int offset_y);

#endif // OPENDRO_SOFTWARE_RPI_CALCULATOR_H
