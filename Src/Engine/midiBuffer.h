#ifndef MidiBuffer_h
#define MidiBuffer_h

#include "midiEvent.h"
#include "que.h"

class MidiBuffer {

public:

	struct Reading {
		uint8_t data;
		uint32_t time;
	};

	Que<Reading, 64> in_que;
	Que<uint8_t, 64> out_que;
	Que<uint8_t, 8> clock_out_que;

	void init(int port_index) {
		num_data_bytes = 0;
		last_message = 0x00;

		event.port = port_index;
		clock_event.port = port_index;
	}

	void write_out(MidiEvent::Event *event) {
		if (event->message == last_message) {
			force_write_out(event, 2);
		} else {
			last_message = event->message;
			force_write_out(event, 3);
		}
	}

	bool write_out_non_blocking(MidiEvent::Event *event) {
		size_t available = out_que.available_size();
		uint8_t requested = event->message != last_message ? 3 : 2;

		if (available >= requested) {
			if (requested == 3) {
				last_message = event->message;
				out_que.write(event->message);
			}
			out_que.write(event->data[0]);
			out_que.write(event->data[1]);
			return true;
		}
		return false;
	}

	bool event_received(MidiEvent::Event &e) {
		while (in_que.readable()) {
			Reading reading = in_que.read();

			if (reading.data >= 0xF8) {
				clock_event.message = reading.data;
				clock_event.time = reading.time;
				e = clock_event;
				return true;
			}

			if (parse(reading.data, reading.time) == true) {
				e = event;
				return true;
			}
		}
		return false;
	}

private:
	MidiEvent::Event event;
	MidiEvent::Event clock_event;

	uint8_t num_data_bytes;
	uint8_t last_message;


	void force_write_out(MidiEvent::Event *event, const int size) {
		while (out_que.available_size() < size);

		if (size == 3) {
			out_que.write(event->message);
		}

		out_que.write(event->data[0]);
		out_que.write(event->data[1]);
	}

	bool parse(uint8_t reading, uint32_t time) {
		if (reading >= 0x80) {
			event.message = reading;
			num_data_bytes = 0;
		} else {
			event.data[num_data_bytes] = reading;
			++num_data_bytes;

			if (num_data_bytes >= 2) {
				num_data_bytes = 0;
				event.time = time;
				return true;
			}
		}
		return false;
	}
};

#endif
