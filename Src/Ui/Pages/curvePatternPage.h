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


	int last_y;
	float last_value;
	float curr_value;

	const int box_x = 0;
	const int box_y = 12;
	const int box_h = 40;
	const int box_w = 256;
	const int step_w = box_w / 16;

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

	}

	void msTick(uint16_t ticks) {

	}

	void draw_sample(int x, float sample) {
		int y, h;
		int curr_y = box_h - (box_h * sample);

		if (x == 0) {
			last_y = curr_y;
		}

		if (curr_y > last_y) {
			y = last_y;
			h = curr_y - last_y + 1;
		} else {
			y = curr_y;
			h = last_y - curr_y + 1;
		}

		canvas.vertical_line(x + box_x, y + box_y, h, Canvas::BLACK);

		last_y = curr_y;
	}

	void draw_step(int step, bool trigger, int value, int shape, const char *text) {
		float value_ = (1.f / 127.f) * value;
		float shape_ = (1.f / 8.f) * shape;

		last_value = curr_value;
		if (trigger) {
			curr_value = value_;
		}

		for (int i = 0; i < step_w; ++i) {
			int x = i + (step * step_w);
			float phase = Curve::bend(i * (1.f / step_w), shape_);
			float sample = Dsp::cross_fade(last_value, curr_value, phase);
			draw_sample(x, sample);
		}

		canvas.draw_text((step * step_w), box_y, step_w, 64, text, Canvas::CENTER, Canvas::BOTTOM);
	}


	void drawDisplay() {
		int item = settings.selected_step_item();
		int pattern = settings.selected_pattern();
		CurveTrack &track = settings.selected_curve_track();

		// set init value
		curr_value = (1.f / 127.f) * track.read(CurveTrack::INIT_VALUE);

		// draw background
		for (int i = 0; i < 16; ++i) {
			if (i % 2) {
				int x = step_w * i;
				canvas.fill(x, box_y, step_w, box_h, Canvas::LIGHT_GRAY);
			}
		}

		const int y = box_y + (box_h / 2);
		canvas.horizontal_line(box_x, y, box_w, Canvas::GRAY);


		// draw steps
		CurveTrack::StepItem item_ = CurveTrack::StepItem(item);

		for (int i = 0; i < TrackData::kMaxStepsPerPattern; ++i) {
			int value = track.read_step(pattern, i, item_);
			int shape = track.read_step(pattern, i, CurveTrack::SHAPE);
			int cc_value = track.read_step(pattern, i, CurveTrack::CC_VALUE);
			bool trigger = track.read_step(pattern, i, CurveTrack::TRIGGER);
			draw_step(i, trigger, cc_value, shape, CurveTrack::step_value_text(item_, value));
		}
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
