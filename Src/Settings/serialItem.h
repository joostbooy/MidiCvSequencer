#ifndef SerialItem_h
#define SerialItem_h

#include "serial.h"

class SerialItem {

public:
	const int min_;
	const int max_;
	const int inc_shifted_;
	const int init_value_;
	SerialMask &mask_;
	Serial serial;

	constexpr int min() const { return min_; }
	constexpr int max() const { return max_; }
	constexpr int init_value() const { return init_value_; }

	inline int inc(int inc_, bool shifted) {
		return shifted ? inc_ * inc_shifted_ : inc_;
	}

	inline int read(uint32_t *ptr) {
		return serial.read(mask_, ptr);
	}

	inline void write(uint32_t *ptr, int value) {
		if (value >= min_ && value <= max_) {
			serial.write(mask_, ptr, value);
		}
	}

};


#endif
