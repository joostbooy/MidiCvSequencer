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
		bool succes = false;
		Track& track = settings.selected_track();

		engine.suspend();

		switch (track.type())
		{
		case BaseTrack::NOTE_TRACK:
			succes = track.note.add_pattern();
			break;
		case BaseTrack::CHORD_TRACK:
			succes = track.chord.add_pattern();
			break;
		case BaseTrack::DRUM_TRACK:
			succes = settings.drumKit.add_pattern();
			break;
		case BaseTrack::CURVE_TRACK:
			succes = track.curve.add_pattern();
			break;
		default:
			break;
		}

		engine.resume();

		if (succes) {
			settings.select_pattern(track.num_patterns() - 1);
			MessagePainter::show("PATTERN ADDED");
		} else {
			MessagePainter::show("FAILED! NOT ENOUGH RAM");
		}
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

		MessagePainter::show("PATTERN CLEARED");
	}

	void remove_pattern(int pattern) {
		bool succes = false;
		Track &track = settings.selected_track();

		engine.suspend();

		switch (track.type())
		{
		case BaseTrack::NOTE_TRACK:
			succes = track.note.remove_pattern(pattern);
			break;
		case BaseTrack::CHORD_TRACK:
			succes = track.chord.remove_pattern(pattern);
			break;
		case BaseTrack::DRUM_TRACK:
			succes = settings.drumKit.remove_pattern(pattern);
			break;
		case BaseTrack::CURVE_TRACK:
			succes = track.curve.remove_pattern(pattern);
			break;
		default:
			break;
		}

		engine.resume();

		if (succes) {
			settings.select_pattern(pattern - 1);
			MessagePainter::show("PATTERN REMOVED");
		}
	}

	void edit_label(int pattern) {
		const int kMax = TrackData::kMaxLabelLength;
		const char* label = settings.selected_track().pattern.label(pattern);

		TextInputPage::set(label, kMax, "SET PATTERN LABEL");

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
					MessagePainter::show("PATTERN COPIED");
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
			LedPainter::set_copy(Matrix::ORANGE);
		} else {
			LedPainter::set_copy(Matrix::GREEN);
		}

		// paste
		if (clipboard.pattern_pasteable(track)) {
			LedPainter::set_paste(Matrix::GREEN);
		}

		// mute
		if (settings.song.track_is_muted(track.index())) {
			LedPainter::set_mute(Matrix::RED);
		}

		// solo
		if (settings.song.track_is_solod(track.index())) {
			LedPainter::set_solo(Matrix::RED);
		}

		// clear / delete
		LedPainter::set_clear(Matrix::GREEN);

		if (settings.selected_track().num_patterns() > 1) {
			LedPainter::set_delete(Matrix::GREEN);
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
