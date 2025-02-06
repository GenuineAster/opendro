#ifndef OPENDRO_SOFTWARE_RPI_KEYPAD_H
#define OPENDRO_SOFTWARE_RPI_KEYPAD_H

#define KEYPAD_GRID_MAX_COLUMNS 6
#define KEYPAD_GRID_MAX_ROWS 6

typedef struct ui_t ui_t;

typedef void (*keypad_button_clicked_callback)(int column, int row, void *user_ptr);

typedef struct keypad_button_t {
	keypad_button_clicked_callback callback;
	const char *button_text;
	void *user_ptr;
} keypad_button_t;

typedef struct keypad_grid_t {
	keypad_button_t buttons[KEYPAD_GRID_MAX_COLUMNS][KEYPAD_GRID_MAX_ROWS];
	int num_rows;
	int num_columns;
	int button_size_x;
	int button_size_y;
} keypad_grid_t;

void init_keypad(keypad_grid_t *keypad, int num_columns, int num_rows, int button_size_x, int button_size_y);
void draw_keypad(keypad_grid_t *keypad, ui_t *ui, int offset_x, int offset_y);

#endif // OPENDRO_SOFTWARE_RPI_KEYPAD_H
