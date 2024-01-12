#ifndef DiskRetryPage_h
#define DiskRetryPage_h

#include "topPage.h"

namespace DiskRetryPage {

	//Declarations
	void init();
	void enter();
	void exit();
	void msTick(uint16_t ticks);
	void drawDisplay();
	void drawLeds();
	void onButton(uint8_t id, uint8_t value);
	void onEncoder(uint8_t id, int inc);
	const uint16_t targetFps();

	//variables

	typedef void (*Callback)();
	Callback refresh_dir_callback;

	enum FooterOptions {
		RETRY,
		NUM_OPTIONS
	};

	const char* const footer_text[NUM_OPTIONS] = {
		"RETRY"
	};

	void set_callback(Callback callback) {
		refresh_dir_callback = callback;
	}

	void init() {

	}

	void enter() {

	}

	void exit() {

	}

	void onEncoder(uint8_t id, int inc) {

	}

	void onButton(uint8_t id, uint8_t state) {
		if (!state) {
			return;
		}

		switch (controller.button_to_function(id))
		{
		case RETRY:
			disk.mount();

			if (disk.mounted()) {
				disk.reset();
				pages.close(Pages::DISK_RETRY_PAGE);	// close page before calling refresh_dir_callback() !
				MessagePainter::show("CARD DETECTED");
				refresh_dir_callback();
			} else {
				MessagePainter::show(DiskUtils::result_text(disk.last_result()));
			}
			break;
		default:
			break;
		}
	}

	void drawLeds() {
		LedPainter::footer_buttons(NUM_OPTIONS);
	}

	void msTick(uint16_t ticks) {

	}

	// Bottom to top
	void drawDisplay() {
		canvas.set_font(Font::SMALL);
		canvas.fill(0, 12, 256, 56, Canvas::WHITE);
		canvas.draw_text(0, 12, 256, 56, "NO CARD FOUND", Canvas::CENTER, Canvas::CENTER);
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
