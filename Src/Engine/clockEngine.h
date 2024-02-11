#ifndef ClockEngine_h
#define ClockEngine_h

#include "lookupTables.h"
#include "stringBuilder.h"
#include "uiText.h"
#include "rng.h"

class ClockEngine {

public:

	enum Speed {
		DOTTED_1TH,
		WHOLE_1TH,
		TRIPLET_1TH,
		DOTTED_2TH,
		WHOLE_2TH,
		TRIPLET_2TH,
		DOTTED_4TH,
		WHOLE_4TH,
		TRIPLET_4TH,
		DOTTED_8TH,
		WHOLE_8TH,
		TRIPLET_8TH,
		DOTTED_16TH,
		WHOLE_16TH,
		TRIPLET_16TH,
		DOTTED_32TH,
		WHOLE_32TH,
		TRIPLET_32TH,

		NUM_SIGNATURES
	};

	static const char* speed_text(int value) {
		switch (value)
		{
		case DOTTED_1TH: 	return "1/1D";
		case WHOLE_1TH:		return "1/1";
		case TRIPLET_1TH:	return "1/1T";
		case DOTTED_2TH: 	return "1/2D";
		case WHOLE_2TH:		return "1/2";
		case TRIPLET_2TH:	return "1/2T";
		case DOTTED_4TH:	return "1/4D";
		case WHOLE_4TH:		return "1/4";
		case TRIPLET_4TH:	return "1/4T";
		case DOTTED_8TH:	return "1/8D";
		case WHOLE_8TH:		return "1/8";
		case TRIPLET_8TH:	return "1/8T";
		case DOTTED_16TH:	return "1/16D";
		case WHOLE_16TH:	return "1/16";
		case TRIPLET_16TH:	return "1/16T";
		case DOTTED_32TH:	return "1/32D";
		case WHOLE_32TH:	return "1/32";
		case TRIPLET_32TH:	return "1/32T";
		default:			return nullptr;
			break;
		}
	}

	void set_speed(uint8_t speed) {
		speed_ = speed;
	}

	void reset() {
		triggered_ = 0;
		ticks_passed_ = 0;

		prescaler_32th_triplet = 0;
		prescaler_32th_whole = 0;
		prescaler_32th_dotted = 0;

		ticks_32th_triplet = 0;
		ticks_32th_whole = 0;
		ticks_32th_dotted = 0;
	}

	bool tick() {
		triggered_ = trigger();
		ticks_passed_ = ticks();

		if (++prescaler_32th_triplet >= step_duration_[TRIPLET_32TH]) {
			prescaler_32th_triplet = 0;
			++ticks_32th_triplet;
		}

		if (++prescaler_32th_whole >= step_duration_[WHOLE_32TH]) {
			prescaler_32th_whole = 0;
			++ticks_32th_whole;
		}

		if (++prescaler_32th_dotted >= step_duration_[DOTTED_32TH]) {
			prescaler_32th_dotted = 0;
			++ticks_32th_dotted;
		}

		return triggered_;
	}


	inline uint32_t humanise(int amount) {
		return Rng::reciprocal() * amount * (step_duration() / 8);
	}

	inline uint32_t swing(int amount) {
		if (amount < 8) {
			return (ticks_passed_ & 1) == 0 ? (8 - amount) * (step_duration() / 8) : 0;
		} else if (amount > 8) {
			return (ticks_passed_ & 1) == 1 ? (amount - 8) * (step_duration() / 8) : 0;
		}
		return 0;
	}

	inline bool triggered() {
		return triggered_;
	}

	inline uint32_t ticks_passed() {
		return ticks_passed_;
	}

	inline const uint32_t step_duration() {
		return step_duration(speed_);
	}

	inline const float step_duration_reciprocal() {
		return step_duration_reciprocal(speed_);
	}

	inline const uint32_t gate_duration(uint8_t index) {
		return gate_duration(speed_, index);
	}

	//static
	static inline const uint32_t step_duration(uint8_t speed) {
		return step_duration_[speed];
	}

	static inline const float step_duration_reciprocal(uint8_t speed) {
		return step_duration_reciprocal_[speed];
	}

	static inline const uint32_t gate_duration(uint8_t speed, uint8_t index) {
		uint32_t whole_step = step_duration_[speed];
		uint32_t eighth_step = whole_step / 8;
		return (whole_step * (index / 8)) + (eighth_step * (index % 8));
	}

	static const char* swing_text(int value) {
		return UiText::str.write(value - 8);
	}

	static const char* gate_duration_text(uint8_t value) {
		if (value < 127) {
			int steps = value / 8;
			int fraction = value % 8;
			return UiText::str.write(steps, ".", 125 * fraction);
		}
		return "TIE";
	}

private:

	uint8_t speed_;

	bool triggered_;
	uint32_t ticks_passed_;

	uint8_t prescaler_32th_triplet;
	uint8_t prescaler_32th_whole;
	uint8_t prescaler_32th_dotted;

	uint32_t ticks_32th_triplet;
	uint32_t ticks_32th_whole;
	uint32_t ticks_32th_dotted;

	static const uint32_t step_duration_[NUM_SIGNATURES];
	static const float step_duration_reciprocal_[NUM_SIGNATURES];

	inline uint32_t ticks() {
		switch (speed_)
		{
		case DOTTED_1TH: 	return ticks_32th_dotted 	/ 32;
		case WHOLE_1TH:		return ticks_32th_whole 	/ 32;
		case TRIPLET_1TH:	return ticks_32th_triplet 	/ 32;
		case DOTTED_2TH: 	return ticks_32th_dotted 	/ 16;
		case WHOLE_2TH:		return ticks_32th_whole 	/ 16;
		case TRIPLET_2TH:	return ticks_32th_triplet	/ 16;
		case DOTTED_4TH:	return ticks_32th_dotted 	/ 8;
		case WHOLE_4TH:		return ticks_32th_whole 	/ 8;
		case TRIPLET_4TH:	return ticks_32th_triplet 	/ 8;
		case DOTTED_8TH:	return ticks_32th_dotted	/ 4;
		case WHOLE_8TH:		return ticks_32th_whole 	/ 4;
		case TRIPLET_8TH:	return ticks_32th_triplet	/ 4;
		case DOTTED_16TH:	return ticks_32th_dotted 	/ 2;
		case WHOLE_16TH:	return ticks_32th_whole 	/ 2;
		case TRIPLET_16TH:	return ticks_32th_triplet 	/ 2;
		case DOTTED_32TH:	return ticks_32th_dotted;
		case WHOLE_32TH:	return ticks_32th_whole;
		case TRIPLET_32TH:	return ticks_32th_triplet;
		default:			break;
		}
		return 0;
	}

	inline bool trigger() {
		switch (speed_)
		{
		case DOTTED_1TH: 	return !prescaler_32th_dotted 	&& !(ticks_32th_dotted 	% 32);
		case WHOLE_1TH:		return !prescaler_32th_whole 	&& !(ticks_32th_whole 	% 32);
		case TRIPLET_1TH:	return !prescaler_32th_triplet 	&& !(ticks_32th_triplet	% 32);
		case DOTTED_2TH: 	return !prescaler_32th_dotted 	&& !(ticks_32th_dotted	% 16);
		case WHOLE_2TH:		return !prescaler_32th_whole	&& !(ticks_32th_whole	% 16);
		case TRIPLET_2TH:	return !prescaler_32th_triplet	&& !(ticks_32th_triplet % 16);
		case DOTTED_4TH:	return !prescaler_32th_dotted	&& !(ticks_32th_dotted 	% 8);
		case WHOLE_4TH:		return !prescaler_32th_whole 	&& !(ticks_32th_whole 	% 8);
		case TRIPLET_4TH:	return !prescaler_32th_triplet 	&& !(ticks_32th_triplet % 8);
		case DOTTED_8TH:	return !prescaler_32th_dotted 	&& !(ticks_32th_dotted 	% 4);
		case WHOLE_8TH:		return !prescaler_32th_whole 	&& !(ticks_32th_whole 	% 4);
		case TRIPLET_8TH:	return !prescaler_32th_triplet 	&& !(ticks_32th_triplet % 4);
		case DOTTED_16TH:	return !prescaler_32th_dotted 	&& !(ticks_32th_dotted 	% 2);
		case WHOLE_16TH:	return !prescaler_32th_whole 	&& !(ticks_32th_whole 	% 2);
		case TRIPLET_16TH:	return !prescaler_32th_triplet 	&& !(ticks_32th_triplet % 2);
		case DOTTED_32TH:	return !prescaler_32th_dotted;
		case WHOLE_32TH:	return !prescaler_32th_whole;
		case TRIPLET_32TH:	return !prescaler_32th_triplet;
		default:			break;
		}
		return false;
	}
};

#endif
