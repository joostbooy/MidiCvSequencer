#ifndef PatternHeaderPage_h
#define PatternHeaderPage_h

#include "topPage.h"
#include "patternSelectionUiList.h"
#include "PatternEditPage.h"

namespace PatternHeaderPage {
	//Declarations
	void init();
	void enter();
	void exit();
	void msTick(uint16_t ticks);
	void drawDisplay();
	void updateLeds();
	void onButton(uint8_t id, uint8_t state);
	void onEncoder(uint8_t id, int inc);
	const uint16_t targetFps();

	//variables
	int follow_ticks;
	int selected_item;
	const int delay_ticks = 2000;


	enum Mode {
		SELECT,
		EDIT
	}mode;


	enum Item {
		TRACK,
		PATTERN,
		STEP_ITEM,
		FOLLOW_PATTERN,
		ADD_PATTERN,

		NUM_ITEMS
	};

	Window window = {
		.x = 0,
		.y = 0,
		.width = canvas.width(),
		.height = 12,
		.collumns = NUM_ITEMS,
		.rows = 1,
	};

	const char *step_item_text(int track_type, int step_item) {
		switch (track_type)
		{
		case BaseTrack::NOTE_TRACK:		return NoteTrack::step_item_text(NoteTrack::StepItem(step_item));
		case BaseTrack::CHORD_TRACK:	return ChordTrack::step_item_text(ChordTrack::StepItem(step_item));
		case BaseTrack::DRUM_TRACK:		return DrumTrack::step_item_text(DrumTrack::StepItem(step_item));
		case BaseTrack::CURVE_TRACK:	return CurveTrack::step_item_text(CurveTrack::StepItem(step_item));
		default:
			break;
		}
		return nullptr;
	}

	const char* item_text(int item)  {
		Track &track = settings.selected_track();

		switch (item)
		{
		case TRACK:				return track.name();
		case PATTERN:			return track.pattern.label(settings.selected_pattern());
		case STEP_ITEM:			return step_item_text(track.type(), settings.selected_step_item());
		case FOLLOW_PATTERN:	return UiText::str.write("FOLLOW ", settings.follow_pattern() ? "ON" : "OFF");
		case ADD_PATTERN:		return "ADD";
		default:
			return nullptr;
		}
	};

	void edit(int item, int inc, bool shifted = false) {
		switch (item)
		{
		case TRACK:
			settings.select_track_index(settings.selected_track_index() + inc);
			break;
		case PATTERN:
			settings.select_pattern(settings.selected_pattern() + inc);
			break;
		case STEP_ITEM:
			settings.select_step_item(settings.selected_step_item() + inc);
			break;
		case FOLLOW_PATTERN:
			settings.set_follow_pattern(inc > 0);
			break;
		default:
			break;
		}
	}


	void init() {

	}

	void enter() {
		mode = SELECT;
		follow_ticks = 0;
		selected_item = 0;
		window.scroll_to_collumn(selected_item);
	}

	void exit() {

	}

	void onEncoder(uint8_t id, int inc) {
		follow_ticks = 0;

		bool shift = controller.is_pressed(Controller::SHIFT_BUTTON);

		switch (id)
		{
		case Controller::MENU_ENC:
			if (mode == SELECT) {
				selected_item = clip(0, NUM_ITEMS - 1, selected_item + inc);
				window.scroll_to_collumn(selected_item);
			} else {
				edit(selected_item, inc, shift);
			}
			break;
		case Controller::X_ENC:
			edit(PATTERN, inc);
			break;
		case Controller::Y_ENC:
			if (controller.encoder_is_pressed(Controller::Y_ENC)) {
				edit(TRACK, inc);
			} else {
				edit(STEP_ITEM, inc);
			}
			break;
		default:
			break;
		}
	}

	void onButton(uint8_t id, uint8_t state) {
		follow_ticks = 0;

		if ((id == Controller::MENU_ENC_PUSH || id == Controller::EDIT_BUTTON) && state >= 1) {
			mode = (mode == SELECT) ? EDIT : SELECT;

			switch (selected_item)
			{
			case ADD_PATTERN:
				PatternEditPage::add_pattern();
				mode = SELECT;
				break;
			default:
				break;
			}
		}
	}

	void drawLeds() {

	}

	void msTick(uint16_t ticks) {
		follow_ticks += ticks;
		if (follow_ticks >= delay_ticks) {
			follow_ticks = delay_ticks;
		}
	}

	void refresh_follow() {
		// disabled
		if (settings.follow_pattern() == false || (engine.state() != Engine::RUNNING)) {
			return;
		}

		// time out
		if (follow_ticks < delay_ticks) {
			int frame = (1.0f / delay_ticks) * follow_ticks * 9;
			canvas.draw_bitmap(246, 10, Bitmap::TIMER, frame);
			return;
		}

		// refresh
		int track_index = settings.selected_track_index();
		int pattern_index = engine.trackEngine.state(track_index).pattern_index();
		settings.select_pattern(pattern_index);

	}

	void drawDisplay() {
		// draw list
		canvas.set_font(Font::SMALL);

		painters.window.fill(window, Canvas::BLACK, Canvas::WHITE);

		for (int i = window.coll().first; i <= window.coll().last; ++i) {
			Canvas::Color color = (selected_item == i) ? Canvas::BLACK : Canvas::LIGHT_GRAY;
			painters.window.text(window.cell(i, 0), item_text(i), Canvas::LEFT, Canvas::CENTER, color);
		}

		if (mode == EDIT) {
			painters.window.highlight(window.cell(selected_item, 0));
		}

		refresh_follow();
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
