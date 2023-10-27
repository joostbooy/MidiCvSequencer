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

		// reset
		painters.curve.reset(track.read(CurveTrack::INIT_CC_VALUE));

		// draw steps
		CurveTrack::StepItem item_ = CurveTrack::StepItem(item);

		for (int i = 0; i < TrackData::kMaxStepsPerPattern; ++i) {
			int value = track.read_step(pattern, i, item_);
			int shape = track.read_step(pattern, i, CurveTrack::SHAPE);
			int cc_value = track.read_step(pattern, i, CurveTrack::CC_VALUE);
			bool trigger = track.read_step(pattern, i, CurveTrack::TRIGGER);
			painters.curve.draw_step(i, trigger, cc_value, shape, CurveTrack::step_value_text(item_, value));
		}
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
