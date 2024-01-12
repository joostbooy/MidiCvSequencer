#ifndef Calibration_h
#define Calibration_h

#include "dac.h"
#include "fileReader.h"
#include "fileWriter.h"

class Calibration {

public:

	void init() {
		min_ = 0;
		max_ = 65535;
	}

	int min() {
		return min_;
	}

	// should result in -5V
	void set_min(int value) {
		min_ = stmlib::clip(0, max_, value);
	}

	int max() {
		return max_;
	}

	// should result in +5V
	void set_max(int value) {
		max_ = stmlib::clip(min_, 65535, value);
	}

	// methods
	uint16_t read(uint16_t value) {
		float x = (1.f / 65535.f) * value;
		return Dsp::cross_fade(max_, min_, x);
	}

	uint16_t note_to_value(int note) {
		float x = (1.f / kMaxNotes) * stmlib::clip(0, kMaxNotes - 1, note);
		return Dsp::cross_fade(max_, min_, x);
	}

	uint16_t semi_note_value() {
		return note_to_value(0) - note_to_value(1);
	}

	// storage
	void save(FileWriter &fileWriter) {
		fileWriter.write(min_);
		fileWriter.write(max_);
	}

	void load(FileReader &fileReader) {
		fileReader.read(min_);
		fileReader.read(max_);
	}

private:
	static const int kMaxVolts = 10; //5V p.p
	static const int kMaxNotes = kMaxVolts * 12;
	uint16_t min_ = 0;
	uint16_t max_ = 65535;
};

#endif
