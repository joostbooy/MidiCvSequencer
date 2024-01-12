#ifndef Pattern_h
#define Pattern_h

#include "trackData.h"
#include "uiText.h"

// base pattern

class Pattern {

public:

	void init(TrackData::List *list) {
		list_ = list;
	}

	constexpr TrackData::Pattern &data(uint8_t index) {
		return TrackData::pattern_[list_->entry_[index]];
	}

	constexpr uint32_t* step(uint8_t index, uint8_t step_index) {
		return &data(index).step_[step_index].serial_[0];
	}

	constexpr uint32_t* random_min(uint8_t index) {
		return &data(index).random_min_.serial_[0];
	}

	constexpr uint32_t* random_max(uint8_t index) {
		return &data(index).random_max_.serial_[0];
	}

	constexpr uint16_t* random_mask(uint8_t index, int step_item) {
		return &data(index).random_mask_[step_item];
	}

	const char* label(uint8_t index) {
		return data(index).label_;
	};

	void set_label(uint8_t index, const char* new_label) {
		StringUtils::copy(data(index).label_, const_cast<char*>(new_label), TrackData::kMaxLabelLength);
	}

	// Methods
	const char* name(uint8_t index) {
		return UiText::str.write("P", index + 1, "-", label(index));
	}

	void clear(int index) {
		for (int i = 0; i < TrackData::kMaxStepItems; ++i) {
			*random_mask(index, i) = 0;
		}
	}

	bool random_is_enabled(int index, int step_item, int step_index) {
		return *random_mask(index, step_item) & (1 << step_index);
	}

	void toggle_random_enable(int index, int step_item, int step_index) {
		uint16_t flags = *random_mask(index, step_item);
		*random_mask(index, step_item) = flags ^ (1 << step_index);
	}

	int shift_step(uint8_t index, uint8_t step_index, int direction) {
		uint32_t swap_data[2];
		uint8_t swap_index = (step_index + direction) % TrackData::kMaxStepsPerPattern;

		swap_data[0] = step(index, swap_index)[0];
		swap_data[1] = step(index, swap_index)[1];

		step(index, swap_index)[0] = step(index, step_index)[0];
		step(index, swap_index)[1] = step(index, step_index)[1];
		step(index, step_index)[0] = swap_data[0];
		step(index, step_index)[1] = swap_data[1];

		for (int i = 0; i < TrackData::kMaxStepItems; ++i) {
			uint16_t mask = *random_mask(index, i);
			*random_mask(index, i) = swap_bits(mask, step_index, swap_index);
		}

		return swap_index;
	}

	bool duplicate(uint8_t src_index, uint8_t dest_index) {
		char label_[TrackData::kMaxLabelLength];

		if (src_index < list_->size_ && dest_index < list_->size_) {
			copy_label(dest_index, label_);
			data(dest_index) = data(src_index);
			set_label(dest_index, label_);
			return true;
		}
		return false;
	}

private:
	TrackData::List *list_;

	uint16_t swap_bits(uint16_t data, uint8_t index_1, uint8_t index_2) {
		uint16_t bit_1 = (data >> index_1) & 1;
		uint16_t bit_2 = (data >> index_2) & 1;
		uint16_t x = (bit_1 ^ bit_2);
		x = (x << index_1) | (x << index_2);
		return data ^ x;
	}

	void copy_label(int index, char *buffer) {
		StringUtils::copy(buffer, data(index).label_, TrackData::kMaxLabelLength);
	}

};

#endif
