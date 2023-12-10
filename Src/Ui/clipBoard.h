#ifndef ClipBoard_h
#define ClipBoard_h

#include "track.h"

class ClipBoard {

public:

	void init() {
		pattern_type_ = -1;
	}

	bool pattern_pasteable(Track &track) {
		return track.type() == pattern_type_ && has_pattern() == true;
	}

	bool has_pattern() {
		return pattern_type_ >= 0;
	}

	bool copy_pattern(Track &track, int pattern_index);
	bool paste_pattern(Track &track, int pattern_index);

private:
	int pattern_type_;
	TrackData::Pattern pattern_;
	StringBuilderBase<TrackData::kMaxLabelLength>str;
};

extern ClipBoard clipboard;

#endif
