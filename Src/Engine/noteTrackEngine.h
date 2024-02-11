#ifndef NoteTrackEngine_h
#define NoteTrackEngine_h

#include "settings.h"
#include "trackState.h"
#include "rng.h"

template<const bool allow_randomization>
class NoteTrackEngine {

public:

	void init(int track_index, TrackState *trackState) {
		track_index_ = track_index;
		trackState_ = trackState;
		repeats_ = &trackState->repeat;
		noteTrack_ = &settings.song.track(track_index).note;
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
		uint8_t note = get_step_value(pattern, step, NoteTrack::NOTE);
		uint8_t velocity = get_step_value(pattern, step, NoteTrack::VELOCITY);
		uint8_t delay = get_step_value(pattern, step, NoteTrack::DELAY);
		uint8_t gate_length = get_step_value(pattern, step, NoteTrack::GATE_LENGTH);
		uint8_t repeats = get_step_value(pattern, step, NoteTrack::NUM_REPEATS);
		uint8_t spread = get_step_value(pattern, step, NoteTrack::REPEAT_SPREAD);
		uint8_t repeat_velocity = get_step_value(pattern, step, NoteTrack::REPEAT_VELOCITY);

		if (noteTrack_->read(NoteTrack::USE_SCALE)) {
			note = settings.song.scale.read_map(note);
		}


		MidiEvent::Event &event = trackState_->event;

		event.port = noteTrack_->port();
		event.tie = gate_length >= 127;
		event.message = MidiEvent::NOTE_ON | noteTrack_->channel();
		event.data[0] = note;
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

	NoteTrack *noteTrack_;
	TrackState *trackState_;
	RepeatEngine *repeats_;

	uint8_t track_index_;
	uint32_t length_;
	uint32_t when_;

	inline int get_step_value(uint8_t pattern, uint8_t step, NoteTrack::StepItem item) {
		if (allow_randomization) {
			if (noteTrack_->pattern.random_is_enabled(pattern, item, step)) {
				int min = noteTrack_->read_random_min(pattern, item);
				int max = noteTrack_->read_random_max(pattern, item);
				return Rng::u16(min, max);
			}
		}
		return noteTrack_->read_step(pattern, step, item);
	}

	inline bool step_triggered(uint8_t pattern, uint8_t step) {
		uint8_t step_trigger = noteTrack_->read_step(pattern, step, NoteTrack::TRIGGER);
		uint8_t probability = get_step_value(pattern, step, NoteTrack::PROBABILITY);
		return step_trigger && (probability >= Rng::u16(1, 6));
	}

};

#endif
