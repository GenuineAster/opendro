#include "keypad.h"

#include <stddef.h>

#include "ui.h"

void init_keypad(keypad_grid_t *keypad, int num_columns, int num_rows, int button_size_x, int button_size_y) {
	for (int col = 0; col < KEYPAD_GRID_MAX_COLUMNS; ++col) {
		for (int row = 0; row < KEYPAD_GRID_MAX_ROWS; ++row) {
			keypad->buttons[col][row].callback = NULL;
			keypad->buttons[col][row].button_text = NULL;
			keypad->buttons[col][row].user_ptr = NULL;
		}
	}

	keypad->num_columns = num_columns;
	keypad->num_rows = num_rows;
	keypad->button_size_x = button_size_x;
	keypad->button_size_y = button_size_y;
}

void draw_keypad(keypad_grid_t *keypad, ui_t *ui, int offset_x, int offset_y) {
	for (int col = 0; col < keypad->num_columns; ++col) {
		for (int row = 0; row < keypad->num_rows; ++row) {
			ui_rect_t rect = {.x = offset_x + (keypad->button_size_x + 10.f) * col, .y = offset_y + (keypad->button_size_y + 10.f) * row, .w = keypad->button_size_x, .h = keypad->button_size_y};
			if (ui_button(ui, rect, keypad->buttons[col][row].button_text)) {
				if (keypad->buttons[col][row].callback)
					keypad->buttons[col][row].callback(col, row, keypad->buttons[col][row].user_ptr);
			}
		}
	}
}
