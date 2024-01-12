#ifndef TrackEngine_h
#define TrackEngine_h

#include "settings.h"
#include "noteTrackEngine.h"
#include "chordTrackEngine.h"
#include "drumTrackEngine.h"
#include "curveTrackEngine.h"

#include "debug.h"

class TrackEngine {

public:

	void init(MidiOutputEngine *outputEngine) {
		for (int i = 0; i < kMaxTracks; ++i) {
			track_state[i].init(i, outputEngine);

			note_track[i].init(i, &track_state[i]);
			chord_track[i].init(i, &track_state[i]);
			drum_track[i].init(i, &track_state[i]);
			curve_track[i].init(i, &track_state[i]);
		}

		reset();
	}


	TrackState &state(uint8_t index) {
		return track_state[index];
	}

	void reset() {
		ticks = 0;
		processed_ticks = 0;
		suspended = false;
		audition.is_playing = false;

		for (int i = 0; i < kMaxTracks; ++i) {
			reset(i);
		}
	}

	void suspend() {
		suspended = true;
	}

	void resume() {
		suspended = false;
	}

	void tick() {
		++ticks;
		if (suspended) {
			return;
		}

		while (processed_ticks != ticks) {
			++processed_ticks;
			for (int i = 0; i < kMaxTracks; ++i) {
				tick(i);
			}
		}
	}

	void start_audition(uint8_t track, uint8_t pattern, uint8_t step) {
		ticks = 0;
		processed_ticks = 0;

		stop_audition();
		process_step(track, pattern, step);
		send_step(track);

		audition.track = track;
		audition.is_playing = true;
	}

	void tick_audition() {
		if (audition.is_playing) {
			++ticks;
			if (suspended) {
				return;
			}

			while (processed_ticks != ticks) {
				++processed_ticks;
				tick_step(audition.track);
			}
		}
	}

	void stop_audition() {
		if (audition.is_playing) {
			reset();
		}
	}

	float step_phase(uint8_t index) {
		uint32_t length = state(index).clock.step_duration();
		uint32_t tick = processed_ticks % length;
		return (1.f / length) * tick;
	}

private:
	static const int kMaxTracks = settings.song.max_tracks();

	volatile bool suspended;
	volatile uint32_t ticks;
	volatile uint32_t processed_ticks;

	struct Audition {
		uint8_t track;
		bool is_playing;
	};

	Audition audition;
	TrackState track_state[kMaxTracks];
	NoteTrackEngine<true>note_track[kMaxTracks];
	ChordTrackEngine<true>chord_track[kMaxTracks];
	DrumTrackEngine<true>drum_track[kMaxTracks];
	CurveTrackEngine<true>curve_track[kMaxTracks];

	void tick(uint8_t);
	void reset(uint8_t);
	void tick_step(uint8_t);
	void send_step(uint8_t);
	void process_step(uint8_t, uint8_t, uint8_t);
};

#endif
