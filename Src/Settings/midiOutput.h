#ifndef MidiOutput_h
#define MidiOutput_h

#include "midiPort.h"

class MidiOutput {

public:

	void init() {
		set_send_clock(true);
		set_send_clock_continues(false);
	}

	// send clock
	bool send_clock() {
		return send_clock_;
	}

	void set_send_clock(bool value) {
		send_clock_ = value;
	}

	const char* send_clock_text() {
		return send_clock() ? "ON" : "OFF";
	}

	// send continues clock
	bool send_clock_continues() {
		return send_continues_clock_;
	}

	void set_send_clock_continues(bool value) {
		send_continues_clock_ = value;
	}

	const char* send_clock_continues_text() {
		return send_clock_continues() ? "ON" : "OFF";
	}

	// storage
	void load(FileReader &fileReader) {
		fileReader.read(send_clock_);
		fileReader.read(send_continues_clock_);
	}

	void save(FileWriter &fileWriter) {
		fileWriter.write(send_clock_);
		fileWriter.write(send_continues_clock_);
	}

private:
	bool send_clock_;
	bool send_continues_clock_;
};

#endif
