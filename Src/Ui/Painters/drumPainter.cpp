#include "drumPainter.h"

Window DrumPainter::window = {
	.x = 0,
	.y = 8,
	.width = canvas.width() - 1,
	.height = 48,
	.collumns = 18,
	.rows = 8,
};

Window::Cell DrumPainter::cell;

float DrumPainter::wf;

int DrumPainter::selected_track;
int DrumPainter::last_top_row;
int DrumPainter::track_scroll_bar_frames;
int DrumPainter::pattern_scroll_bar_frames;
int DrumPainter::curr_pattern_;
int DrumPainter::last_step_value;
int DrumPainter::step_value_frames;
