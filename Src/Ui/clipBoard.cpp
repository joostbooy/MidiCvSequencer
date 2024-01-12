#include "clipboard.h"

ClipBoard clipboard;


bool ClipBoard::copy_pattern(Track &track, int pattern_index) {
	if (pattern_index < track.num_patterns()) {
		pattern_type_ = track.type();
		pattern_ = track.pattern.data(pattern_index);
		return true;
	}
	return false;
}

bool ClipBoard::paste_pattern(Track &track, int pattern_index) {
	if (pattern_pasteable(track)) {
		str.write(track.pattern.label(pattern_index));
		track.pattern.data(pattern_index) = pattern_;
		track.pattern.set_label(pattern_index, str.read());
		return true;
	}
	return false;
}
