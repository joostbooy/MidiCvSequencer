#ifndef ConfirmationPage_h
#define ConfirmationPage_h

#include "ui.h"
#include "controller.h"
#include "pages.h"

namespace ConfirmationPage {

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
	typedef void(*Callback)(uint8_t);

	Callback callback_;
	StringBuilderBase<32>message_;

	uint16_t x = 0;
	uint16_t y = 16;
	uint16_t w = 0;
	uint16_t h = 32;

	enum Option {
		CONFIRM,
		CANCEL,
		NUM_OPTIONS
	};

	const char* const option_text[NUM_OPTIONS] = {
		"CONFIRM",
		"CANCEL"
	};

	void init() {

	}

	void enter() {

	}

	void exit() {

	}

	void set(const char* message, Callback callback) {
		message_.write(message);
		callback_ = callback;

		w = font.string_width(message_.read()) + 20;
		x = (canvas.width() - w) / 2;
	}

	void onEncoder(uint8_t id, int inc) {

	}

	void onButton(uint8_t id, uint8_t state) {
		int selected_option = controller.button_to_function(id);
		if (state >= 1 && selected_option >= 0 && selected_option < NUM_OPTIONS) {
			callback_(selected_option);
			pages.close(Pages::CONFIRMATION_PAGE);
		}
	}

	void drawLeds() {
		LedPainter::footer_buttons(Matrix::GREEN, Matrix::RED, Matrix::BLACK, Matrix::BLACK);
	}

	void msTick(uint16_t ticks) {

	}

	// Bottom to top
	void drawDisplay() {
		canvas.set_font(Font::SMALL);
		canvas.fill(x + 4, y + 4, w, h, Canvas::SUBTRACTED);
		canvas.box(x, y, w, h, Canvas::WHITE, Canvas::BLACK);
		canvas.frame(x + 4, y + 4, w - 8, h - 8, Canvas::WHITE);
		canvas.draw_text(x, y, w, h, message_.read(), Canvas::CENTER, Canvas::CENTER, Canvas::WHITE);

		WindowPainter::draw_footer(option_text, NUM_OPTIONS);
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
