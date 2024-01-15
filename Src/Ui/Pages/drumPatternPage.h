#ifndef DrumPatternPage_h
#define DrumPatternPage_h

#include "topPage.h"
#include "drumPainter.h"

namespace DrumPatternPage {
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
	int last_touched_step;
	TrackState trackState;
	DrumTrackEngine<false>drumTrackEngine;


	int read_step(int track_index, int step, int item) {
		DrumTrack &track = settings.song.track(track_index).drum;
		int pattern = settings.selected_pattern();
		return track.read_step(pattern, step, DrumTrack::StepItem(item));
	}

	int read_step(int step, int item) {
		return read_step(settings.selected_track_index(), step, item);
	}

	bool step_is_randomised(int track_index, int step) {
		DrumTrack &track = settings.song.track(track_index).drum;
		int pattern_index = settings.selected_pattern();
		int item = settings.selected_step_item();

		if (item == DrumTrack::PROBABILITY) {
			if (read_step(track_index, step, DrumTrack::PROBABILITY) < 7) {
				return true;
			}
		}
		return track.pattern.random_is_enabled(pattern_index, DrumTrack::StepItem(item), step);
	}

	void init() {

	}

	void enter() {
	//	DrumPainter::select_track(settings.selected_track_index());
	}

	void exit() {

	}

	void on_step_control(int step) {
		last_touched_step = step;
		DrumPainter::reset_step_value();
	}

	void onEncoder(uint8_t id, int inc) {
		int step = controller.encoder_to_step(id);
		if (step >= 0) {
			on_step_control(step);
			return;
		}

		bool shift = controller.is_pressed(Controller::SHIFT_BUTTON);
		bool pressed = controller.is_pressed(Controller::Y_ENC_PUSH);

		if (id == Controller::Y_ENC && shift == false) {
			DrumPainter::scroll_y(pressed ? inc * 10 : inc);
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

	void draw_step(int trk_index, int step, int curr_tick) {
		uint32_t x = curr_tick + drumTrackEngine.when();
		uint32_t w = drumTrackEngine.length();

		// track index to row
		trackState.event.data[0] = trk_index;

		// dont show velocity if track is not selected
		if (trk_index != settings.selected_track_index()) {
			trackState.event.data[1] = 0;
		}

		DrumPainter::draw_note(step, trackState.event, x, w, step_is_randomised(trk_index, step));
	}

	void draw_last_touched_step(int pattern_index) {
		auto &track = settings.selected_drum_track();
		int item = settings.selected_step_item();
		int value = track.read_step(pattern_index, last_touched_step, DrumTrack::StepItem(item));

		const char *text = DrumTrack::step_value_text(DrumTrack::StepItem(item), value);
		DrumPainter::draw_step_value(last_touched_step, text);
	}

	void draw_drum_track(int trk_index, int pat_index, int step_duration) {
		DrumTrack &track = settings.song.track(trk_index).drum;
		trackState.clock.set_speed(track.clock_speed());

		drumTrackEngine.init(trk_index, &trackState);
		drumTrackEngine.reset();

		const bool send_midi = false;

		for (int step = 0; step < 16; ++step) {
			if (track.read_step(pat_index, step, DrumTrack::TRIGGER)) {
				drumTrackEngine.process_step(pat_index, step);
				draw_step(trk_index, step, 0);
			}

			for (int i = 0; i < step_duration; ++i) {
				if (drumTrackEngine.tick_step(send_midi)) {
					draw_step(trk_index, step, i);
				}
			}
		}
	}

	void drawDisplay() {
		DrumPainter::select_track(settings.selected_track_index());

		auto track = settings.selected_drum_track();
		int pat_index = settings.selected_pattern();
		int step_duration = ClockEngine::step_duration(track.clock_speed());

		DrumPainter::reset(settings.song);
		DrumPainter::set_step_duration(step_duration);

		for (int i = 0; i < settings.song.max_tracks(); ++i) {
			if (settings.song.track(i).type() == Track::DRUM_TRACK) {
				draw_drum_track(i, pat_index, step_duration);
			}
		}

		draw_last_touched_step(pat_index);
		DrumPainter::draw_scrollbar();
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
