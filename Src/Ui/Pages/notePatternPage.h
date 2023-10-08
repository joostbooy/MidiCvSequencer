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


	void init() {

	}

	void enter() {

	}

	void exit() {

	}

	void on_step_control(int step) {

	}

	void onEncoder(uint8_t id, int inc) {

	}

	void onButton(uint8_t id, uint8_t state) {

	}

	void drawLeds() {

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
