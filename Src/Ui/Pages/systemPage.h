#ifndef SystemPage_h
#define SystemPage_h

#include "topPage.h"
#include "disk.h"

namespace SystemPage {
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
	enum FooterOptions {
		CALIBRATE_CV,
		TEST_HARDWARE,
		VERSION,
		NUM_OPTIONS
	};

	const char* const footer_text[NUM_OPTIONS] = {"CALIBRATE CV", "TEST HARDWARE", "VERSION"};

	Window window = {
		.x = 0,
		.y = 12,
		.width = canvas.width() - 1,
		.height = canvas.height() - 12,
		.collumns = 2,
		.rows = 5,
	};

	uint32_t sd_size_total;
	uint32_t sd_size_free;

	void init() {

	}

	void enter() {
		if (disk.mounted()) {
			disk.available_kb(&sd_size_total, &sd_size_free);
		} else {
			sd_size_total = 0;
			sd_size_free = 0;
		}
	}


	void exit() {

	}

	void onEncoder(uint8_t id, int inc) {

	}

	void onButton(uint8_t id, uint8_t state) {
		if (state) {
			switch (controller.button_to_function(id))
			{
			case CALIBRATE_CV:
				pages.open(Pages::CALIBRATION_PAGE);
				break;
			case TEST_HARDWARE:
				pages.open(Pages::HARDWARE_PAGE);
				break;
			case VERSION:
				MessagePainter::show(TopPage::str.write("FIRMWARE ", settings.current_version_text()));
				break;
			default:
				break;
			}
		}
	}

	void drawLeds() {
		LedPainter::footer_buttons(NUM_OPTIONS);
	}

	void msTick(uint16_t ticks) {

	}

	void draw_graph(int x, int y, int w, int h, float value, const char* title, const char* value_text) {
		int fill_h = value * h;
		int fill_y = (h - fill_h) + y;
		canvas.fill(x, y, w, h, Canvas::GRAY);
		canvas.fill(x, fill_y, w, fill_h, Canvas::BLACK);

		canvas.draw_text(x + w + 2, y + h - 14, title);
		canvas.draw_text(x + w + 2, y + h - 6, value_text);
	}

	void draw_processing_time(int x, int y, int w, int h, float time) {
		draw_graph(x, y, w, h, time, "CPU", UiText::str.write(100 * time, "%"));
	}

	void draw_available_patterns(int x, int y, int w, int h, uint32_t total, uint32_t free) {
		uint32_t used = total - free;
		float value = (1.f / total) * used;
		draw_graph(x, y, w, h, value, "PATTERNS", TopPage::str.write(used, "/", total));
	}

	void draw_sd_card_available_kb(int x, int y, int w, int h, uint32_t total, uint32_t free) {
		uint32_t used = total - free;
		float value = (1.f / total) * used;

		TopPage::str.write(UiText::kb_to_mem_size_text(used), "/", UiText::kb_to_mem_size_text(total));
		draw_graph(x, y, w, h, value, "SD CARD", TopPage::str.read());
	}

	void drawDisplay() {
		WindowPainter::draw_header();

		canvas.set_font(Font::SMALL);

		draw_available_patterns(37, 15, 10, 30, settings.song.max_patterns(), settings.song.available_patterns());
		draw_sd_card_available_kb(122, 15, 10, 30, sd_size_total, sd_size_free);
		draw_processing_time(207, 15, 10, 30, engine.processing_time());

		WindowPainter::draw_footer(footer_text, NUM_OPTIONS);
	}

	const uint16_t targetFps() {
		return 1000 / 24;
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
