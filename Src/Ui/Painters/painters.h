#ifndef Painters_h
#define Painters_h

#include "ledPainter.h"
#include "notePainter.h"
#include "curvePainter.h"
#include "messagePainter.h"
#include "windowPainter.h"
#include "patternPainter.h"
#include "textBufferPainter.h"

class Painters {

public:
	void init();

	LedPainter leds;
	NotePainter note;
	CurvePainter curve;
	WindowPainter window;
	MessagePainter message;
	PatternPainter pattern;
	TextBufferPainter text_buffer;
};

extern Painters painters;

#endif
