#ifndef PianoRollPainter_h
#define PianoRollPainter_h

#include "settings.h"
#include "canvas.h"
#include "windowPainter.h"

class PianoRollPainter {

public:

	static void init() {
		window.set_row_items_total(128);
		window.set_coll_items_total(17);
		scroll_bar_frames = 0;
		step_value_frames = 0;
	}

	static int top_note() {
		return 127 - window.row().first;
	}

	static int bottom_note() {
		return 127 - window.row().last;
	}

	static void set_last_touched_note(int note) {
		window.scroll_to_row(127 - note);
	}

	static void scroll_y(int inc) {
		if (inc > 0) {
			window.scroll_to_row(window.row().last + inc);
		} else {
			window.scroll_to_row(window.row().first + inc);
		}
	}

	static void set_step_duration(int step_duration) {
		float cell_width = window.cell(0, 0).w;
		wf = float(cell_width / step_duration);
	}

	static void draw_scrollbar() {
		if (last_top_row != window.row().first) {
			last_top_row = window.row().first;
			scroll_bar_frames = 32;
		}

		if (scroll_bar_frames > 0) {
			--scroll_bar_frames;
			draw_scroll_bar();
		}
	}

	static void draw_pattern_scrollbar(int patterns_total, int curr_pattern) {
		if (curr_pattern != curr_pattern_) {
			curr_pattern_ = curr_pattern;
			pattern_scroll_bar_frames = 32;
		}

		if (pattern_scroll_bar_frames > 0) {
			--pattern_scroll_bar_frames;
			draw_pattern_scrollbar(patterns_total, curr_pattern_);
		}
	}

	static void reset_step_value() {
		step_value_frames = 32;
	}

	static void draw_step_value(int step, const char *text) {
		if (step_value_frames > 0) {
			--step_value_frames;

			int coll = step;
			int w = window.cell(coll, 0).w;
			int h = window.cell(coll, 0).h;
			int x = window.cell(coll, 0).x;
			int y = (window.y + window.height) + 1;
			canvas.fill(x, y, w, h, Canvas::WHITE);
			canvas.draw_text(x, y, w, h, text, Canvas::CENTER, Canvas::BOTTOM);
		}
	}

	static void draw_position(int step, float phase) {
		int w =  window.coll_width;
		int x = w + (step * w) + (phase * w);
		canvas.vertical_line(x, window.y, window.height, Canvas::DARK_GRAY);
	}

	static void reset() {
		// clear note preview
		for (int i = 0; i < (128 / 32); ++i) {
			notes[i] = 0;
		}

		// background color
		canvas.fill(window.x, window.y, window.width, window.height, Canvas::BLACK);

		// step lines
		for (int i = 2; i < 17; ++i) {
			int x = window.cell(i, 0).x - 1;
			canvas.vertical_line(x, window.y, window.height, Canvas::GRAY);
		}

		// note rows & keys
		canvas.vertical_line(window.coll_width - 1, window.y, window.height, Canvas::WHITE);

		for (int i = window.row().first; i <= window.row().last; ++i) {
			int note = 127 - i;

			if (is_black_key(note)) {
				int y = window.cell(0, i).y;
				int w = window.cell(0, i).w;
				canvas.fill(window.coll_width, y, window.width, window.row_height, Canvas::DARK_GRAY);
				canvas.fill(0, y - 1, w / 2, 6, Canvas::WHITE);
			} else if ((note % 12) == 0) {
				WindowPainter::text(window.cell(0, i), UiText::note_text(note), Canvas::RIGHT, Canvas::CENTER, Canvas::LIGHT_GRAY);
			}
		}
	}

	static void draw_note(int step, MidiEvent::Event &e, int delay, int length, bool is_random) {
		// row 0 is highest note
		int note = 127 - e.data[0];
		int velo = e.data[1];
		cell = window.cell(step + 1, note);

		note_on(note);

		int y = cell.y;
		int h = cell.h - 1;
		int x = float(wf * delay) + cell.x;
		int w = float(wf * length);
		if (w < 1) {
			w = 1;
		}

		// note
		if (row_is_visible(note)) {
			canvas.box(x, y, w, h, Canvas::LIGHT_GRAY, Canvas::GRAY);
			canvas.horizontal_line(x, y + h, w, Canvas::LIGHT_GRAY);
			if (is_random) {
				draw_tiny_question_mark(x, y, w, h);
			}
		}

		// velocity
		if (velo > 0) {
			h = (cell.h / 128.0f) * velo + 1;
			y = (window.y + window.height) + (cell.h - h) + 1;
			canvas.vertical_line(x, y, h, Canvas::GRAY);
		}
	}

private:

	static Window window;
	static Window::Cell cell;

	static int last_top_row;
	static int scroll_bar_frames;
	static int last_step_value;
	static int step_value_frames;

	static int curr_pattern_;
	static int pattern_scroll_bar_frames;


	static float wf;
	static uint32_t notes[128 / 32];

	static bool is_black_key_[12];

	static bool is_black_key(int key) {
		return is_black_key_[key % 12];
	}

	static bool row_is_visible(uint8_t row) {
		return row >= window.row().first && row <= window.row().last;
	}

	static bool note_is_active(uint8_t note) {
		uint32_t reg = note / 32;
		uint32_t bit = note % 32;
		return notes[reg] & (1 << bit);
	}

	static void note_on(uint8_t note) {
		uint32_t reg = note / 32;
		uint32_t bit = note % 32;
		notes[reg] |= (1 << bit);
	}

	static void draw_scroll_bar() {
		const int w = 6;
		const int x = window.width - w;
		const int h = window.height - 2;
		const int y = window.y + 1;

		int y1 = (1.f / 127.f) * window.row().first * h;
		int y2 = (1.f / 127.f) * window.row().last * h;
		canvas.fill(x - 1, y - 1, w + 2, h + 2, Canvas::BLACK);
		canvas.fill(x, y, w , h, Canvas::WHITE);
		canvas.fill(x - 2, y + y1, w + 2, y2 - y1, Canvas::BLACK);

		for (int note = 0; note < 128; ++note) {
			if (note_is_active(note)) {
				int note_y = (1.f / 127.f) * note * h;
				canvas.fill(x + 1, note_y + y, w - 2, 1, Canvas::GRAY);
			}
		}
	}

	static void draw_horizontal_scroll_bar(int patterns_total, int curr_pattern) {
		int x = window.cell(0, 0).w;
		const int h = 6;
		const int w = canvas.width() - x - 8;
		const int y = canvas.height() - h - 1;

		int w1 = (1.f / patterns_total) * w;
		int x1 = w1 * curr_pattern;
		canvas.fill(x - 1, y - 1, w + 2, h + 2, Canvas::BLACK);
		canvas.fill(x, y, w , h, Canvas::WHITE);
		canvas.fill(x + x1, y, w1, h, Canvas::BLACK);
	}

	static void draw_tiny_question_mark(int x, int y, int w, int h) {
		canvas.get_xy_allignment(&x, &y, w, h, 2, 4, Canvas::CENTER, Canvas::CENTER);
		canvas.draw_pixel(x, y, Canvas::BLACK);
		canvas.draw_pixel(x + 1, y, Canvas::BLACK);
		canvas.draw_pixel(x + 1, y + 1, Canvas::BLACK);
		canvas.draw_pixel(x + 1, y + 3, Canvas::BLACK);
	}

};

#endif
