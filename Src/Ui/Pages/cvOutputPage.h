#ifndef CvOutputPage_h
#define CvOutputPage_h

#include "topPage.h"
#include "cvOutputUiList.h"
#include "monitorPage.h"

namespace CvOutputPage {
	//Declarations
	void init();
	void enter();
	void exit();
	void msTick(uint16_t ticks);
	void drawDisplay();
	void updateLeds();
	void onButton(uint8_t id, uint8_t state);
	void onEncoder(uint8_t id, int inc);
	const uint16_t targetFps();

	//variables
	CvOutputUiList cvOutputUiList;

	enum FooterOptions {
		MONITOR,
		NUM_FOOTER_OPTIONS
	};

	const char* const footer_text[NUM_FOOTER_OPTIONS] = { "MONITOR" };

	void clear_settings() {
		settings.cvOutput(cvOutputUiList.selected_port()).init();
	}

	void copy_settings() {
		clipboard.copy(settings.cvOutput(cvOutputUiList.selected_port()));
	}

	bool paste_settings() {
		return clipboard.paste(settings.cvOutput(cvOutputUiList.selected_port()));
	}

	bool pasteable_callback() {
		return clipboard.cv_out_pasteable();
	}


	void init() {

	}

	void enter() {
		ListPage::set_clear_callback(&clear_settings);
		ListPage::set_paste_callback(&paste_settings);
		ListPage::set_copy_callback(&copy_settings);
		ListPage::set_pasteable_callback(&pasteable_callback);
		ListPage::set_list(&cvOutputUiList);
		ListPage::enter();
	}

	void exit() {
		ListPage::exit();
	}

	void onEncoder(uint8_t id, int inc) {
		ListPage::onEncoder(id, inc);
	}

	void onButton(uint8_t id, uint8_t state) {
		ListPage::onButton(id, state);

		switch (controller.button_to_function(id))
		{
		case MONITOR:
			if (state) {
				MonitorPage::set_port(MonitorPage::CV_OUT, cvOutputUiList.selected_port());
				pages.open(Pages::MONITOR_PAGE);
			}
			break;
		default:
			break;
		}
	}

	void drawLeds() {
		ListPage::drawLeds();
		LedPainter::set_step_encoder(cvOutputUiList.selected_port(), Matrix::GREEN);
		LedPainter::footer_buttons(NUM_FOOTER_OPTIONS);
	}

	void msTick(uint16_t ticks) {
		ListPage::msTick(ticks);
	}

	// Bottom to top
	void drawDisplay() {
		ListPage::drawDisplay();
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
