#ifndef NotePatternPage_h
#define NotePatternPage_h

#include "topPage.h"
#include "pianoRollPainter.h"

namespace NotePatternPage {
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
	NoteTrackEngine<false>noteTrackEngine;

	int read_step(int pattern, int step, int item) {
		NoteTrack &track = settings.selected_note_track();

		int value = track.read_step(pattern, step, NoteTrack::StepItem(item));

		if ((item == NoteTrack::NOTE) && track.read(NoteTrack::USE_SCALE)) {
			value = settings.song.scale.read_map(value);
		}

		return value;
	}

	int read_step(int step, int item) {
		return read_step(settings.selected_pattern(), step, item);
	}

	bool step_is_randomised(int step) {
		NoteTrack &track = settings.selected_note_track();
		int pattern_index = settings.selected_pattern();
		int item = settings.selected_step_item();

		if (item == NoteTrack::PROBABILITY) {
			if (read_step(step, NoteTrack::PROBABILITY) < 7) {
				return true;
			}
		}
		return track.pattern.random_is_enabled(pattern_index, NoteTrack::StepItem(item), step);
	}

	void init() {

	}

	void enter() {
		int note = 60;

		for (int i = 0; i < 16; ++i) {
			if (read_step(i, NoteTrack::TRIGGER)) {
				note = read_step(i, NoteTrack::NOTE);
				break;
			}
		}
		PianoRollPainter::set_last_touched_note(note);
	}

	void exit() {

	}

	void on_step_control(int step) {
		if (read_step(step, NoteTrack::TRIGGER)) {
			int note = read_step(step, NoteTrack::NOTE);
			PianoRollPainter::set_last_touched_note(note);
			PianoRollPainter::reset_step_value();
			last_touched_step = step;
		}
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
			PianoRollPainter::scroll_y(pressed ? inc * 12 : inc);
		}
	}

	void onButton(uint8_t id, uint8_t state) {
		int step_button = controller.button_to_step(id);
		if (step_button >= 0) {
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

	void draw_step(int step, int curr_tick) {
		uint32_t x = noteTrackEngine.when() + curr_tick;
		uint32_t w = noteTrackEngine.length();
		PianoRollPainter::draw_note(step, trackState.event, x, w, step_is_randomised(step));
	}

	void draw_last_touched_step(int pattern_index) {
		int item = settings.selected_step_item();
		int value = read_step(pattern_index, last_touched_step, item);
		const char *text = NoteTrack::step_value_text(NoteTrack::StepItem(item), value);

		PianoRollPainter::draw_step_value(last_touched_step, text);
	}

	void drawDisplay() {
		auto track = settings.selected_note_track();
		int trk_index = settings.selected_track_index();
		int pat_index = settings.selected_pattern();
		int step_duration = ClockEngine::step_duration(track.clock_speed());

		trackState.clock.set_speed(track.clock_speed());
		noteTrackEngine.init(trk_index, &trackState);
		noteTrackEngine.reset();

		PianoRollPainter::set_step_duration(step_duration);
		PianoRollPainter::reset();

		const bool send_midi = false;

		for (int step = 0; step < 16; ++step) {
			if (track.read_step(pat_index, step, NoteTrack::TRIGGER)) {
				noteTrackEngine.process_step(pat_index, step);
				draw_step(step, 0);
			}

			for (int i = 0; i < step_duration; ++i) {
				if (noteTrackEngine.tick_step(send_midi)) {
					draw_step(step, i);
				}
			}
		}

		draw_last_touched_step(pat_index);
		PianoRollPainter::draw_scrollbar();
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
