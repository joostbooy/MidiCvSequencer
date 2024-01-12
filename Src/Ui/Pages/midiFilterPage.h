#ifndef MidiFilterPage_h
#define MidiFilterPage_h

#include "pages.h"
#include "canvas.h"

namespace MidiFilterPage {
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
		CANCEL,
		SET_ALL,
		CLEAR_ALL,
		NUM_FOOTER_OPTIONS
	};

	const char* const footer_text[NUM_FOOTER_OPTIONS] = {
		"CONFIRM",
		"CANCEL",
		"SET ALL",
		"CLEAR ALL"
	};

	int cursor = 0;
	MidiFilter *midiFilter_;
	bool list_buffer[MidiFilter::NUM_FILTERS];

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

	void set(MidiFilter *midiFilter) {
		midiFilter_ = midiFilter;
		for (int i = 0; i < MidiFilter::NUM_FILTERS; ++i) {
			list_buffer[i] = midiFilter_->message_allowed(MidiFilter::MessageType(i));
		}
	}

	void init() {
		midiFilter_ = nullptr;
		window.set_row_items_total(MidiFilter::NUM_FILTERS);
	}

	void enter() {
		cursor = 0;
		window.scroll_to_row(cursor);

		if (midiFilter_ == nullptr) {
			pages.close(Pages::MIDI_FILTER_PAGE);
		}
	}

	void exit() {
		midiFilter_ = nullptr;
	}

	void onEncoder(uint8_t id, int inc) {
		if (controller.encoder_to_function(id) >= 0 || id == Controller::MENU_ENC_PUSH) {
			cursor = stmlib::clip(0, MidiFilter::NUM_FILTERS - 1, cursor + inc);
			window.scroll_to_row(cursor);
		}
	}

	void onButton(uint8_t id, uint8_t state) {
		if (!state) {
			return;
		}

		if (id == Controller::EDIT_BUTTON || id == Controller::MENU_ENC_PUSH) {
			bool state = list_buffer[cursor];
			list_buffer[cursor] = !state;
			return;
		}

		switch (controller.button_to_function(id))
		{
		case CONFIRM:
			for (int i = 0; i < MidiFilter::NUM_FILTERS; ++i) {
				midiFilter_->set_message(MidiFilter::MessageType(i), list_buffer[i]);
			}
			pages.close(Pages::MIDI_FILTER_PAGE);
			break;
		case SET_ALL:
			for (int i = 0; i < MidiFilter::NUM_FILTERS; ++i) {
				list_buffer[i] = true;
			}
			break;
		case CLEAR_ALL:
			for (int i = 0; i < MidiFilter::NUM_FILTERS; ++i) {
				list_buffer[i] = false;
			}
			break;
		case CANCEL:
			pages.close(Pages::MIDI_FILTER_PAGE);
			break;
		default:
			break;
		}
	}

	void drawLeds() {
		LedPainter::footer_encoders(4);
		LedPainter::footer_buttons(Matrix::GREEN, Matrix::RED, Matrix::ORANGE, Matrix::ORANGE);
	}

	void msTick(uint16_t ticks) {

	}

	// Bottom to top
	void drawDisplay() {
		canvas.set_font(Font::SMALL);

		// title
		canvas.set_font(Font::SMALL);
		canvas.fill(window.x, window.y - 8, window.width, 10, Canvas::BLACK);
		canvas.horizontal_line(window.x + 1, window.y - 9, window.width - 2, Canvas::BLACK);
		canvas.draw_text(window.x, window.y - 9, window.width, 10, "MIDI FILTER", Canvas::CENTER, Canvas::CENTER, Canvas::WHITE);

		// list
		WindowPainter::fill(window, Canvas::BLACK, Canvas::WHITE);

		for (int i = window.row().first; i <= window.row().last; ++i) {
			Canvas::Color color = (cursor == i) ? Canvas::BLACK : Canvas::LIGHT_GRAY;
			const char* glyph = list_buffer[i] ? font.glyph(Font::CHECKMARK) : " ";
			WindowPainter::text(window.cell(0, i), MidiFilter::messageTypeText(i), Canvas::LEFT, Canvas::CENTER, color);
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
