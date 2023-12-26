#include "chord.h"

Scale* Chord::scale_;

StringBuilderBase<16>Chord::str_;

void Chord::init(Scale *scale) {
	scale_ = scale;
}

const uint8_t Chord::lut_chords_major_scale[7] = {
	MAJOR, MINOR ,MINOR, MAJOR, MAJOR, MINOR, DIMINISHED,
};

const uint8_t Chord::lut_chords_minor_scale[7] = {
	MINOR, DIMINISHED, MAJOR, MINOR, MINOR, MAJOR, MAJOR,
};

const uint8_t Chord::lut_chords_mixolydian_scale[7] = {
	MAJOR, MINOR, DIMINISHED, MAJOR, MINOR, MINOR, MAJOR,
};

const uint8_t Chord::lut_chords_dorian_scale[7] = {
	MINOR, DIMINISHED, MAJOR, MAJOR, MINOR, DIMINISHED, MAJOR,
};

const uint8_t Chord::lut_chords_phrygian_scale[7] = {
	MINOR, MAJOR, MAJOR, MINOR, DIMINISHED, MAJOR, MINOR,
};

const uint8_t Chord::lut_chords_lydian_scale[7] = {
	MAJOR, MAJOR, MINOR, DIMINISHED, MAJOR, MINOR, MINOR,
};

const uint8_t Chord::lut_chords_aeolyian_scale[7] = {
	MINOR, DIMINISHED, MAJOR, MINOR, MINOR, MAJOR, MAJOR,
};

const uint8_t Chord::lut_chords_locrian_scale[7] = {
	DIMINISHED, MAJOR, MINOR, MINOR, MAJOR, MAJOR, MINOR,
};
