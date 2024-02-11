#include "drumTrack.h"


// step masks
SerialMask DrumTrack::step_mask_[NUM_STEP_ITEMS] = {
	[TRIGGER]			= { .reg = 0, .bits = 1, .shifts = 0 },
	[PROBABILITY]		= { .reg = 0, .bits = 3, .shifts = 1 },
	[VELOCITY]			= { .reg = 0, .bits = 7, .shifts = 4 },
	[GATE_LENGTH]		= { .reg = 0, .bits = 7, .shifts = 11 },
	[NUM_REPEATS]		= { .reg = 0, .bits = 4, .shifts = 18 },
	[REPEAT_SPREAD]		= { .reg = 0, .bits = 3, .shifts = 22 },
	[REPEAT_VELOCITY]	= { .reg = 0, .bits = 2, .shifts = 25 },
	[DELAY]				= { .reg = 0, .bits = 3, .shifts = 27 },
};

// step items
SerialItem DrumTrack::step_item_[NUM_STEP_ITEMS] = {
	[TRIGGER] = {
		.min_ = 0,
		.max_ = 1,
		.inc_shifted_ = 1,
		.init_value_ = 0,
		.mask_ = step_mask_[TRIGGER]
	},
	[PROBABILITY] = {
		.min_ = 0,
		.max_ = 7,
		.inc_shifted_ = 2,
		.init_value_ = 7,
		.mask_ = step_mask_[PROBABILITY]
	},
	[VELOCITY] = {
		.min_ = 0,
		.max_ = 127,
		.inc_shifted_ = 10,
		.init_value_ = 100,
		.mask_ = step_mask_[VELOCITY]
	},
	[GATE_LENGTH] = {
		.min_ = 1,
		.max_ = 127,
		.inc_shifted_ = 8,
		.init_value_ = 8,
		.mask_ = step_mask_[GATE_LENGTH]
	},
	[NUM_REPEATS] = {
		.min_ = 0,
		.max_ = 14,
		.inc_shifted_ = 2,
		.init_value_ = 7,
		.mask_ = step_mask_[NUM_REPEATS]
	},
	[REPEAT_SPREAD] = {
		.min_ = 0,
		.max_ = 6,
		.inc_shifted_ = 2,
		.init_value_ = 3,
		.mask_ = step_mask_[REPEAT_SPREAD]
	},
	[REPEAT_VELOCITY] = {
		.min_ = 0,
		.max_ = 2,
		.inc_shifted_ = 1,
		.init_value_ = 1,
		.mask_ = step_mask_[REPEAT_VELOCITY]
	},
	[DELAY] = {
		.min_ = 0,
		.max_ = 7,
		.inc_shifted_ = 2,
		.init_value_ = 0,
		.mask_ = step_mask_[DELAY]
	},
};


// track masks
SerialMask DrumTrack::track_mask_[NUM_TRACK_ITEMS] = {
	[DRUM_NOTE]	= { .reg = 0, .bits = 7, .shifts = 0 },
};

// track items
SerialItem DrumTrack::track_item_[NUM_TRACK_ITEMS] = {
	[DRUM_NOTE] = {
		.min_ = 0,
		.max_ = 127,
		.inc_shifted_ = 10,
		.init_value_ = 0,
		.mask_ = track_mask_[DRUM_NOTE]
	},
};
