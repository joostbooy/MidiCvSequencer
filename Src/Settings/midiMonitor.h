#ifndef MidiMonitor_h
#define MidiMonitor_h

#include "midiEvent.h"
#include "midiFilter.h"

class MidiMonitor {

public:

	void init(MidiFilter *filter) {
		size_ = 0;
		filter_ = filter;
	}

	void clear() {
		size_ = 0;
	}

	int size() {
		return size_;
	}

	void push(MidiEvent::Event &event) {
		if ((size_ < kMax) && filter_->event_allowed(event)) {
			event_[size_] = event;
			++size_;
		}
	}

	bool pull(MidiEvent::Event &event) {
		if (size_) {
			event = event_[--size_];
			return true;
		}
		return false;
	}


public:
	int size_;
	static const int kMax = 32;
	MidiEvent::Event event_[kMax];
	MidiFilter *filter_;
};

#endif
