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

	TrackState trackState;
	CurveTrackEngine<false>curveTrackEngine;

	const int clock_speed = ClockEngine::TRIPLET_32TH;
	const int step_duration = ClockEngine::step_duration(clock_speed);
	const float inc = float(step_w) / float(step_duration);

	void init() {
		trackState.clock.set_speed(clock_speed);
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

	void drawDisplay() {
		// draw background
		for (int i = 0; i < 16; ++i) {
			if (i % 2) {
				int x = step_w * i;
				canvas.fill(x, box_y, step_w, box_h, Canvas::LIGHT_GRAY);
			}
		}

		const int y = box_y + (box_h / 2);
		canvas.horizontal_line(box_x, y, box_w, Canvas::GRAY);

		// draw pattern
		curveTrackEngine.init(settings.selected_track_index(), &trackState);
		curveTrackEngine.reset();

		for (int step = 0; step < 16; ++step) {
			if (settings.selected_curve_track().read_step(settings.selected_pattern(), step, CurveTrack::TRIGGER)) {
				curveTrackEngine.process_step(settings.selected_pattern(), step);
			}

			int last_x = -1;
			
			for (int i = 0; i < step_duration; ++i) {
				curveTrackEngine.tick_step(false);

				int x = (i * inc) + (step * step_w);
				if (x != last_x) {
					last_x = x;
					draw_sample(x, curveTrackEngine.curr_value() * (1.f / 65535.f));
				}
			}
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
