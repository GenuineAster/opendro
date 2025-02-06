#include "calculator.h"

#include <SDL3/SDL_render.h>
#include <stdio.h>

#include "dro.h"
#include "ui.h"
#include "calculator.h"

static void input_1(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	calculator->scratch *= 10;
	calculator->scratch += 1;
}

static void input_2(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	calculator->scratch *= 10;
	calculator->scratch += 2;
}

static void input_3(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	calculator->scratch *= 10;
	calculator->scratch += 3;
}

static void input_4(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	calculator->scratch *= 10;
	calculator->scratch += 4;
}

static void input_5(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	calculator->scratch *= 10;
	calculator->scratch += 5;
}

static void input_6(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	calculator->scratch *= 10;
	calculator->scratch += 6;
}

static void input_7(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	calculator->scratch *= 10;
	calculator->scratch += 7;
}

static void input_8(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	calculator->scratch *= 10;
	calculator->scratch += 8;
}

static void input_9(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	calculator->scratch *= 10;
	calculator->scratch += 9;
}

static void input_0(int column, int row, void *user_ptr) {
	calculator_t *calculator = user_ptr;
	calculator->scratch *= 10;
}

void init_calculator(calculator_t *calculator, dro_t *dro) {
	calculator->dro = dro;
	calculator->operand = 0.f;
	calculator->scratch = 0.f;

	init_keypad(&calculator->keypad, 5, 5, 80, 80);

	// set all user pointers on keypad
	for (int col = 0; col < KEYPAD_GRID_MAX_COLUMNS; ++col) {
		for (int row = 0; row < KEYPAD_GRID_MAX_COLUMNS; ++row) {
			calculator->keypad.buttons[col][row].user_ptr = calculator;
		}
	}

	{
		calculator->keypad.buttons[0][0].button_text = "C";
		calculator->keypad.buttons[1][0].button_text = "CE";
		calculator->keypad.buttons[2][0].button_text = "DEL";
		calculator->keypad.buttons[3][0].button_text = NULL;
		calculator->keypad.buttons[4][0].button_text = "+";
		calculator->keypad.buttons[0][1].button_text = "1";
		calculator->keypad.buttons[0][1].callback = input_1;
		calculator->keypad.buttons[1][1].button_text = "2";
		calculator->keypad.buttons[1][1].callback = input_2;
		calculator->keypad.buttons[2][1].button_text = "3";
		calculator->keypad.buttons[2][1].callback = input_3;
		calculator->keypad.buttons[3][1].button_text = NULL;
		calculator->keypad.buttons[4][1].button_text = "-";
		calculator->keypad.buttons[0][2].button_text = "4";
		calculator->keypad.buttons[0][2].callback = input_4;
		calculator->keypad.buttons[1][2].button_text = "5";
		calculator->keypad.buttons[1][2].callback = input_5;
		calculator->keypad.buttons[2][2].button_text = "6";
		calculator->keypad.buttons[2][2].callback = input_6;
		calculator->keypad.buttons[3][2].button_text = NULL;
		calculator->keypad.buttons[4][2].button_text = "x";
		calculator->keypad.buttons[0][3].button_text = "7";
		calculator->keypad.buttons[0][3].callback = input_7;
		calculator->keypad.buttons[1][3].button_text = "8";
		calculator->keypad.buttons[1][3].callback = input_8;
		calculator->keypad.buttons[2][3].button_text = "9";
		calculator->keypad.buttons[2][3].callback = input_9;
		calculator->keypad.buttons[3][3].button_text = NULL;
		calculator->keypad.buttons[4][3].button_text = "/";
		calculator->keypad.buttons[1][4].button_text = "0";
		calculator->keypad.buttons[1][4].callback = input_0;
		calculator->keypad.buttons[2][4].button_text = ".";
	}
}

void draw_calculator(calculator_t *calculator, ui_t *ui, int offset_x, int offset_y) {
	ui_rect_t rect = {.x = offset_x, .y = offset_y};
	char buf[16];
	snprintf(buf, 16, "% .2f", calculator->scratch);
	ui_text(ui, rect, buf);

	draw_keypad(&calculator->keypad, ui, offset_x, offset_y + SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * 2.f + 10.f);
}
