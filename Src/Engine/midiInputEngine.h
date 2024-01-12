#ifndef MidiInputEngine_h
#define MidiInputEngine_h

#include "settings.h"
#include "noteStack.h"
#include "arpeggiatorEngine.h"
#include "midiOutputEngine.h"

class MidiInputEngine {

public:

	void init(MidiOutputEngine *outputEngine) {
		ticks = 0;
		reset_request = 0;

		outputEngine_ = outputEngine;

		for (int i = 0; i < MidiPort::NUM_PORTS; ++i) {
			cc_value_[i] = 0;
			bend_value_[i] = 0;
			note_stack_[i].init();
			channel_receive_[i] = 0;
			arpeggiatorEngine[i].init(&settings.midiInput(i).arpeggiator);
			arpeggiator_state_[i] = 0;
		}
	};

	void reset() {
		reset_request = true;
	}

	void tick() {
		++ticks;

		while (entry_que.readable()) {
			Entry e = entry_que.read();

			if (e.type == SCHEDULE) {
				outputEngine_->schedule_note(e.event, e.when, e.length);
			} else if (e.type == SEND) {
				switch (MidiEvent::get_type(&e.event))
				{
				case MidiEvent::NOTE_ON:
					outputEngine_->send_note_on(e.event);
					break;
				case MidiEvent::NOTE_OFF:
					outputEngine_->send_note_off(e.event);
					break;
				case MidiEvent::CONTROLLER_CHANGE:
					outputEngine_->send_cc(e.event);
					break;
				case MidiEvent::PITCH_BEND:
					outputEngine_->send_bend(e.event);
					break;
				default:
					break;
				}
			}
		}
	}

	void process_event(MidiEvent::Event &event) {
		MidiInput &midiInput = settings.midiInput(event.port);

		midiInput.monitor.push(event);

		// clocks are handled straight after being received
		if (MidiEvent::is_clock_message(&event)) {
			return;
		}

		uint8_t channel = event.message & 0x0F;
		uint8_t message = event.message & 0xF0;

		if (!midiInput.channel_accepted(channel)) {
			return;
		}

		switch (message)
		{
		case MidiEvent::NOTE_OFF:
			note_off(event);
			break;
		case MidiEvent::NOTE_ON:
			if (MidiEvent::convert_fake_note_off(&event)) {
				note_off(event);
			} else {
				note_on(event);
			}
			break;
		case MidiEvent::CONTROLLER_CHANGE:
			if (midiInput.cc_accepted(event.data[0])) {
				cc_change(event);
			}
			break;
		case MidiEvent::PITCH_BEND:
			if (midiInput.pitch_bend_receive()) {
				pitch_bend(event);
			}
			break;
		default:
			break;
		}
	}

	void update() {
		// reset
		if (reset_request) {
			ticks = 0;
			for (int i = 0; i < MidiPort::NUM_PORTS; ++i) {
				arpeggiatorEngine[i].reset();
			}
			reset_request = false;
		}

		for (int i = 0; i < MidiPort::NUM_PORTS; ++i) {
			// clear notes if arpeggiator state changed
			if (arpeggiator_state_[i] != settings.midiInput(i).arpeggiator.enabled()) {
				arpeggiator_state_[i] = settings.midiInput(i).arpeggiator.enabled();
				all_note_off(i);
			}

			// clear notes if channel receive changed
			if (channel_receive_[i] != settings.midiInput(i).channel_receive()) {
				channel_receive_[i] = settings.midiInput(i).channel_receive();
				all_note_off(i);
			}
		}

		// tick the arpeggiators
		while (ticks) {
			--ticks;
			for (int i = 0; i < MidiPort::NUM_PORTS; ++i) {
				tick_arpeggiator(i);
			}
		}
	}

private:

	enum Type {
		SEND,
		SCHEDULE
	};

	struct Entry {
		Type type;
		uint32_t when;
		uint32_t length;
		MidiEvent::Event event;
	};

	Que<Entry, 64>entry_que;

	void add_event(Type type, MidiEvent::Event &e, uint32_t when, uint32_t length) {
		while (!entry_que.writeable());

		entry_que.write({
			.type = type,
			.when = when,
			.length = length,
			.event = e,
		});
	}


	volatile int ticks;
	volatile bool reset_request;

	MidiOutputEngine *outputEngine_;

	int channel_receive_[MidiPort::NUM_PORTS];;
	uint8_t cc_value_[MidiPort::NUM_PORTS];
	uint16_t bend_value_[MidiPort::NUM_PORTS];
	bool arpeggiator_state_[MidiPort::NUM_PORTS];
	NoteStack note_stack_[MidiPort::NUM_PORTS];
	ArpeggiatorEngine arpeggiatorEngine[MidiPort::NUM_PORTS];


	void cc_change(MidiEvent::Event &event) {
		if (cc_value_[event.port] != event.data[1]) {
			cc_value_[event.port] = event.data[1];

			event.message = MidiEvent::CONTROLLER_CHANGE;
			event.message |= settings.midiInput(event.port).channel_send();

			add_event(SEND, event, 0, 0);
		}
	}

	void pitch_bend(MidiEvent::Event &event) {
		uint16_t value = MidiEvent::get_14bit_data(&event);

		if (bend_value_[event.port] != value) {
			bend_value_[event.port] = value;

			event.message = MidiEvent::PITCH_BEND;
			event.message |= settings.midiInput(event.port).channel_send();

			add_event(SEND, event, 0, 0);
		}
	}

	void note_on(MidiEvent::Event &event) {
		NoteStack &noteStack = note_stack_[event.port];
		MidiInput &midiInput = settings.midiInput(event.port);

		event.message = MidiEvent::NOTE_ON;
		event.message |= midiInput.channel_send();

		uint8_t key = event.data[0];
		if (midiInput.quantise_note()) {
			event.data[0] = settings.song.scale.snap_and_transpose(key);
		}

		if (noteStack.add_note(key, event)) {
			if (arpeggiator_state_[event.port] == false) {
				add_event(SEND, event, 0, 0);
			}
		}
	}

	void note_off(MidiEvent::Event &event) {
		MidiEvent::Event off_event;

		NoteStack &noteStack = note_stack_[event.port];
		uint8_t key = event.data[0];

		if (noteStack.remove_note(key, off_event)) {
			if (arpeggiator_state_[event.port] == false) {
				add_event(SEND, off_event, 0, 0);
			}
		}
	}

	void all_note_off(int port) {
		MidiEvent::Event event;

		while (note_stack_[port].pull(event)) {
			MidiEvent::convert_to_note_off(&event);
			add_event(SEND, event, 0, 0);
		}
	}

	void tick_arpeggiator(uint8_t port) {
		MidiEvent::Event e;

		NoteStack &noteStack = note_stack_[port];
		ArpeggiatorEngine &arpeggiator = arpeggiatorEngine[port];

		arpeggiator.clear_notes();

		for (int i = 0; i < noteStack.size(); ++i) {
			arpeggiator.set_note(noteStack.read(i).data[0]);
		}

		e.tie = false;
		e.port = port;
		e.source = noteStack.read(0).source;
		e.message = noteStack.read(0).message;

		if (arpeggiator.tick() && arpeggiator_state_[port] && noteStack.size()) {
			e.data[0] = arpeggiator.note();
			e.data[1] = arpeggiator.velocity();
			add_event(SCHEDULE, e, arpeggiator.swing(), arpeggiator.gate_length());
		}
	}

};

#endif
