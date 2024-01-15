#ifndef ChordPatternPage_h
#define ChordPatternPage_h

#include "topPage.h"
#include "chordTrackEngine.h"

namespace ChordPatternPage {
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
	Chord chord;
	ChordTrack *track;
	TrackState trackState;
	ChordTrackEngine<false>chordTrackEngine;

	int step_duration;
	int last_touched_step;
	int last_top;
	int last_bottom;


	int read_step(int step, int item) {
		ChordTrack &track = settings.selected_chord_track();
		int pattern = settings.selected_pattern();
		return track.read_step(pattern, step, ChordTrack::StepItem(item));
	}

	void set_last_touched_chord(Chord &chord, int oct_offset) {
		int bottom = chord.note(0) + oct_offset;
		int top = chord.note(chord.size() - 1) + oct_offset;

		if (last_bottom != bottom && bottom < PianoRollPainter::bottom_note()) {
			PianoRollPainter::set_last_touched_note(bottom);
		} else if (last_top != top && top > PianoRollPainter::top_note()) {
			PianoRollPainter::set_last_touched_note(top);
		}

		last_bottom = bottom;
		last_top = top;
	}

	void build_step_chord(int step) {
		int type = read_step(step, ChordTrack::CHORD_TYPE);
		int variation = read_step(step, ChordTrack::CHORD_VARIATION);
		int inversion = read_step(step, ChordTrack::CHORD_INVERSION);
		int root_shift = read_step(step, ChordTrack::CHORD_ROOT_SHIFT);
		int oct_offset = 12 * settings.selected_chord_track().read(ChordTrack::CHORD_OCT_OFFSET);

		chord.build(type, root_shift, variation, inversion);

		set_last_touched_chord(chord, oct_offset);
		last_touched_step = step;
	}


	void init() {

	}

	void enter() {
		last_bottom = -1;
		last_top = -1;

		for (int i = 0; i < 16; ++i) {
			if (read_step(i, ChordTrack::TRIGGER)) {
				build_step_chord(i);
				return;
			}
		}
	}

	void exit() {

	}

	void on_step_control(int step) {
		if (read_step(step, ChordTrack::TRIGGER)) {
			build_step_chord(step);
			PianoRollPainter::reset_step_value();
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

		if (id == Controller::Y_ENC && shift == true) {
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

	bool step_is_randomised(int step) {
		auto &track = settings.selected_chord_track();
		int pattern_index = settings.selected_pattern();
		int item = settings.selected_step_item();

		if (item == ChordTrack::PROBABILITY) {
			if (read_step(step, ChordTrack::PROBABILITY) < 7) {
				return true;
			}
		}
		return track.pattern.random_is_enabled(pattern_index, NoteTrack::StepItem(item), step);
	}

	void draw_step(int step, int curr_tick) {
		uint32_t x = curr_tick + chordTrackEngine.when();
		uint32_t w = chordTrackEngine.length();
		bool is_random = step_is_randomised(step);
		PianoRollPainter::draw_note(step, trackState.event, x, w, is_random);
	}

	void draw_chord(int step) {
		for (int i = 0; i < chordTrackEngine.chord().size(); ++i) {
			trackState.event.data[0] = chordTrackEngine.chord().note(i) + chordTrackEngine.oct_offset();
			draw_step(step, 0);
		}
	}

	void draw_last_touched_step(int pattern_index) {
		auto track = settings.selected_chord_track();
		int item = settings.selected_step_item();
		int value = track.read_step(pattern_index, last_touched_step, ChordTrack::StepItem(item));
		const char *text = ChordTrack::step_value_text(ChordTrack::StepItem(item), value);
		PianoRollPainter::draw_step_value(last_touched_step, text);
	}

	void drawDisplay() {
		auto track = settings.selected_chord_track();
		int trk_index = settings.selected_track_index();
		int pat_index = settings.selected_pattern();
		int step_duration = ClockEngine::step_duration(track.clock_speed());

		trackState.clock.set_speed(track.clock_speed());
		chordTrackEngine.init(trk_index, &trackState);
		chordTrackEngine.reset();

		PianoRollPainter::set_step_duration(step_duration);
		PianoRollPainter::reset();

		const bool send_midi = false;

		for (int step = 0; step < 16; ++step) {
			if (track.read_step(pat_index, step, ChordTrack::TRIGGER)) {
				chordTrackEngine.process_step(pat_index, step);
				if (chordTrackEngine.arpeggiator_enabled() == false) {
					draw_chord(step);
				}
			}

			if (chordTrackEngine.arpeggiator_enabled()) {
				for (int i = 0; i < step_duration; ++i) {
					if (chordTrackEngine.tick_step(send_midi)) {
						draw_step(step, i);
					}
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
