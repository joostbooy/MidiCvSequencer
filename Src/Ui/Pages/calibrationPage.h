#ifndef CalibrationPage_h
#define CalibrationPage_h

#include "topPage.h"
#include "calibrationUiList.h"

namespace CalibrationPage {
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
	CalibrationUiList calibrationList;

	enum FooterOptions {
		SAVE,
		LOAD,
		CLOSE,
		NUM_FOOTER_OPTIONS
	};

	const char* const footer_text[NUM_FOOTER_OPTIONS] = { "SAVE", "LOAD", "CLOSE" };

	void clear_settings() {
		settings.calibration.init();
	}

	void init() {

	}

	void enter() {
		ListPage::set_clear_callback(&clear_settings);
		ListPage::set_list(&calibrationList);
		ListPage::enter();
		engine.cvOutputEngine.start_calibration();
	}

	void exit() {
		ListPage::exit();
		engine.cvOutputEngine.stop_calibration();
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
		case SAVE:
			ConfirmationPage::set("OVERWRITE SETTINGS ?", [](uint8_t option) {
				if (option == ConfirmationPage::CONFIRM) {
					if (settings.save_calibration()) {
						MessagePainter::show("CALIBRATION SAVED");
					} else {
						MessagePainter::show("FAILED");
					}
				}
			});
			pages.open(Pages::CONFIRMATION_PAGE);
			break;
		case LOAD:
			ConfirmationPage::set("OVERWRITE SETTINGS ?", [](uint8_t option) {
				if (option == ConfirmationPage::CONFIRM) {
					if (settings.load_calibration()) {
						MessagePainter::show("CALIBRATION LOADED");
					} else {
						MessagePainter::show("FAILED");
					}
				}
			});
			pages.open(Pages::CONFIRMATION_PAGE);
			break;
		case CLOSE:
			pages.close(Pages::CALIBRATION_PAGE);
			break;
		default:
			break;
		}
	}

	void drawLeds() {
		ListPage::drawLeds();
		LedPainter::footer_buttons(NUM_FOOTER_OPTIONS);
	}

	void msTick(uint16_t ticks) {
		ListPage::msTick(ticks);
	}

	// Bottom to top
	void drawDisplay() {
		canvas.buffer.clear();
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
