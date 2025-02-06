#ifndef OPENDRO_SOFTWARE_RPI_UI_H
#define OPENDRO_SOFTWARE_RPI_UI_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_BUTTONS 128

typedef struct ui_rect_t {
	int32_t x, y, w, h;
} ui_rect_t;

enum {
	UI_BUTTON_STATE_HOVER = 1 << 0,
	UI_BUTTON_STATE_PRESSED = 1 << 1,
	UI_BUTTON_STATE_RELEASED = 1 << 2,
};

typedef struct ui_button_t {
	ui_rect_t rect;
	uint8_t button_state;
} ui_button_t;

enum {
	UI_MOUSE_STATE_LEFT_DOWN = 1 << 0,
};

typedef struct ui_t {
	ui_button_t _buttons[MAX_BUTTONS];
	uint32_t _frame_button_id;
	uint32_t _prev_frame_button_id;
	struct SDL_Renderer *_renderer;
	uint8_t _mouse_state;
	int _mouse_x;
	int _mouse_y;
	uint32_t _hover_id;
} ui_t;

void init_ui(ui_t *ui, struct SDL_Renderer *renderer);
void ui_frame(ui_t *ui);
bool ui_handle_input(ui_t *ui, union SDL_Event *event);
bool ui_button(ui_t *ui, ui_rect_t rect, const char *text);
void ui_text(ui_t *ui, ui_rect_t rect, const char *text);

#endif OPENDRO_SOFTWARE_RPI_UI_H
