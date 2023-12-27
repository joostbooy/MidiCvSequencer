#ifndef BaseTrack_h
#define BaseTrack_h

#include "stmf4lib.h"
#include "stringBuilder.h"
#include "stringUtils.h"
#include "trackData.h"
#include "pattern.h"
#include "midiPort.h"
#include "clockEngine.h"
#include "positionEngine.h"
#include "repeatEngine.h"
#include "uiText.h"

#define ASSERT_STEP_ITEMS(count) static_assert(count < TrackData::kMaxStepItems, "To many step items!");

class BaseTrack {

public:

	enum Type {
		NOTE_TRACK,
		CHORD_TRACK,
		DRUM_TRACK,
		CURVE_TRACK,

		NUM_TYPES
	};

	static const char* type_text(uint8_t type) {
		switch (type)
		{
		case NOTE_TRACK:	return "NOTE";
		case CHORD_TRACK:	return "CHORD";
		case DRUM_TRACK:	return "DRUM";
		case CURVE_TRACK:	return "CURVE";
		default:			return nullptr;
			break;
		}
	}

	Pattern pattern;

	void init(TrackData *data) {
		data_ = data;

		pattern_list = &data_->pattern_list_;
		pattern_pool = &data_->pattern_pool_;
		pattern_list->size_ = 0;

		pattern.init(pattern_list);
	}

	static constexpr uint8_t max_label_length() {
		return TrackData::kMaxLabelLength;
	}

	static constexpr uint16_t max_patterns() {
		return TrackData::kMaxPatterns;
	}

	static constexpr uint8_t max_steps_per_pattern() {
		return TrackData::kMaxStepsPerPattern;
	}

	constexpr uint32_t* settings() {
		return &data_->serial_[0];
	}

	// type
	uint8_t type() {
		return data_->type_;
	}

	void set_type(int value) {
		data_->type_ = stmlib::clip(0, NUM_TYPES - 1, value);
	}

	const char* type_text() {
		return type_text(type());
	}

	// port
	uint8_t port() {
		return data_->port_;
	}

	void set_port(int port) {
		data_->port_ = stmlib::clip(0, MidiPort::NUM_PORTS - 1, port);
	}

	const char* port_text() {
		return MidiPort::port_text(port());
	}

	// channel
	int channel() {
		return data_->channel_;
	}

	void set_channel(int channel) {
		data_->channel_ = stmlib::clip(0, 15, channel);
	}

	const char* channel_text() {
		return UiText::midi_channel_text(channel());
	}

	// clock mode
	uint8_t clock_mode() {
		return data_->clock_mode_;
	}

	void set_clock_mode(int clock_mode) {
		data_->clock_mode_ = stmlib::clip(0, PositionEngine::NUM_MODES - 1, clock_mode);
	}

	const char* clock_mode_text() {
		return PositionEngine::mode_text(clock_mode());
	}

	// clock speed
	uint8_t clock_speed() {
		return data_->clock_speed_;
	}

	void set_clock_speed(int clock_speed) {
		data_->clock_speed_ = stmlib::clip(0, ClockEngine::NUM_SIGNATURES - 1, clock_speed);
	}

	const char* clock_speed_text() {
		return ClockEngine::speed_text(clock_speed());
	}

	// delay
	uint8_t delay() {
		return data_->delay_;
	}

	void set_delay(int delay) {
		data_->delay_ = stmlib::clip(0, 3, delay);
	}

	const char* delay_text() {
		return ClockEngine::gate_duration_text(delay());
	}

	// swing
	uint8_t swing() {
		return data_->swing_;
	}

	void set_swing(int value) {
		data_->swing_ = stmlib::clip(1, 15, value);
	}

	const char* swing_text() {
		return ClockEngine::swing_text(swing());
	}

	// humanise
	uint8_t humanise() {
		return data_->humanise_;
	}

	void set_humanise(int value) {
		data_->humanise_ = stmlib::clip(0, 7, value);
	}

	const char* humanise_text() {
		return UiText::percentage_text(humanise(), 7);
	}

	// label
	const char* label() {
		return data_->label_;
	};

	void set_label(const char* new_label) {
		StringUtils::copy(data_->label_, const_cast<char*>(new_label), max_label_length());
	}

	// grouped steps
	uint16_t *grouped_steps() {
		return &data_->grouped_steps_;
	}

	bool step_is_grouped(int step_index) {
		return *grouped_steps() & (1 << step_index);
	}

	void toggle_grouped_step(int step_index) {
		uint16_t flags = *grouped_steps();
		*grouped_steps() = flags ^ (1 << step_index);
	}

	/***************
	*	Methods
	***************/

	void clear() {
		set_label("");
		set_port(0);
		set_channel(0);
		set_delay(0);
		set_swing(8);
		set_humanise(0);
		*grouped_steps() = 0xFFFF;
		set_clock_mode(PositionEngine::FORWARD);
		set_clock_speed(ClockEngine::WHOLE_16TH);
		clear_pattern_list();
	}

	// patterns
	inline uint8_t available_patterns() {
		return pattern_pool->size_;
	}

	inline uint8_t num_patterns() {
		return pattern_list->size_;
	}

	int add_pattern() {
		if (available_patterns() > 0) {
			return append_pattern_list();
		}
		return -1;
	}

	bool remove_pattern(uint8_t list_index) {
		if (num_patterns() > 1 && list_index < num_patterns()) {
			remove_from_list(list_index);
			return true;
		}
		return false;
	}

	bool swap_pattern_with_left(uint8_t list_index) {
		if (list_index > 0 && list_index < num_patterns()) {
			swap_with_left(list_index);
			return true;
		}
		return false;
	}

	bool swap_pattern_with_right(uint8_t list_index) {
		int far_right = num_patterns() - 1;

		if (far_right > 0 && list_index < far_right) {
			swap_with_right(list_index);
			return true;
		}
		return false;
	}

	const char* generate_pattern_label() {
		int value = read_pattern_list(num_patterns() - 1);
		int len = 1 + (value / 26);
		char c = 'A' + (value % 26);

		for (int i = 0; i < len; ++i) {
			text[i] = c;
		}
		text[len + 1] = '\0';

		return text;
	}


private:
	TrackData *data_;
	TrackData::List* pattern_list;
	TrackData::List* pattern_pool;
	static char text[TrackData::kMaxLabelLength];

	void clear_pattern_list();
	int append_pattern_list();
	void remove_from_list(int list_index);
	void swap_with_left(int list_index);
	void swap_with_right(int list_index);

	inline uint16_t read_pattern_list(uint8_t list_index) {
		return pattern_list->entry_[list_index];
	}
};


#endif
