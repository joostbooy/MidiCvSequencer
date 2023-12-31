#ifndef Song_h
#define Song_h

#include "stmf4lib.h"
#include "track.h"
#include "scale.h"
#include "chord.h"
#include "midiClock.h"
#include "uiList.h"

class Song {

public:

	Scale scale;
	MidiClock midiClock;

	Track &track(int index) {
		return track_[index];
	}

	void init() {
		scale.init();
		midiClock.init();
		Chord::init(&scale);
		TrackData::init();

		set_name("NEW_SONG");

		for (int i = 0; i < Track::NUM_TYPES; ++i) {
			clear_list(&track_type_list_[i]);
		}

		for (int i = 0; i < kMaxTracks; ++i) {
			track(i).init();
			track(i).set_index(i);
			create_track(i, Track::NOTE_TRACK);
		}
	}

	// name
	const char* name() {
		return name_;
	}

	constexpr uint8_t max_name_length() {
		return kMaxNameLength;
	}

	void set_name(const char* new_name) {
		StringUtils::copy(name_, const_cast<char*>(new_name), kMaxNameLength);
	}

	// patterns
	int available_patterns() {
		return TrackData::pattern_pool_.size_;
	}

	int max_patterns() {
		return TrackData::kMaxPatterns;
	}

	// tracks
	constexpr int max_tracks() {
		return kMaxTracks;
	}

	bool create_track(uint8_t index, uint8_t type) {
		Track &t = track(index);

		if (index < kMaxTracks) {
			// remove old type form list
			remove_from_list(&track_type_list_[t.type()], index);

			t.create(type);
			set_track_mute(index, false);
			set_track_solo(index, false);

			add_to_list(&track_type_list_[type], index);
			return true;
		}
		return false;
	}

	uint8_t num_tracks(uint8_t track_type) {
		return track_type_list_[track_type].size_;
	}

	uint8_t read_track_list(uint8_t list_index, uint8_t track_type) {
		return track_type_list_[track_type].entry_[list_index];
	}

	// mute & solo
	bool track_is_muted(uint8_t track_index) {
		return mute_flags & (1 << track_index);
	}

	bool track_is_solod(uint8_t track_index) {
		return solo_flags & (1 << track_index);
	}

	bool track_is_audible(uint8_t track_index) {
		if (solo_flags != 0) {
			return track_is_solod(track_index);
		}
		return !track_is_muted(track_index);
	}

	void set_track_mute(uint8_t track_index, bool state) {
		uint32_t flags = mute_flags;
		uint32_t mask = 1 << track_index;
		mute_flags = state ? flags | mask : flags & ~mask;
	}

	void set_track_solo(uint8_t track_index, bool state) {
		uint32_t flags = solo_flags;
		uint32_t mask = 1 << track_index;
		solo_flags = state ? flags | mask : flags & ~mask;
	}

	const char *mute_text(uint8_t track_index) {
		return track_is_muted(track_index) ? "ON" : "OFF";
	}

	const char *solo_text(uint8_t track_index) {
		return track_is_solod(track_index) ? "ON" : "OFF";
	}

	void save(FileWriter &fileWriter);
	void load(FileReader &fileReader);

private:
	static const uint8_t kMaxTracks = 16;
	static const uint8_t kMaxNameLength = 9;

	struct List {
		uint8_t size_;
		uint8_t entry_[kMaxTracks];
	};

	List track_type_list_[Track::NUM_TYPES];

	uint32_t mute_flags;
	uint32_t solo_flags;
	char name_[kMaxNameLength];
	Track track_[kMaxTracks];

	void clear_list(List* list);
	void add_to_list(List* list, uint8_t track_index);
	void remove_from_list(List* list, uint8_t track_index);
};

#endif
