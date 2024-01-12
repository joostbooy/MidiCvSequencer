#ifndef DiskUtilPage_h
#define DiskUtilPage_h

#include "pages.h"
#include "canvas.h"
#include "diskUtils.h"
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


	enum FooterOptions {
		CONFIRM,
		CANCEL,
		NUM_FOOTER_OPTIONS
	};

	const char* const footer_text[NUM_FOOTER_OPTIONS] = { "CONFIRM", "CANCEL" };


	enum EditOptions {
		ADD_FOLDER,
		RENAME,
		DELETE,

		NUM_OPTIONS
	};

	const char* const edit_option_text[NUM_OPTIONS] = { "ADD FOLDER", "RENAME", "DELETE", };

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
		cursor = stmlib::clip(0, num_options - 1, cursor + inc);
		window.scroll_to_row(cursor);
	}

	void delete_entry_callback(uint8_t option) {
		if (option == ConfirmationPage::CONFIRM) {
			if (disk.dir.remove(entry->name.read())) {
				MessagePainter::show(entry->is_dir ? "FOLDER DELETED" : "FILE DELETED");
			} else {
				MessagePainter::show("FAILED");
			}
			refresh_dir_callback();
			pages.close(Pages::DISK_UTIL_PAGE);
		}
	}

	void add_folder_callback(bool confirmed) {
		if (confirmed) {
			if (disk.dir.make(TopPage::str.read())) {
				MessagePainter::show("FOLDER CREATED");
				refresh_dir_callback();
			} else {
				MessagePainter::show("FAILED !");
			}
		}
		pages.close(Pages::DISK_UTIL_PAGE);
	}

	void rename_entry_callback(bool confirmed) {
		if (confirmed) {
			if (disk.entry.rename(entry->name.read(), TopPage::str.read())) {
				MessagePainter::show("RENAMED");
				refresh_dir_callback();
			} else {
				MessagePainter::show("FAILED !");
			}
		}
		pages.close(Pages::DISK_UTIL_PAGE);
	}

	void edit(int option) {
		switch (option)
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
		default:
			break;
		}
	}

	void onButton(uint8_t id, uint8_t value) {
		if (value >= 1) {
			switch (controller.button_to_function(id))
			{
			case CONFIRM:
				edit(cursor);
				break;
			case CANCEL:
				pages.close(Pages::DISK_UTIL_PAGE);
				break;
			default:
				break;
			}
		}
	}

	void drawLeds() {
		LedPainter::footer_buttons(Matrix::GREEN, Matrix::RED, Matrix::BLACK, Matrix::BLACK);
	}

	void msTick(uint16_t ticks) {

	}

	// Bottom to top
	void drawDisplay() {
		// title
		canvas.set_font(Font::SMALL);
		canvas.fill(window.x, window.y - 8, window.width, 10, Canvas::BLACK);
		canvas.horizontal_line(window.x + 1, window.y - 9, window.width - 2, Canvas::BLACK);
		canvas.draw_text(window.x, window.y - 9, window.width, 10, "EDIT", Canvas::CENTER, Canvas::CENTER, Canvas::WHITE);

		// list
		WindowPainter::fill(window, Canvas::BLACK, Canvas::WHITE);

		for (int i = window.row().first; i <= window.row().last; ++i) {
			Canvas::Color color = i == cursor ? Canvas::BLACK : Canvas::LIGHT_GRAY;
			WindowPainter::text(window.cell(0, i), edit_option_text[i], Canvas::LEFT, Canvas::CENTER, color);
		}
		WindowPainter::vertical_scrollbar(window);

		//footer
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
