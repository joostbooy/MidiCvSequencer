#ifndef TrackPage_h
#define TrackPage_h

#include "topPage.h"
#include "listPage.h"
#include "noteTrackUiList.h"
#include "chordTrackUiList.h"
#include "drumTrackUiList.h"
#include "curveTrackUiList.h"

namespace TrackPage {
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

	void clear_track();
	void update_list_type(bool force = false);

	void audition_track(Track &track) {
		MidiEvent::Event e;

		MidiEvent::set_source(&e, MidiEvent::TRACK, track.index());

		e.port = track.port();
		e.message = track.channel();
		e.tie = false;

		switch (settings.selected_track().type())
		{
		case Track::DRUM_TRACK:
			e.message |= MidiEvent::NOTE_ON;
			e.data[0] = track.drum.read(DrumTrack::DRUM_NOTE);
			e.data[1] = 100;
			engine.midiInputEngine.add_event(MidiInputEngine::SCHEDULE, e, 0, 96);
			break;
		case Track::NOTE_TRACK:
			e.message |= MidiEvent::NOTE_ON;
			e.data[0] = 60;
			e.data[1] = 100;
			engine.midiInputEngine.add_event(MidiInputEngine::SCHEDULE, e, 0, 96);
			break;
		case Track::CHORD_TRACK:
			e.message |= MidiEvent::NOTE_ON;
			e.data[0] = 60;
			e.data[1] = 100;
			engine.midiInputEngine.add_event(MidiInputEngine::SCHEDULE, e, 0, 96);
			break;
		case Track::CURVE_TRACK:
			e.message |= MidiEvent::CONTROLLER_CHANGE;
			e.data[0] = track.curve.read(CurveTrack::CC_NUMBER);
			e.data[1] = track.curve.read(CurveTrack::INIT_VALUE);
			engine.midiInputEngine.add_event(MidiInputEngine::SEND, e, 0, 0);
			break;
		default:
			break;
		}
	}

	// Footer options
	enum FooterOptions {
		SET_TYPE,
		EDIT_LABEL,
		PATTERNS,
		AUDITION,
		NUM_FOOTER_OPTIONS
	};

	const char* const footer_text[NUM_FOOTER_OPTIONS] = { "SET TYPE", "EDIT LABEL", "PATTERNS", "AUDITION"};

	// List types
	NoteTrackUiList noteTrackUiList;
	ChordTrackUiList chordTrackUiList;
	DrumTrackUiList drumTrackUiList;
	CurveTrackUiList curveTrackUiList;

	// Track create options
	enum TrackCreateOptions {
		NUM_CREATE_OPTIONS = Track::NUM_TYPES
	};

	const char* const create_text[NUM_CREATE_OPTIONS] = {
		[Track::NOTE_TRACK]		= "NOTE",
		[Track::CHORD_TRACK]	= "CHORD",
		[Track::DRUM_TRACK]		= "DRUM",
		[Track::CURVE_TRACK]	= "CURVE",
	};

	void create_track(uint8_t option, bool confirmed) {
		if (!confirmed) {
			return;
		}

		bool succes = false;
		int index = settings.selected_track_index();

		engine.suspend();
		engine.request_track_clear_wait(index);

		if (option == Track::DRUM_TRACK) {
			succes = settings.drumKit.create_track(index);
		} else {
			succes = settings.song.create_track(index, option);
		}

		engine.resume();

		if (succes) {
			settings.select_pattern(0);
			settings.select_step_item(0);
			MessagePainter::show("TRACK CREATED");
		} else {
			MessagePainter::show("FAILED !");
		}
	}

	void clear_track() {
		Track &track = settings.selected_track();

		engine.suspend();

		switch (track.type())
		{
		case Track::NOTE_TRACK:
			track.note.clear();
			break;
		case Track::CHORD_TRACK:
			track.chord.clear();
			break;
		case Track::DRUM_TRACK:
			settings.drumKit.clear_track(track.index());
			break;
		case Track::CURVE_TRACK:
			track.curve.clear();
			break;
		default:
			break;
		}

		settings.song.set_track_mute(track.index(), false);
		settings.song.set_track_solo(track.index(), false);

		engine.resume();

		MessagePainter::show("TRACK CLEARED");
	}


	UiList *selected_list() {
		switch (settings.selected_track().type())
		{
		case Track::NOTE_TRACK:		return &noteTrackUiList;
		case Track::CHORD_TRACK:	return &chordTrackUiList;
		case Track::DRUM_TRACK:		return &drumTrackUiList;
		case Track::CURVE_TRACK:	return &curveTrackUiList;
		default:
			break;
		}
		return nullptr;
	}

	void update_list_type(bool force) {
		UiList *list_ = selected_list();

		// invalid type, close all & return
		if (list_ == nullptr) {
			chapter.open(Chapter::SONG);
			return;
		}

		if (force == true || list_ != ListPage::list_) {
			UiList::Mode mode = ListPage::list_->mode();

			ListPage::set_list(list_);
			ListPage::enter();

			ListPage::list_->set_mode(mode);
		}

	}


	void init() {

	}


	void enter() {
		update_list_type(true);
		ListPage::set_clear_callback(&clear_track);
	}

	void exit() {
		ListPage::exit();
		noteTrackUiList.set_mode(UiList::SELECT);
		chordTrackUiList.set_mode(UiList::SELECT);
		drumTrackUiList.set_mode(UiList::SELECT);
		curveTrackUiList.set_mode(UiList::SELECT);
	}

	void onEncoder(uint8_t id, int inc) {
		ListPage::onEncoder(id, inc);
		update_list_type();
	}

	void onButton(uint8_t id, uint8_t state) {
		ListPage::onButton(id, state);
		update_list_type();

		if (!state) {
			return;
		}

		// mute / solo
		int index = settings.selected_track().index();

		if (id == Controller::MUTE_BUTTON) {
			settings.song.set_track_mute(index, !settings.song.track_is_muted(index));
			return;
		}

		if (id == Controller::SOLO_BUTTON) {
			settings.song.set_track_solo(index, !settings.song.track_is_solod(index));
			return;
		}

		// functions
		switch (controller.button_to_function(id))
		{
		case SET_TYPE:
			OptionListPage::set(create_text, NUM_CREATE_OPTIONS, "SET TYPE", create_track);
			pages.open(Pages::OPTION_LIST_PAGE);
			break;
		case EDIT_LABEL:
			TextInputPage::set(settings.selected_track().label(), TrackData::kMaxLabelLength, "SET TRACK LABEL");
			pages.open(Pages::TEXT_INPUT_PAGE);
			break;
		case PATTERNS:
			pages.open(Pages::PATTERN_LIST_PAGE);
			break;
		case AUDITION:
			if (engine.state() == Engine::STOPPED) {
				audition_track(settings.selected_track());
			}
		default:
			break;
		}
	}

	void drawLeds() {
		ListPage::drawLeds();
		LedPainter::set_step_encoder(settings.selected_track_index(), Matrix::GREEN);
		LedPainter::footer_buttons(NUM_FOOTER_OPTIONS);

		int index = settings.selected_track().index();
		if (settings.song.track_is_muted(index)) {
			LedPainter::set_mute(Matrix::GREEN);
		}

		if (settings.song.track_is_solod(index)) {
			LedPainter::set_solo(Matrix::GREEN);
		}
	}

	void msTick(uint16_t ticks) {

	}

	// Bottom to top
	void drawDisplay() {
		ListPage::drawDisplay();
		WindowPainter::draw_footer(footer_text, NUM_FOOTER_OPTIONS);
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
