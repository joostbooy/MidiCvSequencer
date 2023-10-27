#ifndef CurvePainter_h
#define CurvePainter_h

#include "canvas.h"
#include "curveTrack.h"

class CurvePainter {

public:

	void draw(CurveTrack &track, int pattern) {
		const int step_w = box_w / 16;
		const float inc = 1.f / step_w;

		//reset
		if (pattern == 0) {
			curr_value = track.read(CurveTrack::INIT_CC_VALUE);
		} else {
			curr_value = (1.f / 127.f) * track.read_step(pattern - 1, 15, CurveTrack::CC_VALUE);
		}

		for (int step = 0; step < 16; ++step) {

			last_value = curr_value;
			if (track.read_step(pattern, step, CurveTrack::TRIGGER)) {
				curr_value = (1.f / 127.f) * track.read_step(pattern, step, CurveTrack::CC_VALUE);
			}

			float shape = (1.f / 8.f) * track.read_step(pattern, step, CurveTrack::SHAPE);

			for (int i = 0; i < step_w; ++i) {
				int x = i + (step * step_w);
				float phase = Curve::bend(i * inc, shape);
				float sample = Dsp::cross_fade(last_value, curr_value, phase);
				draw_sample(x, sample);
			}
		}
	}

private:

	int last_y;
	float last_value;
	float curr_value;

	static const int box_x = 0;
	static const int box_y = 12;
	static const int box_h = 40;
	static const int box_w = 256;

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
