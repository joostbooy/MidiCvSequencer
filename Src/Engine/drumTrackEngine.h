#ifndef DrumTrackEngine_h
#define DrumTrackEngine_h

#include "trackState.h"
#include "rng.h"

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
		if (repeats_->tick()) {
			length = repeats_->interval();
			send_step();
		}
	}

	void send_step() {
		if (settings.song.track_is_audible(track_index_)) {
			trackState_->send_note_event(when, length);
		}
	}

	void process_step(uint8_t pattern, uint8_t step) {
		uint8_t note_index = get_step_value(pattern, step, DrumTrack::DRUM_NOTE);
		uint8_t velocity = get_step_value(pattern, step, DrumTrack::VELOCITY);
		uint8_t delay = get_step_value(pattern, step, DrumTrack::DELAY);
		uint8_t gate_length = get_step_value(pattern, step, DrumTrack::GATE_LENGTH);
		uint8_t repeats = get_step_value(pattern, step, DrumTrack::NUM_REPEATS);
		uint8_t spread = get_step_value(pattern, step, DrumTrack::REPEAT_SPREAD);

		MidiEvent::Event &event = trackState_->event;

		event.port = drumTrack_->port();
		event.tie = gate_length >= 64;
		event.message = MidiEvent::NOTE_ON | drumTrack_->channel();
		event.data[0] = get_drum_note(note_index);
		event.data[1] = velocity;

		when = trackState_->clock.gate_duration(delay);
		length = trackState_->clock.gate_duration(gate_length);

		repeats_->process(repeats, length, spread);
		if (repeats_->next_interval()) {
			length = repeats_->interval();
		}
	}

private:
	DrumTrack *drumTrack_;
	TrackState *trackState_;
	RepeatEngine* repeats_;

	uint8_t track_index_;
	uint32_t when;
	uint32_t length;

	inline uint8_t get_drum_note(uint8_t index) {
		switch (index)
		{
		case 0: return drumTrack_->read(DrumTrack::DRUM_NOTE_1);
		case 1: return drumTrack_->read(DrumTrack::DRUM_NOTE_2);
		case 2:	return drumTrack_->read(DrumTrack::DRUM_NOTE_3);
		case 3:	return drumTrack_->read(DrumTrack::DRUM_NOTE_4);
		default:
			break;
		}
		return 0;
	}

	inline int get_step_value(uint8_t pattern, uint8_t step, DrumTrack::StepItem item) {
		if (drumTrack_->pattern.random_is_enabled(pattern, item, step)) {
			int min = drumTrack_->read_random_min(pattern, item);
			int max = drumTrack_->read_random_max(pattern, item);
			return Rng::u16(min, max);
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
