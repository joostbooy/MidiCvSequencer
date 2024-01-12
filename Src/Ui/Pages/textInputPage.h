#ifndef TextInputPage_h
#define TextInputPage_h

#include "pages.h"
#include "canvas.h"

// Todo : I hate this page, make it cleaner

namespace TextInputPage {
	//Declarations
	void onInit();
	void onPageEnter();
	void onPageExit();
	void onMsTick(uint16_t ticks);
	void drawDisplay();
	void updateLeds();
	void onPotChange(uint8_t id, uint8_t value);
	void onButton(uint8_t id, uint8_t value);
	void onEncoder(uint8_t id, int inc);
	const uint16_t fps();


	//variables
	uint8_t box_x;
	uint8_t box_w;
	int char_cursor = 0;
	int text_cursor = 0;
	uint16_t text_cursor_ticks = 0;

	Disk *disk;
	char *dest_ptr;

	typedef void (*Callback)(bool confirmed);
	Callback callback;

	uint8_t dest_max_;
	const uint8_t kCharTableSize = 51;
	const uint8_t kCharsPerRow = 17;
	const uint8_t kStackMax = 64;
	Stack <char, kStackMax> char_stack;
	StringBuilderBase<kStackMax> message;
	StringBuilderBase<kStackMax> text_copy;

	enum FooterOptions {
		FINISH,
		CANCEL,
		BACKSPACE,
		INSERT,

		NUM_FOOTER_OPTIONS
	};

	const char* const footer_text[NUM_FOOTER_OPTIONS] = {
		"FINISH",
		"CANCEL",
		"BACKSPACE",
		"INSERT"
	};

	// These should all be valid fatfs chars
	const char char_table[kCharTableSize] = {
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
		'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '!', '#', '$', '%', '&', '-', '1', '2',
		'3', '4', '5', '6', '7', '8', '9', '0', '(', ')', '@', '^', '_', '`', '{', '}', '~'
	};

	void init() {
		char_stack.clear();
		message.clear();
		dest_max_ = 0;
		callback = nullptr;
		disk = nullptr;
	}

	void enter() {

	}

	void exit() {

	}

	void write_to_dest() {
		for (int i = 0; i < char_stack.size(); ++i) {
			dest_ptr[i] = char_stack.read(i);
		}
	}

	void write_to_buffer(char* text, int max) {
		char_stack.clear();

		int length = max - 1;
		while (length-- && *text != '\0') {
			char_stack.push(*text++);
		}
		char_stack.push('\0');

		// set cursor far right
		text_cursor = 0;

		for (int i = 0; i < char_stack.size(); ++i) {
			char c = char_stack.read(i);
			if (c == '\0' || c == '.') {
				return;
			}
			++text_cursor;
		}
	}

	void set(const char* dest_ptr_, uint8_t max, const char* message_, Disk* disk_, Callback callback_){
		char_cursor = 0;
		text_cursor_ticks = 0;
		message.write(message_);

		disk = disk_;
		callback = callback_;

		dest_ptr = const_cast<char*>(dest_ptr_);
		dest_max_ = max;
		if (dest_max_ >= kStackMax) {
			dest_max_ = kStackMax;
		}

		box_w = (dest_max_ * 6) + 2;
		box_x = (canvas.width() - box_w) / 2;

		write_to_buffer(&dest_ptr[0], dest_max_);
	}


	// text edit without disk check
	void set(const char* dest_ptr_, uint8_t max_, const char* message_) {
		set(dest_ptr_, max_, message_, nullptr, nullptr);
	}


	void insert(char c) {
		if (char_stack.size() < dest_max_) {
			char_stack.insert(text_cursor, c);
			++text_cursor;
		}
	}

	void backspace() {
		if (text_cursor > 0) {
			--text_cursor;
			char_stack.remove_indexed(text_cursor);
		}
	}

	void text_cursor_left() {
		if (text_cursor > 0) {
			--text_cursor;
		}
	}

	void text_cursor_right() {
		if (char_stack.read(text_cursor) != '\0' && char_stack.read(text_cursor) != '.') {
			++text_cursor;
		}
	}

	void char_cursor_left() {
		if (char_cursor > 0) {
			--char_cursor;
		}
	}

	void char_cursor_right() {
		if (char_cursor < kCharTableSize - 1) {
			++char_cursor;
		}
	}

	void char_cursor_up() {
		int up = char_cursor - kCharsPerRow;
		if (up >= 0) {
			char_cursor = up;
		}
	}

	void char_cursor_down() {
		int down = char_cursor + kCharsPerRow;
		if (down < kCharTableSize - 1) {
			char_cursor = down;
		}
	}

	void onEncoder(uint8_t id, int inc) {
		switch (id)
		{
		case Controller::MENU_ENC:
			inc > 0 ? text_cursor_right() : text_cursor_left();
			break;
		case Controller::X_ENC:
			inc > 0 ? char_cursor_right() : char_cursor_left();
			break;
		case Controller::Y_ENC:
			inc > 0 ? char_cursor_down() : char_cursor_up();
			break;
		default:
			break;
		}
	}

	bool text_is_empty() {
		return disk != nullptr && (char_stack.read(0) == '\0' || char_stack.read(0) == '.');
	}

	bool name_excists() {
		return disk != nullptr && disk->entry.excists(char_stack.pointer());
	}

	void onButton(uint8_t id, uint8_t state) {
		if (!state) {
			return;
		}

		if (id == Controller::X_ENC_PUSH || id == Controller::Y_ENC_PUSH) {
			insert(char_table[char_cursor]);
			return;
		}

		if (id == Controller::CLEAR_BUTTON) {
			char c = '\0';
			write_to_buffer(&c, dest_max_);
			return;
		}

		if (id == Controller::COPY_BUTTON) {
			if (char_stack.size() > 1) {
				text_copy.write(char_stack.pointer());
				MessagePainter::show("TEXT COPIED");
			}
			return;
		}

		if ((id == Controller::PASTE_BUTTON) && text_copy.read()) {
			write_to_buffer(const_cast<char*>(text_copy.read()), dest_max_);
			MessagePainter::show("TEXT PASTED");
			return;
		}

		switch (controller.button_to_function(id))
		{
		case BACKSPACE:
			backspace();
			break;
		case FINISH:
			if (text_is_empty()) {
				MessagePainter::show("TEXT CANNOT BE EMPTY !");
				return;
			}

			if (name_excists()) {
				MessagePainter::show("NAME ALREADY EXCISTS !");
				return;
			}

			write_to_dest();

			pages.close(Pages::TEXT_INPUT_PAGE);
			if (callback) {
				callback(true);
			}
			break;
		case CANCEL:
			pages.close(Pages::TEXT_INPUT_PAGE);
			if (callback) {
				callback(false);
			}
			break;
		case INSERT:
			insert(char_table[char_cursor]);
			break;
		default:
			break;
		}
	}


	void drawLeds() {
		if (text_copy.read()) {
			LedPainter::set_paste(Matrix::GREEN);
			LedPainter::set_copy(Matrix::ORANGE);
		} else {
			LedPainter::set_copy(Matrix::GREEN);
		}

		LedPainter::set_x(Matrix::GREEN);
		LedPainter::set_y(Matrix::GREEN);
		LedPainter::set_menu(Matrix::GREEN);

		LedPainter::set_clear(Matrix::GREEN);

		LedPainter::footer_buttons(Matrix::GREEN, Matrix::RED, Matrix::ORANGE, Matrix::ORANGE);
	}

	void msTick(uint16_t inc) {
		if ((text_cursor_ticks += inc) >= 1000){
			text_cursor_ticks = 0;
		}
	}

	// Bottom to top
	void drawDisplay() {
		canvas.buffer.clear();
		canvas.set_font(Font::SMALL);

		//draw message
		canvas.fill(0, 0, canvas.width(), 10, Canvas::BLACK);
		canvas.draw_text(0, 0, canvas.width(), 10, message.read(), Canvas::CENTER, Canvas::CENTER, Canvas::WHITE);

		//draw char buffer
		canvas.fill(box_x, 14, box_w, 11, Canvas::DARK_GRAY);

		int cursor_x = box_x;
		int char_x = box_x + 2;
		int char_y = 16;
		int length = char_stack.size();

		canvas.set_font(Font::SMALL, Canvas::WHITE);

		for (int i = 0; i < length; ++i) {
			char c = char_stack.read(i);
			canvas.draw_char(char_x, char_y, c);
			char_x += font.width(c);

			if (i < text_cursor) {
				cursor_x = char_x;
			}
		}

		//draw text_cursor
		if (text_cursor_ticks < 500) {
			uint8_t h = font.height();
			cursor_x = stmlib::clip_min(box_x, cursor_x - 1);
			canvas.vertical_line(cursor_x, char_y, h, Canvas::WHITE);
		}

		//draw char table
		int x = 2;
		int y = 27;
		Canvas::Color color;

		for (int i = 0; i < kCharTableSize; ++i) {
			color = char_cursor == i ? Canvas::BLACK : Canvas::LIGHT_GRAY;
			canvas.set_font(Font::SMALL, color);
			canvas.draw_char(x, y, char_table[i]);
			x = canvas.text_cursor() + 10;
			if ((i % kCharsPerRow) == (kCharsPerRow - 1)) {
				x = 2;
				y += 8;
			}
		}

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
