#ifndef MidiPort_h
#define MidiPort_h

#include "uart.h"
#include "usb.h"
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
		usb_status = 0x00;

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
		for (int i = 0; i < MIDI_UART_4; ++i) {
			if (uart.readable(i)) {
				last_received[i] = uart.read(i);
				if (buffer_[i].in_que.writeable()) {
					buffer_[i].in_que.write( {.data = last_received[i], .time = time } );
				}
			}
		}

		receive_usb(time);
	}

	void send() {
		for (int i = 0; i < MIDI_UART_4; ++i) {
			if (uart.writeable(i)) {
				if (buffer_[i].clock_out_que.readable()) {
					uart.write(i, buffer_[i].clock_out_que.read());
				} else if (buffer_[i].out_que.readable()) {
					uart.write(i, buffer_[i].out_que.read());
				}
			}
		}

		send_usb();
	}

private:

	MidiBuffer buffer_[NUM_PORTS];
	uint8_t last_received[NUM_PORTS];
	uint8_t usb_status;

	inline void receive_usb(uint32_t time) {
		uint8_t data[4];
		MidiBuffer &buff = buffer_[MIDI_USB];

		if ((buff.in_que.available_size() >= 3) && usb.read(data)) {
			last_received[MIDI_USB] = data[1];

			buff.in_que.write( {.data = data[1], .time = time } );
			if (data[1] < 0xF8) {
				buff.in_que.write( {.data = data[2], .time = time } );
				buff.in_que.write( {.data = data[3], .time = time } );
			}
		}
	}

	void send_usb() {
		uint8_t data[4];
		MidiBuffer &buff = buffer_[MIDI_USB];

		if (buff.clock_out_que.readable()) {
			data[0] = 0x0F;
			data[1] = buff.clock_out_que.read();
			data[2] = 0x00;
			data[3] = 0x00;
		} else if (buff.out_que.readable()) {
			uint8_t byte = buff.out_que.read();

			if (byte & 0x80) {
				usb_status = byte;
				data[0] = usb_status >> 4;
				data[1] = usb_status;
				data[2] = buff.out_que.read();
				data[3] = buff.out_que.read();
			} else {
				data[0] = usb_status >> 4;
				data[1] = usb_status;
				data[2] = byte;
				data[3] = buff.out_que.read();
			}
		}

		usb.write(data);
	}

};

#endif
