#ifndef PatternListPage_h
#define PatternListPage_h

#include "topPage.h"

namespace PatternListPage {
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
	int index;
	bool moving;

	enum FooterOptions {
		ADD,
		MOVE_RELEASE,
		EDIT_LABEL,
		CLOSE,
		NUM_FOOTER_OPTIONS
	};

	const char* const footer_text_move[NUM_FOOTER_OPTIONS] = { "ADD", "RELEASE", "EDIT LABEL", "CLOSE" };
	const char* const footer_text_release[NUM_FOOTER_OPTIONS] = { "ADD", "MOVE", "EDIT LABEL", "CLOSE" };


	Window window = {
		.x = 0,
		.y = 12,
		.width = canvas.width() - 1,
		.height = 40,
		.collumns = 4,
		.rows = 4,
	};

	int max() {
		return settings.selected_track().num_patterns();
	}

	void init() {
		index = 0;
	}

	void enter() {
		moving = false;
		index = settings.selected_pattern();
		window.set_row_items_total(max());
		window.scroll_to_row(index);
	}

	void exit() {
		settings.select_pattern(index);
	}

	void move_left() {
		engine.suspend();

		if (settings.selected_track().swap_pattern_with_left(index)) {
			window.scroll_to_row(--index);
		}

		engine.resume();
	}

	void move_right() {
		engine.suspend();

		if (settings.selected_track().swap_pattern_with_right(index)) {
			window.scroll_to_row(++index);
		}

		engine.resume();
	}

	void onEncoder(uint8_t id, int inc) {
		if (id == Controller::MENU_ENC || id == Controller::Y_ENC) {
			if (moving) {
				inc > 0 ? move_right() : move_left();
			} else {
				index = stmlib::clip(0, max() - 1, index + inc);
				window.scroll_to_row(index);
			}
		}
	}

	void onButton(uint8_t id, uint8_t value) {
		if (!value) {
			return;
		}

		switch (controller.button_to_function(id))
		{
		case ADD:
			PatternEditPage::add_pattern();
			enter();
			break;
		case MOVE_RELEASE:
			moving = !moving;
			break;
		case EDIT_LABEL:
			PatternEditPage::edit_label(index);
			break;
		case CLOSE:
			pages.close(Pages::PATTERN_LIST_PAGE);
			break;
		default:
			break;
		}

		//delete
		if (id == Controller::DELETE_BUTTON) {
			ConfirmationPage::set(TopPage::str.write("DELETE PATTERN ", index + 1, "?"), [](uint8_t option) {
				if (option == ConfirmationPage::CONFIRM) {
					PatternEditPage::remove_pattern(index);
					enter();
				}
			});

			pages.open(Pages::CONFIRMATION_PAGE);
			return;
		}

		//clear
		if (id == Controller::CLEAR_BUTTON) {
			ConfirmationPage::set(TopPage::str.write("CLEAR PATTERN ", index + 1, "?"), [](uint8_t option) {
				if (option == ConfirmationPage::CONFIRM) {
					PatternEditPage::clear_pattern(index);
				}
			});

			pages.open(Pages::CONFIRMATION_PAGE);
			return;
		}
	}

	void drawLeds() {
		LedPainter::set_clear(Matrix::GREEN);
		LedPainter::set_delete(Matrix::GREEN);
		LedPainter::footer_buttons(NUM_FOOTER_OPTIONS);
	}

	void msTick(uint16_t ticks) {

	}

	// Bottom to top
	void drawDisplay() {
		canvas.buffer.clear();
		WindowPainter::draw_header();

		Canvas::Color color;
		canvas.set_font(Font::SMALL);

		for (int i = window.row().first; i <= window.row().last; ++i) {
			color = (index == i) ? Canvas::BLACK : Canvas::LIGHT_GRAY;
			const char *a = TopPage::str.write("P", i + 1);
			const char *b = settings.selected_track().pattern.label(i);
			WindowPainter::text(window.cell(0, i), a, Canvas::LEFT, Canvas::CENTER, color);
			WindowPainter::text(window.cell(1, i), b, Canvas::LEFT, Canvas::CENTER, color);
		}

		if (moving) {
			WindowPainter::highlight(window.cell(1, index));
			WindowPainter::draw_footer(footer_text_move, NUM_FOOTER_OPTIONS);
		} else {
			WindowPainter::draw_footer(footer_text_release, NUM_FOOTER_OPTIONS);
		}

		WindowPainter::vertical_scrollbar(window);
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
