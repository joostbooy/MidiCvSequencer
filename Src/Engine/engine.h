#ifndef Engine_h
#define Engine_h

#include <functional>

#include "micros.h"
#include "stack.h"
#include "midiEvent.h"
#include "midiInputEngine.h"
#include "midiOutputEngine.h"
#include "cvOutputEngine.h"
#include "cvInputEngine.h"
#include "trackEngine.h"
#include "settings.h"

class Engine {

public:

	enum State {
		STOPPED,
		PAUSED,
		RUNNING
	};

	enum Request {
		START			= (1 << 0),
		STOP			= (1 << 1),
		PAUSE			= (1 << 2),
		CONTINUE		= (1 << 3),
		START_AUDITION	= (1 << 4),
		STOP_AUDITION	= (1 << 5),
		CLEAR_TRACK		= (1 << 6),
	};

	void init();
	void tick();
	void update();
	void suspend();
	void resume();
	void process();

	float processing_time();

	// states
	State state() {
		return state_;
	}

	// requests
	void add_request_wait(Request type) {
		add_request(type);
		while (requests & type);
	}

	void request_track_clear_wait(uint8_t track) {
		track_to_clear = track;
		add_request_wait(CLEAR_TRACK);
	}

	void request_audition_wait(uint8_t track, uint8_t pattern, uint8_t step) {
		track_to_audition = track;
		pattern_to_audition = pattern;
		step_to_audition = step;
		add_request_wait(START_AUDITION);
	}

	void clear_request(Request type) {
		uint8_t flags = requests;
		requests = flags & ~type;
	}

	MidiPort midiPort;
	MidiClock &midiClock = settings.song.midiClock;
	MidiInputEngine midiInputEngine;
	MidiOutputEngine midiOutputEngine;
	CvOutputEngine cvOutputEngine;
	CvInputEngine cvInputEngine;
	TrackEngine trackEngine;

private:
	volatile State state_;

	volatile uint8_t requests = 0x00;
	volatile uint32_t clock_ticks = 0;
	volatile uint32_t processed_ticks = 0;
	volatile uint32_t processing_time_ = 0;

	volatile uint8_t track_to_clear = 0;
	volatile uint8_t track_to_audition = 0;
	volatile uint8_t pattern_to_audition = 0;
	volatile uint8_t step_to_audition = 0;

	void start();
	void stop();
	void process_requests();
	void send_midi_clock_start();
	void send_midi_clock_stop();
	void send_midi_clock_continue();
	void send_midi_24PPQN_clock_tick();

	void add_request(Request type) {
		uint8_t flags = requests;
		requests = flags | type;
	}
};

extern Engine engine;

#endif
