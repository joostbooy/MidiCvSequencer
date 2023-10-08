#ifndef ArpeggiatorEngine_h
#define ArpeggiatorEngine_h

#include "stmf4lib.h"
#include "arpeggiator.h"
#include "clockEngine.h"
#include "reciprocal.h"

class ArpeggiatorEngine {

public:

	void init(Arpeggiator *arppegiator) {
		arpeggiator_ = arppegiator;
		clear_notes();
		reset();
	}

	void reset() {
		trigger_position_ = 0;
		clockEngine.reset();
		reset(&note_clock, arpeggiator_->note_mode());
		reset(&gate_clock, arpeggiator_->gate_mode());
		reset(&velocity_clock, arpeggiator_->velocity_mode());
	}

	bool tick() {
		clockEngine.set_speed(arpeggiator_->speed());

		if (clockEngine.tick() && trigger()) {
			tick(&note_clock, arpeggiator_->note_mode());
			tick(&gate_clock, arpeggiator_->gate_mode());
			tick(&velocity_clock, arpeggiator_->velocity_mode());
			return true;
		}
		return false;
	}

	void clear_notes() {
		size_ = 0;
	}

	void set_note(uint8_t note) {
		if (size_ < kMaxSize) {
			notes_[size_++] = note;
		}
	}

	void set_velocity(uint8_t velocity) {
		velocity_ = velocity;
	}

	void set_gate_length(uint8_t gate_length) {
		gate_length_ = clockEngine.gate_duration(gate_length);
	}

	uint8_t note() {
		if (arpeggiator_->note_mode() == Arpeggiator::OFF) {
			return notes_[0];
		}
		return notes_[note_clock.curr];
	}

	uint8_t velocity() {
		if (arpeggiator_->velocity_mode() == Arpeggiator::OFF) {
			return velocity_;
		}
		return reciprocal(size_) * (velocity_clock.curr + 1) * velocity_;
	}

	uint32_t gate_length() {
		if (arpeggiator_->gate_mode() == Arpeggiator::OFF) {
			return gate_length_;
		}
		return reciprocal(size_) * (gate_clock.curr + 1) * gate_length_;
	}

private:
	static const int kMaxSize = 16;
	uint8_t size_;
	uint8_t notes_[kMaxSize];
	uint8_t trigger_position_;
	uint8_t velocity_;
	uint32_t gate_length_;

	static Reciprocal<kMaxSize>reciprocal;

	struct Clock {
		int phase;
		int curr;
		int next;
	};

	Clock note_clock;
	Clock velocity_clock;
	Clock gate_clock;

	Arpeggiator *arpeggiator_;
	ClockEngine clockEngine;

	void reset(Clock *clock, uint8_t mode);
	void tick(Clock *clock, uint8_t mode);

	bool trigger() {
		uint8_t pos = trigger_position_++;
		uint8_t mask = arpeggiator_->trigger_mask();
		return mask & (1 << (pos & 3));
	}
};

#endif
