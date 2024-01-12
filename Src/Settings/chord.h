#ifndef Chord_h
#define Chord_h

#include "stmf4lib.h"
#include "stringBuilder.h"
#include "scale.h"

class Chord {

public:

	enum ChordType {
		MAJOR,
		MINOR,
		DIMINISHED,

		NUM_TYPES
	};

	enum Variation {
		TRIAD,
		FIFTH,
		ADDED_6TH,
		ADDED_7TH,
		ADDED_9TH,
		AUGMENTED,
		SUSPENDED_2TH,
		SUSPENDED_4TH,

		NUM_VARIATIONS
	};

	enum InversionType {
		NO_INVERSION,
		FIRST_INVERSION,
		SECOND_INVERSION,
		THIRD_INVERSION,

		NUM_INVERSION_TYPES
	};

	static void init(Scale *scale);

	uint8_t size() {
		return size_;
	}

	uint8_t note(uint8_t index) {
		return note_[index];
	}

	void build(uint8_t root, uint8_t root_shifts, uint8_t variation, uint8_t inversion) {
		uint8_t shifted_root = stmlib::clip_min(0, root - root_shifts);

		note_[0] = scale_->read(shifted_root);
		note_[1] = scale_->read(root + 2);
		note_[2] = scale_->read(root + 4);
		size_ = 3;

		switch (variation)
		{
		case TRIAD:
			break;
		case FIFTH:
			note_[1] = note_[2];
			size_ = 2;
			break;
		case ADDED_6TH:
			note_[3] = scale_->read(root + 5);
			size_ = 4;
			break;
		case ADDED_7TH:
			note_[3] = scale_->read(root + 6);
			size_ = 4;
			break;
		case ADDED_9TH:
			note_[3] = scale_->read(root + 8);
			size_ = 4;
			break;
		case AUGMENTED:
			note_[2] += 1;
			break;
		case SUSPENDED_2TH:
			note_[1] = scale_->read(root + 1);
			break;
		case SUSPENDED_4TH:
			note_[1] = scale_->read(root + 3);
			break;
		default:
			break;
		}

		if (inversion == NO_INVERSION) {
			return;
		}

		if (inversion >= FIRST_INVERSION) {
			note_[0] += 12;
		}

		if (inversion >= SECOND_INVERSION && size_ >= 3) {
			note_[1] += 12;
		}

		if (inversion >= THIRD_INVERSION && size_ >= 4) {
			note_[2] += 12;
		}

	}

	// text
	static const char* inversion_text(uint8_t inversion) {
		switch (inversion)
		{
		case NO_INVERSION: 		return nullptr;
		case FIRST_INVERSION: 	return "1TH";
		case SECOND_INVERSION:	return "2TH";
		case THIRD_INVERSION:	return "3TH";
		default:				return nullptr;
			break;
		}
	}

	static const char* variation_text(uint8_t variation) {
		switch (variation)
		{
		case TRIAD: 		return nullptr;
		case FIFTH: 		return "5TH";
		case ADDED_6TH:		return "ADD6";
		case ADDED_7TH:		return "ADD7";
		case ADDED_9TH:		return "ADD9";
		case AUGMENTED:		return "AUG";
		case SUSPENDED_2TH:	return "SUS2";
		case SUSPENDED_4TH:	return "SUS4";
		default:			return nullptr;
			break;
		}
	}

	static const char* root_text(uint8_t root) {
		return UiText::note_text(scale_->read(root), false);
	}

	static const char* root_to_type_text(uint8_t root) {
		root %= NOTES_IN_SCALE;

		switch (scale_->type())
		{
		case Scale::MAJOR:		return type_to_text(lut_chords_major_scale[root]);
		case Scale::MINOR:		return type_to_text(lut_chords_minor_scale[root]);
		case Scale::MIXOLYDIAN:	return type_to_text(lut_chords_mixolydian_scale[root]);
		case Scale::DORIAN:		return type_to_text(lut_chords_dorian_scale[root]);
		case Scale::PHRYGIAN:	return type_to_text(lut_chords_phrygian_scale[root]);
		case Scale::LYDIAN:		return type_to_text(lut_chords_lydian_scale[root]);
		case Scale::AEOLYIAN:	return type_to_text(lut_chords_aeolyian_scale[root]);
		case Scale::LOCRIAN:	return type_to_text(lut_chords_locrian_scale[root]);
		default:				return nullptr;
			break;
		}
	}

	static const char *name(uint8_t root) {
		return str_.write(root_text(root), " ", root_to_type_text(root));
	}

private:
	uint8_t size_ = 0;
	uint8_t note_[4];
	static Scale *scale_;
	static StringBuilderBase<16> str_;

	static const uint8_t lut_chords_major_scale[7];
	static const uint8_t lut_chords_minor_scale[7];
	static const uint8_t lut_chords_mixolydian_scale[7];
	static const uint8_t lut_chords_dorian_scale[7];
	static const uint8_t lut_chords_phrygian_scale[7];
	static const uint8_t lut_chords_lydian_scale[7];
	static const uint8_t lut_chords_aeolyian_scale[7];;
	static const uint8_t lut_chords_locrian_scale[7];

	static const char* type_to_text(uint8_t type) {
		switch (type)
		{
		case MAJOR: 		return "MAJ";
		case MINOR: 		return "MIN";
		case DIMINISHED:	return "DIM";
		default:			return nullptr;
			break;
		}
	}

	// Not sure if i'll need this
	void remove_duplicate_notes(){
		uint8_t index = 1;

		while (index < size_) {
			if (note_[0] == note_[index]) {
				--size_;
				for (int i = index; i < size_; ++i) {
					note_[i] = note_[i + 1];
				}
			} else {
				++index;
			}
		}
	}
};

#endif
