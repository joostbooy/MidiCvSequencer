#ifndef CurvePatternPage_h
#define CurvePatternPage_h

#include "topPage.h"

namespace CurvePatternPage {
	//Declarations
	void init();
	void enter();
	void exit();
	void msTick(uint16_t ticks);
	void drawDisplay();
	void updateLeds();
	void onButton(uint8_t id, uint8_t value);
	void onEncoder(uint8_t id, int inc);
	const uint16_t targetFps();


	//variables
	void draw_step_view(int pattern, int item) {
		CurveTrack &track = settings.selected_curve_track();

		CurveTrack::StepItem item_ = CurveTrack::StepItem(item);
		int min = track.item(item_).min();
		int max = track.item(item_).max();

		for (int i = 0; i < TrackData::kMaxStepsPerPattern; ++i) {
			int value = track.read_step(pattern, i, item_);
			bool fill = track.read_step(pattern, i, CurveTrack::TRIGGER);
			painters.pattern.draw_step(i, min, max, fill, value, CurveTrack::step_value_text(item_, value));
		}

		painters.pattern.draw_separators();
	}

	void draw_steps() {
		int pattern = settings.selected_pattern();
		int item = settings.selected_step_item();
		draw_step_view(pattern, item);
	}

	void init() {

	}

	void enter() {

	}

	void exit() {

	}

	void onEncoder(uint8_t id, int inc) {

	}

	void onButton(uint8_t id, uint8_t state) {

	}

	void drawLeds() {
	//	Matrix::LedColor color;

	//	int pattern = settings.selected_pattern();
	//	CurveTrack &track = settings.selected_curve_track();

	//	for (int i = 0; i < 16; ++i) {
	//		color = track.read_step(pattern, i, CurveTrack::TRIGGER) ? Matrix::GREEN : Matrix::BLACK;
	//		painters.leds.set_step_button(i, color);
	//	}
	}

	void msTick(uint16_t ticks) {

	}

	void drawDisplay() {
		draw_steps();
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
