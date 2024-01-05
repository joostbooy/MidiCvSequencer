#ifndef CvInputEngine_h
#define CvInputEngine_h

#include "cvInput.h"
#include "que.h"
#include "adc.h"
#include "dsp.h"

class CvInputEngine {

public:

	void init() {
		for (int i = 0; i < CvInput::NUM_PORTS; ++i) {
			cv_que[i].init();
			filterd[i] = 0.0f;
			note_is_playing[i] = false;
			last_value[i] = 0xFF;
		}
	}

	void reset() {
		for (int i = 0; i < CvInput::NUM_PORTS; ++i) {
			send_note_off(i);
		}
	}

	void receive(uint32_t time) {
		CvEvent cvEvent;

		if (adc.ready()) {
			int chn = adc.curr_channel();

			cvEvent.gate = gateIO.read_input(chn);
			cvEvent.data = adc.read();
			cvEvent.time = time;
			cv_que[chn].write(cvEvent);

			adc.convert_next_channel();
		}
	}

	bool event_received(uint8_t port, MidiEvent::Event &event) {
		if (midi_que[port].readable()) {
			event = midi_que[port].read();
			return true;
		}
		return false;
	}

	float current_value(uint8_t port) {
		return filterd[port];
	}

	bool current_gate(uint8_t port) {
		return last_gate[port];
	}

	void process(uint8_t port) {
		CvEvent cvEvent;

		int midi_port = settings.cvInput(port).forward_port();
		if (midi_que[midi_port].writeable() == false) {
			return;
		}

		while (cv_que[port].readable()) {
			CvInput &cvInput = settings.cvInput(port);
			cvEvent = cv_que[port].read();

			// filter adc
			float in = cvEvent.data * (1.0f / 4095.f);
			float out = filterd[port];
			filterd[port] = Dsp::one_pole_filter(in, out, 0.05f);

			// update gate
			bool last = last_gate[port];
			bool gate = cvEvent.gate;
			last_gate[port] = gate;

			// nothing todo
			if (cvInput.forward_channel() < 0) {
				continue;
			}

			// convert to midi
			float value = filterd[port];
			uint32_t time = cvEvent.time;

			switch (cvInput.cv_mode())
			{
			case CvInput::CC:
				send_as_cc(port, cvInput, value * 127, time);
				break;
			case CvInput::CC_SAMPLE_HOLD:
				if (gate != last && gate == 1) {
					send_as_cc(port, cvInput, value * 127, time);
				}
				break;
			case CvInput::PITCH_BEND:
				send_as_bend(port, cvInput, value * 16382, time);
				break;
			case CvInput::NOTE:
				if (gate != last) {
					send_note_off(port);	// always turn note off on either a new note or a note off (monophonic)

					if (gate >= 1) {
						send_as_note(port, cvInput, value * 127, time);
					}
				}
				break;
			default:
				break;
			}
		}
	}


private:
	float filterd[CvInput::NUM_PORTS];
	bool note_is_playing[CvInput::NUM_PORTS];
	bool last_gate[CvInput::NUM_PORTS];
	uint8_t last_value[CvInput::NUM_PORTS];
	MidiEvent::Event note_event[CvInput::NUM_PORTS];

	struct CvEvent {
		bool gate;
		uint16_t data;
		uint32_t time;
	};

	Que<CvEvent, 32> cv_que[CvInput::NUM_PORTS];
	Que<MidiEvent::Event, 32>midi_que[MidiPort::NUM_PORTS];


	void send_note_off(uint8_t cv_port) {
		if (note_is_playing[cv_port]) {
			note_is_playing[cv_port] = false;

			MidiEvent::Event &event = note_event[cv_port];
			MidiEvent::convert_to_note_off(&event);

			midi_que[event.port].write(event);
		}
	}

	void send_as_note(uint8_t cv_port, CvInput &cvInput, uint8_t value, uint32_t time) {
		MidiEvent::Event &event = note_event[cv_port];

		uint8_t port = cvInput.forward_port();
		uint8_t channel = cvInput.forward_channel();

		event.time = time;
		event.port = port;
		event.message = channel | MidiEvent::NOTE_ON;
		event.data[0] = value;
		event.data[1] = 100;
		MidiEvent::set_source(&event, MidiEvent::PORT, port);

		note_is_playing[cv_port] = true;

		midi_que[port].write(event);
	}

	void send_as_cc(uint8_t cv_port, CvInput &cvInput, uint8_t value, uint32_t time) {
		if (last_value[cv_port] != value) {
			last_value[cv_port] = value;

			int port = cvInput.forward_port();
			int channel = cvInput.forward_channel();
			int number = settings.midiInput(port).cc_receive();

			if (number < 0) {
				return;
			}

			MidiEvent::Event event;

			event.time = time;
			event.port = port;
			event.message = channel | MidiEvent::CONTROLLER_CHANGE;
			event.data[0] = number;
			event.data[1] = value;
			MidiEvent::set_source(&event, MidiEvent::PORT, port);

			midi_que[port].write(event);
		}
	}

	void send_as_bend(uint8_t cv_port, CvInput &cvInput, uint16_t value, uint32_t time) {
		if (last_value[cv_port] != value >> 5) {
			last_value[cv_port] = value >> 5;

			uint8_t port = cvInput.forward_port();
			uint8_t channel = cvInput.forward_channel();

			MidiEvent::Event event;

			event.time = time;
			event.port = port;
			event.message = channel | MidiEvent::PITCH_BEND;
			MidiEvent::set_14bit_data(&event, value);
			MidiEvent::set_source(&event, MidiEvent::PORT, port);

			midi_que[port].write(event);
		}
	}
};

#endif
