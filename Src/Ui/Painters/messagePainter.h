#ifndef MessagePainter_h
#define MessagePainter_h

#include "canvas.h"
#include "stringBuilder.h"

class MessagePainter {

public:

	inline void show(const char* text_, uint16_t duration_ms_ = 1200) {
		str.write(text_);
		w = font.string_width(str.read()) + 20;
		x = (canvas.width() - w) / 2;
		is_error = false;
		is_visible = true;
		duration_ms = duration_ms_;
	}

	inline void show_error(const char* text_, uint16_t duration_ms_ = 1200) {
		show(text_, duration_ms_);
		is_error = true;
	}

	inline void hide() {
		is_visible = false;
	}

	inline bool visible(){
		return is_visible;
	}

	inline void draw() {
		if (is_visible) {
			canvas.set_font(Font::SMALL);
			canvas.fill(x + 4, y + 4, w, h, Canvas::SUBTRACTED);
			canvas.box(x, y, w, h, Canvas::WHITE, Canvas::BLACK);
			canvas.frame(x + 4, y + 4, w - 8, h - 8, Canvas::WHITE);
			canvas.draw_text(x, y, w, h, str.read(), Canvas::CENTER, Canvas::CENTER, Canvas::WHITE);

			if (is_error) {
				canvas.draw_bitmap(x, y, w, h, Bitmap::WARNING_ICON, Canvas::CENTER, Canvas::BOTTOM);
			}
		}
	}

	inline void tick(uint16_t ticks) {
		if (duration_ms >= ticks){
			duration_ms -= ticks;
		} else {
			duration_ms = 0;
			is_visible = false;
		}
	}

private:
	bool is_error;
	bool is_visible;
	uint16_t duration_ms = 0;
	uint16_t interval_;
	StringBuilderBase<32>str;

	uint16_t x = 0;
	uint16_t y = 16;
	uint16_t w = 0;
	uint16_t h = 32;
};

#endif
