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
	}

	static void set_last_touched_note(int note) {
		window.scroll_to_row(127 - note);
	}

	static void set_step_duration(int step_duration) {
		float cell_width = window.cell(0, 0).w;
		w_scale = float(cell_width / step_duration);
	}

	static void draw_scrollbar() {
		if (last_top_row != window.row().first) {
			last_top_row = window.row().first;
			scroll_bar_frames = 32;
		}

		if (scroll_bar_frames > 0) {
			--scroll_bar_frames;
			WindowPainter::vertical_scrollbar(window);
			//draw_preview();
		}
	}

	static void draw_background() {
		clear_notes();

		// notes
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

	static void draw_note(int step, MidiEvent::Event &e, int delay, int length) {
		// row 0 is highest note
		int note = 127 - e.data[0];
		int velo = e.data[1];
		cell = window.cell(step + 1, note);

		note_on(e, 0, 0);

		int y = cell.y + 1;
		int h = cell.h - 2;
		int x = std::ceil(float(w_scale * delay) + cell.x);
		int w = std::ceil(float(w_scale * length));
		if (w < 1) {
			w = 1;
		}

		// note
		if (note_is_visible(note)) {
			canvas.box(x, y, w, h, Canvas::LIGHT_GRAY, Canvas::GRAY);
		}

		// velocity (always show)
		h = (cell.h / 128.0f) * velo + 1;
		y = (window.y + window.height) + (cell.h - h) + 1;
		canvas.vertical_line(x, y, h, Canvas::GRAY);
	}

	void draw_tiny_question_mark(int x, int y, int w, int h) {
		canvas.get_xy_allignment(&x, &y, w, h, 2, 4, Canvas::CENTER, Canvas::CENTER);
		canvas.draw_pixel(x, y, Canvas::BLACK);
		canvas.draw_pixel(x + 1, y, Canvas::BLACK);
		canvas.draw_pixel(x + 1, y + 1, Canvas::BLACK);
		canvas.draw_pixel(x + 1, y + 3, Canvas::BLACK);
	}

private:

	static Window window;
	static Window::Cell cell;

	static int last_top_row;
	static int scroll_bar_frames;

	static float w_scale;


	static int note_x;
	static uint32_t notes[128 / 32];
	static constexpr float wf = 255.f / 17.f;

	static void clear_notes() {
		for (int i = 0; i < (128 / 32); ++i) {
			notes[i] = 0;
		}
	}

	static void note_on(MidiEvent::Event &e, int delay, int length) {
		uint8_t note = 127 - e.data[0];
		uint32_t reg = note / 32;
		uint32_t bit = note % 32;
		notes[reg] |= (1 << bit);

		//draw_note(note, delay, length, Canvas::LIGHT_GRAY);
		//draw_velocity(e.data[1], delay);
	}

	static void note_off(MidiEvent::Event &e) {
		uint8_t note = e.data[0];
		uint32_t reg = note / 32;
		uint32_t bit = note % 32;
		notes[reg] &= ~(1 << bit);
	}

	static bool note_is_visible(uint8_t note) {
		return note >= window.row().first && note <= window.row().last;
	}

	static bool note_is_active(uint8_t note) {
		uint32_t reg = note / 32;
		uint32_t bit = note % 32;
		return notes[reg] & (1 << bit);
	}

	static void draw_note(uint8_t note, int delay, int length, Canvas::Color color) {
		if (note_is_visible(note)) {
			int x = note_x;
			int y = window.cell(0, note).y + 1;
			int h = window.cell(0, note).h - 2;
			canvas.box(x + (delay * wf), y, length * wf, h, Canvas::LIGHT_GRAY, color);
		}
	}

	static void draw_velocity(uint8_t velocity, int delay) {
		int x = note_x;
		int h = (window.cell(0, 0).h / 128.f) * velocity + 1;
		int y = (window.y + window.height) + (cell.h - h) + 1;
		canvas.vertical_line(x + (delay * wf), y, h, Canvas::GRAY);
	}

	static void draw_preview() {
		const int w = 4;
		const int x = window.width - 10;
		const int h = window.height;

		canvas.fill(x, 0, w, h, Canvas::BLACK);

		for (int note = 0; note < 128; ++note) {
			if (note_is_active(note)) {
				int y = (h / note) + window.y;
				canvas.fill(x, y, w, 1, Canvas::WHITE);
			}
		}
	}

};

#endif
