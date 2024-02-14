#ifndef UiText_h
#define UiText_h

#include "stringBuilder.h"
#include "midiEvent.h"
#include "midiPort.h"
#include "font.h"
#include "dsp.h"

class UiText {

public:

	static StringBuilderBase<31>str;

	static const char* int_to_text(int value) {
		return str.write(value);
	}

//	static const char* unsigned_int_to_text(uint32_t value) {
//		return str.write(value);
//	}

	static const char* bool_to_on_off(bool state) {
		return state ? "ON" : "OFF";
	}

	static const char* bool_to_checkmark(bool state) {
		return state ? font.glyph(Font::CHECKMARK) : " ";
	}

	static const char* percentage_text(uint32_t ammount, uint32_t total) {
		float percentage = (100.f / total) * ammount;
		return str.write(static_cast<int>(percentage), "%");
	}

	static const char* midi_channel_text(int8_t chn) {
		if (chn < 0) {
			return "NONE";
		} else if (chn >= 16) {
			return "OMNI";
		} else {
			return str.write(chn + 1);
		}
	}

	static const char* midi_channel_text(MidiEvent::Event &event) {
		uint8_t channel = 0;
		if (MidiEvent::get_channel(&event, &channel)) {
			return str.write("CHN", channel + 1);
		}
		return nullptr;
	}

	static const char* midi_message_text(MidiEvent::Event& event) {
		switch (MidiEvent::get_type(&event))
		{
		case MidiEvent::CLOCK_PULSE:		return "CLOCK PULSE";
		case MidiEvent::CLOCK_START:		return "CLOCK START";
		case MidiEvent::CLOCK_CONTINUE:		return "CLOCK CONTINUE";
		case MidiEvent::CLOCK_STOP:			return "CLOCK STOP";
		case MidiEvent::SONG_POSITION:		return "SONG POSITION";
		case MidiEvent::NOTE_OFF:			return "NOTE OFF";
		case MidiEvent::NOTE_ON:			return "NOTE ON";
		case MidiEvent::KEY_PRESSURE:		return "KEY PRESSURE";
		case MidiEvent::CONTROLLER_CHANGE:	return "CC";
		case MidiEvent::PROGRAM_CHANGE:		return "PROGRAM CHANGE";
		case MidiEvent::CHANNEL_PRESSURE:	return "CHANNEL PRESSURE";
		case MidiEvent::PITCH_BEND:			return "PITCH BEND";
		default:							return nullptr;
			break;
		}
	}

	static const char* midi_data_text(MidiEvent::Event& event, uint8_t byte) {
		switch (MidiEvent::get_type(&event))
		{
		case MidiEvent::NOTE_ON:
		case MidiEvent::NOTE_OFF:
			if (byte == 0) {
				return note_text(event.data[0]);
			} else {
				return str.write("VEL ", event.data[1]);
			}
		case MidiEvent::CONTROLLER_CHANGE:
			if (byte == 0) {
				return str.write("NUM ", event.data[0]);
			} else {
				return str.write("VAL ", event.data[1]);
			}
		case MidiEvent::PITCH_BEND:
			return str.write(MidiEvent::get_14bit_data(&event));
		default:
			break;
		}
		return nullptr;
	}

	static const char* note_text(uint8_t note, bool print_octave = true) {
		uint8_t oct = 0;
		while (note >= 12) {
			note -= 12;
			++oct;
		}

		str.write(note_to_text[note]);

		if (print_octave) {
			if (oct == 10) {
				str.append("X");
			} else {
				str.append(oct);
			}
		}

		return str.read();
	}

	static const char* cv_to_volt_text(float cv) {
		int value = Dsp::cross_fade(-500, 500, cv);
		int intergral = std::floor(value / 100);
		int fractional = std::abs(value - (intergral * 100));
		return str.write(intergral, ".", fractional, "V");
	};

	static const char* kb_to_mem_size_text(uint32_t kb) {
		uint32_t gb = kb / 1000000;
		uint32_t mb = kb / 1000;

		if (gb > 0) {
			uint32_t gb_frac = (kb - (gb * 1000000)) / 10000;
			return str.write(gb, ".", gb_frac, " GB");
		} else if (mb > 0){
			uint32_t mb_frac = (kb - (mb * 1000)) / 10;
			return str.write(mb, ".", mb_frac, " MB");
		} else {
			return str.write(kb, " KB");
		}
	}

private:
	static const char* note_to_text[12];
};

#endif
