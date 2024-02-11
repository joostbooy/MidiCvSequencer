#ifndef ChordTrackEngine_h
#define ChordTrackEngine_h

#include "settings.h"
#include "trackState.h"
#include "rng.h"
#include "chord.h"
#include "arpeggiator.h"
#include "arpeggiatorEngine.h"

template<const bool allow_randomization>
class ChordTrackEngine {

public:

	void init(int track_index, TrackState *trackState) {
		track_index_ = track_index;
		trackState_ = trackState;
		chordTrack_ = &settings.song.track(track_index).chord;

		arpeggiator.init();
		arpeggiatorEngine.init(&arpeggiator);
	}

	void reset() {
		trackState_->reset();
		arpeggiator.init();
		arpeggiatorEngine.reset();
	}

	uint32_t when() {
		return when_;
	}

	uint32_t length() {
		return length_;
	}

	uint8_t oct_offset() {
		return oct_offset_;
	}

	bool arpeggiator_enabled() {
		return arpeggiator.enabled();
	}

	Chord &chord() {
		return chord_;
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

	void send_step() {
		if (arpeggiator.enabled() == false) {
			send_chord();
		}
	}

	bool tick_step(const bool send_midi = true) {
		if (arpeggiator.enabled() && arpeggiatorEngine.tick()) {
			send_arpeggiator_note(send_midi);
			return true;
		}
		return false;
	}

	void process_step(uint8_t pattern, uint8_t step) {
		uint8_t delay = get_step_value(pattern, step, ChordTrack::DELAY);
		uint8_t velocity = get_step_value(pattern, step, ChordTrack::VELOCITY);
		uint8_t gate_length = get_step_value(pattern, step, ChordTrack::GATE_LENGTH);
		oct_offset_ = 12 * chordTrack_->read(ChordTrack::CHORD_OCT_OFFSET);

		MidiEvent::Event &event = trackState_->event;

		event.port = chordTrack_->port();
		event.tie = gate_length >= 127;
		event.message = MidiEvent::NOTE_ON | chordTrack_->channel();
		event.data[1] = velocity;

		when_ = trackState_->clock.gate_duration(delay);
		length_ = trackState_->clock.gate_duration(gate_length);

		// chord_
		uint8_t type = get_step_value(pattern, step, ChordTrack::CHORD_TYPE);
		uint8_t variation = get_step_value(pattern, step, ChordTrack::CHORD_VARIATION);
		uint8_t inversion = get_step_value(pattern, step, ChordTrack::CHORD_INVERSION);
		uint8_t root_shift = get_step_value(pattern, step, ChordTrack::CHORD_ROOT_SHIFT);

		chord_.build(type, root_shift, variation, inversion);

		// arpeggiator
		uint8_t arp_trigger = get_step_value(pattern, step, ChordTrack::ARP_TRIGGER);
		uint8_t arp_offset = get_step_value(pattern, step, ChordTrack::ARP_OFFSET);
		uint8_t arp_speed = get_step_value(pattern, step, ChordTrack::ARP_SPEED);
		uint8_t arp_mode = get_step_value(pattern, step, ChordTrack::ARP_MODE);
		uint8_t arp_gate = get_step_value(pattern, step, ChordTrack::ARP_GATE);
		uint8_t arp_velocity = get_step_value(pattern, step, ChordTrack::ARP_VELOCITY);

		arpeggiator.set_speed(arp_speed, false);
		arpeggiator.set_trigger_mode(arp_trigger, false);
		arpeggiator.set_note_mode(arp_mode, false);
		arpeggiator.set_gate_mode(arp_gate, false);
		arpeggiator.set_velocity_mode(arp_velocity, false);
		arpeggiator.set_offset(arp_offset, false);
		arpeggiator.set_velocity(velocity, false);
		arpeggiator.set_gate_length(gate_length, false);

		arpeggiatorEngine.clear_notes();
		for (int i = 0; i < chord_.size(); ++i) {
			arpeggiatorEngine.set_note(chord_.note(i));
		}

		arpeggiatorEngine.reset();
	}

private:

	ChordTrack *chordTrack_;
	TrackState *trackState_;

	uint8_t track_index_;
	uint32_t length_;
	uint32_t when_;

	uint8_t oct_offset_;

	Chord chord_;
	Arpeggiator arpeggiator;
	ArpeggiatorEngine arpeggiatorEngine;


	inline int get_step_value(uint8_t pattern, uint8_t step, ChordTrack::StepItem item) {
		if (allow_randomization) {
			if (chordTrack_->pattern.random_is_enabled(pattern, item, step)) {
				int min = chordTrack_->read_random_min(pattern, item);
				int max = chordTrack_->read_random_max(pattern, item);
				return Rng::u16(min, max);
			}
		}
		return chordTrack_->read_step(pattern, step, item);
	}

	inline bool step_triggered(uint8_t pattern, uint8_t step) {
		uint8_t step_trigger = chordTrack_->read_step(pattern, step, ChordTrack::TRIGGER);
		uint8_t probability = get_step_value(pattern, step, ChordTrack::PROBABILITY);
		return step_trigger && (probability >= Rng::u16(1, 6));
	}

	inline void send_chord() {
		if (settings.song.track_is_audible(track_index_)) {
			for (int i = 0; i < chord_.size(); ++i) {
				trackState_->event.data[0] = chord_.note(i) + oct_offset_;
				trackState_->send_note_event(when_, length_);
			}
		}
	}

	inline void send_arpeggiator_note(const bool send_midi) {
		trackState_->event.data[0] = arpeggiatorEngine.note() + oct_offset_;
		trackState_->event.data[1] = arpeggiatorEngine.velocity();
		length_ = arpeggiatorEngine.gate_length();

		if (send_midi && settings.song.track_is_audible(track_index_)) {
			trackState_->send_note_event(when_, length_);
		}
	}

};

#endif
