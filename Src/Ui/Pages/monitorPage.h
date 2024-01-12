#ifndef MonitorPage_h
#define MonitorPage_h

#include "topPage.h"
#include "midiFilterPage.h"

namespace MonitorPage {
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
	int port_;
	int cv_value;
	bool gate_value;
	bool is_running;
	StringBuilderBase<32>str;

	enum Type {
		MIDI_IN,
		CV_IN,
		CV_OUT,
		NUM_TYPES
	}type_;

	enum FooterOptions {
		CLEAR,
		FILTER,
		RUN_STOP,
		CLOSE,
		NUM_FOOTER_OPTIONS
	};

	const char* const footer_text_running[NUM_FOOTER_OPTIONS] = {"CLEAR", "FILTER", "STOP", "CLOSE"};
	const char* const footer_text_stopped[NUM_FOOTER_OPTIONS] = {"CLEAR", "FILTER", "RUN", "CLOSE"};


	void set_port(Type type, int port) {
		type_ = type;
		port_ = port;
	}

	void write_title(const char* title) {
		canvas.draw_text(0, 0, canvas.width(), 10, title, Canvas::CENTER, Canvas::CENTER);
	}

	void write_cv_text(float cv, bool gate) {
		str.write(UiText::cv_to_volt_text(cv));

		int value = cv * 1024;

		if (value != cv_value) {
			cv_value = value;
			TextBufferPainter::write(str.read());
		}

		if (gate != gate_value) {
			gate_value = gate;
			TextBufferPainter::write(str.write("GATE ", gate));
		}
	}

	void write_midi_text(MidiEvent::Event &event) {
		str.clear();

		str.append(UiText::midi_channel_text(event));
		str.append(" ");
		str.append(UiText::midi_message_text(event));
		str.append(" ");
		str.append(UiText::midi_data_text(event, 0));
		str.append(" ");
		str.append(UiText::midi_data_text(event, 1));

		TextBufferPainter::write(str.read());
	}

	void init() {
		port_ = 0;
		cv_value = 0;
		type_ = MIDI_IN;
		is_running = true;
	}

	void enter() {
		TextBufferPainter::clear();
	}

	void exit() {

	}

	void onEncoder(uint8_t id, int inc) {

	}

	void onButton(uint8_t id, uint8_t value) {
		if (!value) {
			return;
		}

		if (id == Controller::CLEAR_BUTTON) {
			TextBufferPainter::clear();
			return;
		}

		switch (controller.button_to_function(id))
		{
		case CLEAR:
			TextBufferPainter::clear();
			break;
		case FILTER:
			if (type_ == MIDI_IN) {
				MidiFilterPage::set(&settings.midiInput(port_).filter);
				pages.open(Pages::MIDI_FILTER_PAGE);
			}
			break;
		case RUN_STOP:
			is_running = !is_running;
			break;
		case CLOSE:
			pages.close(Pages::MONITOR_PAGE);
			break;
		default:
			break;
		}
	}

	void drawLeds() {
		LedPainter::set_clear(Matrix::GREEN);
		LedPainter::footer_buttons(NUM_FOOTER_OPTIONS);
	}

	void msTick(uint16_t ticks) {

	}

	// Bottom to top
	void drawDisplay() {
		canvas.set_font(Font::SMALL);

		MidiEvent::Event event;

		canvas.buffer.clear();

		switch (type_)
		{
		case CV_IN:
			write_title(CvInput::port_text(port_));

			if (is_running) {
				write_cv_text(engine.cvInputEngine.current_value(port_), engine.cvInputEngine.current_gate(port_));
			}
			break;
		case CV_OUT:
			write_title(CvOutput::port_text(port_));

			if (is_running) {
				write_cv_text(engine.cvOutputEngine.current_value(port_), engine.cvOutputEngine.current_gate(port_));
			}
			break;
		case MIDI_IN:
			write_title(MidiPort::port_text(port_));

			while (settings.midiInput(port_).monitor.pull(event)) {
				if (is_running) {
					write_midi_text(event);
				}
			}
			break;
		default:
			break;
		}

		TextBufferPainter::draw();
		WindowPainter::draw_footer(is_running ? footer_text_running : footer_text_stopped, NUM_FOOTER_OPTIONS);
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
