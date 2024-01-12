#ifndef PatternPage_h
#define PatternPage_h

#include "topPage.h"

#include "patternHeaderPage.h"
#include "PatternEditPage.h"
#include "stepEditor.h"

#include "notePatternPage.h"
#include "chordPatternPage.h"
#include "curvePatternPage.h"
#include "drumPatternPage.h"

#include "pianoRollPainter.h"

namespace PatternPage {
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


	void init() {
		PatternHeaderPage::init();
		PatternEditPage::init();
		StepEditPage::init();

		NotePatternPage::init();
		ChordPatternPage::init();
		CurvePatternPage::init();
		DrumPatternPage::init();

		PianoRollPainter::init();
		DrumPainter::init();
	}

	void enter() {
		PatternHeaderPage::enter();
		PatternEditPage::enter();
		StepEditPage::enter();

		switch (settings.selected_track().type())
		{
		case Track::NOTE_TRACK:
			NotePatternPage::enter();
			break;
		case Track::CHORD_TRACK:
			ChordPatternPage::enter();
			break;
		case Track::CURVE_TRACK:
			CurvePatternPage::enter();
			break;
		case Track::DRUM_TRACK:
			DrumPatternPage::enter();
			break;
		default:
			break;
		}
	}

	void exit() {
		PatternHeaderPage::exit();
		PatternEditPage::exit();
		StepEditPage::exit();

		switch (settings.selected_track().type())
		{
		case Track::NOTE_TRACK:
			NotePatternPage::exit();
			break;
		case Track::CHORD_TRACK:
			ChordPatternPage::exit();
			break;
		case Track::CURVE_TRACK:
			CurvePatternPage::exit();
			break;
		case Track::DRUM_TRACK:
			DrumPatternPage::exit();
			break;
		default:
			break;
		}
	}

	void onEncoder(uint8_t id, int inc) {
		PatternHeaderPage::onEncoder(id, inc);
		PatternEditPage::onEncoder(id, inc);
		StepEditPage::onEncoder(id, inc);

		switch (settings.selected_track().type())
		{
		case Track::NOTE_TRACK:
			NotePatternPage::onEncoder(id, inc);
			break;
		case Track::CHORD_TRACK:
			ChordPatternPage::onEncoder(id, inc);
			break;
		case Track::CURVE_TRACK:
			CurvePatternPage::onEncoder(id, inc);
			break;
		case Track::DRUM_TRACK:
			DrumPatternPage::onEncoder(id, inc);
			break;
		default:
			break;
		}
	}

	void onButton(uint8_t id, uint8_t state) {
		PatternHeaderPage::onButton(id, state);
		PatternEditPage::onButton(id, state);
		StepEditPage::onButton(id, state);

		switch (settings.selected_track().type())
		{
		case Track::NOTE_TRACK:
			NotePatternPage::onButton(id, state);
			break;
		case Track::CHORD_TRACK:
			ChordPatternPage::onButton(id, state);
			break;
		case Track::CURVE_TRACK:
			CurvePatternPage::onButton(id, state);
			break;
		case Track::DRUM_TRACK:
			DrumPatternPage::onButton(id, state);
			break;
		default:
			break;
		}
	}

	void drawLeds() {
		int track = settings.selected_track_index();
		int pattern = engine.trackEngine.state(track).pattern_index();
		int step = engine.trackEngine.state(track).step_index();

		// step position
		if (pattern == settings.selected_pattern()) {
			LedPainter::set_clock_position(step);
		}

		PatternHeaderPage::drawLeds();
		PatternEditPage::drawLeds();
		StepEditPage::drawLeds();

		switch (settings.selected_track().type())
		{
		case Track::NOTE_TRACK:
			NotePatternPage::drawLeds();
			break;
		case Track::CHORD_TRACK:
			ChordPatternPage::drawLeds();
			break;
		case Track::CURVE_TRACK:
			CurvePatternPage::drawLeds();
			break;
		case Track::DRUM_TRACK:
			DrumPatternPage::drawLeds();
			break;
		default:
			break;
		}
	}

	void msTick(uint16_t ticks) {
		PatternHeaderPage::msTick(ticks);
		PatternEditPage::msTick(ticks);
		StepEditPage::msTick(ticks);

		switch (settings.selected_track().type())
		{
		case Track::NOTE_TRACK:
			NotePatternPage::msTick(ticks);
			break;
		case Track::CHORD_TRACK:
			ChordPatternPage::msTick(ticks);
			break;
		case Track::CURVE_TRACK:
			CurvePatternPage::msTick(ticks);
			break;
		case Track::DRUM_TRACK:
			DrumPatternPage::msTick(ticks);
			break;
		default:
			break;
		}
	}

	void drawDisplay() {
		PatternHeaderPage::drawDisplay();
		PatternEditPage::drawDisplay();
		StepEditPage::drawDisplay();

		switch (settings.selected_track().type())
		{
		case Track::NOTE_TRACK:
			NotePatternPage::drawDisplay();
			break;
		case Track::CHORD_TRACK:
			ChordPatternPage::drawDisplay();
			break;
		case Track::CURVE_TRACK:
			CurvePatternPage::drawDisplay();
			break;
		case Track::DRUM_TRACK:
			DrumPatternPage::drawDisplay();
			break;
		default:
			break;
		}
	}

	const uint16_t targetFps() {
		switch (settings.selected_track().type())
		{
		case Track::NOTE_TRACK:
			return NotePatternPage::targetFps();
		case Track::CHORD_TRACK:
			return ChordPatternPage::targetFps();
		case Track::CURVE_TRACK:
			return CurvePatternPage::targetFps();
		case Track::DRUM_TRACK:
			return DrumPatternPage::targetFps();
		default:
			break;
		}

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
