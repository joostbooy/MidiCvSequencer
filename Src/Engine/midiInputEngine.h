#ifndef MidiInputEngine_h
#define MidiInputEngine_h

#include "settings.h"
#include "noteStack.h"
#include "arpeggiatorEngine.h"
#include "midiOutputEngine.h"

class MidiInputEngine {

public:

	void init(MidiOutputEngine *outputEngine) {
		num_ticks_ = 0;
		suspended = false;
		outputEngine_ = outputEngine;

		for (int i = 0; i < MidiPort::NUM_PORTS; ++i) {
			bend_value_[i] = 0;
			cc_value_[i] = 0;
			note_stack_[i].init();
		}
	};

	void suspend() {
		suspended = true;
	}

	void resume() {
		suspended = false;
	}

	void tick() {
		MidiEvent::Event e;

		//++num_ticks_;
		if (suspended) {
			return;
		}

		while (midi_que.readable()) {
			e = midi_que.read();
			switch (MidiEvent::get_type(&e))
			{
			case MidiEvent::NOTE_ON:
				outputEngine_->send_note_on(e);
				break;
			case MidiEvent::NOTE_OFF:
				outputEngine_->send_note_off(e);
				break;
			case MidiEvent::CONTROLLER_CHANGE:
				outputEngine_->send_cc(e);
				break;
			case MidiEvent::PITCH_BEND:
				outputEngine_->send_bend(e);
				break;
			default:
				break;
			}
		}
	}

	void clear() {
		all_notes_off();
		reset();
	}

	void reset() {
		num_ticks_ = 0;
	}

	void all_notes_off() {
		MidiEvent::Event event;

		for (int i = 0; i < MidiPort::NUM_PORTS; ++i) {
			while (note_stack_[i].pull(event)) {
				MidiEvent::convert_to_note_off(&event);
				midi_que.write(event);
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

		suspend();

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

		resume();
	}

private:
	bool suspended;
	uint32_t num_ticks_;
	MidiOutputEngine *outputEngine_;

	uint8_t cc_value_[MidiPort::NUM_PORTS];
	uint16_t bend_value_[MidiPort::NUM_PORTS];
	NoteStack note_stack_[MidiPort::NUM_PORTS];
	Que<MidiEvent::Event, 64>midi_que;

	void cc_change(MidiEvent::Event &event) {
		if (cc_value_[event.port] != event.data[1]) {
			cc_value_[event.port] = event.data[1];
			midi_que.write(event);
		}
	}

	void pitch_bend(MidiEvent::Event &event) {
		uint16_t value = MidiEvent::get_14bit_data(&event);
		if (bend_value_[event.port] != value) {
			bend_value_[event.port] = value;
			midi_que.write(event);
		}
	}

	void note_on(MidiEvent::Event &event) {
		MidiInput &midiInput = settings.midiInput(event.port);

		event.message &= ~(0x0F);
		event.message |= (midiInput.channel_send() & 0x0F);

		uint8_t key = event.data[0];
		if (midiInput.quantise_note()) {
			event.data[0] = settings.song.scale.snap_and_transpose(key);
		}

		note_stack_[event.port].add_note(key, event);
		midi_que.write(event);
	}

	void note_off(MidiEvent::Event &event) {
		MidiEvent::Event off_event;
		uint8_t key = event.data[0];

		if (note_stack_[event.port].remove_note(key, off_event)) {
			midi_que.write(off_event);
		}
	}
};

#endif
