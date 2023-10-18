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

		calibrate();
	}

	int min() {
		return min_;
	}

	// should result in -5V
	void set_min(int value) {
		min_ = clip(0, max_, value);
		dac.set(0, min_);
	}

	int max() {
		return max_;
	}

	// should result in +5V
	void set_max(int value) {
		max_ = clip(min_, 65535, value);
		dac.set(0, max_);
	}

	uint16_t read(int note) {
		return table_[clip(0, kTableSize - 1, note)];
	}

	void calibrate() {
		int offset = min_;
		int range = max_ - min_;
		float inc = 1.0f / (kTableSize - 1);

		for (int i = 0; i < kTableSize; ++i) {
			table_[i] = offset + (range * i * inc);
		}
	}

	void save(FileWriter &fileWriter) {
		fileWriter.write(min_);
		fileWriter.write(max_);
		calibrate();
	}

	void load(FileReader &fileReader) {
		fileReader.read(min_);
		fileReader.read(max_);
		calibrate();
	}

private:
	static const int kMaxOctaves = 10;
	static const int kTableSize = kMaxOctaves * 12;
	uint16_t table_[kTableSize];
	uint16_t min_ = 0;
	uint16_t max_ = 65535;
};

#endif
