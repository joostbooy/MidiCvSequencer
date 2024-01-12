#ifndef PageSelectionPage_h
#define PageSelectionPage_h

#include "pages.h"
#include "canvas.h"
#include "chapter.h"

namespace PageSelectionPage {
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
	Chapter::Id id_ = Chapter::SONG;

	const int w = 190;
	const int h = 40;

	Window window = {
		.x = (canvas.width() - w) / 2,
		.y = (canvas.height() - h) / 2,
		.width = w,
		.height = h,
		.collumns = 4,
		.rows = 2,
	};

	void set_chapter(Chapter::Id id) {
		id_ = id;
	}

	void init() {

	}

	void enter() {
		chapter.open(id_);
		window.set_coll_items_total(chapter.num_pages());
		window.scroll_to_collumn(chapter.curr_page());
		pages.move_to_top(Pages::PAGE_SELECTION_PAGE);
	}

	void exit() {

	}

	void onEncoder(uint8_t id, int inc) {
		chapter.open_page(chapter.curr_page() + inc);
		window.scroll_to_collumn(chapter.curr_page());
		pages.move_to_top(Pages::PAGE_SELECTION_PAGE);
	}

	void onButton(uint8_t id, uint8_t state) {

	}

	void drawLeds() {

	}

	void msTick(uint16_t ticks) {

	}

	void drawDisplay() {
		canvas.set_font(Font::SMALL);

		// title
		canvas.fill(window.x, window.y - 8, window.width, 10, Canvas::BLACK);
		canvas.horizontal_line(window.x + 1, window.y - 9, window.width - 2, Canvas::BLACK);
		canvas.draw_text(window.x, window.y - 9, window.width, 10, chapter.name(), Canvas::CENTER, Canvas::CENTER, Canvas::WHITE);

		// list
		WindowPainter::fill(window, Canvas::BLACK, Canvas::WHITE);

		for (int i = window.coll().first; i <= window.coll().last; ++i) {
			Canvas::Color color = i == chapter.curr_page() ? Canvas::BLACK : Canvas::LIGHT_GRAY;
			WindowPainter::text(window.cell(i, 1), chapter.page_name(i), Canvas::CENTER, Canvas::CENTER, color);
			WindowPainter::bitmap(window.cell(i, 0), chapter.page_icon(i), Canvas::CENTER, Canvas::CENTER);
		}
		WindowPainter::horizontal_scrollbar(window);

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
