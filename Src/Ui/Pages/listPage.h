#ifndef ListPage_h
#define ListPage_h

#include "pages.h"
#include "canvas.h"
#include "uiList.h"
#include "confirmationPage.h"

namespace ListPage {
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
	void(*clear_callback_)() = nullptr;
	void(*copy_callback_)() = nullptr;
	bool(*paste_callback_)() = nullptr;
	bool(*check_clipboard_callback_)() = nullptr;

	UiList *list_;

	Window window = {
		.x = 0,
		.y = 12,
		.width = canvas.width() - 1,
		.height = 40,
		.collumns = 3,
		.rows = 4,
	};

	void set_list(UiList *list) {
		list_ = list;
		list_->select_item(0);
		list_->set_mode(UiList::SELECT);

		window.set_row_items_total(list_->num_items());
		window.scroll_to_row(list_->selected_item());
	}

	void set_clear_callback(void(*callback)()) {
		clear_callback_ = callback;
	}

	void set_paste_callback(bool(*callback)()) {
		paste_callback_ = callback;
	}

	void set_copy_callback(void(*callback)()) {
		copy_callback_ = callback;
	}

	void set_check_clipboard_callback(bool(*callback)()) {
		check_clipboard_callback_ = callback;
	}

	void init() {

	}

	void enter() {

	}

	void exit() {
		clear_callback_ = nullptr;
		paste_callback_ = nullptr;
		copy_callback_ = nullptr;
		check_clipboard_callback_ = nullptr;
	}

	void onEncoder(uint8_t id, int inc) {
		if (id == Controller::MENU_ENC || id == Controller::Y_ENC) {
			bool shifted = controller.is_pressed(Controller::SHIFT_BUTTON);
			list_->on_encoder(inc, shifted);
			window.scroll_to_row(list_->selected_item());
		}
	}

	void onButton(uint8_t id, uint8_t value) {
		if (id == Controller::MENU_ENC_PUSH || id == Controller::Y_ENC_PUSH || id == Controller::EDIT_BUTTON) {
			if (value > 0) {
				list_->on_click();
			}
			return;
		}

		if (id == Controller::CLEAR_BUTTON && value >= 1 && clear_callback_ != nullptr) {
			ConfirmationPage::set("CLEAR SETTINGS ?", [](uint8_t option) {
				if (option == ConfirmationPage::CONFIRM) {
					clear_callback_();
				}
			});
			pages.open(Pages::CONFIRMATION_PAGE);
			return;
		}

		if (id == Controller::COPY_BUTTON && value >= 1 && copy_callback_ != nullptr) {
			copy_callback_();
			MessagePainter::show("SETTINGS COPIED");
			return;
		}

		if (id == Controller::PASTE_BUTTON && value >= 1 && paste_callback_ != nullptr && check_clipboard_callback_() == true) {
			ConfirmationPage::set("OVERWRITE SETTINGS ?", [](uint8_t option) {
				if (option == ConfirmationPage::CONFIRM) {
					paste_callback_();
					MessagePainter::show("SETTINGS PASTED");
				}
			});
			pages.open(Pages::CONFIRMATION_PAGE);
			return;
		}

	}

	void drawLeds() {
		if (clear_callback_) {
			LedPainter::set_clear(Matrix::GREEN);
		}

		LedPainter::set_y(Matrix::GREEN);
		LedPainter::set_edit(Matrix::GREEN);
		LedPainter::set_menu(Matrix::GREEN);

		if (copy_callback_ && paste_callback_) {
			if (check_clipboard_callback_()) {
				LedPainter::set_paste(Matrix::GREEN);
				LedPainter::set_copy(Matrix::ORANGE);
			} else {
				LedPainter::set_copy(Matrix::GREEN);
			}
		}
	}

	void msTick(uint16_t ticks) {

	}

	// Bottom to top
	void drawDisplay() {
		WindowPainter::draw_header();

		Canvas::Color color;
		canvas.set_font(Font::SMALL);
		int item = list_->selected_item();

		for (int i = window.row().first; i <= window.row().last; ++i) {
			color = (item == i) ? Canvas::BLACK : Canvas::LIGHT_GRAY;
			WindowPainter::text(window.cell(0, i), list_->item_text(i), Canvas::LEFT, Canvas::CENTER, color);
			WindowPainter::text(window.cell(1, i), list_->value_text(i), Canvas::LEFT, Canvas::CENTER, color);
		}

		if (list_->mode() == UiList::EDIT) {
			WindowPainter::highlight(window.cell(1, item));
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
