#ifndef TimeOut_h
#define TimeOut_h

#include "micros.h"

class TimeOut {

public:
	void start(uint32_t ms) {
		interval_ = ms * 1000;
		start_ = micros.read();
	}

	volatile bool status() {
		return static_cast<uint32_t>((micros.read() - start_) < interval_);
	}

private:
	volatile uint32_t start_ = 0;
	volatile uint32_t interval_ = 0;
};

#endif
