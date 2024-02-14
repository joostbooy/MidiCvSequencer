#ifndef CvOutput_h
#define CvOutput_h

#include "stmf4lib.h"
#include "song.h"

class CvOutput {

public:

	enum Port {
		PORT_1,
		PORT_2,
		PORT_3,
		PORT_4,
		PORT_5,
		PORT_6,
		PORT_7,
		PORT_8,

		NUM_PORTS
	};

	static const char* port_text(int port) {
		switch (port)
		{
		case PORT_1:	return "CV OUT 1";
		case PORT_2:	return "CV OUT 2";
		case PORT_3:	return "CV OUT 3";
		case PORT_4:	return "CV OUT 4";
		case PORT_5:	return "CV OUT 5";
		case PORT_6:	return "CV OUT 6";
		case PORT_7:	return "CV OUT 7";
		case PORT_8:	return "CV OUT 8";
		default:		return nullptr;
			break;
		}
	}

	enum GateMode {
		GATE,
		TRIGGER,
		CLOCK_START,
		CLOCK_STOP,
		CLOCK_PULSE,

		NUM_GATE_MODES
	};

	static const char* gate_mode_text(int mode) {
		switch (mode)
		{
		case GATE:			return "GATE";
		case TRIGGER:		return "TRIGGER";
		case CLOCK_START:	return "CLOCK START";
		case CLOCK_STOP:	return "CLOCK STOP";
		case CLOCK_PULSE:	return "CLOCK PULSE";
		default:			return nullptr;
			break;
		}
	}

	enum CvMode {
		NOTE,
		VELOCITY,
		CC,

		NUM_CV_MODES
	};

	static const char* cv_mode_text(int mode) {
		switch (mode)
		{
		case NOTE:			return "NOTE";
		case VELOCITY:		return "VELOCITY";
		case CC:			return "CC";
		default:			return nullptr;
			break;
		}
	}

	enum SlideMode {
		OFF,
		ON,
		LEGATO,

		NUM_SLIDE_MODES
	};

	static const char* slide_mode_text(int mode) {
		switch (mode)
		{
		case OFF:		return "OFF";
		case ON:		return "ON";
		case LEGATO:	return "LEGATO";
		default:		return nullptr;
			break;
		}
	}

	enum CvRange {
		BIPOLAR,
		UNIPOLAR,
		NUM_CV_RANGES
	};

	static const char* cv_range_text(int value) {
		switch (value)
		{
		case BIPOLAR:	return "BIPOLAR";
		case UNIPOLAR:	return "UNIPOLAR";
		default:		return nullptr;
			break;
		}
	}

	static void init(Song *song);

	void init() {
		set_cv_source(0);
		set_cv_mode(NOTE);
		set_cv_range(BIPOLAR);
		set_gate_source(0);
		set_gate_mode(GATE);
		set_gate_invert(false);
		set_slide_mode(OFF);
		set_clock_speed(ClockEngine::WHOLE_16TH);
		set_slide_speed(ClockEngine::WHOLE_16TH);
		set_bend_semitones(2);
		set_bend_source(-1);
	}

	// cv source
	int cv_source() {
		return cv_source_;
	}

	void set_cv_source(int value) {
		cv_source_ = stmlib::clip(0, MidiEvent::NUM_SOURCES - 1, value);
	}

	const char* cv_source_text() {
		return source_text(cv_source());
	}

	// cv mode
	uint8_t cv_mode() {
		return cv_mode_;
	}

	void set_cv_mode(int mode) {
		cv_mode_ = stmlib::clip(0, NUM_CV_MODES - 1, mode);

		// Always use bipolar scaling in note mode,
		// because with unipolar we would lose 1V/oct accuracy
		if (cv_mode_ == NOTE) {
			set_cv_range(BIPOLAR);
		}
	}

	const char* cv_mode_text() {
		return cv_mode_text(cv_mode());
	}

	// cv range
	int cv_range() {
		return cv_range_;
	}

	void set_cv_range(int value) {
		if (cv_mode() == NOTE) {
			cv_range_ = BIPOLAR;
		} else {
			cv_range_ = stmlib::clip(0, NUM_CV_RANGES - 1, value);
		}
	}

	const char* cv_range_text() {
		return cv_range_text(cv_range());
	}

	// gate source
	int gate_source() {
		return gate_source_;
	}

	void set_gate_source(int value) {
		gate_source_ = stmlib::clip(0, MidiEvent::NUM_SOURCES - 1, value);
	}

	const char* gate_source_text() {
		return source_text(gate_source());
	}

	// gate mode
	uint8_t gate_mode() {
		return gate_mode_;
	}

	void set_gate_mode(int mode) {
		gate_mode_ = stmlib::clip(0, NUM_GATE_MODES - 1, mode);
	}

	const char* gate_mode_text() {
		return gate_mode_text(gate_mode());
	}

	// gate invert
	bool gate_invert() {
		return gate_invert_;
	}

	void set_gate_invert(bool state) {
		gate_invert_ = state;
	}

	const char* gate_invert_text() {
		return gate_invert() ? "ON" : "OFF";
	}

	// clock speed
	uint8_t clock_speed() {
		return clock_speed_;
	}

	void set_clock_speed(int speed) {
		if (gate_mode() == CLOCK_PULSE) {
			clock_speed_ = stmlib::clip(0, ClockEngine::NUM_SIGNATURES - 1, speed);
		}
	}

	const char* clock_speed_text() {
		if (gate_mode() == CLOCK_PULSE) {
			return ClockEngine::speed_text(clock_speed());
		}
		return "-";
	}

	// slide speed
	uint8_t slide_speed() {
		return slide_speed_;
	}

	void set_slide_speed(int value) {
		if (cv_mode() == NOTE) {
			slide_speed_ = stmlib::clip(0, ClockEngine::NUM_SIGNATURES - 1, value);
		}
	}

	const char* slide_speed_text() {
		if (cv_mode() == NOTE) {
			return ClockEngine::speed_text(slide_speed());
		}
		return "-";
	}

	// slide mode
	uint8_t slide_mode() {
		return slide_mode_;
	}

	void set_slide_mode(int value) {
		if (cv_mode() == NOTE) {
			slide_mode_ = stmlib::clip(0, NUM_SLIDE_MODES - 1, value);
		}
	}

	const char* slide_mode_text() {
		if (cv_mode() == NOTE) {
			return slide_mode_text(slide_mode());
		}
		return "-";
	}

	// bend source
	int bend_source() {
		return bend_source_;
	}

	void set_bend_source(int value) {
		if (cv_mode() == NOTE) {
			bend_source_ = stmlib::clip(-1, MidiEvent::NUM_SOURCES - 1, value);
		}
	}

	const char* bend_source_text() {
		if (cv_mode() == NOTE) {
			return bend_source() < 0 ? "NONE" : source_text(bend_source());
		}
		return "-";
	}

	bool bend_enabled() {
		return bend_source() >= 0;
	}

	// bend semitones
	uint8_t bend_semitones() {
		return bend_semitones_;
	}

	void set_bend_semitones(int value) {
		if (cv_mode() == NOTE && bend_enabled() == true) {
			bend_semitones_ = stmlib::clip(1, 12, value);
		}
	}

	const char* bend_semitones_text() {
		if (cv_mode() == NOTE && bend_enabled() == true) {
			return UiText::str.write(bend_semitones());
		}
		return "-";
	}


	/***************
	*	Methods
	***************/

	// storage
	void save(FileWriter &fileWriter) {
		fileWriter.write(cv_source_);
		fileWriter.write(cv_mode_);
		fileWriter.write(cv_range_);
		fileWriter.write(gate_source_);
		fileWriter.write(gate_mode_);
		fileWriter.write(gate_invert_);
		fileWriter.write(clock_speed_);
		fileWriter.write(slide_mode_);
		fileWriter.write(slide_speed_);
		fileWriter.write(bend_source_);
		fileWriter.write(bend_semitones_);
	}

	void load(FileReader &fileReader) {
		fileReader.read(cv_source_);
		fileReader.read(cv_mode_);

		if (fileReader.version() >= 2) {
			fileReader.read(cv_range_);
		}

		fileReader.read(gate_source_);
		fileReader.read(gate_mode_);
		fileReader.read(gate_invert_);
		fileReader.read(clock_speed_);
		fileReader.read(slide_mode_);
		fileReader.read(slide_speed_);
		fileReader.read(bend_source_);
		fileReader.read(bend_semitones_);
	}

private:
	uint8_t cv_source_;
	uint8_t cv_mode_;
	uint8_t cv_range_;
	uint8_t gate_source_;
	uint8_t gate_mode_;
	bool gate_invert_;
	uint8_t clock_speed_;
	uint8_t slide_mode_;
	uint8_t slide_speed_;
	int8_t bend_source_;
	uint8_t bend_semitones_;

	static Song *song_;

	static const char* source_text(uint8_t source) {
		uint8_t type, index;
		MidiEvent::deserialise_source(source, &type, &index);

		if (type == MidiEvent::PORT) {
			return MidiPort::port_text(index);
		} else {
			return song_->track(index).name();
		}
	}
};

#endif
