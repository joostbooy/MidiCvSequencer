#ifndef StepEditor_h
#define StepEditor_h

#include "settings.h"
#include "engine.h"

class StepEditor {

public:

	enum Mode {
		EDIT,
		SHIFT,
		GROUPED_SHIFT,
		GROUPED_EDIT,
		AUDITION,

		NUM_MODES
	};

	const char *mode_text() {
		switch (mode_)
		{
		case EDIT:			return "EDIT";
		case SHIFT:			return "SHIFT";
		case GROUPED_SHIFT:	return "GROUPED SHIFT";
		case GROUPED_EDIT:	return "GROUPED EDIT";
		case AUDITION:		return "AUDITION";
		default:
			break;
		}
		return nullptr;
	}

	Mode mode() {
		return mode_;
	}

	int num_selected_steps() {
		return num_selected_steps_;
	}

	int selected_step(int index) {
		return selected_step_[index];
	}

	int original_step(int index) {
		return original_step_[index];
	}

	void set_mode(Mode mode) {
		reset();
		mode_ = mode;
	}

	void reset() {
		num_selected_steps_ = 0;
		oldest_pressed_encoder_ = -1;
		engine.add_request_wait(Engine::STOP_AUDITION);
	}

	void on_encoder_push(int step, bool state) {
		if (state == 1 && oldest_pressed_encoder_ == -1) {
			oldest_pressed_encoder_ = step;
		} else if (state == 0 && oldest_pressed_encoder_ == step) {
			oldest_pressed_encoder_ = -1;
		}
	}

	void on_button(int step, bool state) {
		int track_index = settings.selected_track_index();
		int pattern = settings.selected_pattern();

		switch (mode_)
		{
		case EDIT:
			if (state) {
				toggle(step);
			}
			break;
		case SHIFT:
			if (state) {
				toggle(step);
			}
			break;
		case GROUPED_EDIT:
			if (state) {
				settings.song.track(track_index).toggle_grouped_step(step);
			}
			break;
		case GROUPED_SHIFT:
			if (state > 0 && num_selected_steps() == 0) {
				settings.song.track(track_index).toggle_grouped_step(step);
			}
			break;
		case AUDITION:
			if (state) {
				engine.request_audition_wait(track_index, pattern, step);
			} else {
				engine.add_request_wait(Engine::STOP_AUDITION);
			}
			break;
		default:
			break;
		}
	}

	void on_encoder(int step, int inc, bool pressed) {
		Track &track = settings.selected_track();
		int track_index = settings.selected_track_index();
		int pattern = settings.selected_pattern();

		switch (mode())
		{
		case EDIT:
			edit(step, inc, pressed);
			if (oldest_pressed_encoder_ >= 0) {
				interpolate(oldest_pressed_encoder_, step);
			}
			break;
		case AUDITION:
			edit(step, inc, pressed);
			engine.request_audition_wait(track_index, pattern, step);
			break;
		case SHIFT:
			if (num_selected_steps() == 0) {
				select_step(step);
			}
			shift(track, pattern, inc);
			break;
		case GROUPED_EDIT:
			for (int i = 0; i < TrackData::kMaxStepsPerPattern; ++i) {
				if (track.step_is_grouped(i)) {
					edit(i, inc, pressed);
				}
			}
			break;
		case GROUPED_SHIFT:
			if (num_selected_steps() == 0) {
				for (int i = 0; i < TrackData::kMaxStepsPerPattern; ++i) {
					if (track.step_is_grouped(i)) {
						select_step(i);
					}
				}
			}
			shift(track, pattern, inc);
			break;
		default:
			break;
		}
	}

	int read(int step);
	int read(int step, int item);
	void toggle(int step);
	void write(int step, int value);
	void edit(int step, int inc, bool pressed);

private:
	Mode mode_ = EDIT;
	int oldest_pressed_encoder_ = -1;
	int num_selected_steps_ = 0;
	int selected_step_[TrackData::kMaxStepsPerPattern];
	int original_step_[TrackData::kMaxStepsPerPattern];

	void select_step(int step) {
		int index = num_selected_steps_++;
		selected_step_[index] = step;
		original_step_[index] = step;
	}

	void shift(Track &track, int pattern, int inc) {
		int next_left = selected_step_[0] - 1;
		int next_right = selected_step_[num_selected_steps_ - 1] + 1;

		if (inc > 0 && next_right <= 15) {
			shift_right(track, pattern);
		} else if (inc < 0 && next_left >= 0) {
			shift_left(track, pattern);
		}
	}

	void shift_right(Track &track, int pattern) {
		for (int i = num_selected_steps_; i > 0; --i) {
			selected_step_[i - 1] = track.pattern.shift_step(pattern, selected_step_[i - 1], 1);
		}
	}

	void shift_left(Track &track, int pattern) {
		for (int i = 0; i < num_selected_steps_; ++i) {
			selected_step_[i] = track.pattern.shift_step(pattern, selected_step_[i], -1);
		}
	}

	void interpolate(int first_step, int last_step) {
		if (first_step > last_step) {
			stmlib::swap(first_step, last_step);
		}

		float first_value = read(first_step);
		float last_value = read(last_step);
		float distance = last_step - first_step;
		float inc = (last_value - first_value) / distance;

		for (int i = first_step; i < last_step; ++i) {
			write(i, first_value);
			first_value += inc;
		}
	}
};

#endif
