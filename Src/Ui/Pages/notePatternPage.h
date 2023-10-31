#ifndef NotePatternPage_h
#define NotePatternPage_h

#include "topPage.h"

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
	int last_touched_note;

	int read_step(int step, int item) {
		NoteTrack &track = settings.selected_note_track();

		int pattern = settings.selected_pattern();
		int value = track.read_step(pattern, step, NoteTrack::StepItem(item));

		if ((item == NoteTrack::NOTE) && track.read(NoteTrack::USE_SCALE)) {
			value = settings.song.scale.read_map(value);
		}

		return value;
	}

	bool step_is_randomised(int step) {
		NoteTrack &track = settings.selected_note_track();
		int pattern_index = settings.selected_pattern();
		int item = settings.selected_step_item();

		return track.pattern.random_is_enabled(pattern_index, NoteTrack::StepItem(item), step);
	}

	void draw_steps() {
		canvas.set_font(Font::SMALL);

		NoteTrack &track = settings.selected_note_track();
		int pattern = settings.selected_pattern();

		NoteTrack::StepItem item_ = NoteTrack::StepItem(settings.selected_step_item());
		int min = track.item(item_).min();
		int max = track.item(item_).max();

		for (int i = 0; i < TrackData::kMaxStepsPerPattern; ++i) {
			int value = track.read_step(pattern, i, item_);
			bool fill = track.read_step(pattern, i, NoteTrack::TRIGGER);

			if (item_ == NoteTrack::NOTE && track.read(NoteTrack::USE_SCALE) == true) {
				value = settings.song.scale.read_map(value);
			}

			painters.pattern.draw_step(i, min, max, fill, value, NoteTrack::step_value_text(item_, value));
		}

		painters.pattern.draw_separators();

	}

	void draw_notes() {
		int speed = settings.selected_note_track().clock_speed();

		painters.note.set_step_duration(ClockEngine::step_duration(speed));
		painters.note.set_last_touched_note(last_touched_note);
		painters.note.draw_background();

		for (int i = 0; i < 16; ++i) {
			if (read_step(i, NoteTrack::TRIGGER)) {
				int note = read_step(i, NoteTrack::NOTE);
				int velo = read_step(i, NoteTrack::VELOCITY);
				int delay = ClockEngine::gate_duration(speed, read_step(i, NoteTrack::DELAY));
				int length = ClockEngine::gate_duration(speed, read_step(i, NoteTrack::GATE_LENGTH));
				int random = step_is_randomised(i);
				painters.note.draw_note(i, random, note, velo, delay, length);
			}
		}

		painters.note.draw_scrollbar();
	}

	void init() {

	}

	void enter() {
		for (int i = 0; i < 16; ++i) {
			if (read_step(i, NoteTrack::TRIGGER)) {
				last_touched_note = read_step(i, NoteTrack::NOTE);
				return;
			}
		}

		last_touched_note = 60;
	}

	void exit() {

	}

	void on_step_control(int step) {
		if (read_step(step, NoteTrack::TRIGGER)) {
			last_touched_note = read_step(step, NoteTrack::NOTE);
		}
	}

	void onEncoder(uint8_t id, int inc) {
		int step = controller.encoder_to_step(id);
		if (step >= 0) {
			on_step_control(step);
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
		//draw_steps();
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
