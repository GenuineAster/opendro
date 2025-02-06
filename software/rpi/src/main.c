#include <SDL3/SDL.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "dro.h"
#include "ui.h"

#define KEYPAD_GRID_MAX_COLUMNS 6
#define KEYPAD_GRID_MAX_ROWS 6

typedef void (*keypad_button_clicked_callback)(int column, int row, struct keypad_button_t *button);

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

void draw_keypad(ui_t *ui, keypad_grid_t *keypad, int offset_x, int offset_y) {
	for (int col = 0; col < keypad->num_columns; ++col) {
		for (int row = 0; row < keypad->num_rows; ++row) {
			ui_rect_t rect = {.x = offset_x + (keypad->button_size_x + 10.f) * col, .y = offset_y + (keypad->button_size_y + 10.f) * row, .w = keypad->button_size_x, .h = keypad->button_size_y};
			if (ui_button(ui, rect, keypad->buttons[col][row].button_text)) {
				if (keypad->buttons[col][row].callback)
					keypad->buttons[col][row].callback(col, row, &keypad->buttons[col][row]);
			}
		}
	}
}

int main(int argc, char **argv) {
	opendro_config_t config;
	populate_default_config(&config);

	dro_t dro;
	configure_default_dro(&dro, 2);

	keypad_grid_t calculator_keypad;
	init_keypad(&calculator_keypad, 5, 5, 80, 80);

	{
		calculator_keypad.buttons[0][0].button_text = "C";
		calculator_keypad.buttons[1][0].button_text = "CE";
		calculator_keypad.buttons[2][0].button_text = "DEL";
		calculator_keypad.buttons[3][0].button_text = NULL;
		calculator_keypad.buttons[4][0].button_text = "+";
		calculator_keypad.buttons[0][1].button_text = "1";
		calculator_keypad.buttons[1][1].button_text = "2";
		calculator_keypad.buttons[2][1].button_text = "3";
		calculator_keypad.buttons[3][1].button_text = NULL;
		calculator_keypad.buttons[4][1].button_text = "-";
		calculator_keypad.buttons[0][2].button_text = "4";
		calculator_keypad.buttons[1][2].button_text = "5";
		calculator_keypad.buttons[2][2].button_text = "6";
		calculator_keypad.buttons[3][2].button_text = NULL;
		calculator_keypad.buttons[4][2].button_text = "x";
		calculator_keypad.buttons[0][3].button_text = "7";
		calculator_keypad.buttons[1][3].button_text = "8";
		calculator_keypad.buttons[2][3].button_text = "9";
		calculator_keypad.buttons[3][3].button_text = NULL;
		calculator_keypad.buttons[4][3].button_text = "/";
		calculator_keypad.buttons[1][4].button_text = "0";
		calculator_keypad.buttons[2][4].button_text = ".";
	}

	if ( ! SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "Error: %s (%s)", "Failed to initialize SDL video subsystem.", SDL_GetError());
	}

	float mouse_x, mouse_y;
	SDL_GetGlobalMouseState(&mouse_x, &mouse_y);

	struct SDL_Point point = {.x = (int)mouse_x, .y = (int)mouse_y};
	SDL_DisplayID display = SDL_GetDisplayForPoint(&point);
	struct SDL_Rect display_rect;
	SDL_GetDisplayBounds(display, &display_rect);

	// in production, we want to default to a full-screen borderless window
	// struct SDL_Window *window = SDL_CreateWindow("opendro", display_rect.w, display_rect.h, SDL_WINDOW_BORDERLESS);
	// in development, that's impractical!
	struct SDL_Window *window = SDL_CreateWindow("opendro", 480, 800, 0);

	struct SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

	ui_t ui;
	init_ui(&ui, renderer);

	int window_size_x, window_size_y;
	SDL_GetWindowSize(window, &window_size_x, &window_size_y);

	bool open = true;
	while (open) {
		ui_frame(&ui);

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (ui_handle_input(&ui, &event)) {
				continue;
			}

			switch (event.common.type) {
			case SDL_EVENT_WINDOW_CLOSE_REQUESTED: {
				open = false;
			} break;
			case SDL_EVENT_WINDOW_RESIZED: {
				window_size_x = event.window.data1;
				window_size_y = event.window.data2;
			} break;
			// for testing without hardware connected
			case SDL_EVENT_KEY_DOWN: {
				switch (event.key.key) {
					case SDLK_LEFT: {
						dro.axes[0].curr_pos += 1;
					} break;
					case SDLK_RIGHT: {
						dro.axes[0].curr_pos -= 1;
					} break;
					case SDLK_UP: {
						dro.axes[1].curr_pos += 1;
					} break;
					case SDLK_DOWN: {
						dro.axes[1].curr_pos -= 1;
					} break;
				}
			}
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		char buf[12];
		float axis_text_offset = 10;
		for (int axis = 0; axis < dro.num_axes; ++axis) {
			int line_offset = 10.f;
			if (config.display_readout) {
				snprintf(buf, 12, "%s: % 08.2f", dro.axis_configs[axis].name, (float)(dro.axes[axis].curr_pos - dro.axes[axis].ref) / (float)(dro.axis_configs->divider));

				SDL_SetRenderScale(renderer, 2.f, 2.f);
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				SDL_RenderDebugText(renderer, line_offset / 2.f, axis_text_offset / 2.f, buf);
				SDL_SetRenderScale(renderer, 1.f, 1.f);

				line_offset += strlen(buf) * SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * 2.f + 10.f;
			}

			snprintf(buf, 12, "ZERO %s", dro.axis_configs[axis].name);
			ui_rect_t button_rect = {.x = line_offset, .y = axis_text_offset - 2.f, .w = 12.f * SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * 2.f, .h = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * 2.f + 4.f};
			if (ui_button(&ui, button_rect, buf)) {
				dro.axes[axis].ref = dro.axes[axis].curr_pos;
			}

			axis_text_offset += SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * 2.f + 10.f;
		}

		const float axis_skip = (10.f + SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * 2.f) * MAX_NUM_AXES + 10.f;

		draw_keypad(&ui, &calculator_keypad, 10, axis_skip);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyWindow(window);
}
