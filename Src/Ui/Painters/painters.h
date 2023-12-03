#ifndef Painters_h
#define Painters_h

#include "messagePainter.h"
#include "textBufferPainter.h"

class Painters {

public:
	void init();
	MessagePainter message;
	TextBufferPainter text_buffer;
};

extern Painters painters;

#endif
