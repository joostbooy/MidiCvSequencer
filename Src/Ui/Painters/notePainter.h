#ifndef PianoRollPainter_h
#define PianoRollPainter_h

#include "window.h"
#include "canvas.h"

class NotePainter {

public:

	void init() {
		window_.set_row_items_total(128);
		window_.set_coll_items_total(17);
	}

	void set_last_touched_note(int note) {
		window_.scroll_to_row(note);
	}

	void set_step_duration(int step_duration) {
		const float cell_w = 255.f / 17.f;
		w_scale = float(cell_w / step_duration);
	}

	Window &window() {
		return window_;
	}

	void draw_background() {
		// notes
		for (int i = window_.row().first; i <= window_.row().last; ++i) {
			int y = window_.cell(0, i).y;
			int w =  window_.cell(0, i).w;
			int h = window_.cell(0, i).h;

			canvas.fill(0, y, canvas.width(), h, i & 1 ? Canvas::BLACK : Canvas::DARK_GRAY);
			canvas.fill(0, y, w, h, Canvas::WHITE);
			canvas.draw_text(2, y, UiText::note_text(i), Canvas::BLACK);
		}

		// step lines
		for (int i = 1; i < 17; ++i) {
			int x = window_.cell(i, 0).x - 1;
			canvas.vertical_line(x, window_.y, window_.height, Canvas::WHITE);
		}
	}

	void draw_note(int step, int note, int velo, int delay, int length) {
		Window::Cell cell;

		cell = window_.cell(step + 1, note);

		int y = cell.y + 1;
		int h = cell.h - 2;
		int x = float(w_scale * delay) + cell.x;
		int w = float(w_scale * length);
		if (w < 1) {
			w = 1;
		}

		if (note >= window_.row().first && note <= window_.row().last) {
			canvas.box(x, y, w, h, Canvas::LIGHT_GRAY, Canvas::GRAY);
		}

		// velocity
		h = (cell.h / 128.0f) * velo + 1;
		y = (window_.y + window_.height) + (cell.h - h) + 1;
		canvas.vertical_line(x, y, h, Canvas::GRAY);
	}

private:

	Window window_ = {
		.x = 0,
		.y = 8,
		.width = 255,
		.height = 48,
		.collumns = 1 + 16,
		.rows = 8,
	};

	float w_scale;
};

#endif
