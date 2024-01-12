#ifndef Serial_h
#define Serial_h

#include "stmf4lib.h"

struct SerialMask {
	const uint8_t reg;		// 32 bits per register
	const uint8_t bits;		// max 32
	const uint8_t shifts;	// prev bits + prev shifts
	const uint32_t value = ((1 << bits) - 1) << shifts;
};

class Serial {

public:

	inline uint32_t read(SerialMask &mask, uint32_t *ptr) {
		uint32_t data = ptr[mask.reg];
		return (data & mask.value) >> mask.shifts;
	}

	inline void write(SerialMask &mask, uint32_t *ptr, int value) {
		uint32_t data = ptr[mask.reg];
		ptr[mask.reg] = (data & ~mask.value) | (value << mask.shifts);
	}
};


#endif
