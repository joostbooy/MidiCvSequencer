#ifndef DiskNavigatorPage_h
#define DiskNavigatorPage_h

#include "pages.h"
#include "canvas.h"
#include "stringBuilder.h"
#include "textInputPage.h"
#include "diskUtilPage.h"
#include "diskRetryPage.h"

namespace DiskNavigatorPage {
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


	void refresh_dir();

	enum Options {
		ENTER,
		EXIT,
		CONFIRM,
		EDIT,

		NUM_OPTIONS
	};

	const char* const option_text[NUM_OPTIONS] = {
		"ENTER",
		"EXIT",
		"CONFIRM",
		"EDIT"
	};

	typedef void (*Callback)();

	int cursor = 0;
	int cursor_max = 0;
	int entries_total = 0;

	Callback confirm_callback;
	StringBuilderBase<32>message;

	//Window::Cell cell;
	Window window = {
		.x = 0,
		.y = 20,
		.width = canvas.width(),
		.height = canvas.height() - 32,
		.collumns = 4,
		.rows = 4,
	};

	bool check_connection() {
		if (!disk.mounted() || !disk.detected()) {
			DiskRetryPage::set_callback(&refresh_dir);
			pages.open(Pages::DISK_RETRY_PAGE);
			return false;
		}
		return true;
	}

	void open_disk(const char* path, Entry::Filter filter) {
		if (!check_connection()) {
			return;
		}

		disk.entry.set_list_filter(filter);

		bool opened = false;

		if (path) {
			opened = disk.dir.open_path(path);
		} else {
			opened = disk.dir.reopen();
		}

		if (!opened) {
			disk.dir.reset();
		}
	}

	void set_confirm_callback(Callback confirm_callback_) {
		confirm_callback = confirm_callback_;
	}

	inline void refresh_dir() {
		if (!check_connection()) {
			return;
		}

		disk.dir.reopen();

		int last_entries_total = entries_total;
		entries_total = disk.entry.num_visible();

		if (last_entries_total != entries_total) {
			window.set_row_items_total(entries_total);
		}

		// update cursor
		cursor_max = 0;
		if (entries_total > 0) {
			cursor_max = entries_total - 1;
		}
		cursor = stmlib::clip(0, cursor_max, cursor);
		window.scroll_to_row(cursor);

		disk.entry.make_list(window.row().first, window.row().max_visible);
		disk.dir.close();
	}

	const char* curr_path() {
		return disk.dir.read_path();
	}

	const char* curr_entry_name() {
		Entry::List* entry = disk.entry.read_list(cursor - window.row().first);
		return entry->name.read();
	}

	bool curr_entry_is_dir() {
		Entry::List* entry = disk.entry.read_list(cursor - window.row().first);
		return entry->is_dir;
	}

	void init() {
		cursor = 0;
		confirm_callback = nullptr;
	}

	void enter() {
		cursor = 0;
		refresh_dir();
	}

	void exit() {
		confirm_callback = nullptr;
	}

	void onEncoder(uint8_t id, int inc) {
		if (id == Controller::MENU_ENC || id == Controller::Y_ENC) {
			cursor = stmlib::clip(0, cursor_max, cursor + inc);

			uint8_t first = window.row().first;
			window.scroll_to_row(cursor);
			if (first != window.row().first) {
				refresh_dir();
			}
		}
	}

	void onButton(uint8_t id, uint8_t value) {
		if (value == 0) {
			return;
		}

		Entry::List* entry = disk.entry.read_list(cursor - window.row().first);

		switch (controller.button_to_function(id))
		{
		case ENTER:
			if (entry->is_dir) {
				if (disk.dir.enter(entry->name.read())) {
					cursor = 0;
					refresh_dir();
				}
			}
			break;
		case EXIT:
			if (disk.dir.exit()) {
				cursor = 0;
				refresh_dir();
			}
			break;
		case CONFIRM:
			if (confirm_callback) {
				confirm_callback();
			}
			break;
		case EDIT:
			DiskUtilPage::set(entry, &refresh_dir);
			pages.open(Pages::DISK_UTIL_PAGE);
			break;
		default:
			break;
		}
	}

	void drawLeds() {
		LedPainter::set_y(Matrix::GREEN);
		LedPainter::set_menu(Matrix::GREEN);
		LedPainter::footer_buttons(NUM_OPTIONS);
	}

	void msTick(uint16_t ticks) {

	}

	// Bottom to top
	void drawDisplay() {
		WindowPainter::draw_header();

		canvas.set_font(Font::SMALL);

		// draw path & message
		canvas.draw_text(0, 12, canvas.width(), canvas.height(), disk.dir.read_path(), Canvas::LEFT, Canvas::TOP);

		// draw entries
		for (int i = window.row().first; i <= window.row().last; ++i) {
			Entry::List* e = disk.entry.read_list(i - window.row().first);

			if (e->is_dir) {
				TopPage::str.write("/", e->name.read());
				WindowPainter::text(window.cell(0, i), TopPage::str.read(), Canvas::LEFT, Canvas::CENTER);
			} else {
				WindowPainter::text(window.cell(0, i), e->name.read(), Canvas::LEFT, Canvas::CENTER);
				WindowPainter::text(window.cell(1, i), UiText::kb_to_mem_size_text(e->size / 1000), Canvas::LEFT, Canvas::CENTER);
			}

			if (i == cursor && disk.entry.list_size() > 0) {
				WindowPainter::highlight(window.cell(0, cursor));
			}
		}

		WindowPainter::vertical_scrollbar(window);

		// draw footer
		WindowPainter::draw_footer(option_text, NUM_OPTIONS);

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
