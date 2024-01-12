#include "baseTrack.h"

char BaseTrack::text[TrackData::kMaxLabelLength];

void BaseTrack::clear_pattern_list() {
	int pool_index;

	for (int i = 0; i < pattern_list->size_; ++i) {
		pool_index = pattern_pool->size_;
		pattern_pool->entry_[pool_index] = pattern_list->entry_[i];
		++pattern_pool->size_;
	}

	pattern_list->size_ = 0;
}

int BaseTrack::append_pattern_list() {
	int list_index = pattern_list->size_;
	int pool_index = pattern_pool->size_;
	int pattern_index = pattern_pool->entry_[pool_index - 1];

	pattern_list->entry_[list_index] = pattern_index;

	++pattern_list->size_;
	--pattern_pool->size_;

	return list_index;
}

void BaseTrack::remove_from_list(int list_index) {
	int pattern_index = pattern_list->entry_[list_index];
	int pool_index = pattern_pool->size_;

	pattern_pool->entry_[pool_index] = pattern_index;

	--pattern_list->size_;
	++pattern_pool->size_;

	for (int i = list_index; i < pattern_list->size_; ++i) {
		pattern_list->entry_[i] = pattern_list->entry_[i + 1];
	}
}

void BaseTrack::swap_with_left(int list_index) {
	int left = pattern_list->entry_[list_index - 1];
	pattern_list->entry_[list_index - 1] = pattern_list->entry_[list_index];
	pattern_list->entry_[list_index] = left;
}

void BaseTrack::swap_with_right(int list_index) {
	int right = pattern_list->entry_[list_index + 1];
	pattern_list->entry_[list_index + 1] = pattern_list->entry_[list_index];
	pattern_list->entry_[list_index] = right;
}
