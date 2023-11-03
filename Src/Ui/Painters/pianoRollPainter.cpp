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
int PianoRollPainter::last_top_row;
int PianoRollPainter::scroll_bar_frames;
float PianoRollPainter::w_scale;


 int PianoRollPainter::note_x = 0;
 uint32_t PianoRollPainter::notes[128 / 32];
