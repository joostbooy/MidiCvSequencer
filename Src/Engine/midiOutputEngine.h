#ifndef MidiOutputEngine_h
#define MidiOutputEngine_h

#include "midiPort.h"
#include "midiScheduler.h"
#include "cvOutputEngine.h"

class MidiOutputEngine {

public:

	void init(MidiPort *midiPort, CvOutputEngine *cvOuput) {
		cvOutput_ = cvOuput;
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
			send_note_off(event);
		}
	}

	void send_note_off(MidiEvent::Event &event) {
		cvOutput_->write_note_off(event);
		midiPort_->buffer(event.port).write_out(&event);
		curr_event_[event.source] = event;
	}

	void send_note_on(MidiEvent::Event &event) {
		cvOutput_->write_note_on(event);
		if (midiPort_->buffer(event.port).write_out_non_blocking(&event)) {
			curr_event_[event.source] = event;
		}
	}

	void send_cc(MidiEvent::Event &event, int value_16_bit = -1) {
		cvOutput_->write_cc(event, value_16_bit);
		if (midiPort_->buffer(event.port).write_out_non_blocking(&event)) {
			curr_event_[event.source] = event;
		}
	}

	void send_bend(MidiEvent::Event &event, int value_16_bit = -1) {
		cvOutput_->write_bend(event, value_16_bit);
		if (midiPort_->buffer(event.port).write_out_non_blocking(&event)) {
			curr_event_[event.source] = event;
		}
	}

	void tick() {
		MidiEvent::Event event;

		for (int i = 0; i < MidiPort::NUM_PORTS; ++i) {
			while (scheduler_[i].tick(event)) {
				if (MidiEvent::is_note_on(&event)) {
					handle_note_on(event);
				} else {
					send_note_off(event);
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
					send_note_off(event);
				}
			}
		}
	}


private:
	MidiPort *midiPort_;
	CvOutputEngine *cvOutput_;
	MidiScheduler<63>scheduler_[MidiPort::NUM_PORTS];
	MidiEvent::Event tied_event_[MidiEvent::NUM_SOURCES];
	MidiEvent::Event curr_event_[MidiEvent::NUM_SOURCES];

	void handle_note_on(MidiEvent::Event &event) {
		// handle tied notes
		send_tied_note_off(event.source);

		if (event.tie) {
			MidiEvent::copy_as_note_off(&tied_event_[event.source], &event);
			send_note_on(event);
			return;
		}

		// handle retriggers
		MidiEvent::Event off_event;
		MidiEvent::copy_as_note_off(&off_event, &event);

		if (scheduler_[event.port].find_and_remove(off_event)) {
			send_note_off(off_event);
		}

		// discard note if schedule is full
		if (!scheduler_[event.port].writeable()) {
			return;
		}

		// schedule note off & send note on
		scheduler_[event.port].add(off_event, off_event.time);
		send_note_on(event);
	}

};

#endif
