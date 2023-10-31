#ifndef PatternEditPage_h
#define PatternEditPage_h

#include "topPage.h"
#include "stepEditPage.h"

namespace PatternEditPage {
	//Declarations
	void init();
	void enter();
	void exit();
	void msTick(uint16_t ticks);
	void drawDisplay();
	void updateLeds();
	void onButton(uint8_t id, uint8_t value);
	void onEncoder(uint8_t id, uint8_t state);
	const uint16_t targetFps();

	//variables
	int last_pattern = 0;

	void add_pattern() {
		auto& track = settings.selected_track();

		if (settings.song.available_patterns() == 0) {
			painters.message.show("FAILED! MEMORY FULL");
			return;
		}

		engine.suspend();

		switch (track.type())
		{
		case BaseTrack::NOTE_TRACK:
			track.note.add_pattern();
			break;
		case BaseTrack::CHORD_TRACK:
			track.chord.add_pattern();
			break;
		case BaseTrack::DRUM_TRACK:
			track.drum.add_pattern();
			break;
		case BaseTrack::CURVE_TRACK:
			track.curve.add_pattern();
			break;
		default:
			break;
		}

		engine.resume();

		settings.select_pattern(track.num_patterns() - 1);
		painters.message.show("PATTERN ADDED");
	}

	void clear_pattern(int pattern) {
		engine.suspend();

		auto track = settings.selected_track();

		switch (track.type())
		{
		case BaseTrack::NOTE_TRACK:
			track.note.clear_pattern(pattern);
			break;
		case BaseTrack::CHORD_TRACK:
			track.chord.clear_pattern(pattern);
			break;
		case BaseTrack::DRUM_TRACK:
			track.drum.clear_pattern(pattern);
			break;
		case BaseTrack::CURVE_TRACK:
			track.curve.clear_pattern(pattern);
			break;
		default:
			break;
		}

		engine.resume();

		painters.message.show("PATTERN CLEARED");
	}

	void remove_pattern(int pattern) {
		auto track = settings.selected_track();

		engine.suspend();

		if (track.num_patterns() == 1) {
			clear_pattern(pattern);
		} else if (track.remove_pattern(pattern)) {
			settings.select_pattern(pattern - 1);
			painters.message.show("PATTERN REMOVED");
		}

		engine.resume();
	}

	void edit_label(int pattern) {
		const int kMax = TrackData::kMaxLabelLength;
		const char* label = settings.selected_track().pattern.label(pattern);

		TextInputPage::set(label, kMax, "SET LABEL");

		pages.open(Pages::TEXT_INPUT_PAGE);
	}


	void init() {

	}

	void enter() {

	}

	void exit() {

	}

	void onEncoder(uint8_t id, int inc) {

	}

	void onButton(uint8_t id, uint8_t state) {
		int track_index = settings.selected_track_index();
		int pattern = settings.selected_pattern();
		Track& track = settings.song.track(track_index);

		if (state) {
			switch (id)
			{
			case Controller::MUTE_BUTTON:
				if (controller.is_pressed(Controller::SHIFT_BUTTON) == false) {
					settings.song.set_track_mute(track_index, !settings.song.track_is_muted(track_index));
				}
				break;
			case Controller::SOLO_BUTTON:
				if (controller.is_pressed(Controller::SHIFT_BUTTON) == false) {
					settings.song.set_track_solo(track_index, !settings.song.track_is_solod(track_index));
				}
				break;
			case Controller::COPY_BUTTON:
				if (clipboard.copy_pattern(track, pattern)) {
					painters.message.show("PATTERN COPIED");
				}
				break;
			case Controller::PASTE_BUTTON:
				if (clipboard.pattern_pasteable(track)) {

					last_pattern = settings.selected_pattern();

					ConfirmationPage::set(TopPage::str.write("OVERWRITE PATTERN ", last_pattern + 1, "?"), [](uint8_t option) {
						if (option == ConfirmationPage::CONFIRM) {
							clipboard.paste_pattern(settings.selected_track(), last_pattern);
						}
					});

					pages.open(Pages::CONFIRMATION_PAGE);
				}
				break;
			case Controller::CLEAR_BUTTON:
				last_pattern = settings.selected_pattern();

				ConfirmationPage::set(TopPage::str.write("CLEAR PATTERN ", last_pattern + 1, "?"), [](uint8_t option) {
					if (option == ConfirmationPage::CONFIRM) {
						clear_pattern(last_pattern);
					}
				});

				pages.open(Pages::CONFIRMATION_PAGE);
				break;
			case Controller::DELETE_BUTTON:
				last_pattern = settings.selected_pattern();

				ConfirmationPage::set(TopPage::str.write("DELETE PATTERN ", last_pattern + 1, "?"), [](uint8_t option) {
					if (option == ConfirmationPage::CONFIRM) {
						remove_pattern(last_pattern);
					}
				});

				pages.open(Pages::CONFIRMATION_PAGE);
				break;
			default:
				break;
			}
		}
	}

	void drawLeds() {
		Track &track = settings.selected_track();

		// copy
		if (clipboard.has_pattern()) {
			painters.leds.set_copy(Matrix::ORANGE);
		} else {
			painters.leds.set_copy(Matrix::GREEN);
		}

		// paste
		if (clipboard.pattern_pasteable(track)) {
			painters.leds.set_paste(Matrix::GREEN);
		}

		// mute
		if (settings.song.track_is_muted(track.index())) {
			painters.leds.set_mute(Matrix::RED);
		}

		// solo
		if (settings.song.track_is_solod(track.index())) {
			painters.leds.set_solo(Matrix::RED);
		}

		// clear / delete
		painters.leds.set_clear(Matrix::GREEN);

		if (settings.selected_track().num_patterns() > 1) {
			painters.leds.set_delete(Matrix::GREEN);
		}
	}

	void msTick(uint16_t ticks) {

	}

	void drawDisplay() {

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
