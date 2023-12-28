#ifndef DrumKit_h
#define DrumKit_h

#include "song.h"

class DrumKit {

public:

	void init(Song *song) {
		song_ = song;
	}

	void update_clock_speed(int value) {
		for (int i = 0; i < num_tracks(); ++i) {
			track(i).set_clock_speed(value);
		}
	}

	bool create_track(int index) {
		if (song_->available_patterns() >= num_patterns() && song_->available_patterns() > 0) {
			song_->create_track(index, Track::DRUM_TRACK);

			int clock_speed = read_clock_speed();
			int list_index = track_to_list_index(index);
			track(list_index).set_clock_speed(clock_speed);

			int pattern_count = num_patterns() - 1;
			for (int i = 0; i < pattern_count; ++i) {
				track(list_index).add_pattern();
			}
			return true;
		}

		return false;
	}

	void clear_track(int index) {
		int list_index = track_to_list_index(index);
		int pattern_count = num_patterns() - 1;
		int clock_speed = read_clock_speed();

		if (list_index >= 0) {
			track(list_index).clear();
			track(list_index).set_clock_speed(clock_speed);
			for (int i = 0; i < pattern_count; ++i) {
				track(list_index).add_pattern();
			}
		}
	}

	bool add_pattern() {
		if (song_->available_patterns() >= num_tracks() && song_->available_patterns() > 0) {
			int index = num_patterns() - 1;
			for (int i = 0; i < num_tracks(); ++i) {
				track(i).add_pattern();
				track(i).pattern.duplicate(index, index + 1);
			}
			return true;
		}
		return false;
	}

	bool remove_pattern(int pattern_index) {
		if (num_patterns() > 1) {
			for (int i = 0; i < num_tracks(); ++i) {
				track(i).remove_pattern(pattern_index);
			}
			return true;
		}
		return false;
	}

private:

	Song *song_;

	inline int num_tracks() {
		return song_->num_tracks(Track::DRUM_TRACK);
	}

	inline int read_list(int list_index) {
		return song_->read_track_list(list_index, Track::DRUM_TRACK);
	}

	inline DrumTrack &track(int list_index) {
		int track_index = read_list(list_index);
		return song_->track(track_index).drum;
	}

	inline int num_patterns() {
		if (num_tracks() > 0) {
			return track(0).num_patterns();
		} else {
			return 0;
		}
	}

	inline int track_to_list_index(int track_index) {
		int total = num_tracks();
		for (int i = 0; i < total; ++i) {
			if (read_list(i) == track_index) {
				return i;
			}
		}
		return -1;
	}

	inline int read_clock_speed() {
		if (num_tracks() > 0) {
			return track(0).clock_speed();
		} else {
			return ClockEngine::WHOLE_16TH;
		}
	}
	
};

#endif
