#ifndef RepeatEngine_h
#define RepeatEngine_h

#include "reciprocal.h"
#include "curve.h"

class RepeatEngine {

public:

	void reset() {
		index_ = 0;
		interval_ = 0;
		num_repeats_ = 0;
	}

	void process(uint8_t repeats, uint32_t gate_length, uint8_t spread) {
		reset();

		if (repeats < 7) {
			int repeats_ = 7 - repeats + 1;
			fill_table(repeats_, gate_length, spread);
		} else if (repeats > 7) {
			int repeats_ = 1 + repeats - 7;
			fill_table(repeats_, gate_length * repeats_, spread);
		}
	}

	uint32_t interval() {
		return interval_;
	}

	bool tick() {
		if (interval_ > 0) {
			--interval_;
			return false;
		}
		return next_interval();
	}

	bool next_interval() {
		if (index_ < num_repeats_) {
			interval_ = table_[index_++];
			return true;
		}
		return false;
	}

	static const char* repeat_text(uint8_t repeats) {
		if (repeats < 7) {
			return UiText::str.write("/", 7 - repeats + 1);
		} else if (repeats > 7) {
			return UiText::str.write("X", 1 + repeats - 7);
		}
		return "0";
	}

private:
	uint8_t num_repeats_;
	uint8_t index_;
	uint32_t interval_;
	uint32_t table_[8];
	Reciprocal<16>reciprocal;

	void fill_table(uint8_t repeats, uint32_t gate_length, uint8_t spread) {
		uint8_t index = 0;
		uint32_t total_length = 0;

		float phase = 0.f;
		float inc = reciprocal(repeats);
		float spread_ = reciprocal(6) * spread;

		for (int i = 0; i < repeats; ++i) {
			uint32_t repeat_length = gate_length * Curve::bend(phase += inc, spread_);

			repeat_length -= total_length;
			total_length += repeat_length;

			if (repeat_length > 0) {
				table_[index] = repeat_length;
				++index;
			}
		}

		num_repeats_ = index;
	}
};

#endif
