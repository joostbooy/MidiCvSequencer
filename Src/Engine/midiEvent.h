#ifndef MidiEvent_h
#define MidiEvent_h

#include "lookUpTables.h"

class MidiEvent {

public:

	enum SourceType {
		TRACK		= 0,
		PORT		= 16,
		NUM_SOURCES = PORT + 5,
	};

	enum MessageType {
		NOTE_OFF			= 0x80,
		NOTE_ON				= 0x90,
		KEY_PRESSURE		= 0xA0,
		CONTROLLER_CHANGE	= 0xB0,
		PROGRAM_CHANGE		= 0xC0,
		CHANNEL_PRESSURE	= 0xD0,
		PITCH_BEND			= 0xE0,
		CLOCK_PULSE			= 0xF8,
		CLOCK_START			= 0xFA,
		CLOCK_CONTINUE		= 0xFB,
		CLOCK_STOP			= 0xFC,
		SONG_POSITION		= 0xF2,
	};

	struct Event {
		uint8_t message;
		uint8_t data[2];
		uint32_t time;
		uint8_t port;
		uint8_t source;
		uint8_t tie;
	};

	// message types

	static inline bool is_note_on(Event *event) {
		return (event->message & 0xF0) == NOTE_ON;
	}

	static inline bool is_note_off(Event *event) {
		return (event->message & 0xF0) == NOTE_OFF;
	}

	static inline bool is_bend(Event *event) {
		return (event->message & 0xF0) == PITCH_BEND;
	}

	static inline bool is_cc(Event *event) {
		return (event->message & 0xF0) == CONTROLLER_CHANGE;
	}

	static inline bool is_clock_message(Event* event) {
		return event->message >= 0xF8;
	}

	static inline uint8_t get_type(Event* event) {
		return is_clock_message(event) ? event->message : event->message & 0xF0;
	}

	static inline bool get_channel(Event* event, uint8_t *channel) {
		*channel = event->message & 0x0F;
		return !is_clock_message(event);
	}

	// source
	static inline void set_source(Event *event, SourceType type, uint8_t index) {
		event->source = serialise_source(type, index);
	}

	static inline void get_source(Event *event, uint8_t *type, uint8_t *index) {
		deserialise_source(event->source, index, type);
	}

	static inline int serialise_source(SourceType type, uint8_t index) {
		return type + index;
	}

	static inline void deserialise_source(uint8_t source, uint8_t *type, uint8_t *index) {
		if (source >= PORT) {
			*type = PORT;
			*index = source - PORT;
		} else {
			*type = TRACK;
			*index = source;
		}
	}

	// operators
	static inline uint16_t get_14bit_data(Event *event) {
		return (event->data[0] & 0x7F) | (event->data[1] << 7);
	}

	static inline void set_14bit_data(Event *event, uint16_t value) {
		event->data[0] = value & 0x7F;
		event->data[1] = (value >> 7) & 0x7F;
	}

	static inline uint32_t get_time_to_ticks(Event *event, uint32_t bpm) {
		return event->time / (60000 / (bpm * PPQN * 4));
	}

	static inline void convert_to_note_off(Event *event) {
		event->message &= ~0xF0;
		event->message |= NOTE_OFF;
		event->data[1] = 0x00;
	}

	static inline bool convert_fake_note_off(Event *event) {
		if (is_note_on(event) && (event->data[1] == 0x00)) {
			event->message &= ~0xF0;
			event->message |= NOTE_OFF;
			return true;
		}
		return false;
	}

	static inline void copy(Event *dest, Event *src) {
		//	memcpy(dest, src, sizeof(Event));
		dest->message = src->message;
		dest->data[0] = src->data[0];
		dest->data[1] = src->data[1];
		dest->time = src->time;
		dest->port = src->port;
		dest->tie = src->tie;
		dest->source = src->source;
	}

	static inline void copy_as_note_off(Event *dest, Event *src) {
		dest->message = NOTE_OFF | (src->message & 0x0F);
		dest->data[0] = src->data[0];
		dest->data[1] = 0x00;
		dest->time = src->time;
		dest->port = src->port;
		dest->tie = src->tie;
		dest->source = src->source;
	}

};

#endif
