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

	void set_last_touched_step(int step) {
		last_touched_step = step;
	}

	void scroll_y(int inc) {
		PianoRollPainter::scroll_y(inc);
	}

	void draw_pattern(int index) {
		PianoRollPainter::reset();

		noteTrackEngine.reset();
		set_speed(track->clock_speed());


		const bool send_midi = false;

		for (int step = 0; step < 16; ++step) {
			if (track->read_step(index, step, NoteTrack::TRIGGER)) {
				noteTrackEngine.process_step(index, step);
				draw_step(step, 0);
			}

			for (int i = 0; i < step_duration; ++i) {
				if (noteTrackEngine.tick_step(send_midi)) {
					draw_step(step, i);
				}
			}
		}


		// draw last touched step
		int item = settings.selected_step_item();
		int value = track->read_step(index, last_touched_step, NoteTrack::StepItem(item));
		const char *text = NoteTrack::step_value_text(NoteTrack::StepItem(item), value);
		PianoRollPainter::draw_step_value(last_touched_step, value, text);

		PianoRollPainter::draw_scrollbar();
	}


private:

	NoteTrack *track;
	TrackState trackState;
	NoteTrackEngine<false>noteTrackEngine;

	int step_duration;
	int last_touched_step;

	void set_speed(int speed) {
		trackState.clock.set_speed(speed);
		step_duration = ClockEngine::step_duration(speed);
		PianoRollPainter::set_step_duration(step_duration);
	}

	void draw_step(int step, int curr_tick) {
		uint32_t x = curr_tick + noteTrackEngine.when();
		uint32_t w = noteTrackEngine.length();
		bool is_random = step_is_random(step);
		PianoRollPainter::draw_note(step, trackState.event, x, w, is_random);
	}

	bool step_is_random(int step) {
		int item = settings.selected_step_item();
		int pattern_ = settings.selected_pattern();
		return track->pattern.random_is_enabled(pattern_, item, step);
	}

};

#endif
