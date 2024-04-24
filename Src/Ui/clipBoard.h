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
		has_cv_in_data_ = false;
		has_cv_out_data_ = false;
		has_midi_in_data_ = false;
		has_midi_out_data_ = false;
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
	bool has_midi_in_data() {
		return has_midi_in_data_;
	}

	void copy(MidiInput &src) {
		midiInput_.paste(&src);
		has_midi_in_data_ = true;
	}

	bool paste(MidiInput &dest) {
		if (has_midi_in_data_) {
			dest.paste(&midiInput_);
			return true;
		}
		return false;
	}

	// Midi Out
	bool has_midi_out_data() {
		return has_midi_out_data_;
	}

	void copy(MidiOutput &src) {
		midiOutput_.paste(&src);
		has_midi_out_data_ = true;
	}

	bool paste(MidiOutput &dest) {
		if (has_midi_out_data_) {
			dest.paste(&midiOutput_);
			return true;
		}
		return false;
	}

	// Cv in
	bool has_cv_in_data() {
		return has_cv_in_data_;
	}

	void copy(CvInput &src) {
		cvInput_.paste(&src);
		has_cv_in_data_ = true;
	}

	bool paste(CvInput &dest) {
		if (has_cv_in_data_) {
			dest.paste(&cvInput_);
			return true;
		}
		return false;
	}

	// Cv Out
	bool has_cv_out_data() {
		return has_cv_out_data_;
	}

	void copy(CvOutput &src) {
		cvOutput_.paste(&src);
		has_cv_out_data_ = true;
	}

	bool paste(CvOutput &dest) {
		if (has_cv_out_data_) {
			dest.paste(&cvOutput_);
			return true;
		}
		return false;
	}

private:
	CvInput cvInput_;
	bool has_cv_in_data_;

	CvOutput cvOutput_;
	bool has_cv_out_data_;

	MidiInput midiInput_;
	bool has_midi_in_data_;

	MidiOutput midiOutput_;
	bool has_midi_out_data_;

	int pattern_type_;
	TrackData::Pattern pattern_;
	StringBuilderBase<TrackData::kMaxLabelLength>str;
};

extern ClipBoard clipboard;

#endif
