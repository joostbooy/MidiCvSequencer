#ifndef DrumTrackPainter_h
#define DrumTrackPainter_h

#include "settings.h"

class DrumTrackPainter {

public:

	static void draw() {
		// draw background

		for (int i = 0; i < song.num_tracks(Track::DRUM_TRACK); ++i) {
			int track_index = song.read_track_list(i, Track::DRUM_TRACK);
			int pattern_index = engine.trackEngine().state(track_index).pattern_index();
			auto &track = song.track(track_index).drum;

			// draw triggers
			for (int step = 0; step < 16; ++step) {
				bool trigger = track.read_step(pattern_index, step, DrumTrack::StepItem(TRIGGER));
				int note = track.read_step(pattern_index, step, DrumTrack::StepItem(DRUM_NOTE));

			}

			if (track_index == settings.selected_track_index()) {
				// highlight
			}
		}
	}

private:
	Song &song = settings.song;
};

#endif
