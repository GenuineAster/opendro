#include <SDL3/SDL.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "dro.h"
#include "ui.h"
#include "keypad.h"
#include "calculator.h"

int main(int argc, char **argv) {
	opendro_config_t config;
	populate_default_config(&config);

	dro_t dro;
	configure_default_dro(&dro, 2);

	calculator_t calculator;
	init_calculator(&calculator, &dro);

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
			int line_offset = UI_MARGIN;
			if (config.display_readout) {
				snprintf(buf, 12, "%s: % 08.2f", dro.axis_configs[axis].name, (float)(dro.axes[axis].curr_pos - dro.axes[axis].ref) / (float)(dro.axis_configs->divider));

				ui_rect_t axis_text_rect = {.x = line_offset, .y = axis_text_offset};
				ui_text(&ui, axis_text_rect, buf, 2.f);

				line_offset += strlen(buf) * SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * 2.f + UI_MARGIN;
			}

			snprintf(buf, 7, "ZER %s", dro.axis_configs[axis].name);
			ui_rect_t button_rect = {.x = line_offset, .y = axis_text_offset - 2.f, .w = 6.f * SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * 2.f, .h = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * 2.f + 4.f};
			if (ui_button(&ui, button_rect, buf, 2.f)) {
				dro.axes[axis].ref = dro.axes[axis].curr_pos;
			}

			axis_text_offset += SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * 2.f + UI_MARGIN;
		}

		const float axis_skip = (UI_MARGIN + SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * 2.f) * MAX_NUM_AXES + UI_MARGIN;

		draw_calculator(&calculator, &ui, 10, axis_skip);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyWindow(window);
}
