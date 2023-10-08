#ifndef DiskUtilPage_h
#define DiskUtilPage_h

#include "pages.h"
#include "canvas.h"
#include "diskUtils.h"
#include "painters.h"
#include "stringBuilder.h"
#include "textInputPage.h"
#include "disk.h"

namespace DiskUtilPage {
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


	enum Options {
		ADD_FOLDER,
		RENAME,
		DELETE,
		CANCEL,

		NUM_OPTIONS
	};

	const char* const option_text[NUM_OPTIONS] = {
		"ADD FOLDER",
		"RENAME",
		"DELETE",
		"CANCEL"
	};

	int cursor = 0;
	uint8_t num_options;

	Entry::List* entry;
	typedef void (*Callback)();
	Callback refresh_dir_callback;

	const int h = 32;
	const int w = 100;

	Window window = {
		.x = (canvas.width() - w) / 2,
		.y = (canvas.height() - h) / 2,
		.width = w,
		.height = h,
		.collumns = 1,
		.rows = 4,
	};


	void set(Entry::List* entry_, Callback refresh_dir_callback_) {
		entry = entry_;
		refresh_dir_callback = refresh_dir_callback_;

		if (entry == nullptr) {
			num_options = 1;	// only allow add folder if its a directory with no entries
		} else {
			num_options = NUM_OPTIONS;
		}
		window.set_row_items_total(num_options);
	}

	void init() {
		cursor = 0;
		entry = nullptr;
		refresh_dir_callback = nullptr;
	}

	void enter() {
		cursor = 0;
		window.scroll_to_row(cursor);
	}

	void exit() {

	}

	void onEncoder(uint8_t id, int inc) {
		cursor = clip(0, num_options - 1, cursor + inc);
		window.scroll_to_row(cursor);
	}

	void delete_entry_callback(uint8_t option) {
		if (option == ConfirmationPage::CONFIRM) {
			if (disk.dir.remove(entry->name.read())) {
				painters.message.show(entry->is_dir ? "FOLDER DELETED" : "FILE DELETED");
			} else {
				painters.message.show("FAILED");
			}
			refresh_dir_callback();
			pages.close(Pages::DISK_UTIL_PAGE);
		}
	}

	void add_folder_callback(bool confirmed) {
		if (confirmed) {
			if (disk.dir.make(TopPage::str.read())) {
				painters.message.show("FOLDER CREATED");
				refresh_dir_callback();
			} else {
				painters.message.show("FAILED !");
			}
		}
		pages.close(Pages::DISK_UTIL_PAGE);
	}

	void rename_entry_callback(bool confirmed) {
		if (confirmed) {
			if (disk.entry.rename(entry->name.read(), TopPage::str.read())) {
				painters.message.show("RENAMED");
				refresh_dir_callback();
			} else {
				painters.message.show("FAILED !");
			}
		}
		pages.close(Pages::DISK_UTIL_PAGE);
	}

	void onButton(uint8_t id, uint8_t value) {
		if (value == 0 || id != Controller::EDIT_BUTTON) {
			return;
		}

		switch (cursor)
		{
		case ADD_FOLDER:
			TopPage::str.clear();
			TextInputPage::set(TopPage::str.read(), 16, "ENTER FOLDER NAME", &disk, &add_folder_callback);
			pages.open(Pages::TEXT_INPUT_PAGE);
			break;
		case RENAME:
			TopPage::str.write(entry->name.read());
			TextInputPage::set(TopPage::str.read(), 16, "ENTER NEW NAME", &disk, &rename_entry_callback);
			pages.open(Pages::TEXT_INPUT_PAGE);
			break;
		case DELETE:
			TopPage::str.write("DELETE ", entry->name.read(), " ?");
			ConfirmationPage::set(TopPage::str.read(), &delete_entry_callback);
			pages.open(Pages::CONFIRMATION_PAGE);
			break;
		case CANCEL:
			pages.close(Pages::DISK_UTIL_PAGE);
			break;
		default:
			break;
		}
	}

	void drawLeds() {
		painters.leds.footer_buttons(NUM_OPTIONS);
	}

	void msTick(uint16_t ticks) {

	}

	// Bottom to top
	void drawDisplay() {
		canvas.set_font(Font::LARGE);
		
		painters.window.shadow(window, 4);

		// list
		Canvas::Color color;

		painters.window.shadow(window, 5);
		painters.window.outline(window, Canvas::BLACK, Canvas::WHITE);

		for (int i = window.row().first; i <= window.row().last; ++i) {
			color = (cursor == i) ? Canvas::BLACK : Canvas::LIGHT_GRAY;
			painters.window.text(window.cell(0, i), option_text[i], Canvas::LEFT, Canvas::CENTER, color);
		}

		painters.window.vertical_scrollbar(window);

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
