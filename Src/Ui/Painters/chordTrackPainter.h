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

	void scroll_y(int inc) {
		PianoRollPainter::scroll_y(inc);
	}

	void draw_pattern(int index) {
		PianoRollPainter::draw_background();

		chordTrackEngine.reset();
		set_speed(track->clock_speed());

		const bool send_midi = false;

		for (int step = 0; step < 16; ++step) {
			if (track->read_step(index, step, ChordTrack::TRIGGER)) {
				chordTrackEngine.process_step(index, step);
				if (chordTrackEngine.arpeggiator_enabled() == false) {
					draw_chord(step);
				}
			}

			if (chordTrackEngine.arpeggiator_enabled()) {
				for (int i = 0; i < step_duration; ++i) {
					if (chordTrackEngine.tick_step(send_midi)) {
						draw_step(step, i);
					}
				}
			}
		}

		PianoRollPainter::draw_scrollbar();
	}

private:

	ChordTrack *track;
	TrackState trackState;
	ChordTrackEngine<false>chordTrackEngine;

	int step_duration;

	void set_speed(int speed) {
		trackState.clock.set_speed(speed);
		step_duration = ClockEngine::step_duration(speed);
		PianoRollPainter::set_step_duration(step_duration);
	}

	void draw_step(int step, int curr_tick) {
		uint32_t x = curr_tick + chordTrackEngine.when();
		uint32_t w = chordTrackEngine.length();
		PianoRollPainter::draw_note(step, trackState.event, x, w);
	}

	void draw_chord(int step) {
		for (int i = 0; i < chordTrackEngine.chord().size(); ++i) {
			trackState.event.data[0] = chordTrackEngine.chord().note(i) + chordTrackEngine.oct_offset();
			draw_step(step, 0);
		}
	}
};

#endif
