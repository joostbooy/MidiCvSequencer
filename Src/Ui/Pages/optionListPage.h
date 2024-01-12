#ifndef OptionListPage_h
#define OptionListPage_h

#include "pages.h"
#include "canvas.h"

namespace OptionListPage {
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

	// Variables
	typedef void (*Callback)(uint8_t, bool);

	enum FooterOptions {
		CONFIRM,
		CANCEL,
		NUM_FOOTER_OPTIONS
	};

	const char* const footer_text[NUM_FOOTER_OPTIONS] = { "CONFIRM", "CANCEL" };

	int cursor = 0;
	int num_options_;
	bool confirmed_;
	Callback callback_;
	StringBuilderBase<32>message_;
	const char* const* option_text_;

	const int h = 40;
	const int w = 150;

	Window window = {
		.x = (canvas.width() - w) / 2,
		.y = (canvas.height() - h) / 2,
		.width = w,
		.height = h,
		.collumns = 1,
		.rows = 4,
	};

	void set(const char* const* option_text, uint8_t num_options, const char* message, Callback callback) {
		callback_ = callback;
		num_options_ = num_options;
		option_text_ = option_text;
		message_.write(message);
		window.set_row_items_total(num_options_);
	}

	void init() {

	}

	void enter() {
		cursor = 0;
		confirmed_ = false;
		window.scroll_to_row(cursor);
	}

	void exit() {
		callback_(cursor, confirmed_);
	}

	void onEncoder(uint8_t id, int inc) {
		cursor = stmlib::clip(0, num_options_ - 1, cursor + inc);
		window.scroll_to_row(cursor);
	}

	void onButton(uint8_t id, uint8_t value) {
		if (!value) {
			return;
		}

		if (id == Controller::EDIT_BUTTON) {
			confirmed_ = true;
			pages.close(Pages::OPTION_LIST_PAGE);
			return;
		}

		switch (controller.button_to_function(id))
		{
		case CONFIRM:
			confirmed_ = true;
			pages.close(Pages::OPTION_LIST_PAGE);
			break;
		case CANCEL:
			confirmed_ = false;
			pages.close(Pages::OPTION_LIST_PAGE);
			break;
		default:
			break;
		}
	}

	void drawLeds() {
		LedPainter::footer_buttons(Matrix::GREEN, Matrix::RED, Matrix::BLACK, Matrix::BLACK);
	}

	void msTick(uint16_t ticks) {

	}

	// Bottom to top
	void drawDisplay() {
		// header
		WindowPainter::draw_header();

		WindowPainter::shadow(window, 5);

		// title
		canvas.set_font(Font::SMALL);
		canvas.fill(window.x, window.y - 8, window.width, 10, Canvas::BLACK);
		canvas.horizontal_line(window.x + 1, window.y - 9, window.width - 2, Canvas::BLACK);
		canvas.draw_text(window.x, window.y - 9, window.width, 10, message_.read(), Canvas::CENTER, Canvas::CENTER, Canvas::WHITE);

		// list
		WindowPainter::fill(window, Canvas::BLACK, Canvas::WHITE);

		for (int i = window.row().first; i <= window.row().last; ++i) {
			Canvas::Color color = i == cursor ? Canvas::BLACK : Canvas::LIGHT_GRAY;
			WindowPainter::text(window.cell(0, i), option_text_[i], Canvas::LEFT, Canvas::CENTER, color);
		}
		WindowPainter::vertical_scrollbar(window);

		// footer
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
