#ifndef PositionEngine_h
#define PositionEngine_h

#include "rng.h"

class PositionEngine {

public:

	enum Mode {
		FORWARD,
		BACKWARD,
		ZIGZAG,
		ZIGZAG_REVERSE,
		RANDOM,
		DRUNKEN_WALK,
		SHUFFLE,

		NUM_MODES
	};

	static const char* mode_text(int mode) {
		switch (mode)
		{
		case FORWARD:			return "FORWARD";
		case BACKWARD:			return "BACKWARD";
		case ZIGZAG:			return "ZIGZAG";
		case ZIGZAG_REVERSE:	return "ZIGZAG REVERSE";
		case RANDOM:			return "RANDOM";
		case DRUNKEN_WALK:		return "DRUNKEN WALK";
		case SHUFFLE:			return "SHUFFLE";
		default:				return nullptr;
			break;
		}
	}

	void reset() {
		step_ = 0;
		pattern_ = 0;
		random_mask = 0;
		exclude_mask = 0;

		for (int i = 0; i < 16; ++i) {
			shuffle_array[i] = i;
		}
	}

	void set_mode(uint8_t mode) {
		mode_ = Mode(mode);
	}

	void set_step_random_mask(uint16_t mask) {
		random_mask = mask;
	}

	uint32_t update_pattern(uint32_t ticks, uint16_t num_patterns) {
		pattern_ = (ticks / 16) % num_patterns;
		return pattern_;
	}

	uint32_t update_step(uint32_t ticks) {
		switch (mode_)
		{
		case FORWARD:
			step_ = ticks % 16;
			break;
		case BACKWARD:
			step_ = 15 - (ticks % 16);
			break;
		case ZIGZAG:
			ticks %= 16;
			step_ = ticks / 2;
			if ((ticks % 2) == 1) {
				step_ = 15 - step_;
			}
			break;
		case ZIGZAG_REVERSE:
			ticks %= 16;
			step_ = ticks / 2;
			if ((ticks % 2) == 0) {
				step_ = 15 - step_;
			}
			break;
		case RANDOM:
			step_ = ticks % 16;
			if (random_mask & (1 << step_)) {
				step_ = Rng::u16() % 16;
			}
			break;
		case DRUNKEN_WALK:
			if (random_mask & (1 << (ticks % 16))) {
				step_ = next_drunken_walk(step_);
			} else {
				step_ = ticks % 16;
			}
			break;
		case SHUFFLE:
			step_ = ticks % 16;
			if (step_ == 0) {
				shuffle_steps();
			}

			if (random_mask & (1 << step_)) {
				int res = next_shuffled_step();
				if (res >= 0) {
					step_ = res;
				}
			}
			break;
		default:
			step_ = 0;
			break;
		}
		return step_;
	}


private:
	Mode mode_;

	uint8_t step_;
	uint8_t pattern_;

	uint16_t random_mask;
	uint16_t exclude_mask;
	uint8_t shuffle_array[16];

	void shuffle_steps() {
		exclude_mask = ~random_mask;
		for (int i = 0; i < 16; ++i) {
			uint8_t rng_value = Rng::u16() % 16;
			stmlib::swap(shuffle_array[i], shuffle_array[rng_value]);
		}
	}

	int next_shuffled_step() {
		for (int i = 0; i < 16; ++i) {
			int step = shuffle_array[i];
			if (!(exclude_mask & (1 << step))) {
				exclude_mask |= (1 << step);
				return step;
			}
		}
		return -1;
	}

	uint8_t next_drunken_walk(uint8_t step) {
		uint8_t dir = Rng::u16() % 4;

		if (dir == 0) {
			return (step - 1) % 16;
		} else if (dir == 1){
			return (step + 1) % 16;
		}
		return step;
	}

};

#endif
