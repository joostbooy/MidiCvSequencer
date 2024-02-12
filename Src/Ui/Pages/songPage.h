#ifndef SongPage_h
#define SongPage_h

#include "topPage.h"
#include "songUiList.h"

namespace SongPage {
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
	SongUiList songUiList;

	enum FooterOptions {
		NEW,
		EDIT_NAME,
		UNMUTE_TRACKS,
		//TRACK_LIST,
		NUM_FOOTER_OPTIONS
	};

	const char* const footer_text[NUM_FOOTER_OPTIONS] = { "NEW", "EDIT NAME", "UNMUTE TRACKS" /*, "TRACK LIST"*/};

	void clear_settings() {
		settings.song.init();
		settings.select_track_index(0);	// this will make sure we arent selecting anything invalid
	}

	void init() {

	}

	void enter() {
		ListPage::set_clear_callback(&clear_settings);
		ListPage::set_list(&songUiList);
		ListPage::enter();
	}

	void exit() {
		ListPage::exit();
	}

	void onEncoder(uint8_t id, int inc) {
		ListPage::onEncoder(id, inc);
	}

	void onButton(uint8_t id, uint8_t state) {
		ListPage::onButton(id, state);

		if (!state) {
			return;
		}

		switch (controller.button_to_function(id))
		{
		case NEW:
			ConfirmationPage::set("CREATE NEW SONG ?", [](uint8_t option) {
				if (option == ConfirmationPage::CONFIRM) {
					engine.add_request_wait(Engine::STOP);
					settings.init();
					MessagePainter::show("NEW SONG CREATED");
				}
			});
			pages.open(Pages::CONFIRMATION_PAGE);
			break;
		case EDIT_NAME:
			TextInputPage::set(settings.song.name(), settings.song.max_name_length(), "SET SONG NAME");
			pages.open(Pages::TEXT_INPUT_PAGE);
			break;
		case UNMUTE_TRACKS:
			for (int i = 0; i < settings.song.max_tracks(); ++i) {
				 settings.song.set_track_mute(i, false);
				 settings.song.set_track_solo(i, false);
			}
			MessagePainter::show("ALL TRACKS UNMUTED");
			break;
		//case TRACK_LIST:
		//	pages.open(Pages::TRACK_LIST_PAGE);
		//	break;
		default:
			break;
		}
	}

	void drawLeds() {
		ListPage::drawLeds();
		LedPainter::footer_buttons(NUM_FOOTER_OPTIONS);
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
