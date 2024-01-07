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
		fill_note_table();
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
	uint16_t cv_to_value(uint16_t cv) {
		float x = (1.f / 65535.f) * cv;
		return Dsp::cross_fade(max_, min_, x);
	}

	uint16_t note_to_value(int note) {
		return note_table_[smtlib::clip(0, kMaxNotes - 1, note)];
	}

	uint16_t semi_note_value() {
		return note_to_value(1) - note_to_value(0);
	}

	// storage
	void save(FileWriter &fileWriter) {
		fileWriter.write(min_);
		fileWriter.write(max_);
		fill_note_table();
	}

	void load(FileReader &fileReader) {
		fileReader.read(min_);
		fileReader.read(max_);
		fill_note_table();
	}

private:
	static const int kMaxOctaves = 10;
	static const int kMaxNotes = kMaxOctaves * 12;
	uint16_t min_ = 0;
	uint16_t max_ = 65535;
	uint16_t note_table_[kMaxNotes];

	uint16_t volt_to_value(float volts) {
		float x = (1.f / kMaxOctaves) * (volts + 5.f);
		return Dsp::cross_fade(max_, min_, x);
	}

	void fill_note_table() {
		for (int note = 0; note < kMaxNotes; ++note) {
			float volt = (note - 60) * (1.f / 12.f)
			note_table_[note] = volt_to_value(volt);
		}
	}
};

#endif
