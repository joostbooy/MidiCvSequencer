#include "engine.h"
#include "scale.h"

#include "debug.h"

Engine engine;

void Engine::init() {
	state_ = STOPPED;
	clock_ticks = 0;
	processed_ticks = 0;

	midiPort.init();
	cvInputEngine.init();
	cvOutputEngine.init();
	midiOutputEngine.init(&midiPort, &cvOutputEngine);
	midiInputEngine.init(&midiOutputEngine);
	trackEngine.init(&midiOutputEngine);
}

/* MidiEvent */
void Engine::send_midi_clock_start() {
	for (int i = 0; i < MidiPort::NUM_PORTS; ++i) {
		if (settings.midiOutput(i).send_clock()) {
			midiPort.send_clock_message(i, MidiEvent::CLOCK_START);
		}
	}
}

void Engine::send_midi_clock_stop() {
	for (int i = 0; i < MidiPort::NUM_PORTS; ++i) {
		if (settings.midiOutput(i).send_clock()) {
			midiPort.send_clock_message(i, MidiEvent::CLOCK_STOP);
		}
	}
}

void Engine::send_midi_clock_continue() {
	for (int i = 0; i < MidiPort::NUM_PORTS; ++i) {
		if (settings.midiOutput(i).send_clock()) {
			midiPort.send_clock_message(i, MidiEvent::CLOCK_CONTINUE);
		}
	}
}

void Engine::send_midi_24PPQN_clock_tick() {
	for (int i = 0; i < MidiPort::NUM_PORTS; ++i) {
		MidiOutput &output = settings.midiOutput(i);

		if (output.send_clock_continues() || ((state_ == RUNNING) && output.send_clock())) {
			midiPort.send_clock_message(i, MidiEvent::CLOCK_PULSE);
		}
	}
}

/*	Engine commands */

void Engine::start() {
	send_midi_clock_start();

	clock_ticks = 0;
	processed_ticks = 0;
	midiClock.reset();
	trackEngine.stop_audition();
	cvOutputEngine.start();

	state_ = RUNNING;
}

void Engine::stop() {
	send_midi_clock_stop();

	trackEngine.reset();
	cvOutputEngine.reset();
	midiInputEngine.clear();
	midiOutputEngine.clear();

	// clearing might take a while,
	// dont reset the clock but reset processed ticks
	processed_ticks = clock_ticks;

	state_ = STOPPED;
}

void Engine::suspend() {
	midiInputEngine.suspend();
	trackEngine.suspend();
}

void Engine::resume() {
	midiInputEngine.resume();
	trackEngine.resume();
}

/*	Tasks */

void Engine::process_requests() {
	if (!requests) {
		return;
	}

	if (requests & STOP) {
		stop();
		clear_request(STOP);
	}

	if (requests & START) {
		start();
		clear_request(START);
	}

	if (requests & PAUSE) {
		state_ = PAUSED;
		clear_request(PAUSE);
	}

	if (requests & CONTINUE) {
		state_ = RUNNING;
		clear_request(CONTINUE);
	}

	if (requests & CLEAR_TRACK) {
		trackEngine.state(track_to_clear).clear();
		clear_request(CLEAR_TRACK);
	}

	if (requests & STOP_AUDITION) {
		trackEngine.stop_audition();
		clear_request(STOP_AUDITION);
	}

	if (requests & START_AUDITION) {
		if (state_ == STOPPED) {
			trackEngine.start_audition(track_to_audition, pattern_to_audition, step_to_audition);
		}
		clear_request(START_AUDITION);
	}
}

// 8 Khz interrupt
void Engine::tick() {
	uint8_t message;
	uint8_t sync_port = midiClock.sync_port();
	uint32_t ticks = micros.read();

	midiPort.receive(ticks);
	cvInputEngine.receive(ticks);

	if (sync_port != MidiClock::INTERNAL) {
		if (midiPort.receive_clock_message(sync_port, &message)) {
			switch (message)
			{
			case MidiEvent::CLOCK_PULSE:
				midiClock.sync_bpm();
				break;
			case MidiEvent::CLOCK_STOP:
				add_request(STOP);
				break;
			case MidiEvent::CLOCK_START:
				add_request(START);
				break;
			case MidiEvent::CLOCK_CONTINUE:
				add_request(CONTINUE);
				break;
			default:
				break;
			}
		}
	}

	if (midiClock.tick()) {
		if ((clock_ticks % (PPQN / 24)) == 0) {
			send_midi_24PPQN_clock_tick();
		}
		++clock_ticks;
	}

	midiPort.send();
}

// low priority
void Engine::process() {
	MidiEvent::Event event;

	for (int i = 0; i < CvInput::NUM_PORTS; ++i) {
		cvInputEngine.process(i);
		while (cvInputEngine.event_received(i, event)) {
			midiInputEngine.process_event(event);
		}
	}

	for (int i = 0; i < MidiPort::NUM_PORTS; ++i) {
		while (midiPort.buffer(i).event_received(event)) {
			midiInputEngine.process_event(event);
		}
	}
}

// 2 khz interrupt
void Engine::update() {

	while (processed_ticks != clock_ticks) {
		++processed_ticks;

		process_requests();

		if (state_ == RUNNING) {
			trackEngine.tick();
		} else if (state_ == STOPPED) {
			trackEngine.tick_audition();
		}

		midiInputEngine.tick();
		//cvInputEngine.tick();
		midiOutputEngine.tick();
		cvOutputEngine.tick();
	}

	cvOutputEngine.send();
}
