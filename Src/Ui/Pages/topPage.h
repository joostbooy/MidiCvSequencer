#ifndef TopPage_h
#define TopPage_h

#include "pages.h"
#include "canvas.h"
#include "controller.h"
#include "engine.h"

#include "windowPainter.h"
#include "ledPainter.h"
#include "textBufferPainter.h"
#include "messagePainter.h"

#include "listPage.h"
#include "pageSelectionPage.h"
#include "confirmationPage.h"

namespace TopPage {
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
	StringBuilderBase<64>str;


	void on_chapter_button(uint8_t id, bool state) {
		if (state) {
			PageSelectionPage::set_chapter(Chapter::Id(id));
			pages.open(Pages::PAGE_SELECTION_PAGE);
		} else {
			pages.close(Pages::PAGE_SELECTION_PAGE);
		}
	}

	void on_save_button() {
		if (settings.save()) {
			MessagePainter::show("SETTINGS SAVED");
		} else {
			MessagePainter::show("SAVE FAILED !");
		}
		ui.clear_que();
	}

	void on_stop_button() {
		if (settings.song.midiClock.sync_port() == MidiClock::INTERNAL) {
			if (engine.state() != Engine::STOPPED) {
				engine.add_request_wait(Engine::STOP);
			}
		}
	}

	void on_play_button() {
		if (settings.song.midiClock.sync_port() == MidiClock::INTERNAL) {
			switch (engine.state())
			{
			case Engine::STOPPED:
				engine.add_request_wait(Engine::START);
				break;
			case Engine::RUNNING:
				engine.add_request_wait(Engine::PAUSE);
				break;
			case Engine::PAUSED:
				engine.add_request_wait(Engine::CONTINUE);
				break;
			default:
				break;
			}
		}
	}

	void init() {
		canvas.set_font(Font::SMALL);
	}

	void enter() {

	}

	void exit() {

	}

	void onEncoder(uint8_t id, int inc) {

	}

	void onButton(uint8_t id, uint8_t state) {
		int chapter_index = controller.button_to_chapter(id);
		if (chapter_index >= 0) {
			on_chapter_button(chapter_index, state);
			return;
		}

		if (state > 0) {
			if (id == Controller::PLAY_BUTTON) {
				on_play_button();
				return;
			}

			if (id == Controller::STOP_BUTTON) {
				on_stop_button();
				return;
			}

			if (id == Controller::SAVE_BUTTON) {
				on_save_button();
				return;
			}
		}

	}

	void drawLeds() {
		switch (engine.state())
		{
		case Engine::STOPPED:
			LedPainter::set_play(Matrix::BLACK);
			LedPainter::set_stop(Matrix::RED);
			break;
		case Engine::PAUSED:
			LedPainter::set_play(Matrix::ORANGE);
			LedPainter::set_stop(Matrix::BLACK);
			break;
		case Engine::RUNNING:
			LedPainter::set_play(Matrix::GREEN);
			LedPainter::set_stop(Matrix::BLACK);
			break;
		default:
			break;
		}

		if (settings.has_valid_path()) {
			LedPainter::set_save(Matrix::GREEN);
		}

		LedPainter::set_shift(controller.is_pressed(Controller::SHIFT_BUTTON) ? Matrix::RED : Matrix::BLACK);
		LedPainter::paint_chapters(chapter);
	}

	void msTick(uint16_t ticks) {

	}

	// Bottom to top
	void drawDisplay() {
		MessagePainter::tick(pages.targetFps());
		MessagePainter::draw();
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
