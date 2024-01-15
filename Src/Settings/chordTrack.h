#ifndef ChordTrack_h
#define ChordTrack_h

#include "baseTrack.h"
#include "serialItem.h"
#include "arpeggiator.h"
#include "chord.h"

class ChordTrack : public BaseTrack {

public:

	enum Item {
		CHORD_OCT_OFFSET,
		NUM_TRACK_ITEMS
	};

	enum StepItem {
		TRIGGER,
		PROBABILITY,
		CHORD_TYPE,
		CHORD_VARIATION,
		CHORD_INVERSION,
		CHORD_ROOT_SHIFT,
		VELOCITY,
		GATE_LENGTH,
		DELAY,
		ARP_SPEED,
		ARP_MODE,
		ARP_GATE,
		ARP_VELOCITY,
		ARP_TRIGGER,
		ARP_OFFSET,
		NUM_STEP_ITEMS
	};

	ASSERT_STEP_ITEMS(NUM_STEP_ITEMS);

	// text
	static const char* step_item_text(StepItem item) {
		switch (item)
		{
		case TRIGGER:			return "TRIGGER";
		case PROBABILITY:		return "PROBABILITY";
		case CHORD_TYPE:		return "CHORD TYPE";
		case CHORD_VARIATION:	return "VARIATION";
		case CHORD_INVERSION:	return "INVERSION";
		case CHORD_ROOT_SHIFT:	return "ROOT SHIFT";
		case VELOCITY:			return "VELOCITY";
		case GATE_LENGTH:		return "GATE LENGTH";
		case DELAY:				return "DELAY";
		case ARP_SPEED:			return "ARP SPEED";
		case ARP_MODE:			return "ARP MODE";
		case ARP_GATE:			return "ARP GATE";
		case ARP_VELOCITY:		return "ARP VELOCITY";
		case ARP_TRIGGER:		return "ARP TRIGGER";
		case ARP_OFFSET:		return "ARP OFFSET";
		default:
			break;
		}
		return nullptr;
	}

	static const char* step_value_text(StepItem item, int data) {
		switch (item)
		{
		case TRIGGER:			return UiText::bool_to_on_off(data);
		case PROBABILITY:		return UiText::percentage_text(data, 7);
		case CHORD_TYPE:		return Chord::name(data);
		case CHORD_VARIATION:	return Chord::variation_text(data);
		case CHORD_INVERSION:	return Chord::inversion_text(data);
		case CHORD_ROOT_SHIFT:	return UiText::unsigned_int_to_text(data);
		case VELOCITY:			return UiText::unsigned_int_to_text(data);
		case GATE_LENGTH:		return ClockEngine::gate_duration_text(data);
		case DELAY:				return ClockEngine::gate_duration_text(data);
		case ARP_SPEED:			return ClockEngine::speed_text(data);
		case ARP_MODE:			return Arpeggiator::clock_mode_text_small(data);
		case ARP_GATE:			return Arpeggiator::clock_mode_text_small(data);
		case ARP_VELOCITY:		return Arpeggiator::clock_mode_text_small(data);
		case ARP_TRIGGER:		return Arpeggiator::trigger_mode_text(data);
		case ARP_OFFSET:		return UiText::unsigned_int_to_text(data);
		default:
			break;
		}
		return nullptr;
	}

	void init(TrackData *data) {
		BaseTrack::init(data);
	}

	void clear() {
		BaseTrack::clear();
		ChordTrack::add_pattern();
		for (int i = 0; i < NUM_TRACK_ITEMS; ++i) {
			Item item = Item(i);
			write(item, track_item_[item].init_value());
		}
	}

	SerialItem &item(StepItem item) {
		return step_item_[item];
	}

	SerialItem &item(Item item) {
		return track_item_[item];
	}

	// edit
	void edit(Item item, int enc_state, bool shifted) {
		int value = read(item);
		int inc_ = track_item_[item].inc(enc_state, shifted);
		write(item, value + inc_);
	}

	void edit_random_min(int pattern, StepItem item, int enc_state, bool shifted) {
		int value = read_random_min(pattern, item);
		int inc_ = step_item_[item].inc(enc_state, shifted);
		write_random_min(pattern, item, value + inc_);
	}

	void edit_random_max(int pattern, StepItem item, int enc_state, bool shifted) {
		int value = read_random_max(pattern, item);
		int inc_ = step_item_[item].inc(enc_state, shifted);
		write_random_max(pattern, item, value + inc_);
	}

	void edit_step(int pattern, int step, StepItem item, int enc_state, bool shifted) {
		int value = read_step(pattern, step, item);
		int inc_ = step_item_[item].inc(enc_state, shifted);
		write_step(pattern, step, item, value + inc_);
	}

	// reading
	int read(Item item) {
		return track_item_[item].read(BaseTrack::settings());
	}

	int read_step(int pattern_index, int step_index, StepItem item) {
		return step_item_[item].read(BaseTrack::pattern.step(pattern_index, step_index));
	}

	int read_random_min(int pattern_index, StepItem item) {
		return step_item_[item].read(BaseTrack::pattern.random_min(pattern_index));
	}

	int read_random_max(int pattern_index, StepItem item) {
		return step_item_[item].read(BaseTrack::pattern.random_max(pattern_index));
	}

	// writing
	void write(Item item, int value) {
		track_item_[item].write(BaseTrack::settings(), value);
	}

	void write_random_min(int pattern_index, StepItem item, int value) {
		step_item_[item].write(BaseTrack::pattern.random_min(pattern_index), value);
	}

	void write_random_max(int pattern_index, StepItem item, int value) {
		step_item_[item].write(BaseTrack::pattern.random_max(pattern_index), value);
	}

	void write_step(int pattern_index, int step_index, StepItem item, int value) {
		step_item_[item].write(BaseTrack::pattern.step(pattern_index, step_index), value);
	}


	// patterns
	bool add_pattern() {
		int pattern_index = BaseTrack::add_pattern();
		if (pattern_index >= 0) {
			clear_pattern(pattern_index);
			BaseTrack::pattern.set_label(pattern_index, BaseTrack::generate_pattern_label());
			return true;
		}
		return false;
	}

	void clear_pattern(int pattern_index) {
		BaseTrack::pattern.clear(pattern_index);
		clear_random(pattern_index);
		for (int i = 0; i < BaseTrack::max_steps_per_pattern(); ++i) {
			clear_step(pattern_index, i);
		}
	}

private:

	static SerialMask step_mask_[NUM_STEP_ITEMS];
	static SerialMask track_mask_[NUM_TRACK_ITEMS];
	static SerialItem step_item_[NUM_STEP_ITEMS];
	static SerialItem track_item_[NUM_TRACK_ITEMS];

	void clear_step(int8_t pattern_index, int step_index) {
		for (int i = 0; i < NUM_STEP_ITEMS; ++i) {
			write_step(pattern_index, step_index, StepItem(i), step_item_[i].init_value());
		}
	}

	void clear_random(uint8_t pattern_index) {
		*BaseTrack::pattern.random_mask(pattern_index, TRIGGER) = 0xFFFF;
		for (int i = 0; i < NUM_STEP_ITEMS; ++i) {
			write_random_min(pattern_index, StepItem(i), step_item_[i].min());
			write_random_max(pattern_index, StepItem(i), step_item_[i].max());
		}
	}

};

#endif
