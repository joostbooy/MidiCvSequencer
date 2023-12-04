#include "noteTrack.h"


// step masks
SerialMask NoteTrack::step_mask_[NUM_STEP_ITEMS] = {
	[TRIGGER]			= { .reg = 0, .bits = 1, .shifts = 0 },
	[NOTE]				= { .reg = 0, .bits = 7, .shifts = 1 },
	[VELOCITY]			= { .reg = 0, .bits = 7, .shifts = 8 },
	[GATE_LENGTH]		= { .reg = 0, .bits = 7, .shifts = 15 },
	[NUM_REPEATS]		= { .reg = 0, .bits = 4, .shifts = 22 },
	[REPEAT_SPREAD]		= { .reg = 0, .bits = 3, .shifts = 26 },
	[REPEAT_VELOCITY]	= { .reg = 0, .bits = 2, .shifts = 29 },
	[DELAY]				= { .reg = 1, .bits = 2, .shifts = 0 },
	[PROBABILITY]		= { .reg = 1, .bits = 3, .shifts = 2 },
};

// step items
SerialItem NoteTrack::step_item_[NUM_STEP_ITEMS] = {
	[TRIGGER] = {
		.min_ = 0,
		.max_ = 1,
		.inc_shifted_ = 1,
		.init_value_ = 0,
		.mask_ = step_mask_[TRIGGER]
	},
	[NOTE] = {
		.min_ = 0,
		.max_ = 127,
		.inc_shifted_ = 12,
		.init_value_ = 60,
		.mask_ = step_mask_[NOTE]
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
		.max_ = 64,
		.inc_shifted_ = 4,
		.init_value_ = 4,
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
		.max_ = 3,
		.inc_shifted_ = 1,
		.init_value_ = 0,
		.mask_ = step_mask_[DELAY]
	},
	[PROBABILITY] = {
		.min_ = 0,
		.max_ = 7,
		.inc_shifted_ = 2,
		.init_value_ = 7,
		.mask_ = step_mask_[PROBABILITY]
	},
};

// track masks
SerialMask NoteTrack::track_mask_[NUM_TRACK_ITEMS] = {
	[USE_SCALE]	= { .reg = 0, .bits = 1, .shifts = 0 },
};

// track items
SerialItem NoteTrack::track_item_[NUM_TRACK_ITEMS] = {
	[USE_SCALE] = {
		.min_ = 0,
		.max_ = 1,
		.inc_shifted_ = 1,
		.init_value_ = 1,
		.mask_ = track_mask_[USE_SCALE]
	},
};
