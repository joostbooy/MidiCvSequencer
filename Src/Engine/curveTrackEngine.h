#ifndef CurveTrackEngine_h
#define CurveTrackEngine_h

#include "settings.h"
#include "trackState.h"
#include "rng.h"
#include "dsp.h"

class CurveTrackEngine {

public:

	void init(int track_index, TrackState *trackState) {
		track_index_ = track_index;
		trackState_ = trackState;
		curveTrack_ = &settings.song.track(track_index).curve;
	}

	void reset() {
		int init_value = curveTrack_->read(CurveTrack::INIT_VALUE);

		curr_value = init_value << 9;
		last_value = curr_value;
		target_value = curr_value;
		trackState_->event.data[1] = init_value;

		trackState_->reset();
	}

	float phase() {
		return phase_;
	}

	void tick() {
		if (trackState_->tick()) {
			uint8_t pattern = trackState_->pattern_index();
			uint8_t step = trackState_->step_index();
			if (step_triggered(pattern, step)) {
				process_step(pattern, step);
				send_step();
			}
		}
		tick_step();
	}

	void tick_step() {
		float curve = Curve::bend(phase_, shape);
		int value = Dsp::cross_fade(last_value, target_value, curve);

		phase_ += inc;
		if (phase_ >= 1.f) {
			phase_ = 1.f;
		}

		if (value != curr_value) {
			curr_value = value;
			send_step();
		}
	}

	void send_step() {
		if (settings.song.track_is_audible(track_index_)) {
			if (curveTrack_->read(CurveTrack::SEND_CC)) {
				send_as_cc();
			}

			if (curveTrack_->read(CurveTrack::SEND_BEND)) {
				send_as_bend();
			}
		}
	}

	void process_step(uint8_t pattern, uint8_t step) {
		MidiEvent::Event &event = trackState_->event;

		event.tie = false;
		event.port = curveTrack_->port();
		event.message = MidiEvent::CONTROLLER_CHANGE | curveTrack_->channel();

		cc_number = curveTrack_->read(CurveTrack::CC_NUMBER);

		last_value = curr_value;
		target_value = get_step_value(pattern, step, CurveTrack::CC_VALUE);
		target_value <<= 9;	// convert to 16 bit

		phase_ = 0.f;
		shape = (1.f / 8.f) * get_step_value(pattern, step, CurveTrack::SHAPE);
		inc = trackState_->clock.step_duration_reciprocal();
	}

private:
	CurveTrack *curveTrack_;
	TrackState *trackState_;

	float phase_;
	float inc;
	float shape;
	int last_value;
	int target_value;
	int curr_value;

	uint8_t track_index_;
	uint8_t cc_number;

	inline int get_step_value(uint8_t pattern, uint8_t step, CurveTrack::StepItem item) {
		if (curveTrack_->pattern.random_is_enabled(pattern, item, step)) {
			int min = curveTrack_->read_random_min(pattern, item);
			int max = curveTrack_->read_random_max(pattern, item);
			return Rng::u16(min, max);
		}
		return curveTrack_->read_step(pattern, step, item);
	}

	inline bool step_triggered(uint8_t pattern, uint8_t step) {
		uint8_t step_trigger = curveTrack_->read_step(pattern, step, CurveTrack::TRIGGER);
		uint8_t probability = get_step_value(pattern, step, CurveTrack::PROBABILITY);
		return step_trigger && (probability >= Rng::u16(1, 6));
	}

	inline void send_as_bend() {
		trackState_->event.message &= ~(0xF0);
		trackState_->event.message |= MidiEvent::PITCH_BEND;
		trackState_->send_bend_event(curr_value);
	}

	inline void send_as_cc() {
		trackState_->event.message &= ~(0xF0);
		trackState_->event.message |= MidiEvent::CONTROLLER_CHANGE;
		trackState_->send_cc_event(cc_number, curr_value);
	}

};

#endif
