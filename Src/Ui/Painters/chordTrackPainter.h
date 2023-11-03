#ifndef ChordTrackPainter_h
#define ChordTrackPainter_h

#include "settings.h"
#include "chordTrackEngine.h"
#include "trackState.h"
#include "canvas.h"
#include "windowPainter.h"

#include "pianoRollPainter.h"

class ChordTrackPainter {

public:

	void set_track_index(int index) {
		chordTrackEngine.init(index, &trackState);
		track = &settings.song.track(index).chord;
	}

	void set_last_touched_note(int note) {
		PianoRollPainter::set_last_touched_note(note);
	}

	void draw_pattern(int index) {
		PianoRollPainter::draw_background();

		chordTrackEngine.reset();
		set_speed(track->clock_speed());

		const bool send_midi = false;

		for (int step = 0; step < 16; ++step) {
			if (track->read_step(index, step, ChordTrack::TRIGGER)) {
				chordTrackEngine.process_step(index, step);
				draw_step(step, 0);
			}

			for (int i = 0; i < step_duration; ++i) {
				if (chordTrackEngine.tick_step(send_midi)) {
					draw_step(step, i);
				}
			}
		}

		PianoRollPainter::draw_scrollbar();
	}

private:

	ChordTrack *track;
	TrackState trackState;
	ChordTrackEngine chordTrackEngine;

	int step_duration;

	void set_speed(int speed) {
		trackState.clock.set_speed(speed);
		step_duration = ClockEngine::step_duration(speed);
		PianoRollPainter::set_step_duration(step_duration);
	}

	void draw_step(int step, int curr_tick) {
		MidiEvent::Event e = trackState.event;
		int when = chordTrackEngine.when();
		int length = chordTrackEngine.length();
		PianoRollPainter::draw_note(step, e.data[0], e.data[1], when + curr_tick, length);
	}
};

#endif
