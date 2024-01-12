#ifndef ChordRandomPage_h
#define ChordRandomPage_h

#include "topPage.h"
#include "patternEditRandomPage.h"

namespace ChordRandomPage {
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

	void init() {

	}

	void enter() {
		PatternEditRandomPage::enter();
	}

	void exit() {
		PatternEditRandomPage::exit();
	}

	void onEncoder(uint8_t id, int inc) {
		PatternEditRandomPage::onEncoder(id, inc);
	}

	void onButton(uint8_t id, uint8_t state) {
		PatternEditRandomPage::onButton(id, state);

		int step = controller.button_to_step(id);

		if (step >= 0 && state >= 1) {
			if (settings.selected_step_item() != ChordTrack::TRIGGER) {
				PatternEditRandomPage::toggle_random_value_enable(step);
			} else {
				PatternEditRandomPage::toggle_random_clock_enable(step);
			}
		}
	}

	void drawLeds() {
		PatternEditRandomPage::drawLeds();

		Matrix::LedColor color;

		int item = settings.selected_step_item();
		int pattern = settings.selected_pattern();
		auto &track = settings.selected_chord_track();

		for (int i = 0; i < 16; ++i) {
			if (track.pattern.random_is_enabled(pattern, ChordTrack::StepItem(item), i)) {
				LedPainter::set_step_button(i, Matrix::ORANGE);
			} else {
				color = track.read_step(pattern, i, ChordTrack::TRIGGER) ? Matrix::GREEN : Matrix::BLACK;
				LedPainter::set_step_button(i, color);
			}
		}
	}

	void msTick(uint16_t ticks) {
		PatternEditRandomPage::msTick(ticks);
	}

	void drawDisplay() {
		PatternEditRandomPage::drawDisplay();

		canvas.set_font(Font::LARGE);

		int item = settings.selected_step_item();
		int pattern = settings.selected_pattern();
		auto &track = settings.selected_chord_track();

		if (settings.selected_step_item() == ChordTrack::TRIGGER) {
			PatternEditRandomPage::draw_name("RANDOM CLOCK");
			return;
		}

		// item name
		PatternEditRandomPage::draw_name(ChordTrack::step_item_text(ChordTrack::StepItem(item)));

		// min
		int min_value = track.read_random_min(pattern, ChordTrack::StepItem(item));
		PatternEditRandomPage::draw_min(ChordTrack::step_value_text(ChordTrack::StepItem(item), min_value));

		// max
		int max_value = track.read_random_max(pattern, ChordTrack::StepItem(item));
		PatternEditRandomPage::draw_max(ChordTrack::step_value_text(ChordTrack::StepItem(item), max_value));
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
