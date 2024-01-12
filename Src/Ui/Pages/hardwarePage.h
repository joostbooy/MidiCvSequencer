#ifndef HardwarePage_h
#define HardwarePage_h

#include "topPage.h"

namespace HardwarePage {
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
	int led_color;
	int midi_port;
	StringBuilderBase<32>str;

	enum FooterOptions {
		TEST_LEDS,
		CLOSE,
		NUM_FOOTER_OPTIONS
	}footer_option;

	const char* const footer_text[NUM_FOOTER_OPTIONS] = { "TEST LEDS", "CLOSE" };

	const char* controller_to_text(uint8_t id) {
		switch (id)
		{
		case Controller::STEP_ENC_1:				return "STEP_ENC_1";
		case Controller::STEP_ENC_2:				return "STEP_ENC_2";
		case Controller::STEP_ENC_3:				return "STEP_ENC_3";
		case Controller::STEP_ENC_4:				return "STEP_ENC_4";
		case Controller::STEP_ENC_5:				return "STEP_ENC_5";
		case Controller::STEP_ENC_6:				return "STEP_ENC_6";
		case Controller::STEP_ENC_7:				return "STEP_ENC_7";
		case Controller::STEP_ENC_8:				return "STEP_ENC_8";
		case Controller::STEP_ENC_9:				return "STEP_ENC_9";
		case Controller::STEP_ENC_10:				return "STEP_ENC_10";
		case Controller::STEP_ENC_11:				return "STEP_ENC_11";
		case Controller::STEP_ENC_12:				return "STEP_ENC_12";
		case Controller::STEP_ENC_13:				return "STEP_ENC_13";
		case Controller::STEP_ENC_14:				return "STEP_ENC_14";
		case Controller::STEP_ENC_15:				return "STEP_ENC_15";
		case Controller::STEP_ENC_16:				return "STEP_ENC_16";
		case Controller::STEP_ENC_PUSH_1:			return "STEP_ENC_PUSH_1";
		case Controller::STEP_ENC_PUSH_2:			return "STEP_ENC_PUSH_2";
		case Controller::STEP_ENC_PUSH_3:			return "STEP_ENC_PUSH_3";
		case Controller::STEP_ENC_PUSH_4:			return "STEP_ENC_PUSH_4";
		case Controller::STEP_ENC_PUSH_5:			return "STEP_ENC_PUSH_5";
		case Controller::STEP_ENC_PUSH_6:			return "STEP_ENC_PUSH_6";
		case Controller::STEP_ENC_PUSH_7:			return "STEP_ENC_PUSH_7";
		case Controller::STEP_ENC_PUSH_8:			return "STEP_ENC_PUSH_8";
		case Controller::STEP_ENC_PUSH_9:			return "STEP_ENC_PUSH_9";
		case Controller::STEP_ENC_PUSH_10:			return "STEP_ENC_PUSH_10";
		case Controller::STEP_ENC_PUSH_11:			return "STEP_ENC_PUSH_11";
		case Controller::STEP_ENC_PUSH_12:			return "STEP_ENC_PUSH_12";
		case Controller::STEP_ENC_PUSH_13:			return "STEP_ENC_PUSH_13";
		case Controller::STEP_ENC_PUSH_14:			return "STEP_ENC_PUSH_14";
		case Controller::STEP_ENC_PUSH_15:			return "STEP_ENC_PUSH_15";
		case Controller::STEP_ENC_PUSH_16:			return "STEP_ENC_PUSH_16";
		case Controller::STEP_BUTTON_1:				return "STEP_BUTTON_1";
		case Controller::STEP_BUTTON_2:				return "STEP_BUTTON_2";
		case Controller::STEP_BUTTON_3:				return "STEP_BUTTON_3";
		case Controller::STEP_BUTTON_4:				return "STEP_BUTTON_4";
		case Controller::STEP_BUTTON_5:				return "STEP_BUTTON_5";
		case Controller::STEP_BUTTON_6:				return "STEP_BUTTON_6";
		case Controller::STEP_BUTTON_7:				return "STEP_BUTTON_7";
		case Controller::STEP_BUTTON_8:				return "STEP_BUTTON_8";
		case Controller::STEP_BUTTON_9:				return "STEP_BUTTON_9";
		case Controller::STEP_BUTTON_10:			return "STEP_BUTTON_10";
		case Controller::STEP_BUTTON_11:			return "STEP_BUTTON_11";
		case Controller::STEP_BUTTON_12:			return "STEP_BUTTON_12";
		case Controller::STEP_BUTTON_13:			return "STEP_BUTTON_13";
		case Controller::STEP_BUTTON_14:			return "STEP_BUTTON_14";
		case Controller::STEP_BUTTON_15:			return "STEP_BUTTON_15";
		case Controller::STEP_BUTTON_16:			return "STEP_BUTTON_16";
		case Controller::PLAY_BUTTON:				return "PLAY_BUTTON";
		case Controller::STOP_BUTTON:				return "STOP_BUTTON";
		case Controller::RECORD_BUTTON:				return "RECORD_BUTTON";
		case Controller::SHIFT_BUTTON:				return "SHIFT_BUTTON";
		case Controller::GROUP_BUTTON:				return "GROUP_BUTTON";
		case Controller::MUTE_BUTTON:				return "MUTE_BUTTON";
		case Controller::SOLO_BUTTON:				return "SOLO_BUTTON";
		case Controller::COPY_BUTTON:				return "COPY_BUTTON";
		case Controller::PASTE_BUTTON:				return "PASTE_BUTTON";
		case Controller::CLEAR_BUTTON:				return "CLEAR_BUTTON";
		case Controller::DELETE_BUTTON:				return "DELETE_BUTTON";
		case Controller::SAVE_BUTTON:				return "SAVE_BUTTON";
		case Controller::EDIT_BUTTON:				return "EDIT_BUTTON";
		case Controller::MENU_ENC:					return "MENU_ENC";
		case Controller::MENU_ENC_PUSH:				return "MENU_ENC_PUSH";
		case Controller::X_ENC:						return "X_ENC";
		case Controller::X_ENC_PUSH:				return "X_ENC_PUSH";
		case Controller::Y_ENC:						return "Y_ENC";
		case Controller::Y_ENC_PUSH:				return "Y_ENC_PUSH";
		case Controller::SONG_CHAPTER_BUTTON:		return "SONG_CHAPTER_BUTTON";
		case Controller::TRACK_CHAPTER_BUTTON:		return "TRACK_CHAPTER_BUTTON";
		case Controller::PATTERN_CHAPTER_BUTTON:	return "PATTERN_CHAPTER_BUTTON";
		case Controller::MIDI_CHAPTER_BUTTON:		return "MIDI_CHAPTER_BUTTON";
		case Controller::CV_CHAPTER_BUTTON:			return "CV_CHAPTER_BUTTON";
		case Controller::SYSTEM_CHAPTER_BUTTON:		return "SYSTEM_CHAPTER_BUTTON";
		default:
			break;
		}
		return nullptr;
	}

	void init() {
		midi_port = 0;
		led_color = 3; //BLACK
	}

	void enter() {
		TextBufferPainter::clear();
	}

	void exit() {

	}

	void onEncoder(uint8_t id, int inc) {
		str.write(controller_to_text(id));
		if (str.length() > 0) {
			str.append(" ");
			str.append(inc);
			TextBufferPainter::write(str.read());
		}
	}

	void onButton(uint8_t id, uint8_t state) {
		str.write(controller_to_text(id));
		if (str.length() > 0) {
			str.append(" ");
			str.append(state);
			TextBufferPainter::write(str.read());
		}

		switch (controller.button_to_function(id))
		{
		case TEST_LEDS:
			if (state) {
				++led_color %= 4;
			}
			break;
		case CLOSE:
			if (state) {
				pages.close(Pages::HARDWARE_PAGE);
			}
			break;
		default:
			break;
		}

	}

	void drawLeds() {
		Matrix::LedColor color[4] = { Matrix::RED, Matrix::GREEN, Matrix::ORANGE, Matrix::BLACK };

		for (int x = 0; x < 8; ++x) {
			for (int y = 0; y < 16 / 2; ++y) {
				matrix.set_led(x, y, color[led_color]);
			}
		}
	}

	void msTick(uint16_t ticks) {

	}

	// Bottom to top
	void drawDisplay() {
		canvas.buffer.clear();
		WindowPainter::draw_header();
		TextBufferPainter::draw();
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
