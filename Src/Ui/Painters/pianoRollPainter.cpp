#include "pianoRollPainter.h"

Window PianoRollPainter::window = {
	.x = 0,
	.y = 8,
	.width = canvas.width() - 1,
	.height = 48,
	.collumns = 1 + 16,
	.rows = 12,
};

Window::Cell PianoRollPainter::cell;

float PianoRollPainter::wf;

int PianoRollPainter::last_top_row;
int PianoRollPainter::note_scroll_bar_frames;
int PianoRollPainter::last_step_value;
int PianoRollPainter::step_value_frames;

int PianoRollPainter::curr_pattern_;
int PianoRollPainter::pattern_scroll_bar_frames;

uint32_t PianoRollPainter::notes[128 / 32];

bool PianoRollPainter::is_black_key_[12] = { 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0 };
