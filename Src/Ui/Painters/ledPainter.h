#ifndef LedPainter_h
#define LedPainter_h

#include "ui.h"
#include "matrix.h"
#include "engine.h"
#include "stepEditor.h"

class LedPainter {

public:

	inline void clear() {
		matrix.clear_leds();
	}

	inline void set_shift(Matrix::LedColor color) {
		matrix.set_led(0, 0, color);
	}

	inline void set_group(Matrix::LedColor color) {
		matrix.set_led(0, 1, color);
	}

	inline void set_solo(Matrix::LedColor color) {
		matrix.set_led(1, 0, color);
	}

	inline void set_mute(Matrix::LedColor color) {
		matrix.set_led(1, 1, color);
	}


	inline void set_play(Matrix::LedColor color) {
		matrix.set_led(3, 0, color);
	}

	inline void set_stop(Matrix::LedColor color) {
		matrix.set_led(4, 1, color);
	}

	inline void set_record(Matrix::LedColor color) {
		matrix.set_led(5, 1, color);
	}


	inline void set_edit(Matrix::LedColor color) {
		matrix.set_led(2, 5, color);
	}

	inline void set_menu(Matrix::LedColor color) {
		matrix.set_led(3, 5, color);
	}

	inline void set_save(Matrix::LedColor color) {
		matrix.set_led(4, 4, color);
	}


	inline void set_x(Matrix::LedColor color) {
		matrix.set_led(2, 1, color);
	}

	inline void set_y(Matrix::LedColor color) {
		matrix.set_led(5, 5, color);
	}


	inline void set_paste(Matrix::LedColor color) {
		matrix.set_led(6, 4, color);
	}

	inline void set_copy(Matrix::LedColor color) {
		matrix.set_led(6, 5, color);
	}

	inline void set_delete(Matrix::LedColor color) {
		matrix.set_led(7, 4, color);
	}

	inline void set_clear(Matrix::LedColor color) {
		matrix.set_led(7, 5, color);
	}


	inline void set_step_button(int step, Matrix::LedColor color) {
		matrix.set_led(step_button_leds[step].x, step_button_leds[step].y, color);
	}

	inline void set_step_encoder(int step, Matrix::LedColor color) {
		matrix.set_led(step_encoder_leds[step].x, step_encoder_leds[step].y, color);
	}

	inline void set_clock_position(int position) {
		for (int i = 0; i < 16; ++i) {
			Matrix::LedColor color = (i == position) ? Matrix::RED : Matrix::BLACK;
			set_step_encoder(i, color);
		}
	}

	inline void footer_buttons(uint8_t num_active) {
		for (int i = 0; i < 16; ++i) {
			Matrix::LedColor color = Matrix::BLACK;
			if ((i >= 6) && (i <= 9) && ((i - 6) < num_active)) {
				color = Matrix::GREEN;
			}
			set_step_button(i, color);
		}
	}

	inline void footer_buttons(Matrix::LedColor c1, Matrix::LedColor c2, Matrix::LedColor c3, Matrix::LedColor c4) {
		Matrix::LedColor colors[4] = {c1, c2, c3, c4};

		for (int i = 0; i < 16; ++i) {
			Matrix::LedColor color = Matrix::BLACK;
			if (i >= 6 && i <= 9) {
				color = colors[i - 6];
			}
			set_step_button(i, color);
		}
	}

	inline void footer_encoders(uint8_t num_active) {
		for (int i = 0; i < 16; ++i) {
			Matrix::LedColor color = Matrix::BLACK;
			if ((i >= 6) && (i <= 9) && ((i - 6) < num_active)) {
				color = Matrix::GREEN;
			}
			set_step_encoder(i, color);
		}
	}

	inline void footer_encoders(Matrix::LedColor c1, Matrix::LedColor c2, Matrix::LedColor c3, Matrix::LedColor c4) {
		Matrix::LedColor colors[4] = {c1, c2, c3, c4};

		for (int i = 0; i < 16; ++i) {
			Matrix::LedColor color = Matrix::BLACK;
			if (i >= 6 && i <= 9) {
				color = colors[i - 6];
			}
			set_step_encoder(i, color);
		}
	}

	inline void paint_chapters(Chapter& chapter) {
		for (int i = 0; i < Chapter::NUM_CHAPTERS; ++i) {
			Matrix::LedColor color = (i == chapter.active()) ? Matrix::RED : Matrix::BLACK;
			matrix.set_led(chapter_leds[i].x, chapter_leds[i].y, color);
		}
	}

	inline void paint_step_mode(StepEditor &stepEditor, Track &track) {
		switch (stepEditor.mode())
		{
		case StepEditor::AUDITION:
			set_shift(Matrix::GREEN);
			set_solo(Matrix::GREEN);
			break;
		case StepEditor::GROUPED_EDIT:
			set_group(Matrix::GREEN);

			for (int i = 0; i < 16; ++i) {
				if (track.step_is_grouped(i)) {
					set_step_button(i, Matrix::ORANGE);
				}
			}
			break;
		case StepEditor::SHIFT:
			set_shift(Matrix::GREEN);

			if (stepEditor.num_selected_steps() > 0) {
				set_step_encoder(stepEditor.original_step(0), Matrix::ORANGE);
				set_step_encoder(stepEditor.selected_step(0), Matrix::RED);
			}
			break;
		case StepEditor::GROUPED_SHIFT:
			set_group(Matrix::GREEN);
			set_shift(Matrix::GREEN);

			if (stepEditor.num_selected_steps() == 0) {
				for (int i = 0; i < 16; ++i) {
					if (track.step_is_grouped(i)) {
						set_step_button(i, Matrix::ORANGE);
					}
				}
			} else {
				for (int i = 0; i < stepEditor.num_selected_steps(); ++i) {
					set_step_button(stepEditor.original_step(i), Matrix::ORANGE);
				}

				// make we sure we paint over the original steps
				for (int i = 0; i < stepEditor.num_selected_steps(); ++i) {
					set_step_encoder(stepEditor.selected_step(i), Matrix::RED);
				}
			}
			break;
		default:
			break;
		}
	}

private:
	struct Map {
		const uint8_t x;
		const uint8_t y;
	};

	Map step_encoder_leds[16] = {
		{ .x = 0, .y = 2 },
		{ .x = 1, .y = 2 },
		{ .x = 2, .y = 2 },
		{ .x = 3, .y = 2 },
		{ .x = 4, .y = 2 },
		{ .x = 5, .y = 2 },
		{ .x = 6, .y = 2 },
		{ .x = 7, .y = 2 },
		{ .x = 0, .y = 6 },
		{ .x = 1, .y = 6 },
		{ .x = 2, .y = 6 },
		{ .x = 3, .y = 6 },
		{ .x = 4, .y = 6 },
		{ .x = 5, .y = 6 },
		{ .x = 6, .y = 6 },
		{ .x = 7, .y = 6 },
	};

	Map step_button_leds[16] = {
		{ .x = 0, .y = 3 },
		{ .x = 1, .y = 3 },
		{ .x = 2, .y = 3 },
		{ .x = 3, .y = 3 },
		{ .x = 4, .y = 3 },
		{ .x = 5, .y = 3 },
		{ .x = 6, .y = 3 },
		{ .x = 7, .y = 3 },
		{ .x = 0, .y = 7 },
		{ .x = 1, .y = 7 },
		{ .x = 2, .y = 7 },
		{ .x = 3, .y = 7 },
		{ .x = 4, .y = 7 },
		{ .x = 5, .y = 7 },
		{ .x = 6, .y = 7 },
		{ .x = 7, .y = 7 },
	};

	Map chapter_leds[6] = {
		{ .x = 6, .y = 0 },	// song
		{ .x = 7, .y = 0 },	// track
		{ .x = 7, .y = 1 },	// pattern
		{ .x = 0, .y = 4 },	// midi
		{ .x = 0, .y = 5 },	// cv
		{ .x = 1, .y = 5 },	// system
	};
};

#endif
