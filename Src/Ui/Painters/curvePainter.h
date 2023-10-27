#ifndef CurvePainter_h
#define CurvePainter_h

#include "canvas.h"
#include "curveTrack.h"

class CurvePainter {

public:

	void reset(int value) {
		curr_value = (1.f / 127.f) * value;;
	}

	void draw_step(int step, bool trigger, int value, int shape, const char *text) {
		float value_ = (1.f / 127.f) * value;
		float shape_ = (1.f / 8.f) * shape;

		last_value = curr_value;
		if (trigger) {
			curr_value = value_;
		}

		for (int i = 0; i < step_w; ++i) {
			int x = i + (step * step_w);
			float phase = Curve::bend(i * (1.f / step_w), shape_);
			float sample = Dsp::cross_fade(last_value, curr_value, phase);
			draw_sample(x, sample);
		}

		canvas.draw_text((step * step_w), box_y, step_w, 64, text, Canvas::CENTER, Canvas::BOTTOM);
	}

private:

	int last_y;
	float last_value;
	float curr_value;

	static const int box_x = 0;
	static const int box_y = 12;
	static const int box_h = 40;
	static const int box_w = 256;
	static const int step_w = box_w / 16;

	void draw_sample(int x, float sample) {
		int y, h;
		int curr_y = box_h - (box_h * sample);

		if (x == 0) {
			last_y = curr_y;
		}

		if (curr_y > last_y) {
			y = last_y;
			h = curr_y - last_y + 1;
		} else {
			y = curr_y;
			h = last_y - curr_y + 1;
		}

		canvas.vertical_line(x + box_x, y + box_y, h, Canvas::BLACK);

		last_y = curr_y;
	}
};

#endif
