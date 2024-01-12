#ifndef MidiInputnUiList_h
#define MidiInputnUiList_h

#include "uiText.h"
#include "settings.h"

class MidiInputUiList : public UiList {

public:

	enum Item {
		PORT,
		CHANNEL_RECEIVE,
		CHANNEL_SEND,
		QUANTISE_NOTE,
		PITCH_BEND_RECEIVE,
		CC_RECEIVE,
		ARP_OFFSET,
		ARP_SPEED,
		ARP_SWING,
		ARP_NOTE_MODE,
		ARP_VELOCITY,
		ARP_VELOCITY_MODE,
		ARP_GATE_LENGTH,
		ARP_GATE_MODE,
		ARP_TRIGGER_MODE,

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
		case PORT:					return "PORT";
		case CHANNEL_RECEIVE:		return "CHANNEL RECEIVE";
		case CHANNEL_SEND:			return "CHANNEL SEND";
		case QUANTISE_NOTE:			return "QUANTISE NOTE";
		case PITCH_BEND_RECEIVE:	return "PITCH BEND RECEIVE";
		case CC_RECEIVE:			return "CC RECEIVE";
		case ARP_OFFSET:			return "ARP OFFSET";
		case ARP_SPEED:				return "ARP SPEED";
		case ARP_SWING:				return "ARP SWING";
		case ARP_NOTE_MODE:			return "ARP NOTE MODE";
		case ARP_VELOCITY:			return "ARP VELOCITY";
		case ARP_VELOCITY_MODE:		return "ARP VELOCITY MODE";
		case ARP_GATE_MODE:			return "ARP GATE MODE";
		case ARP_GATE_LENGTH:		return "ARP GATE LENGTH";
		case ARP_TRIGGER_MODE:		return "ARP TRIGGER MODE";
		default:					return nullptr;
			break;
		}
	}

	const char* value_text(int item) override {
		MidiInput &input = settings.midiInput(port_);

		switch (item)
		{
		case PORT:					return MidiPort::port_text(port_);
		case CHANNEL_RECEIVE:		return input.channel_receive_text();
		case CHANNEL_SEND:			return input.channel_send_text();
		case QUANTISE_NOTE:			return input.quantise_note_text();
		case PITCH_BEND_RECEIVE:	return input.pitch_bend_receive_text();
		case CC_RECEIVE:			return input.cc_receive_text();
		case ARP_OFFSET:			return input.arpeggiator.offset_text();
		case ARP_SPEED:				return input.arpeggiator.speed_text();
		case ARP_SWING:				return input.arpeggiator.swing_text();
		case ARP_NOTE_MODE:			return input.arpeggiator.note_mode_text();
		case ARP_VELOCITY:			return input.arpeggiator.velocity_text();
		case ARP_VELOCITY_MODE:		return input.arpeggiator.velocity_mode_text();
		case ARP_GATE_LENGTH:		return input.arpeggiator.gate_length_text();
		case ARP_GATE_MODE:			return input.arpeggiator.gate_mode_text();
		case ARP_TRIGGER_MODE:		return input.arpeggiator.trigger_mode_text();
		default:					return nullptr;
			break;
		}
	}

	void edit(int item, int inc, bool shifted) override {
		MidiInput &input = settings.midiInput(port_);

		switch (item)
		{
		case PORT:
			port_ = stmlib::clip(0, MidiPort::NUM_PORTS - 1, port_ + inc);
			break;
		case CHANNEL_RECEIVE:
			input.set_channel_receive(input.channel_receive() + inc);
			break;
		case CHANNEL_SEND:
			input.set_channel_send(input.channel_send() + inc);
			break;
		case QUANTISE_NOTE:
			input.set_quantise_note(inc > 0);
			break;
		case PITCH_BEND_RECEIVE:
			input.set_pitch_bend_receive(inc > 0);
			break;
		case CC_RECEIVE:
			input.set_cc_receive(input.cc_receive() + (shifted ? 10 * inc : inc));
			break;
		case ARP_OFFSET:
			input.arpeggiator.set_offset(input.arpeggiator.offset() + inc);
			break;
		case ARP_SPEED:
			input.arpeggiator.set_speed(input.arpeggiator.speed() + (shifted ? 3 * inc : inc));
			break;
		case ARP_SWING:
			input.arpeggiator.set_swing(input.arpeggiator.swing() + inc);
			break;
		case ARP_NOTE_MODE:
			input.arpeggiator.set_note_mode(input.arpeggiator.note_mode() + inc);
			break;
		case ARP_VELOCITY:
			input.arpeggiator.set_velocity(input.arpeggiator.velocity() + (shifted ? 10 * inc : inc));
			break;
		case ARP_VELOCITY_MODE:
			input.arpeggiator.set_velocity_mode(input.arpeggiator.velocity_mode() + inc);
			break;
		case ARP_GATE_LENGTH:
			input.arpeggiator.set_gate_length(input.arpeggiator.gate_length() + (shifted ? 4 * inc : inc));
			break;
		case ARP_GATE_MODE:
			input.arpeggiator.set_gate_mode(input.arpeggiator.gate_mode() + inc);
			break;
		case ARP_TRIGGER_MODE:
			input.arpeggiator.set_trigger_mode(input.arpeggiator.trigger_mode() + inc);
			break;
		default:
			break;
		}
	}

private:
	int port_ = 0;
};

#endif
