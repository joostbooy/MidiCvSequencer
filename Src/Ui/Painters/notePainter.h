#ifndef PianoRollPainter_h
#define PianoRollPainter_h

#include "window.h"
#include "windowPainter.h"
#include "canvas.h"

class NotePainter {

public:

	void init() {
		window.set_row_items_total(128);
		window.set_coll_items_total(17);
		scroll_bar_frames = 0;
	}

	void set_last_touched_note(int note) {
		window.scroll_to_row(note);
	}

	void set_step_duration(int step_duration) {
		const float cell_w = 255.f / 17.f;
		w_scale = float(cell_w / step_duration);
	}

	void draw_scrollbar() {
		if (last_top_row != window.row().first) {
			last_top_row =  window.row().first;
			scroll_bar_frames = 32;
		}

		if (scroll_bar_frames > 0) {
			--scroll_bar_frames;
			WindowPainter::vertical_scrollbar(window);
		}
	}

	void draw_note(int step, int random, int note, int velo, int delay, int length) {
		note = 127 - note; // row 0 is highest note
		cell = window.cell(step + 1, note);

		int y = cell.y + 1;
		int h = cell.h - 2;
		int x = float(w_scale * delay) + cell.x;
		int w = float(w_scale * length);
		if (w < 1) {
			w = 1;
		}

		// note
		if (note_is_visible(note)) {
			canvas.box(x, y, w, h, Canvas::LIGHT_GRAY, Canvas::GRAY);
			if (random) {
				draw_tiny_question_mark(x, y, w, h);
			}
		}

		// velocity (always show)
		h = (cell.h / 128.0f) * velo + 1;
		y = (window.y + window.height) + (cell.h - h) + 1;
		canvas.vertical_line(x, y, h, Canvas::GRAY);
	}

	void draw_background() {
		// notes (row 0 = note 127)
		for (int i = window.row().first; i <= window.row().last; ++i) {
			int y = window.cell(0, i).y;
			int w =  window.cell(0, i).w;
			int h = window.cell(0, i).h;

			canvas.fill(0, y, canvas.width(), h, i & 1 ? Canvas::BLACK : Canvas::DARK_GRAY);
			canvas.fill(0, y, w, h, Canvas::WHITE);
			canvas.draw_text(2, y, UiText::note_text(127 - i), Canvas::BLACK);
		}

		// step lines
		for (int i = 1; i < 17; ++i) {
			int x = window.cell(i, 0).x - 1;
			canvas.vertical_line(x, window.y, window.height, Canvas::WHITE);
		}
	}

private:

	Window window = {
		.x = 0,
		.y = 8,
		.width = 255,
		.height = 48,
		.collumns = 1 + 16,
		.rows = 8,
	};

	float w_scale;
	int last_top_row;
	int scroll_bar_frames;

	Window::Cell cell;

	bool note_is_visible(int note) {
		return note >= window.row().first && note <= window.row().last;
	}

	void draw_tiny_question_mark(int x, int y, int w, int h) {
		canvas.get_xy_allignment(&x, &y, w, h, 2, 4, Canvas::CENTER, Canvas::CENTER);
		canvas.draw_pixel(x, y, Canvas::BLACK);
		canvas.draw_pixel(x + 1, y, Canvas::BLACK);
		canvas.draw_pixel(x + 1, y + 1, Canvas::BLACK);
		canvas.draw_pixel(x + 1, y + 3, Canvas::BLACK);
	}
};

#endif
