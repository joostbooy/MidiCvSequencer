#ifndef TrackState_h
#define TrackState_h

#include "settings.h"
#include "midiOutputEngine.h"
#include "clockEngine.h"
#include "positionEngine.h"
#include "repeatEngine.h"
#include "reciprocal.h"
#include "curve.h"

class TrackState {

public:

	ClockEngine clock;
	RepeatEngine repeat;
	PositionEngine position;
	MidiEvent::Event event;

	void init(uint8_t track_index, MidiOutputEngine *outputEngine) {
		outputEngine_ = outputEngine;

		track_index_ = track_index;
		track_ = &settings.song.track(track_index_);

		MidiEvent::set_source(&event, MidiEvent::TRACK, track_index_);
	}

	inline Track *track() {
		return track_;
	}

	inline uint32_t pattern_index() {
		return pattern_index_;
	}

	inline uint32_t step_index() {
		return step_index_;
	}

	void reset() {
		delay_ = 0;
		pattern_index_ = 0;
		step_index_ = 0;

		clock.reset();
		position.reset();
		repeat.reset();
	}

	bool tick() {
		// check tick
		clock.set_speed(track_->clock_speed());
		position.set_mode(track_->clock_mode());

		if (clock.tick()) {
			// update pattern position
			uint32_t ticks = clock.ticks_passed();
			uint32_t total = track_->num_patterns();
			pattern_index_ = position.update_pattern(ticks, total);

			// update step position
			uint16_t mask = *track_->pattern.random_mask(pattern_index_, NoteTrack::TRIGGER);
			position.set_step_random_mask(mask);
			step_index_ = position.update_step(ticks);

			// update track delay
			delay_ = clock.swing(track_->swing());
			delay_ += clock.humanise(track_->humanise());
			delay_ += clock.gate_duration(track_->delay());

			return true;
		}

		return false;
	}

	void send_cc_event(uint8_t cc_number, uint16_t cc_value) {
		event.data[0] = cc_number;
		event.data[1] = cc_value >> 9;
		outputEngine_->send_cc(event, cc_value);
	}

	void send_bend_event(uint16_t bend_value) {
		MidiEvent::set_14bit_data(&event, bend_value >> 2);
		outputEngine_->send_bend(event, bend_value);
	}

	void send_note_event(uint32_t when, uint32_t length) {
		outputEngine_->schedule_note(event, delay_ + when, length);
	}

	void clear() {
		if (track_->type() == Track::CURVE_TRACK) {
			send_bend_event(65535 / 2);
		} else {
			outputEngine_->send_tied_note_off(event.source);
		}
	}

private:
	Track *track_;
	MidiOutputEngine *outputEngine_;

	uint8_t track_index_ = 0;
	uint8_t pattern_index_ = 0;
	uint8_t step_index_ = 0;
	uint32_t delay_;
};

#endif
