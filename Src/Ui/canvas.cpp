#include "canvas.h"

Canvas canvas;

void Canvas::draw_bitmap(uint16_t x, uint16_t y, Bitmap::Id id, uint16_t frame) {
	set_bitmap(id);

	uint16_t shift = 0;
	uint16_t x2 = x + bitmap.width;
	uint16_t y2 = y + bitmap.height;
	const uint8_t* ptr = bitmap.pointer(frame);

	for (uint16_t xi = x; xi < x2; ++xi) {
		for (uint16_t yi = y; yi < y2; ++yi) {
			uint8_t pixel = (*ptr >> shift) & 1;
			draw_pixel(xi, yi, pixel ? BLACK : WHITE);
			if (++shift >= 8) {
				++ptr;
				shift = 0;
			}
		}

		if(shift != 0) {
			++ptr;
			shift = 0;
		}
	}
}

void Canvas::draw_char(uint16_t x, uint16_t y, char c) {
	const uint8_t* ptr = font.data(c);
	uint16_t char_width = font.width(c);
	text_cursor_ = x;

	for (int i = 0; i < char_width; ++i) {
		uint8_t data = *ptr++;
		for (uint16_t pixel = 0; pixel < font.height(); ++pixel) {
			if (data & (1 << pixel)) {
				draw_pixel(text_cursor_, y + pixel, font_color);
			}
		}
		++text_cursor_;
	}
}
