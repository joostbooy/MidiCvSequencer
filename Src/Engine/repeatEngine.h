#ifndef RepeatEngine_h
#define RepeatEngine_h

#include "reciprocal.h"
#include "curve.h"

class RepeatEngine {

public:

	enum VelocityRamp {
		UP,
		NONE,
		DOWN
	};

	static const char* velocity_ramp_text(uint8_t type) {
		switch (type)
		{
		case UP:	return "/";
		case NONE:	return "-";
		case DOWN:	return "\\";
		default:	return nullptr;
			break;
		}
	}

	static const char* spread_text(int spread) {
		return UiText::str.write(spread - 3);
	}

	static const char* repeat_text(uint8_t repeats) {
		if (repeats < 7) {
			return UiText::str.write("/", 7 - repeats + 1);
		} else if (repeats > 7) {
			return UiText::str.write("X", 1 + repeats - 7);
		}
		return "0";
	}


	void reset() {
		index_ = 0;
		interval_ = 0;
		num_repeats_ = 0;

		velocity_ramp_ = NONE;
		velocity_base_ = 0;
	}


	void process(uint8_t repeats, uint32_t gate_length, uint8_t spread, uint8_t vel, uint8_t vel_ramp) {
		reset();

		velocity_base_ = vel;
		velocity_ramp_ = vel_ramp;

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

	uint8_t velocity() {
		return velocity_;
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
			velocity_ = next_velocity();
			return true;
		}
		return false;
	}

private:

	uint8_t num_repeats_;
	uint8_t index_;
	uint8_t velocity_;
	uint8_t velocity_base_;
	uint8_t velocity_ramp_;
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

	uint8_t next_velocity() {
		if (velocity_ramp_ == UP) {
			return velocity_base_ * reciprocal(num_repeats_) * index_;
		} else if (velocity_ramp_ == DOWN) {
			return velocity_base_ * reciprocal(num_repeats_) * (num_repeats_ - (index_ - 1));
		}
		return velocity_base_;
	}

};

#endif
