#ifndef DrumPainter_h
#define DrumPainter_h

#include "settings.h"
#include "canvas.h"
#include "windowPainter.h"

class DrumPainter {

public:

	static void init() {
		window.set_row_items_total(16);
		window.set_coll_items_total(18);
		track_scroll_bar_frames = 0;
		step_value_frames = 0;
	}

	static void select_track(int track) {
		if (selected_track != track) {
			selected_track = track;
			window.scroll_to_row(selected_track);
		}
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

	static void draw_track_scrollbar() {
		if (last_top_row != window.row().first) {
			last_top_row = window.row().first;
			track_scroll_bar_frames = 32;
		}

		if (track_scroll_bar_frames > 0) {
			--track_scroll_bar_frames;
			WindowPainter::vertical_scrollbar(window);
		}
	}

	static void draw_pattern_scrollbar(int patterns_total, int curr_pattern) {
		if (curr_pattern != curr_pattern_) {
			curr_pattern_ = curr_pattern;
			pattern_scroll_bar_frames = 32;
		}

		if (pattern_scroll_bar_frames > 0) {
			--pattern_scroll_bar_frames;
			draw_horizontal_scroll_bar(patterns_total, curr_pattern_);
		}
	}


	static void reset_step_value() {
		step_value_frames = 32;
	}

	static void draw_step_value(int step, const char *text) {
		if (step_value_frames > 0) {
			--step_value_frames;

			int coll = step + 1;
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
		int x = (w * 2) + (step * w) + (phase * w);
		canvas.vertical_line(x, window.y, window.height, Canvas::DARK_GRAY);
	}

	static void reset(Song &song) {
		// background color
		canvas.fill(window.x, window.y, window.width, window.height, Canvas::BLACK);

		// step lines
		for (int i = 3; i < 18; ++i) {
			int x = window.cell(i, 0).x - 1;
			canvas.vertical_line(x, window.y, window.height, Canvas::GRAY);
		}

		// rows
		canvas.fill(window.x, window.y, (window.coll_width * 2), window.height, Canvas::WHITE);

		for (int i = window.row().first; i <= window.row().last; ++i) {
			Window::Cell cell = window.cell(0, i);

			if (song.track(i).type() == Track::DRUM_TRACK) {
				WindowPainter::text(cell, song.track(i).name_small(), Canvas::LEFT, Canvas::CENTER, Canvas::DARK_GRAY);
			}

			if (i & 1) {
				canvas.fill(cell.w * 2, cell.y, window.width, cell.h, Canvas::DARK_GRAY);
			}

			if (i == selected_track) {
				canvas.fill(cell.x + 1, cell.y, (cell.w * 2) - 2, cell.h + 1, Canvas::INVERTED);
			}
		}
	}

	static void draw_note(int step, MidiEvent::Event &e, int delay, int length, bool is_random) {
		int note = e.data[0];
		int velo = e.data[1];
		cell = window.cell(step + 2, note);

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

	static int selected_track;
	static int last_top_row;
	static int track_scroll_bar_frames;
	static int pattern_scroll_bar_frames;
	static int curr_pattern_;
	static int last_step_value;
	static int step_value_frames;

	static float wf;

	static bool row_is_visible(uint8_t row) {
		return row >= window.row().first && row <= window.row().last;
	}

	static void draw_tiny_question_mark(int x, int y, int w, int h) {
		canvas.get_xy_allignment(&x, &y, w, h, 2, 4, Canvas::CENTER, Canvas::CENTER);
		canvas.draw_pixel(x, y, Canvas::BLACK);
		canvas.draw_pixel(x + 1, y, Canvas::BLACK);
		canvas.draw_pixel(x + 1, y + 1, Canvas::BLACK);
		canvas.draw_pixel(x + 1, y + 3, Canvas::BLACK);
	}

	static void draw_horizontal_scroll_bar(int items_total, int curr_item) {
		const int h = 6;
		const int x = window.cell(0, 0).w + 1;
		const int w = canvas.width() - x - 8;
		const int y = canvas.height() - h - 1;

		int bw = (1.f / items_total) * w;
		if (bw < 1) {
			bw = 1;
		}
		int bx = bw * curr_item;
		canvas.fill(x - 1, y - 1, w + 2, h + 2, Canvas::BLACK);
		canvas.fill(x, y, w , h, Canvas::WHITE);
		canvas.fill(x + bx + 1, y + 1, bw, h - 2, Canvas::BLACK);
	}
};

#endif
