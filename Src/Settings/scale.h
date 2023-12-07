#ifndef Scale_h
#define Scale_h

#include "stack.h"
#include "uiText.h"
#include "lookUpTables.h"
#include "fileWriter.h"
#include "fileReader.h"

class Scale {

public:

	enum Type {
		MAJOR,
		MINOR,
		MIXOLYDIAN,
		DORIAN,
		PHRYGIAN,
		LYDIAN,
		AEOLYIAN,
		LOCRIAN,

		NUM_TYPES
	};

	static const char* type_text(uint8_t type) {
		switch (type)
		{
		case MAJOR:			return "MAJOR";
		case MINOR:			return "MINOR";
		case MIXOLYDIAN:	return "MIXOLYDIAN";
		case DORIAN:		return "DORIAN";
		case PHRYGIAN:		return "PHRYGIAN";
		case LYDIAN:		return "LYDIAN";
		case AEOLYIAN:		return "AEOLYIAN";
		case LOCRIAN:		return "LOCRIAN";
		default:			return nullptr;
			break;
		}
	}

	inline void init() {
		root_ = 0;
		set_type(MAJOR);
	}

	// root
	inline uint8_t root() {
		return root_;
	}

	inline void set_root(int value) {
		root_ = stmlib::clip(0, 11, value);
		make_keymap();
	}

	const char* root_text() {
		return UiText::note_text(root(), false);
	}

	// type
	inline uint8_t type() {
		return type_;
	}

	inline void set_type(int value) {
		type_ = stmlib::clip(0, NUM_TYPES - 1, value);

		switch (type_)
		{
		case MAJOR:
			lut_ptr = lut_scale_major;
			lut_map_ptr = lut_scale_major_map;
			break;
		case MINOR:
			lut_ptr = lut_scale_minor;
			lut_map_ptr = lut_scale_minor_map;
			break;
		case MIXOLYDIAN:
			lut_ptr = lut_scale_mixolydian;
			lut_map_ptr = lut_scale_mixolydian_map;
			break;
		case DORIAN:
			lut_ptr = lut_scale_dorian;
			lut_map_ptr = lut_scale_dorian_map;
			break;
		case PHRYGIAN:
			lut_ptr = lut_scale_phrygian;
			lut_map_ptr = lut_scale_phrygian_map;
			break;
		case LYDIAN:
			lut_ptr = lut_scale_lydian;
			lut_map_ptr = lut_scale_lydian_map;
			break;
		case AEOLYIAN:
			lut_ptr = lut_scale_aeolyian;
			lut_map_ptr = lut_scale_aeolyian_map;
			break;
		case LOCRIAN:
			lut_ptr = lut_scale_locrian;
			lut_map_ptr = lut_scale_locrian_map;
			break;
		default:
			break;
		}
		make_keymap();
	}

	const char* type_text() {
		return type_text(type());
	}

	// methods
	inline uint8_t read(uint8_t index) {
		return lut_ptr[index] + root_;
	}

	inline uint8_t read_map(uint8_t note) {
		uint8_t scale_note = lut_map_ptr[note] + root_;
		return scale_note > highest_note ? highest_note : scale_note;
	}

	inline uint8_t snap_and_transpose(uint8_t note) {
		uint8_t oct = 0;
		while (note >= 12){
			note -= 12;
			oct += 12;
		}

		uint8_t quantised_note;
		for (uint8_t i = 0; i < NOTES_IN_SCALE; ++i) {
			quantised_note = keymap.read(i);
			if (quantised_note >= note) {
				break;
			}
		}
		quantised_note += oct;

		return quantised_note > highest_note ? highest_note : quantised_note;
	}

	inline bool in_scale(uint8_t note) {
		return snap_and_transpose(note) == note;
	}

	// storage
	void load(FileReader &fileReader) {
		fileReader.read(root_);
		fileReader.read(type_);
		set_root(root_);
		set_type(type_);
	}

	void save(FileWriter &fileWriter) {
		fileWriter.write(root_);
		fileWriter.write(type_);
	}

private:
	uint8_t type_;
	uint8_t root_;
	uint8_t highest_note;
	const uint8_t *lut_ptr;
	const uint8_t *lut_map_ptr;

	Stack<uint8_t, NOTES_IN_SCALE>keymap;

	inline void make_keymap() {
		keymap.clear();

		for (uint8_t i = 0; i < NOTES_IN_SCALE; ++i) {
			uint8_t note = read(i) % 12;
			keymap.sort(note);
		}

		int8_t oct = 120;
		highest_note = 0;

		while (oct >= 0 && highest_note == 0) {
			for (int i = 0; i < NOTES_IN_SCALE; ++i) {
				uint16_t note = keymap.read(i) + oct;
				if (note < 128 && note > highest_note) {
					highest_note = note;
				}
			}
			oct -= 12;
		}
	}
};

#endif
