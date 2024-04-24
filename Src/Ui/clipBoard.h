#ifndef ClipBoard_h
#define ClipBoard_h

#include "track.h"
#include "midiInput.h"
#include "midiOutput.h"
#include "cvInput.h"
#include "cvOutput.h"

class ClipBoard {

public:

	void init() {
		pattern_type_ = -1;
		cv_in_pasteable_ = false;
		cv_out_pasteable_ = false;
		midi_in_pasteable_ = false;
		midi_out_pasteable_ = false;
	}

	bool pattern_pasteable(Track &track) {
		return track.type() == pattern_type_ && has_pattern() == true;
	}

	bool has_pattern() {
		return pattern_type_ >= 0;
	}

	bool copy_pattern(Track &track, int pattern_index);
	bool paste_pattern(Track &track, int pattern_index);

	// Midi In
	bool midi_in_pasteable() {
		return midi_in_pasteable_;
	}

	void copy(MidiInput &src) {
		midiInput_.paste(&src);
		midi_in_pasteable_ = true;
	}

	bool paste(MidiInput &dest) {
		if (midi_in_pasteable_) {
			dest.paste(&midiInput_);
			return true;
		}
		return false;
	}

	// Midi Out
	bool midi_out_pasteable() {
		return midi_out_pasteable_;
	}

	void copy(MidiOutput &src) {
		midiOutput_.paste(&src);
		midi_out_pasteable_ = true;
	}

	bool paste(MidiOutput &dest) {
		if (midi_out_pasteable_) {
			dest.paste(&midiOutput_);
			return true;
		}
		return false;
	}

	// Cv in
	bool cv_in_pasteable() {
		return cv_in_pasteable_;
	}

	void copy(CvInput &src) {
		cvInput_.paste(&src);
		cv_in_pasteable_ = true;
	}

	bool paste(CvInput &dest) {
		if (cv_in_pasteable_) {
			dest.paste(&cvInput_);
			return true;
		}
		return false;
	}

	// Cv Out
	bool cv_out_pasteable() {
		return cv_out_pasteable_;
	}

	void copy(CvOutput &src) {
		cvOutput_.paste(&src);
		cv_out_pasteable_ = true;
	}

	bool paste(CvOutput &dest) {
		if (cv_out_pasteable_) {
			dest.paste(&cvOutput_);
			return true;
		}
		return false;
	}

private:
	CvInput cvInput_;
	bool cv_in_pasteable_;

	CvOutput cvOutput_;
	bool cv_out_pasteable_;

	MidiInput midiInput_;
	bool midi_in_pasteable_;

	MidiOutput midiOutput_;
	bool midi_out_pasteable_;

	int pattern_type_;
	TrackData::Pattern pattern_;
	StringBuilderBase<TrackData::kMaxLabelLength>str;
};

extern ClipBoard clipboard;

#endif
