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

	int last_touched_step = 0;
	int step_value_frames = 0;

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
		step_value_frames = 0;
	}

	void exit() {

	}

	void on_step_control(int step) {
		last_touched_step = step;
		step_value_frames = 32;
	}

	void onEncoder(uint8_t id, int inc) {
		int step = controller.encoder_to_step(id);
		if (step >= 0) {
			on_step_control(step);
		}
	}

	void onButton(uint8_t id, uint8_t state) {
		int step_button = controller.button_to_step(id);
		if (state >= 1 && step_button >= 0) {
			on_step_control(step_button);
			return;
		}

		int step_encoder = controller.encoder_push_to_step(id);
		if (state >= 1 && step_encoder >= 0) {
			on_step_control(step_encoder);
			return;
		}
	}

	void drawLeds() {

	}

	void msTick(uint16_t ticks) {

	}


	void draw_step_value(int step) {
		int item = settings.selected_step_item();
		int value = settings.selected_curve_track().read_step(settings.selected_pattern(), step, CurveTrack::StepItem(item));
		const char *text = CurveTrack::step_value_text(CurveTrack::StepItem(item), value);

		int x = box_x + (step * step_w);
		int y = box_y + box_h;
		canvas.draw_text(x, y, text);
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

		if (step_value_frames) {
			--step_value_frames;
			draw_step_value(last_touched_step);
		}

		int step = engine.trackEngine.state(settings.selected_track_index()).step_index();
		float phase = engine.trackEngine.step_phase(settings.selected_track_index());
		PianoRollPainter::draw_position(step, phase);
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
