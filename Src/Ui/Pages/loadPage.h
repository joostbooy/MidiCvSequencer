#ifndef LoadPage_h
#define LoadPage_h

#include "topPage.h"
#include "diskNavigatorPage.h"

namespace LoadPage {
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
	void load_settings() {
		engine.add_request_wait(Engine::STOP);

		TopPage::str.write(DiskNavigatorPage::curr_path(), "/", DiskNavigatorPage::curr_entry_name());

		if (settings.load(TopPage::str.read())) {
			MessagePainter::show("SETTINGS LOADED");
			chapter.init();
		} else {
			MessagePainter::show("FAILED !");
		}

		ui.clear_que();
	}

	void on_confirm() {
		if (DiskNavigatorPage::curr_entry_is_dir()) {
			return;
		}

		ConfirmationPage::set("LOAD SETTINGS ?", [](uint8_t option) {
			if (option == ConfirmationPage::CONFIRM) {
				load_settings();
			}
		});
		pages.open(Pages::CONFIRMATION_PAGE);
	}

	void init() {

	}

	void enter() {
		DiskNavigatorPage::set_confirm_callback(&on_confirm);
		DiskNavigatorPage::open_disk(nullptr, Entry::PROJECT);
		DiskNavigatorPage::enter();
	}

	void exit() {
		DiskNavigatorPage::exit();
	}

	void onEncoder(uint8_t id, int inc) {
		DiskNavigatorPage::onEncoder(id, inc);
	}

	void onButton(uint8_t id, uint8_t value) {
		DiskNavigatorPage::onButton(id, value);
	}

	void drawLeds() {
		DiskNavigatorPage::drawLeds();
	}

	void msTick(uint16_t ticks) {
		DiskNavigatorPage::msTick(ticks);
	}

	// Bottom to top
	void drawDisplay() {
		DiskNavigatorPage::drawDisplay();
	}

	const uint16_t targetFps() {
		return DiskNavigatorPage::targetFps();
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
