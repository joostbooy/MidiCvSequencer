#ifndef CvInput_h
#define CvInput_h

#include "stmf4lib.h"
//#include "clipBoard.h"

class CvInput {

public:

	enum Port {
		PORT_1,
		PORT_2,
		PORT_3,
		PORT_4,

		NUM_PORTS
	};

	static const char* port_text(int port) {
		switch (port)
		{
		case PORT_1:	return "CV PORT 1";
		case PORT_2:	return "CV PORT 2";
		case PORT_3:	return "CV PORT 3";
		case PORT_4:	return "CV PORT 4";
		default:		return nullptr;
			break;
		}
	}

	enum GateMode {
		GATE,
		CLOCK_START,
		CLOCK_STOP,
		CLOCK_PULSE,

		NUM_GATE_MODES
	};

	static const char* gate_mode_text(int mode) {
		switch (mode)
		{
		case GATE:			return "GATE";
		case CLOCK_START:	return "CLOCK START";
		case CLOCK_STOP:	return "CLOCK STOP";
		case CLOCK_PULSE:	return "CLOCK PULSE";
		default:			return nullptr;
			break;
		}
	}

	enum CvMode {
		CC,
		CC_SAMPLE_HOLD,
		NOTE,
		PITCH_BEND,

		NUM_CV_MODES
	};

	static const char* cv_mode_text(int mode) {
		switch (mode)
		{
		case CC:				return "CC";
		case CC_SAMPLE_HOLD:	return "CC SAMPLE & HOLD";
		case NOTE:				return "NOTE";
		case PITCH_BEND:		return "PITCH BEND";
		default:				return nullptr;
			break;
		}
	}

	void init() {
		set_cv_mode(CC);
		set_gate_mode(GATE);
		set_forward_port(MidiPort::MIDI_UART_1);
		set_forward_channel(-1);
	}


	// cv mode
	uint8_t cv_mode() {
		return cv_mode_;
	}

	void set_cv_mode(int mode) {
		cv_mode_ = stmlib::clip(0, NUM_CV_MODES - 1, mode);
	}

	const char* cv_mode_text() {
		return cv_mode_text(cv_mode());
	}

	// gate mode (not implemented for now)
	uint8_t gate_mode() {
		return gate_mode_;
	}

	void set_gate_mode(int mode) {
		gate_mode_ = stmlib::clip(0, NUM_GATE_MODES - 1, mode);
	}

	const char* gate_mode_text() {
		return gate_mode_text(gate_mode());
	}

	// forward port
	uint8_t forward_port() {
		return forward_port_;
	}

	void set_forward_port(int value) {
		forward_port_ = stmlib::clip(0, MidiPort::NUM_PORTS - 1, value);
	}

	const char* forward_port_text() {
		return MidiPort::port_text(forward_port());
	}

	// forward channel
	int8_t forward_channel() {
		return forward_channel_;
	}

	void set_forward_channel(int value) {
		forward_channel_ = stmlib::clip(-1, 15, value);
	}

	const char* forward_channel_text() {
		return UiText::midi_channel_text(forward_channel());
	}


	/***************
	*	Methods
	***************/

	// storage
	void save(FileWriter &fileWriter) {
		fileWriter.write(cv_mode_);
		fileWriter.write(gate_mode_);
		fileWriter.write(forward_port_);
		fileWriter.write(forward_channel_);
	}

	void load(FileReader &fileReader) {
		fileReader.read(cv_mode_);
		fileReader.read(gate_mode_);

		if (fileReader.version() < 1) {
			fileReader.skip(2);
		} else {
			fileReader.read(forward_port_);
			fileReader.read(forward_channel_);
		}
	}

private:
	uint8_t cv_mode_;
	uint8_t gate_mode_;
	uint8_t forward_port_;
	int8_t forward_channel_;
};

#endif
