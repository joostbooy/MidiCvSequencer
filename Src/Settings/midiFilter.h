#ifndef MidiFilter_h
#define MidiFilter_h

#include "midiEvent.h"

class MidiFilter {

public:

	enum MessageType {
		NOTE,
		CC,
		BEND,
		CLOCK,

		NUM_FILTERS
	};

	static const char* messageTypeText(uint8_t type) {
		switch (type)
		{
		case NOTE:	return "NOTE";
		case CC:	return "CC";
		case BEND:	return "BEND";
		case CLOCK:	return "CLOCK";
		default:	break;
		}
		return nullptr;
	}

	void init() {
		channels = 0xFFFF;
		messages = 0xFFFF;
	}

	bool event_allowed(MidiEvent::Event &event) {
		if (MidiEvent::is_clock_message(&event)) {
			return message_allowed(CLOCK);
		}

		uint8_t channel = event.message & 0x0F;
		uint8_t message = event.message & 0xF0;

		if (!channel_allowed(channel)) {
			return false;
		}

		switch (message)
		{
		case MidiEvent::NOTE_ON:
		case MidiEvent::NOTE_OFF:
			return message_allowed(NOTE);
		case MidiEvent::CONTROLLER_CHANGE:
			return message_allowed(CC);
		case MidiEvent::PITCH_BEND:
			return message_allowed(BEND);
		default:
			break;
		}
		return false;
	}

	bool message_allowed(MessageType type) {
		return messages & (1 << type);
	}

	void set_message(MessageType type, bool state) {
		uint16_t mask = (1 << type);
		state ? messages |= mask : messages &= ~mask;
	}

	bool channel_allowed(uint8_t chn) {
		return channels & (1 << chn);
	}

	void set_channel(uint8_t chn, bool state) {
		uint16_t mask = (1 << chn);
		state ? channels |= mask : channels &= ~mask;
	}

	// storage
	void load(FileReader &fileReader) {
		fileReader.read(channels);
		fileReader.read(messages);
	}

	void save(FileWriter &fileWriter) {
		fileWriter.write(channels);
		fileWriter.write(messages);
	}

private:
	uint16_t channels;
	uint16_t messages;
};

#endif
