#include "pianoRollPainter.h"

Window PianoRollPainter::window = {
	.x = 0,
	.y = 8,
	.width = canvas.width() - 1,
	.height = 48,
	.collumns = 1 + 16,
	.rows = 8,
};

Window::Cell PianoRollPainter::cell;

float PianoRollPainter::wf;

int PianoRollPainter::last_top_row;
int PianoRollPainter::scroll_bar_frames;

uint32_t PianoRollPainter::notes[128 / 32];
