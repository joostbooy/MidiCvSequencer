#ifndef PatternEditRandomPage_h
#define PatternEditRandomPage_h

#include "pages.h"
#include "PatternHeaderPage.h"

namespace PatternEditRandomPage {
	//Declarations
	void init();
	void enter();
	void exit();
	void msTick(uint16_t ticks);
	void drawDisplay();
	void updateLeds();
	void onButton(uint8_t id, uint8_t value);
	void onEncoder(uint8_t id, uint8_t state);
	const uint16_t targetFps();

	//variables
	enum EncoderFunctions {
		MIN = 1,
		MAX = 2
	};

	const int w = 100;
	const int h = 32;

	Window window = {
		.x = (canvas.width() - w) / 2,
		.y = (canvas.height() - h) / 2,
		.width = w,
		.height = h,
		.collumns = 2,
		.rows = 3,
	};


	void init() {

	}

	void enter() {

	}

	void exit() {

	}

	void draw_name(const char* name) {
		WindowPainter::text(window, name, Canvas::CENTER, Canvas::TOP, Canvas::BLACK);
	}

	void draw_min(const char* min) {
		WindowPainter::text(window.cell(0, 1), "MIN", Canvas::CENTER, Canvas::CENTER, Canvas::BLACK);
		WindowPainter::text(window.cell(0, 2), min, Canvas::CENTER, Canvas::CENTER, Canvas::BLACK);
	}

	void draw_max(const char* max) {
		WindowPainter::text(window.cell(1, 1), "MAX", Canvas::CENTER, Canvas::CENTER, Canvas::BLACK);
		WindowPainter::text(window.cell(1, 2), max, Canvas::CENTER, Canvas::CENTER, Canvas::BLACK);
	}

	void edit_random_min(int inc, bool pressed) {
		int item = settings.selected_step_item();
		int pattern = settings.selected_pattern();
		Track &track = settings.selected_track();

		switch (track.type())
		{
		case BaseTrack::NOTE_TRACK:
			track.note.edit_random_min(pattern, NoteTrack::StepItem(item), inc, pressed);
			break;
		case BaseTrack::CHORD_TRACK:
			track.chord.edit_random_min(pattern, ChordTrack::StepItem(item), inc, pressed);
			break;
		case BaseTrack::DRUM_TRACK:
			track.drum.edit_random_min(pattern, DrumTrack::StepItem(item), inc, pressed);
			break;
		case BaseTrack::CURVE_TRACK:
			track.curve.edit_random_min(pattern, CurveTrack::StepItem(item), inc, pressed);
			break;
		default:
			break;
		}
	}

	void edit_random_max(int inc, bool pressed) {
		int item = settings.selected_step_item();
		int pattern = settings.selected_pattern();
		Track &track = settings.selected_track();

		switch (track.type())
		{
		case BaseTrack::NOTE_TRACK:
			track.note.edit_random_max(pattern, NoteTrack::StepItem(item), inc, pressed);
			break;
		case BaseTrack::CHORD_TRACK:
			track.chord.edit_random_max(pattern, ChordTrack::StepItem(item), inc, pressed);
			break;
		case BaseTrack::DRUM_TRACK:
			track.drum.edit_random_max(pattern, DrumTrack::StepItem(item), inc, pressed);
			break;
		case BaseTrack::CURVE_TRACK:
			track.curve.edit_random_max(pattern, CurveTrack::StepItem(item), inc, pressed);
			break;
		default:
			break;
		}
	}

	void toggle_random_value_enable(int step) {
		Track &track = settings.selected_track();
		int pattern = settings.selected_pattern();
		int item = settings.selected_step_item();

		switch (track.type())
		{
		case BaseTrack::NOTE_TRACK:
			track.pattern.toggle_random_enable(pattern, NoteTrack::StepItem(item), step);
			break;
		case BaseTrack::CHORD_TRACK:
			track.pattern.toggle_random_enable(pattern, ChordTrack::StepItem(item), step);
			break;
		case BaseTrack::DRUM_TRACK:
			track.pattern.toggle_random_enable(pattern, DrumTrack::StepItem(item), step);
			break;
		case BaseTrack::CURVE_TRACK:
			track.pattern.toggle_random_enable(pattern, CurveTrack::StepItem(item), step);
			break;
		default:
			break;
		}
	}

	void toggle_random_clock_enable(int step) {
		Track &track = settings.selected_track();
		int pattern = settings.selected_pattern();

		switch (track.type())
		{
		case BaseTrack::NOTE_TRACK:
			track.pattern.toggle_random_enable(pattern, NoteTrack::TRIGGER, step);
			break;
		case BaseTrack::CHORD_TRACK:
			track.pattern.toggle_random_enable(pattern, ChordTrack::TRIGGER, step);
			break;
		case BaseTrack::DRUM_TRACK:
			track.pattern.toggle_random_enable(pattern, DrumTrack::TRIGGER, step);
			break;
		case BaseTrack::CURVE_TRACK:
			track.pattern.toggle_random_enable(pattern, CurveTrack::TRIGGER, step);
			break;
		default:
			break;
		}
	}

	void onEncoder(uint8_t id, int inc) {
		switch (controller.encoder_to_function(id))
		{
		case MIN:
			edit_random_min(inc, controller.encoder_is_pressed(id));
			break;
		case MAX:
			edit_random_max(inc, controller.encoder_is_pressed(id));
			break;
		default:
			break;
		}
	}

	void onButton(uint8_t id, uint8_t state) {

	}

	void drawLeds() {
		LedPainter::footer_encoders(Matrix::BLACK, Matrix::GREEN, Matrix::GREEN, Matrix::BLACK);
	}

	void msTick(uint16_t ticks) {

	}

	void drawDisplay() {
		canvas.fill(window.x - 4, window.y - 4, window.width + 8, window.height + 8, Canvas::DARK_GRAY);
		canvas.fill(window.x - 3, window.y - 3, window.width + 6, window.height + 6, Canvas::WHITE);
	}

	const uint16_t targetFps() {
		return 1000 / 16;
	}

	const Pages::EventHandlers eventHandlers = {
		&init,
		&enter,
		&exit,
		&msTick,
		&drawLeds,
		&drawDisplay,
		&onEncoder,
		&onButton,
		&targetFps,
	};

};

#endif
