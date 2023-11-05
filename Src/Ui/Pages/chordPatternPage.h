#ifndef ChordPatternPage_h
#define ChordPatternPage_h

#include "topPage.h"
#include "chordTrackPainter.h"

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
	ChordTrackPainter chordTrackPainter;

	int read_step(int step, int item) {
		ChordTrack &track = settings.selected_chord_track();
		int pattern = settings.selected_pattern();
		return track.read_step(pattern, step, ChordTrack::StepItem(item));
	}

	void build_step_chord(int step) {
		int type = read_step(step, ChordTrack::CHORD_TYPE);
		int variation = read_step(step, ChordTrack::CHORD_VARIATION);
		int inversion = read_step(step, ChordTrack::CHORD_INVERSION);
		int root_shift = read_step(step, ChordTrack::CHORD_ROOT_SHIFT);
		int oct_offset = 12 * settings.selected_chord_track().read(ChordTrack::CHORD_OCT_OFFSET);

		chord.build(type, root_shift, variation, inversion);

		chordTrackPainter.set_last_touched_note(chord.note(0) + oct_offset);
	}

	void draw_step_view(int pattern, int item) {
		ChordTrack &track = settings.selected_chord_track();

		ChordTrack::StepItem item_ = ChordTrack::StepItem(item);
		int min = track.item(item_).min();
		int max = track.item(item_).max();

		for (int i = 0; i < TrackData::kMaxStepsPerPattern; ++i) {
			int value = track.read_step(pattern, i, item_);
			bool fill = track.read_step(pattern, i, ChordTrack::TRIGGER);
			painters.pattern.draw_step(i, min, max, fill, value, ChordTrack::step_value_text(item_, value));
		}

		painters.pattern.draw_separators();
	}

	void draw_steps() {
		int pattern = settings.selected_pattern();
		int item = settings.selected_step_item();
		draw_step_view(pattern, item);
	}

	void draw_notes() {
		chordTrackPainter.set_track_index(settings.selected_track_index());
		chordTrackPainter.draw_pattern(settings.selected_pattern());
	}

	void init() {

	}

	void enter() {
		for (int i = 0; i < 16; ++i) {
			if (read_step(i, NoteTrack::TRIGGER)) {
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
		}
	}

	void onEncoder(uint8_t id, int inc) {
		int step = controller.encoder_to_step(id);
		if (step >= 0) {
			on_step_control(step);
			return;
		}

		bool shift = controller.is_pressed(Controller::SHIFT_BUTTON);
		if (id == Controller::Y_ENC && shift == true) {
			chordTrackPainter.scroll_y(inc);
		}
	}

	void onButton(uint8_t id, uint8_t state) {
		int step = controller.button_to_step(id);
		if (step >= 0) {
			on_step_control(step);
		}
	}

	void drawLeds() {

	}

	void msTick(uint16_t ticks) {

	}

	void drawDisplay() {
		draw_notes();
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
