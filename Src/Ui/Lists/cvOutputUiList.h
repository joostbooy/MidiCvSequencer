#ifndef CvOutputUiList_h
#define CvOutputUiList_h

#include "uiText.h"
#include "settings.h"

class CvOutputUiList : public UiList {

public:

	enum Item {
		PORT,
		CV_SOURCE,
		CV_MODE,
		SLIDE_MODE,
		SLIDE_SPEED,
		BEND_SOURCE,
		BEND_SEMITONES,
		GATE_SOURCE,
		GATE_MODE,
		GATE_INVERT,
		CLOCK_SPEED,

		NUM_ITEMS
	};

	const int num_items() override {
		return NUM_ITEMS;
	}

	int selected_port() {
		return port_;
	}

	const char* item_text(int item) override {
		switch (item)
		{
		case PORT:				return "PORT";
		case CV_SOURCE:			return "CV SOURCE";
		case CV_MODE:			return "CV MODE";
		case SLIDE_MODE:		return "SLIDE MODE";
		case SLIDE_SPEED:		return "SLIDE SPEED";
		case BEND_SOURCE:		return "BEND SOURCE";
		case BEND_SEMITONES:	return "BEND SEMITONES";
		case GATE_SOURCE:		return "GATE SOURCE";
		case GATE_MODE:			return "GATE MODE";
		case GATE_INVERT:		return "GATE INVERT";
		case CLOCK_SPEED:		return "CLOCK SPEED";
		default:
			return nullptr;
		}
	};

	const char* value_text(int item) override {
		CvOutput &cvOutput = settings.cvOutput(port_);

		switch (item)
		{
		case PORT:				return CvOutput::port_text(port_);
		case CV_SOURCE:			return cvOutput.cv_source_text();
		case CV_MODE:			return cvOutput.cv_mode_text();
		case SLIDE_MODE:		return cvOutput.slide_mode_text();
		case SLIDE_SPEED:		return cvOutput.slide_speed_text();
		case BEND_SOURCE:		return cvOutput.bend_source_text();
		case BEND_SEMITONES:	return cvOutput.bend_semitones_text();
		case GATE_SOURCE:		return cvOutput.gate_source_text();
		case GATE_MODE:			return cvOutput.gate_mode_text();
		case GATE_INVERT:		return cvOutput.gate_invert_text();
		case CLOCK_SPEED:		return cvOutput.clock_speed_text();
		default:
			return nullptr;
		}
	};

	void edit(int item, int inc, bool shifted) override {
		CvOutput &cvOutput = settings.cvOutput(port_);

		switch (item)
		{
		case PORT:
			port_ = stmlib::clip(0, CvOutput::NUM_PORTS - 1, port_ + inc);
			break;
		case CV_SOURCE:
			cvOutput.set_cv_source(cvOutput.cv_source() + inc);
			break;
		case CV_MODE:
			cvOutput.set_cv_mode(cvOutput.cv_mode() + inc);
			break;
		case SLIDE_MODE:
			cvOutput.set_slide_mode(cvOutput.slide_mode() + inc);
			break;
		case SLIDE_SPEED:
			cvOutput.set_slide_speed(cvOutput.slide_speed() + (shifted ? inc * 3 : inc));
			break;
		case BEND_SOURCE:
			cvOutput.set_bend_source(cvOutput.bend_source() + inc);
			break;
		case BEND_SEMITONES:
			cvOutput.set_bend_semitones(cvOutput.bend_semitones() + inc);
			break;
		case GATE_SOURCE:
			cvOutput.set_gate_source(cvOutput.gate_source() + inc);
			break;
		case GATE_MODE:
			cvOutput.set_gate_mode(cvOutput.gate_mode() + inc);
			break;
		case GATE_INVERT:
			cvOutput.set_gate_invert(inc > 0);
			break;
		case CLOCK_SPEED:
			if (shifted) {
				inc *= 3;
			}
			cvOutput.set_clock_speed(cvOutput.clock_speed() + inc);
			break;
		default:
			break;
		}
	}

private:
	int port_ = 0;
};

#endif
