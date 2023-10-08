#ifndef MidiPort_h
#define MidiPort_h

#include "uart.h"
#include "midiBuffer.h"

class MidiPort {

public:

	enum Port {
		MIDI_UART_1,
		MIDI_UART_2,
		MIDI_UART_3,
		MIDI_UART_4,
		MIDI_USB,

		NUM_PORTS
	};

	static const char* port_text(uint8_t port) {
		switch (port)
		{
		case MIDI_UART_1:	return "MIDI PORT 1";
		case MIDI_UART_2:	return "MIDI PORT 2";
		case MIDI_UART_3:	return "MIDI PORT 3";
		case MIDI_UART_4:	return "MIDI PORT 4";
		case MIDI_USB:		return "MIDI USB";
		default:			break;
		}
		return nullptr;
	}

	void init() {
		for (int i = 0; i < NUM_PORTS; ++i) {
			buffer_[i].init(i);
			last_received[i] = 0x00;
		}
	}

	MidiBuffer &buffer(uint8_t port) {
		return buffer_[port];
	}

	void send_clock_message(uint8_t port, uint8_t message) {
		buffer_[port].clock_out_que.write(message);
	}

	bool receive_clock_message(uint8_t port, uint8_t *message) {
		uint8_t last = last_received[port];
		if (last >= 0xF8) {
			*message = last;
			return true;
		}
		return false;
	}

	void receive(uint32_t time) {
		for (int i = 0; i < NUM_PORTS; ++i) {
			MidiBuffer &buff = buffer_[i];

			if (readable(i)) {
				last_received[i] = read(i);
				if (buff.in_que.writeable()) {
					buff.in_que.write( {.data = last_received[i], .time = time } );
				}
			}
		}
	}

	void send() {
		for (int i = 0; i < NUM_PORTS; ++i) {
			MidiBuffer &buff = buffer_[i];

			if (writeable(i)) {
				if (buff.clock_out_que.readable()) {
					write(i, buff.clock_out_que.read());
				} else if (buff.out_que.readable()) {
					write(i, buff.out_que.read());
				}
			}
		}
	}

private:

	MidiBuffer buffer_[NUM_PORTS];
	uint8_t last_received[NUM_PORTS];

	inline bool readable(uint8_t port) {
		if (port < MIDI_USB) {
			return uart.readable(port);
		}
		return true;
	}

	inline uint8_t read(uint8_t port) {
		if (port < MIDI_USB) {
			return uart.read(port);
		}
		return 0x00;
	}

	inline bool writeable(uint8_t port) {
		if (port < MIDI_USB) {
			return uart.writeable(port);
		}
		return true;
	}

	inline void write(uint8_t port, uint8_t data) {
		if (port < MIDI_USB) {
			uart.write(port, data);
			return;
		}
		//usb.write(data);
	}

};

#endif
