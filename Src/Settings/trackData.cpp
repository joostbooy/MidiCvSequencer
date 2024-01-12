#include "trackData.h"

TrackData::List TrackData::pattern_pool_;
TrackData::Pattern TrackData::pattern_[TrackData::kMaxPatterns];

void TrackData::init() {
	pattern_pool_.size_ = kMaxPatterns;

	for (int i = 0; i < kMaxPatterns; ++i) {
		pattern_pool_.entry_[i] = kMaxPatterns - 1 - i;
	}
}
