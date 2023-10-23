#ifndef VoltPerOctave_h
#define VoltPerOctave_h

#include "dac.h"
#include "fileReader.h"
#include "fileWriter.h"

class VoltPerOctave {

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
		min_ = clip(0, max_, value);
	}

	int max() {
		return max_;
	}

	// should result in +5V
	void set_max(int value) {
		max_ = clip(min_, 65535, value);
	}

	// methods
	uint16_t cv_to_value(uint16_t value) {
		float volt = cv_to_volt(value);
		return volt_to_value(volt);
	}

	uint16_t note_to_value(int note) {
		float volt = note_to_volt(note);
		return volt_to_value(volt);
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
	static const int kMaxOctaves = 10;
	static const int kMaxNotes = kMaxOctaves * 12;
	uint16_t min_ = 0;
	uint16_t max_ = 65535;

	float note_to_volt(int note) {
		note = clip(0, kMaxNotes, note);
		return (note - 60) * (1.f / 12.f);
	}

	float cv_to_volt(uint16_t value) {
		float x = (1.f / 65535.f) * value;
		return Dsp::cross_fade(-5.f, 5.f, x);
	}

	uint16_t volt_to_value(float volts) {
		float x = (1.f / kMaxOctaves) * (volts + 5.f);
		return Dsp::cross_fade(max_, min_, x);
	}
};

#endif
