#include "calculator.h"

#include <SDL3/SDL_render.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include "dro.h"
#include "ui.h"
#include "calculator.h"
#include "memory_store.h"

static void reset_scratch(calculator_t *calculator) {
	calculator->scratch_index = 0;
	calculator->scratch[calculator->scratch_index] = '0';
	calculator->scratch[calculator->scratch_index+1] = 0;
}

static void input_1(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	calculator->scratch[calculator->scratch_index++] = '1';
	calculator->scratch[calculator->scratch_index] = 0;
}

static void input_2(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	calculator->scratch[calculator->scratch_index++] = '2';
	calculator->scratch[calculator->scratch_index] = 0;
}

static void input_3(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	calculator->scratch[calculator->scratch_index++] = '3';
	calculator->scratch[calculator->scratch_index] = 0;
}

static void input_4(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	calculator->scratch[calculator->scratch_index++] = '4';
	calculator->scratch[calculator->scratch_index] = 0;
}

static void input_5(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	calculator->scratch[calculator->scratch_index++] = '5';
	calculator->scratch[calculator->scratch_index] = 0;
}

static void input_6(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	calculator->scratch[calculator->scratch_index++] = '6';
	calculator->scratch[calculator->scratch_index] = 0;
}

static void input_7(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	calculator->scratch[calculator->scratch_index++] = '7';
	calculator->scratch[calculator->scratch_index] = 0;
}

static void input_8(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	calculator->scratch[calculator->scratch_index++] = '8';
	calculator->scratch[calculator->scratch_index] = 0;
}

static void input_9(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	calculator->scratch[calculator->scratch_index++] = '9';
	calculator->scratch[calculator->scratch_index] = 0;
}

static void input_0(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	calculator->scratch[calculator->scratch_index++] = '0';
	calculator->scratch[calculator->scratch_index] = 0;
}

static void input_decimal(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	calculator->scratch[calculator->scratch_index++] = '.';
	calculator->scratch[calculator->scratch_index] = 0;
}

static void clear_all(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	reset_scratch(calculator);
	calculator->operand = 0.f;
	calculator->result = 0.f;
	calculator->operation = CALCULATOR_OPERATION_NONE;
}

static void clear_entry(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	reset_scratch(calculator);
}

static void input_del(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	if (calculator->scratch_index > 0) {
		calculator->scratch[--calculator->scratch_index] = 0;
	}
}

static void submit_operation(calculator_t *calculator) {
	sscanf(calculator->scratch, "%f", &calculator->operand);
	switch (calculator->operation) {
	case CALCULATOR_OPERATION_ADD: {
		calculator->result += calculator->operand;
	} break;
	case CALCULATOR_OPERATION_SUB: {
		calculator->result -= calculator->operand;
	} break;
	case CALCULATOR_OPERATION_MUL: {
		calculator->result *= calculator->operand;
	} break;
	case CALCULATOR_OPERATION_DIV: {
		calculator->result /= calculator->operand;
	} break;
	case CALCULATOR_OPERATION_SIN: {
		calculator->result = sin(calculator->result * M_PI / 180.f);
	} break;
	case CALCULATOR_OPERATION_COS: {
		calculator->result = cos(calculator->result * M_PI / 180.f);
	} break;
	case CALCULATOR_OPERATION_TAN: {
		calculator->result = tan(calculator->result * M_PI / 180.f);
	} break;
	}
	reset_scratch(calculator);
}

static void input_op(calculator_t *calculator, uint32_t operation) {
	// only submit if user has entered data, otherwise simply override the operation
	if (calculator->scratch_index != 0) {
		submit_operation(calculator);
	}
	calculator->operation = operation;
	reset_scratch(calculator);
}

static void input_add(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	input_op(calculator, CALCULATOR_OPERATION_ADD);
}

static void input_sub(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	input_op(calculator, CALCULATOR_OPERATION_SUB);
}

static void input_mul(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	input_op(calculator, CALCULATOR_OPERATION_MUL);
}

static void input_div(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	input_op(calculator, CALCULATOR_OPERATION_DIV);
}

static void input_enter(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	input_op(calculator, CALCULATOR_OPERATION_ADD);
}

static void input_sin(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	input_op(calculator, CALCULATOR_OPERATION_SIN);
	submit_operation(calculator);
	calculator->operation = CALCULATOR_OPERATION_ADD;
}

static void input_cos(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	input_op(calculator, CALCULATOR_OPERATION_COS);
	submit_operation(calculator);
	calculator->operation = CALCULATOR_OPERATION_ADD;
}

static void input_tan(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	input_op(calculator, CALCULATOR_OPERATION_TAN);
	submit_operation(calculator);
	calculator->operation = CALCULATOR_OPERATION_ADD;
}

static void view_mem(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	calculator->calculator_menu = CALCULATOR_MENU_MEMORY;
}

void init_calculator(calculator_t *calculator, dro_t *dro, memory_store_t *memory_store) {
	calculator->dro = dro;
	calculator->memory_store = memory_store;
	calculator->operand = 0.f;
	calculator->result = 0.f;
	calculator->operation = CALCULATOR_OPERATION_ADD;
	reset_scratch(calculator);
	calculator->calculator_menu = CALCULATOR_MENU_KEYPAD;

	init_keypad(&calculator->keypad, 5, 5, 80, 80);

	// set all user pointers on keypad
	for (int col = 0; col < KEYPAD_GRID_MAX_COLUMNS; ++col) {
		for (int row = 0; row < KEYPAD_GRID_MAX_COLUMNS; ++row) {
			calculator->keypad.buttons[col][row].user_ptr = calculator;
		}
	}

	{
		calculator->keypad.buttons[0][0].button_text = "C";
		calculator->keypad.buttons[0][0].callback = clear_all;
		calculator->keypad.buttons[1][0].button_text = "CE";
		calculator->keypad.buttons[1][0].callback = clear_entry;
		calculator->keypad.buttons[2][0].button_text = "DEL";
		calculator->keypad.buttons[2][0].callback = input_del;
		calculator->keypad.buttons[3][0].button_text = NULL;
		calculator->keypad.buttons[4][0].button_text = "MEM";
		calculator->keypad.buttons[4][0].callback = view_mem;
		calculator->keypad.buttons[0][1].button_text = "1";
		calculator->keypad.buttons[0][1].callback = input_1;
		calculator->keypad.buttons[1][1].button_text = "2";
		calculator->keypad.buttons[1][1].callback = input_2;
		calculator->keypad.buttons[2][1].button_text = "3";
		calculator->keypad.buttons[2][1].callback = input_3;
		calculator->keypad.buttons[3][1].button_text = "+";
		calculator->keypad.buttons[3][1].callback = input_add;
		calculator->keypad.buttons[4][1].button_text = "-";
		calculator->keypad.buttons[4][1].callback = input_sub;
		calculator->keypad.buttons[0][2].button_text = "4";
		calculator->keypad.buttons[0][2].callback = input_4;
		calculator->keypad.buttons[1][2].button_text = "5";
		calculator->keypad.buttons[1][2].callback = input_5;
		calculator->keypad.buttons[2][2].button_text = "6";
		calculator->keypad.buttons[2][2].callback = input_6;
		calculator->keypad.buttons[3][2].button_text = "x";
		calculator->keypad.buttons[3][2].callback = input_mul;
		calculator->keypad.buttons[4][2].button_text = "/";
		calculator->keypad.buttons[4][2].callback = input_div;
		calculator->keypad.buttons[0][3].button_text = "7";
		calculator->keypad.buttons[0][3].callback = input_7;
		calculator->keypad.buttons[1][3].button_text = "8";
		calculator->keypad.buttons[1][3].callback = input_8;
		calculator->keypad.buttons[2][3].button_text = "9";
		calculator->keypad.buttons[2][3].callback = input_9;
		calculator->keypad.buttons[3][3].button_text = "SIN";
		calculator->keypad.buttons[3][3].callback = input_sin;
		calculator->keypad.buttons[4][3].button_text = "COS";
		calculator->keypad.buttons[4][3].callback = input_cos;
		calculator->keypad.buttons[1][4].button_text = "0";
		calculator->keypad.buttons[1][4].callback = input_0;
		calculator->keypad.buttons[2][4].button_text = ".";
		calculator->keypad.buttons[2][4].callback = input_decimal;
		calculator->keypad.buttons[3][4].button_text = "TAN";
		calculator->keypad.buttons[3][4].callback = input_tan;
		calculator->keypad.buttons[4][4].button_text = "ENTER";
		calculator->keypad.buttons[4][4].callback = input_enter;
	}
}

void draw_memory(calculator_t *calculator, memory_store_t *memory_store, ui_t *ui, ui_rect_t rect) {
	char buf[8];

	for (int i = 0; i < NUM_MEMORY_SLOTS; ++i) {
		// center text relative to buttons
		ui_rect_t text_rect = rect;
		text_rect.y += UI_MARGIN;

		const bool in_use = memory_slot_in_use(memory_store, i);
		const bool is_vec = memory_slot_is_vector(memory_store, i);
		if (in_use) {
			if (is_vec) {
				memory_vec_t vec;
				memory_slot_read_vector(memory_store, &vec, i);
				snprintf(buf, 8, "% 6.2f", vec.x);
			} else {
				float scalar;
				memory_slot_read_scalar(memory_store, &scalar, i);
				snprintf(buf, 8, "% 6.2f", scalar);
			}
			ui_text(ui, text_rect, buf, 3.f);
		} else {
			ui_text(ui, text_rect, "<EMPTY>", 3.f);
		}

		ui_rect_t button_rect = rect;
		button_rect.x += 7 * SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * 3.f + UI_MARGIN;
		button_rect.h = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * 2.f + UI_MARGIN * 2.f;
		button_rect.w = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * 2.f * 4 + UI_MARGIN * 2.f;

		if (in_use) {
			if (ui_button(ui, button_rect, "LOAD", 2.f)) {
				if (is_vec) {
					float scalar;
					memory_slot_read_scalar(memory_store, &scalar, i);
					calculator->scratch_index = snprintf(calculator->scratch, sizeof(calculator->scratch), "%.2f", scalar);
				} else {
					memory_vec_t vec;
					memory_slot_read_vector(memory_store, &vec, i);
					calculator->scratch_index = snprintf(calculator->scratch, sizeof(calculator->scratch), "%.2f", vec.x);
				}
			}
		}

		button_rect.x += button_rect.w + UI_MARGIN;

		if (ui_button(ui, button_rect, "STOR", 2.f)) {
			float value;
			if (calculator->scratch_index != 0) {
				sscanf(calculator->scratch, "%f", &value);
			} else {
				value = calculator->result;
			}
			memory_slot_write_scalar(memory_store, i, value);
		}

		button_rect.x += button_rect.w + UI_MARGIN;

		if (ui_button(ui, button_rect, "CLR", 2.f)) {
			memory_slot_free(memory_store, i);
		}

		rect.y += UI_MARGIN * 2.f + SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * 3.f;
	}

	ui_rect_t exit_button_rect = rect;
	exit_button_rect.w = UI_MARGIN * 2.f + 4 * SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * 2.f;
	exit_button_rect.h = UI_MARGIN * 2.f + SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * 2.f;
	if (ui_button(ui, exit_button_rect, "BACK", 2.f)) {
		calculator->calculator_menu = CALCULATOR_MENU_KEYPAD;
	}
}

void draw_calculator(calculator_t *calculator, ui_t *ui, int offset_x, int offset_y) {
	ui_rect_t rect = {.x = offset_x, .y = offset_y};

	// draw current operation
	switch (calculator->operation) {
	case CALCULATOR_OPERATION_ADD: {
		ui_rect_t add_rect = {.x = rect.x, .y = rect.y};
		ui_text(ui, add_rect, "ADD", 2.f);
	} break;
	case CALCULATOR_OPERATION_SUB: {
		// render further down the line from `add` to make it easier to visually distinguish
		// 4 = 3 characters from ADD, as well as a space
		ui_rect_t add_rect = {.x = rect.x + SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * 2.f * 4, .y = rect.y};
		ui_text(ui, add_rect, "SUB", 2.f);
	} break;
	case CALCULATOR_OPERATION_MUL: {
		// offset from sub
		ui_rect_t add_rect = {.x = rect.x + SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * 2.f * 4 * 2, .y = rect.y};
		ui_text(ui, add_rect, "MUL", 2.f);
	} break;
	case CALCULATOR_OPERATION_DIV: {
		// offset from mul
		ui_rect_t add_rect = {.x = rect.x + SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * 2.f * 4 * 3, .y = rect.y};
		ui_text(ui, add_rect, "DIV", 2.f);
	} break;
	default: break;
	}

	char buf[16];

	// always draw result, right-aligned
	int result_len = snprintf(buf, 16, "% 08.2f", calculator->result);
	ui_rect_t result_rect = {.x = ui->ui_size_x - UI_MARGIN - result_len * SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * 2.f, .y = rect.y};
	ui_text(ui, result_rect, buf, 2.f);

	rect.y += SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * 2.f + UI_MARGIN;

	// if scratch is empty, show result
	if (calculator->scratch_index == 0) {
		snprintf(buf, 16, "% 08.2f", calculator->result);
	} else {
		snprintf(buf, 16, "%s", calculator->scratch);
	}

	ui_text(ui, rect, buf, 3.f);

	rect.y += SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * 3.f + UI_MARGIN;

	switch (calculator->calculator_menu) {
		case CALCULATOR_MENU_KEYPAD: {
			draw_keypad(&calculator->keypad, ui, rect.x, rect.y);
		} break;
		case CALCULATOR_MENU_MEMORY: {
			draw_memory(calculator, calculator->memory_store, ui, rect);
		} break;
		default: break;
	}
}
