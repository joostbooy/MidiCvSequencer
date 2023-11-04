#ifndef Arpeggiator_h
#define Arpeggiator_h

#include "stmf4lib.h"
#include "fileReader.h"
#include "fileWriter.h"
#include "clockEngine.h"
#include "uiText.h"

class Arpeggiator {

public:

	enum TriggerMode {
		XOOX,
		XOXX,
		XXOO,
		XXOX,
		XXXO,
		XXXX,

		NUM_TRIGGER_MODES
	};

	static const char* trigger_mode_text(uint8_t mode) {
		switch (mode)
		{
		case XOOX:	return "|..|";
		case XOXX:	return "|.||";
		case XXOO:	return "||..";
		case XXOX:	return "||.|";
		case XXXO:	return "|||.";
		case XXXX:	return "||||";
		default:	break;
		}
		return nullptr;
	}

	const uint8_t trigger_mask() {
		switch (trigger_mode_)
		{
		case XOOX:	return 0x9;
		case XOXX:	return 0xD;
		case XXOO:	return 0x3;
		case XXOX:	return 0xB;
		case XXXO:	return 0x7;
		case XXXX:	return 0xF;
		default:	break;
		}
		return 0x00;
	}

	enum ClockMode {
		OFF,
		FORWARD,
		REVERSE,
		ALTERNATE,
		ALTERNATE_REVERSE,
		PINGPONG,
		PINGPONG_REVERSE,
		ZIGZAG,
		ZIGZAG_REVERSE,

		NUM_CLOCK_MODES
	};

	static const char* clock_mode_text(uint8_t mode) {
		switch (mode)
		{
		case OFF:				return "OFF";
		case FORWARD:			return "FORWARD";
		case REVERSE:			return "REVERSE";
		case ALTERNATE:			return "ALTERNATE";
		case ALTERNATE_REVERSE:	return "ALTERNATE REVERSE";
		case PINGPONG:			return "PINGPONG";
		case PINGPONG_REVERSE:	return "PINGPONG REVERSE";
		case ZIGZAG:			return "ZIGZAG";
		case ZIGZAG_REVERSE:	return "ZIGZAG REVERSE";
		default:				break;
		}
		return nullptr;
	}

	static const char* clock_mode_text_small(uint8_t mode) {
		switch (mode)
		{
		case OFF:				return "OFF";
		case FORWARD:			return ">";
		case REVERSE:			return "<";
		case ALTERNATE:			return ">|<";
		case ALTERNATE_REVERSE:	return "<|>";
		case PINGPONG:			return "><";
		case PINGPONG_REVERSE:	return "<>";
		case ZIGZAG:			return "_-_";
		case ZIGZAG_REVERSE:	return "-_-";
		default:				break;
		}
		return nullptr;
	}

	void init() {
		set_offset(0);
		set_trigger_mode(XXXX);
		set_gate_length(4);
		set_note_mode(OFF);
		set_velocity_mode(OFF);
		set_gate_mode(OFF);
		set_speed(ClockEngine::WHOLE_16TH);
	}


	// optional boundary check to speed up chord track engine

	// speed
	uint8_t speed() {
		return speed_;
	}

	void set_speed(int value, bool boundary_check = 1) {
		speed_ = boundary_check ? clip(0, ClockEngine::NUM_SIGNATURES - 1, value) : value;
	}

	const char* speed_text() {
		return ClockEngine::speed_text(speed());
	}

	// offset
	uint8_t offset() {
		return offset_;
	}

	void set_offset(int value, bool boundary_check = 1) {
		offset_ = boundary_check ? clip(0, 15, value) : value;
	}

	const char* offset_text() {
		return UiText::str.write(offset());
	}

	// trigger mode
	uint8_t trigger_mode() {
		return trigger_mode_;
	}

	void set_trigger_mode(int value, bool boundary_check = 1) {
		trigger_mode_ = boundary_check ? clip(0, NUM_TRIGGER_MODES - 1, value) : value;
	}

	const char* trigger_mode_text() {
		return trigger_mode_text(trigger_mode());
	}

	// note mode
	uint8_t note_mode() {
		return note_mode_;
	}

	void set_note_mode(int value, bool boundary_check = 1) {
		note_mode_ = boundary_check ? clip(0, NUM_CLOCK_MODES - 1, value) : value;
	}


	const char* note_mode_text() {
		return clock_mode_text(note_mode());
	}

	// velocity mode
	uint8_t velocity_mode() {
		return velocity_mode_;
	}


	void set_velocity_mode(int value, bool boundary_check = 1) {
		velocity_mode_ = boundary_check ? clip(0, NUM_CLOCK_MODES - 1, value) : value;
	}

	const char* velocity_mode_text() {
		return clock_mode_text(velocity_mode());
	}

	// gate mode
	uint8_t gate_mode() {
		return gate_mode_;
	}

	void set_gate_mode(int value, bool boundary_check = 1) {
		gate_mode_ = boundary_check ? clip(0, NUM_CLOCK_MODES - 1, value) : value;
	}

	const char* gate_mode_text() {
		return clock_mode_text(gate_mode());
	}

	// gate length
	uint8_t gate_length() {
		return gate_length_;
	}

	void set_gate_length(int value) {
		gate_length_ = clip(1, 63, value);
	}

	const char* gate_length_text() {
		return ClockEngine::gate_duration_text(gate_length());
	}


	/************
	*	Methods
	*************/


	bool enabled() {
		return note_mode_ != OFF;
	}

	// storage
	void load(FileReader &fileReader) {
		fileReader.read(offset_);
		fileReader.read(trigger_mode_);
		fileReader.read(gate_length_);
		fileReader.read(speed_);
		fileReader.read(note_mode_);
		fileReader.read(gate_mode_);
		fileReader.read(velocity_mode_);
	}

	void save(FileWriter &fileWriter) {
		fileWriter.write(offset_);
		fileWriter.write(trigger_mode_);
		fileWriter.write(gate_length_);
		fileWriter.write(speed_);
		fileWriter.write(note_mode_);
		fileWriter.write(gate_mode_);
		fileWriter.write(velocity_mode_);
	}

private:
	uint8_t offset_;
	uint8_t trigger_mode_;
	uint8_t gate_length_;
	uint8_t speed_;
	uint8_t note_mode_;
	uint8_t gate_mode_;
	uint8_t velocity_mode_;
};

#endif
