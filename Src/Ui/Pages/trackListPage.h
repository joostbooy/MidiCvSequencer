#ifndef TrackListPage_h
#define TrackListPage_h

#include "topPage.h"

namespace TrackListPage {
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
		MOVE_RELEASE,
		CLOSE,
		NUM_FOOTER_OPTIONS
	};

	const char* const footer_text_move[NUM_FOOTER_OPTIONS] = { "RELEASE", "CLOSE" };
	const char* const footer_text_release[NUM_FOOTER_OPTIONS] = { "MOVE", "CLOSE" };


	Window window = {
		.x = 0,
		.y = 12,
		.width = canvas.width() - 1,
		.height = 40,
		.collumns = 4,
		.rows = 4,
	};


	void init() {
		index = 0;
	}

	void enter() {
		moving = false;
		index = settings.selected_track_index();
		window.set_row_items_total(16);
		window.scroll_to_row(index);
	}

	void exit() {
		settings.select_track_index(index);
	}

	void move_left() {
		engine.suspend();

		if (settings.song.swap_track_with_left(index)) {
			window.scroll_to_row(--index);
		}

		engine.resume();
	}

	void move_right() {
		engine.suspend();

		if (settings.song.swap_track_with_right(index)) {
			window.scroll_to_row(++index);
		}

		engine.resume();
	}

	void onEncoder(uint8_t id, int inc) {
		if (id == Controller::MENU_ENC || id == Controller::Y_ENC) {
			if (moving) {
				inc > 0 ? move_right() : move_left();
			} else {
				index = stmlib::clip(0, 15, index + inc);
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
		case MOVE_RELEASE:
			moving = !moving;
			break;
		case CLOSE:
			pages.close(Pages::TRACK_LIST_PAGE);
			break;
		default:
			break;
		}
	}

	void drawLeds() {
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
			const char *a = TopPage::str.write("T", i + 1);
			const char *b = settings.song.track(i).name();
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
