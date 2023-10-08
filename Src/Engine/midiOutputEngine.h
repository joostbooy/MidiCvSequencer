#ifndef MidiOutputEngine_h
#define MidiOutputEngine_h

#include "midiPort.h"
#include "midiScheduler.h"
#include "cvOutputEngine.h"

class MidiOutputEngine {

public:

	void init(MidiPort *midiPort, CvOutputEngine *cv) {
		cv_ = cv;
		midiPort_ = midiPort;

		for (int i = 0; i < MidiPort::NUM_PORTS; ++i) {
			scheduler_[i].init();
		}

		for (int i = 0; i < MidiEvent::NUM_SOURCES; ++i) {
			tied_event_[i].tie = false;
		}
	}

	MidiEvent::Event& curr_event(int source) {
		return curr_event_[source];
	}

	void schedule_note(MidiEvent::Event &event, uint32_t when, uint32_t length) {
		event.time = length;
		if (scheduler_[event.port].writeable()) {
			scheduler_[event.port].add(event, when);
		}
	}

	void send_tied_note_off(int source) {
		MidiEvent::Event &event = tied_event_[source];
		if (event.tie) {
			event.tie = false;
			send_event(event);
		}
	}

	//void send_event(MidiEvent::Event &event, uint8_t type, int value_16_bit = -1) {
	//	curr_event_[event.source] = event;

	//	switch (type)
	//	{
//	case MidiEvent::NOTE_OFF:
//		cv_->write_note_off(event);
//		midiPort_->buffer(event.port).write_out(&event);
//		break;
//	case MidiEvent::NOTE_ON:
//			cv_->write_note_on(event);
//			midiPort_->buffer(event.port).write_out_non_blocking(&event);
//			break;
//		case MidiEvent::CONTROLLER_CHANGE:
//			cv_->write_cc(event, value_16_bit);
//			midiPort_->buffer(event.port).write_out_non_blocking(&event);
//			break;
//		case MidiEvent::PITCH_BEND:
//			cv_->write_bend(event, value_16_bit);
//			midiPort_->buffer(event.port).write_out_non_blocking(&event);
//			break;
//		default:
//			break;
//		}
//	}

	void send_cc_event(MidiEvent::Event &event, uint16_t value_16_bit) {
		curr_event_[event.source] = event;
		cv_->write_cc(event, value_16_bit);
		midiPort_->buffer(event.port).write_out_non_blocking(&event);
	}

	void send_bend_event(MidiEvent::Event &event, uint16_t value_16_bit) {
		curr_event_[event.source] = event;
		cv_->write_bend(event, value_16_bit);
		midiPort_->buffer(event.port).write_out_non_blocking(&event);
	}

	void send_event(MidiEvent::Event &event, bool send_cv = true) {
		curr_event_[event.source] = event;

		if (MidiEvent::is_note_off(&event)) {
			midiPort_->buffer(event.port).write_out(&event);
		} else {
			midiPort_->buffer(event.port).write_out_non_blocking(&event);
		}

		if (send_cv) {
			cv_->write_event(event);
		}
	}

	void tick() {
		MidiEvent::Event event;

		for (int i = 0; i < MidiPort::NUM_PORTS; ++i) {
			while (scheduler_[i].tick(event)) {
				if (MidiEvent::is_note_on(&event)) {
					handle_note_on(event);
				} else {
					//send_note_off(event);
					send_event(event);
				}
			}
		}
	}

	void clear() {
		MidiEvent::Event event;

		for (int i = 0; i < MidiEvent::NUM_SOURCES; ++i) {
			send_tied_note_off(i);
		}

		for (int i = 0; i < MidiPort::NUM_PORTS; ++i) {
			while (scheduler_[i].pull_event(event)) {
				if (MidiEvent::is_note_off(&event)) {
					send_event(event);
				}
			}
		}
	}


private:
	MidiPort *midiPort_;
	CvOutputEngine *cv_;
	MidiScheduler<63>scheduler_[MidiPort::NUM_PORTS];
	MidiEvent::Event tied_event_[MidiEvent::NUM_SOURCES];
	MidiEvent::Event curr_event_[MidiEvent::NUM_SOURCES];

	void handle_note_on(MidiEvent::Event &event) {
		// handle tied notes
		send_tied_note_off(event.source);

		if (event.tie) {
			MidiEvent::copy_as_note_off(&tied_event_[event.source], &event);
			send_event(event);
			return;
		}

		// handle retriggers
		MidiEvent::Event off_event;
		MidiEvent::copy_as_note_off(&off_event, &event);

		if (scheduler_[event.port].find_and_remove(off_event)) {
			send_event(off_event);
		}

		// discard note if schedule is full
		if (!scheduler_[event.port].writeable()) {
			return;
		}

		// schedule note off & send note on
		scheduler_[event.port].add(off_event, off_event.time);
		send_event(event);
	}

};

#endif
