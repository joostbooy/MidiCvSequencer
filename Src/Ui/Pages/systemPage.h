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
		NUM_OPTIONS
	};

	const char* const footer_text[NUM_OPTIONS] = {"CALIBRATE CV", "TEST HARDWARE"};

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
			default:
				break;
			}
		}
	}

	void drawLeds() {
		painters.leds.footer_buttons(NUM_OPTIONS);
	}

	void msTick(uint16_t ticks) {

	}

	void drawDisplay() {
		WindowPainter::draw_header();

		canvas.set_font(Font::SMALL);

		// firmware version
		WindowPainter::text(window.cell(0, 0), "FIRMWARE", Canvas::LEFT, Canvas::CENTER);
		WindowPainter::text(window.cell(1, 0), settings.current_version_text(), Canvas::LEFT, Canvas::CENTER);

		// sd card total
		WindowPainter::text(window.cell(0, 1), "SD CARD TOTAL", Canvas::LEFT, Canvas::CENTER);
		WindowPainter::text(window.cell(1, 1), UiText::bytes_to_mem_size_text(sd_size_total * 512), Canvas::LEFT, Canvas::CENTER);

		// sd card free
		WindowPainter::text(window.cell(0, 2), "SD CARD FREE", Canvas::LEFT, Canvas::CENTER);
		WindowPainter::text(window.cell(1, 2), UiText::bytes_to_mem_size_text(sd_size_free * 512), Canvas::LEFT, Canvas::CENTER);

		// settings path
		WindowPainter::text(window.cell(0, 3), "PATH", Canvas::LEFT, Canvas::CENTER);
		WindowPainter::text(window.cell(1, 3), settings.read_path(), Canvas::LEFT, Canvas::CENTER);

		// pattern memory
		int total = settings.song.max_patterns();
		int free = settings.song.available_patterns();
		WindowPainter::text(window.cell(0, 4), "PATTERNS USED", Canvas::LEFT, Canvas::CENTER);
		WindowPainter::text(window.cell(1, 4), TopPage::str.write(free, "/", total), Canvas::LEFT, Canvas::CENTER);

		WindowPainter::draw_footer(footer_text, NUM_OPTIONS);
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
