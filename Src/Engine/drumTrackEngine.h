#ifndef DrumTrackEngine_h
#define DrumTrackEngine_h

#include "settings.h"
#include "trackState.h"
#include "rng.h"

template<const bool allow_randomization>
class DrumTrackEngine {

public:

	void init(int track_index, TrackState *trackState) {
		track_index_ = track_index;
		trackState_ = trackState;
		repeats_ = &trackState->repeat;
		drumTrack_ = &settings.song.track(track_index).drum;
	}

	void reset() {
		trackState_->reset();
	}

	uint32_t when() {
		return when_;
	}

	uint32_t length() {
		return length_;
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

	bool tick_step(bool send_midi = true) {
		if (repeats_->tick()) {
			length_ = repeats_->interval();
			trackState_->event.data[1] = repeats_->velocity();

			if (send_midi) {
				send_step();
			}
			return true;
		}
		return false;
	}

	void send_step() {
		if (settings.song.track_is_audible(track_index_)) {
			trackState_->send_note_event(when_, length_);
		}
	}

	void process_step(uint8_t pattern, uint8_t step) {
		uint8_t velocity = get_step_value(pattern, step, DrumTrack::VELOCITY);
		uint8_t delay = get_step_value(pattern, step, DrumTrack::DELAY);
		uint8_t gate_length = get_step_value(pattern, step, DrumTrack::GATE_LENGTH);
		uint8_t repeats = get_step_value(pattern, step, DrumTrack::NUM_REPEATS);
		uint8_t spread = get_step_value(pattern, step, DrumTrack::REPEAT_SPREAD);
		uint8_t repeat_velocity = get_step_value(pattern, step, DrumTrack::REPEAT_VELOCITY);

		MidiEvent::Event &event = trackState_->event;

		event.port = drumTrack_->port();
		event.tie = gate_length >= 127;
		event.message = MidiEvent::NOTE_ON | drumTrack_->channel();
		event.data[0] = drumTrack_->read(DrumTrack::DRUM_NOTE);
		event.data[1] = velocity;

		when_ = trackState_->clock.gate_duration(delay);
		length_ = trackState_->clock.gate_duration(gate_length);

		// repeats
		repeats_->process(repeats, length_, spread, velocity, repeat_velocity);
		if (repeats_->next_interval()) {
			length_ = repeats_->interval();
			event.data[1] = repeats_->velocity();
		}
	}

private:

	DrumTrack *drumTrack_;
	TrackState *trackState_;
	RepeatEngine *repeats_;

	uint8_t track_index_;
	uint32_t length_;
	uint32_t when_;


	inline int get_step_value(uint8_t pattern, uint8_t step, DrumTrack::StepItem item) {
		if (allow_randomization) {
			if (drumTrack_->pattern.random_is_enabled(pattern, item, step)) {
				int min = drumTrack_->read_random_min(pattern, item);
				int max = drumTrack_->read_random_max(pattern, item);
				return Rng::u16(min, max);
			}
		}
		return drumTrack_->read_step(pattern, step, item);
	}

	inline bool step_triggered(uint8_t pattern, uint8_t step) {
		uint8_t step_trigger = drumTrack_->read_step(pattern, step, DrumTrack::TRIGGER);
		uint8_t probability = get_step_value(pattern, step, DrumTrack::PROBABILITY);
		return step_trigger && (probability >= Rng::u16(1, 6));
	}

};

#endif
