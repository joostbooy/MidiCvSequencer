#ifndef Settings_h
#define Settings_h

#include "song.h"
#include "clipboard.h"
#include "cvOutput.h"
#include "cvInput.h"
#include "midiInput.h"
#include "midiOutput.h"
#include "calibration.h"
#include "drumKit.h"

class Settings {

public:

	const uint32_t current_version() {
		return 1;
	}

	const char* current_version_text() {
		return "VERSION 1.0";
	}

	Song song;
	DrumKit drumKit;
	Calibration calibration;

	MidiInput &midiInput(int port) {
		return midiInput_[port];
	}

	MidiOutput &midiOutput(int port) {
		return midiOutput_[port];
	}

	CvInput &cvInput(int port) {
		return cvInput_[port];
	}

	CvOutput &cvOutput(int port) {
		return cvOutput_[port];
	}

	void init() {
		selected_track_index_ = 0;
		selected_pattern_ = 0;
		selected_step_item_ = 0;
		follow_pattern_ = false;

		song.init();
		drumKit.init(&song);
		clipboard.init();
		path.clear();

		if (!calibration_loaded_) {
			load_calibration();
		}

		for (int i = 0; i < CvOutput::NUM_PORTS; ++i) {
			cvOutput(i).init();
		}

		for (int i = 0; i < CvInput::NUM_PORTS; ++i) {
			cvInput(i).init();
		}

		for (int i = 0; i < MidiPort::NUM_PORTS; ++i) {
			midiInput(i).init();
			midiOutput(i).init();
		}
	}

	// track
	int selected_track_index() {
		return selected_track_index_;
	}

	void select_track_index(int track_index) {
		selected_track_index_ = stmlib::clip(0, song.max_tracks() - 1, track_index);
		select_pattern(selected_pattern_);
		select_step_item(selected_step_item_);
	}

	// track types
	Track& selected_track() {
		return song.track(selected_track_index_);
	}

	NoteTrack& selected_note_track() {
		return song.track(selected_track_index_).note;
	}

	ChordTrack& selected_chord_track() {
		return song.track(selected_track_index_).chord;
	}

	DrumTrack& selected_drum_track() {
		return song.track(selected_track_index_).drum;
	}

	CurveTrack& selected_curve_track() {
		return song.track(selected_track_index_).curve;
	}

	// pattern
	int selected_pattern() {
		return selected_pattern_;
	}

	void select_pattern(int pattern_index) {
		int num_patterns = song.track(selected_track_index_).num_patterns();
		if (num_patterns == 0) {
			selected_pattern_ = 0;
			return;
		}
		selected_pattern_ = stmlib::clip(0, num_patterns - 1, pattern_index);
	}

	// follow pattern
	bool follow_pattern() {
		return follow_pattern_;
	}

	void set_follow_pattern(bool state) {
		follow_pattern_ = state;
	}

	// step item
	int selected_step_item() {
		return selected_step_item_;
	}

	void select_step_item(int step_item) {
		int num_items = song.track(selected_track_index_).num_step_items();
		if (num_items == 0) {
			selected_step_item_ = 0;
			return;
		}
		selected_step_item_ = stmlib::clip(0, num_items - 1, step_item);
	}

	// save & load
	bool save();
	bool save(const char* new_path);
	bool load(const char* new_path);
	bool save_calibration();
	bool load_calibration();

	const char* read_path() {
		return path.read();
	}

	bool has_valid_path() {
		return path.length() > 0; // && StringUtils::is_equal(file_name.read(), song.name());
	}

private:
	int selected_track_index_;
	int selected_pattern_;
	int selected_step_item_;
	bool follow_pattern_;

	bool calibration_loaded_ = false;	// needs to be init to false !

	StringBuilderBase<63>path;
	//StringBuilderBase<63>file_name;

	CvInput cvInput_[CvInput::NUM_PORTS];
	CvOutput cvOutput_[CvOutput::NUM_PORTS];
	MidiInput midiInput_[MidiPort::NUM_PORTS];
	MidiOutput midiOutput_[MidiPort::NUM_PORTS];
};

extern Settings settings;

#endif
