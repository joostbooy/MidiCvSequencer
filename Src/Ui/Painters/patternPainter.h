#ifndef PatternPainter_h
#define PatternPainter_h

#include "midiOutputEngine.h"

class PatternPainter {

public:

	static void draw_background() {
		const int y = 15;
		const int h = 40;
		const int w = canvas.width() / 4;
		canvas.vertical_line((w * 1) - 1, y, h, Canvas::BLACK);
		canvas.vertical_line((w * 2) - 1, y, h, Canvas::BLACK);
		canvas.vertical_line((w * 3) - 1, y, h, Canvas::BLACK);
	}

	static void draw_step(int step, int min, int max, bool trigger, int value, const char* text) {
		const int y = 14;
		const int h = 40;
		const int w = canvas.width() / 16;
		const int bar_w = w / 3;

		int x = w * step;
		int bar_h = (1.f / (max - min)) * value * h;
		int bar_y = y + (h - bar_h);

		Canvas::Color bar_line_color = Canvas::BLACK;
		Canvas::Color bar_fill_color = trigger ? Canvas::DARK_GRAY : Canvas::WHITE;

		// bar & text
		canvas.box(x + bar_w, bar_y, bar_w, bar_h, bar_line_color, bar_fill_color);
		canvas.draw_text(x, y, w, h + 8, text, Canvas::CENTER, Canvas::BOTTOM);
	}

private:

};

#endif
