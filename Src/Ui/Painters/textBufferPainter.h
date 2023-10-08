#ifndef TextBufferPainter_h
#define TextBufferPainter_h

#include "StringBuilder.h"
#include "window.h"

class TextBufferPainter {

public:

	void clear() {
		entries = 0;
		top_row = 0;
		for (int i = 0; i < kMaxRows; ++i) {
			text[i].clear();
		}
	}

	void write(const char* text_) {
		text[entries % kMaxRows].write(text_);

		++entries;
		if (entries >= kMaxRows) {
			top_row = (entries - kMaxRows) % kMaxRows;
		}
	}

	void draw() {
		canvas.set_font(Font::SMALL);

		for (int i = 0; i < kMaxRows; ++i) {
			int text_row = (top_row + i) % kMaxRows;
			cell = window.cell(0, i);
			canvas.draw_text(cell.x, cell.y, cell.w, cell.h, text[text_row].read(), Canvas::LEFT, Canvas::CENTER);
		}
	}

private:

	int entries = 0;
	int top_row = 0;
	static const int kMaxRows = 6;
	StringBuilderBase<32>text[kMaxRows];

	Window window = {
		.x = 0,
		.y = 12,
		.width = canvas.width(),
		.height = canvas.height() - 24,
		.collumns = 1,
		.rows = kMaxRows,
	};

	Window::Cell cell;
};

#endif
