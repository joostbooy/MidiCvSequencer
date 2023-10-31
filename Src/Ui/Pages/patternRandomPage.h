#ifndef PatternRandomPage_h
#define PatternRandomPage_h

#include "topPage.h"
#include "noteRandomPage.h"
#include "chordRandomPage.h"
#include "drumRandomPage.h"
#include "curveRandomPage.h"
#include "patternEditRandomPage.h"

namespace PatternRandomPage {
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
		PatternHeaderPage::init();

		NoteRandomPage::init();
		ChordRandomPage::init();
		DrumRandomPage::init();
		CurveRandomPage::init();
		PatternEditRandomPage::init();
	}

	void enter() {
		PatternHeaderPage::enter();

		switch (settings.selected_track().type())
		{
		case Track::NOTE_TRACK:
			NoteRandomPage::enter();
			break;
		case Track::CHORD_TRACK:
			ChordRandomPage::enter();
			break;
		case Track::DRUM_TRACK:
			DrumRandomPage::enter();
			break;
		case Track::CURVE_TRACK:
			CurveRandomPage::enter();
			break;
		default:
			break;
		}
	}

	void exit() {
		PatternHeaderPage::exit();

		switch (settings.selected_track().type())
		{
		case Track::NOTE_TRACK:
			NoteRandomPage::exit();
			break;
		case Track::CHORD_TRACK:
			ChordRandomPage::exit();
			break;
		case Track::DRUM_TRACK:
			DrumRandomPage::exit();
			break;
		case Track::CURVE_TRACK:
			CurveRandomPage::exit();
			break;
		default:
			break;
		}
	}

	void onEncoder(uint8_t id, int inc) {
		PatternHeaderPage::onEncoder(id, inc);

		switch (settings.selected_track().type())
		{
		case Track::NOTE_TRACK:
			NoteRandomPage::onEncoder(id, inc);
			break;
		case Track::CHORD_TRACK:
			ChordRandomPage::onEncoder(id, inc);
			break;
		case Track::DRUM_TRACK:
			DrumRandomPage::onEncoder(id, inc);
			break;
		case Track::CURVE_TRACK:
			CurveRandomPage::onEncoder(id, inc);
			break;
		default:
			break;
		}
	}

	void onButton(uint8_t id, uint8_t state) {
		PatternHeaderPage::onButton(id, state);

		bool shift = controller.is_pressed(Controller::SHIFT_BUTTON);
		bool mute = controller.is_pressed(Controller::MUTE_BUTTON);

		if ((!shift) || (!mute)) {
			pages.close(Pages::PATTERN_RANDOM_PAGE);
			return;
		}

		switch (settings.selected_track().type())
		{
		case Track::NOTE_TRACK:
			NoteRandomPage::onButton(id, state);
			break;
		case Track::CHORD_TRACK:
			ChordRandomPage::onButton(id, state);
			break;
		case Track::DRUM_TRACK:
			DrumRandomPage::onButton(id, state);
			break;
		case Track::CURVE_TRACK:
			CurveRandomPage::onButton(id, state);
			break;
		default:
			break;
		}
	}

	void drawLeds() {
		PatternHeaderPage::drawLeds();

		switch (settings.selected_track().type())
		{
		case Track::NOTE_TRACK:
			NoteRandomPage::drawLeds();
			break;
		case Track::CHORD_TRACK:
			ChordRandomPage::drawLeds();
			break;
		case Track::DRUM_TRACK:
			DrumRandomPage::drawLeds();
			break;
		case Track::CURVE_TRACK:
			CurveRandomPage::drawLeds();
			break;
		default:
			break;
		}
	}

	void msTick(uint16_t ticks) {
		PatternHeaderPage::msTick(ticks);

		switch (settings.selected_track().type())
		{
		case Track::NOTE_TRACK:
			NoteRandomPage::msTick(ticks);
			break;
		case Track::CHORD_TRACK:
			ChordRandomPage::msTick(ticks);
			break;
		case Track::DRUM_TRACK:
			DrumRandomPage::msTick(ticks);
			break;
		case Track::CURVE_TRACK:
			CurveRandomPage::msTick(ticks);
			break;
		default:
			break;
		}
	}

	void drawDisplay() {
		PatternHeaderPage::drawDisplay();

		switch (settings.selected_track().type())
		{
		case Track::NOTE_TRACK:
			NoteRandomPage::drawDisplay();
			break;
		case Track::CHORD_TRACK:
			ChordRandomPage::drawDisplay();
			break;
		case Track::DRUM_TRACK:
			DrumRandomPage::drawDisplay();
			break;
		case Track::CURVE_TRACK:
			CurveRandomPage::drawDisplay();
			break;
		default:
			break;
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
