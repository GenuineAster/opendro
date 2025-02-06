#include "ui.h"

#include <string.h>

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>

bool ui_rect_same(ui_rect_t a, ui_rect_t b) {
	return a.x == b.x && a.y == b.y && a.w == b.w && a.h == b.h;
}

void init_ui(ui_t *ui, SDL_Renderer *renderer) {
	ui->_renderer = renderer;
	ui->_prev_frame_button_id = 0;
	ui->_frame_button_id = 0;
	ui->_mouse_state = 0;
	ui->_hover_id = 0;
	ui->_mouse_x = 0;
	ui->_mouse_y = 0;
}

void ui_frame(ui_t *ui) {
	ui->_prev_frame_button_id = ui->_frame_button_id;
	ui->_frame_button_id = 0;

	for (int button = 0; button < ui->_prev_frame_button_id; ++button) {
		ui->_buttons[button].button_state = 0;
		const bool within_x = ui->_mouse_x >= ui->_buttons[button].rect.x && ui->_mouse_x <= ui->_buttons[button].rect.x + ui->_buttons[button].rect.w;
		const bool within_y = ui->_mouse_y >= ui->_buttons[button].rect.y && ui->_mouse_y <= ui->_buttons[button].rect.y + ui->_buttons[button].rect.h;
		if (within_x && within_y) {
			ui->_buttons[button].button_state |= UI_BUTTON_STATE_HOVER;
			ui->_hover_id = button;

			if (ui->_mouse_state & UI_MOUSE_STATE_LEFT_DOWN) {
				ui->_buttons[button].button_state |= UI_BUTTON_STATE_PRESSED;
			}
		}
	}
}

bool ui_handle_input(ui_t *ui, SDL_Event *event) {
	bool consumed = false;
	switch (event->common.type) {
		case SDL_EVENT_MOUSE_BUTTON_DOWN: {
			if (event->button.button != 1) {
				break;
			}
			for (int button = 0; button < ui->_prev_frame_button_id; ++button) {
				const bool within_x = event->button.x >= ui->_buttons[button].rect.x && event->button.x <= ui->_buttons[button].rect.x + ui->_buttons[button].rect.w;
				const bool within_y = event->button.y >= ui->_buttons[button].rect.y && event->button.y <= ui->_buttons[button].rect.y + ui->_buttons[button].rect.h;
				if (within_x && within_y) {
					ui->_buttons[button].button_state |= UI_BUTTON_STATE_PRESSED;
					ui->_mouse_state |= UI_MOUSE_STATE_LEFT_DOWN;
					consumed = true;
				}
			}
		} break;
		case SDL_EVENT_MOUSE_BUTTON_UP: {
			if (event->button.button != 1) {
				break;
			}

			for (int button = 0; button < ui->_prev_frame_button_id; ++button) {
				const bool within_x = event->button.x >= ui->_buttons[button].rect.x && event->button.x <= ui->_buttons[button].rect.x + ui->_buttons[button].rect.w;
				const bool within_y = event->button.y >= ui->_buttons[button].rect.y && event->button.y <= ui->_buttons[button].rect.y + ui->_buttons[button].rect.h;
				if (within_x && within_y) {
					consumed = true;

					if (ui->_mouse_state & UI_MOUSE_STATE_LEFT_DOWN) {
						ui->_buttons[button].button_state |= UI_BUTTON_STATE_RELEASED;
					}
				}
			}

			ui->_mouse_state &= ~UI_MOUSE_STATE_LEFT_DOWN;
		} break;
		case SDL_EVENT_MOUSE_MOTION: {
			ui->_mouse_x = event->motion.x;
			ui->_mouse_y = event->motion.y;

			for (int button = 0; button < ui->_prev_frame_button_id; ++button) {
				const bool within_x = event->motion.x >= ui->_buttons[button].rect.x && event->motion.x <= ui->_buttons[button].rect.x + ui->_buttons[button].rect.w;
				const bool within_y = event->motion.y >= ui->_buttons[button].rect.y && event->motion.y <= ui->_buttons[button].rect.y + ui->_buttons[button].rect.h;
				if (within_x && within_y) {
					if (ui->_hover_id != button) {
						ui->_buttons[button].button_state |= UI_BUTTON_STATE_HOVER;
						ui->_buttons[ui->_hover_id].button_state &= ~UI_BUTTON_STATE_HOVER;
						ui->_hover_id = button;
						ui->_mouse_state &= ~UI_MOUSE_STATE_LEFT_DOWN;
					}
				}
			}
		} break;
	}

	return consumed;
}

bool ui_button(ui_t *ui, ui_rect_t rect, const char *text) {
	ui_button_t *button = &ui->_buttons[ui->_frame_button_id++];
	if ( ! ui_rect_same(button->rect, rect)) {
		// different button, discard state
		button->button_state = 0;
	}

	button->rect = rect;

	if (button->button_state & UI_BUTTON_STATE_RELEASED) {
		SDL_SetRenderDrawColor(ui->_renderer, 105, 105, 105, 255);
	} else if (button->button_state & UI_BUTTON_STATE_PRESSED) {
		SDL_SetRenderDrawColor(ui->_renderer, 85, 85, 85, 255);
	} else if (button->button_state & UI_BUTTON_STATE_HOVER) {
		SDL_SetRenderDrawColor(ui->_renderer, 155, 155, 155, 255);
	} else {
		SDL_SetRenderDrawColor(ui->_renderer, 125, 125, 125, 255);
	}

	SDL_FRect sdl_rect = {.x = rect.x, .y = rect.y, .w = rect.w, .h = rect.h};
	SDL_RenderFillRect(ui->_renderer, &sdl_rect);
	SDL_SetRenderDrawColor(ui->_renderer, 185, 185, 185, 255);
	SDL_RenderRect(ui->_renderer, &sdl_rect);

	if (text != NULL) {
		SDL_SetRenderDrawColor(ui->_renderer, 0, 0, 0, 255);
		SDL_SetRenderScale(ui->_renderer, 2.f, 2.f);
		const size_t text_len = strlen(text);
		SDL_RenderDebugText(ui->_renderer, (rect.x + rect.w / 2 - text_len * SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * 2.f / 2.f) / 2.f, (rect.y + rect.h / 2 - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * 2.f / 2.f) / 2.f, text);
		SDL_SetRenderScale(ui->_renderer, 1.f, 1.f);
	}

	if (button->button_state & UI_BUTTON_STATE_RELEASED) {
		return true;
	} else {
		return false;
	}
}
