#ifndef CheckListPage_h
#define CheckListPage_h

#include "pages.h"
#include "canvas.h"

namespace CheckListPage {
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
	enum FooterOptions {
		CONFIRM,
		SET_ALL,
		CLEAR_ALL,
		CANCEL,
		NUM_FOOTER_OPTIONS
	};

	const char* const footer_text[NUM_FOOTER_OPTIONS] = {
		"CONFIRM",
		"SET ALL",
		"CLEAR ALL",
		"CANCEL"
	};

	const char* const* list_text;
	const uint8_t kMaxListSize = 32;
	bool list_buffer[kMaxListSize];
	bool* list_ptr;
	int list_size;

	StringBuilderBase<16>message;
	StringBuilderBase<16>text;
	int cursor = 0;

	const int h = 32;
	const int w = 100;

	Window window = {
		.x = (canvas.width() - w) / 2,
		.y = (canvas.height() - h) / 2,
		.width = w,
		.height = h,
		.collumns = 2,
		.rows = 4,
	};

	void set(bool* list_ptr_, const char* const* list_text_, uint8_t list_size_, const char* message_ /*,Callback callback*/) {
		list_ptr = list_ptr_;
		list_text = list_text_;
		//callback_ = callback;
		message.write(message_);

		list_size = list_size_;
		if (list_size > kMaxListSize) {
			list_size = kMaxListSize;
		}

		for (int i = 0; i < list_size; ++i) {
			list_buffer[i] = list_ptr[i];
		}

		window.set_row_items_total(list_size);
	}

	void init() {

	}

	void enter() {
		cursor = 0;
		window.scroll_to_row(cursor);
	}

	void exit() {

	}

	void onEncoder(uint8_t id, int inc) {
		if (id == Controller::MENU_ENC) {
			cursor = stmlib::clip(0, list_size - 1, cursor + inc);
			window.scroll_to_row(cursor);
		}
	}

	void onButton(uint8_t id, uint8_t state) {
		if (!state) {
			return;
		}

		if (id == Controller::MENU_ENC_PUSH || id == Controller::EDIT_BUTTON ) {
			bool state = list_buffer[cursor];
			list_buffer[cursor] = !state;
			return;
		}

		switch (controller.button_to_function(id))
		{
		case CONFIRM:
			for (int i = 0; i < list_size; ++i) {
				list_ptr[i] = list_buffer[i];
			}
			//callback_();
			pages.close(Pages::CHECK_LIST_PAGE);
			break;
		case SET_ALL:
			for (int i = 0; i < list_size; ++i) {
				list_buffer[i] = true;
			}
			break;
		case CLEAR_ALL:
			for (int i = 0; i < list_size; ++i) {
				list_buffer[i] = false;
			}
			break;
		case CANCEL:
			pages.close(Pages::CHECK_LIST_PAGE);
			break;
		default:
			break;
		}
	}

	void drawLeds() {
		LedPainter::set_edit(Matrix::GREEN);
		LedPainter::set_menu(Matrix::GREEN);
		LedPainter::footer_buttons(Matrix::GREEN, Matrix::GREEN, Matrix::GREEN, Matrix::RED);
	}

	void msTick(uint16_t ticks) {

	}

	// Bottom to top
	void drawDisplay() {
		canvas.set_font(Font::SMALL);

		// message
		canvas.draw_text(0, 0, message.read());
		WindowPainter::shadow(window, 4);

		// list
		const char* glyph;
		Canvas::Color color;

		WindowPainter::shadow(window, 5);
		WindowPainter::fill(window, Canvas::BLACK, Canvas::WHITE);

		for (int i = window.row().first; i <= window.row().last; ++i) {
			color = (cursor == i) ? Canvas::BLACK : Canvas::DARK_GRAY;
			glyph = list_buffer[i] ? font.glyph(Font::CHECKMARK) : " ";
			WindowPainter::text(window.cell(0, i), list_text[i], Canvas::LEFT, Canvas::CENTER, color);
			WindowPainter::text(window.cell(1, i), glyph, Canvas::LEFT, Canvas::CENTER, color);
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
