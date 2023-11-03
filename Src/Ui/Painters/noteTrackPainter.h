#ifndef NoteTrackPainter_h
#define NoteTrackPainter_h

#include "settings.h"
#include "noteTrackEngine.h"
#include "trackState.h"
#include "canvas.h"
#include "windowPainter.h"

#include "pianoRollPainter.h"

class NoteTrackPainter {

public:

	void set_track_index(int index) {
		noteTrackEngine.init(index, &trackState);
		track = &settings.song.track(index).note;
	}

	void set_last_touched_note(int note) {
		PianoRollPainter::set_last_touched_note(note);
	}

	void draw_pattern(int index) {
		PianoRollPainter::draw_background();

		noteTrackEngine.reset();
		set_speed(track->clock_speed());

		const bool send_midi = false;
		const bool allow_random = false;

		for (int step = 0; step < 16; ++step) {
			if (track->read_step(index, step, NoteTrack::TRIGGER)) {
				noteTrackEngine.process_step(index, step, allow_random);
				draw_step(step, 0);
			}

			for (int i = 0; i < step_duration; ++i) {
				if (noteTrackEngine.tick_step(send_midi)) {
					draw_step(step, i);
				}
			}
		}

		PianoRollPainter::draw_scrollbar();
	}

private:

	NoteTrack *track;
	TrackState trackState;
	NoteTrackEngine noteTrackEngine;

	int step_duration;

	void set_speed(int speed) {
		trackState.clock.set_speed(speed);
		step_duration = ClockEngine::step_duration(speed);
		PianoRollPainter::set_step_duration(step_duration);
	}

	void draw_step(int step, int curr_tick) {
		MidiEvent::Event e = trackState.event;
		int when = noteTrackEngine.when();
		int length = noteTrackEngine.length();
		PianoRollPainter::draw_note(step, e.data[0], e.data[1], when + curr_tick, length);
	}
};

#endif
