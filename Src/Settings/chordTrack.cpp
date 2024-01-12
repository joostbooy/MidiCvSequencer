#include "chordTrack.h"

// step masks
SerialMask ChordTrack::step_mask_[NUM_STEP_ITEMS] = {
	[TRIGGER]			= { .reg = 0, .bits = 1, .shifts = 0 },
	[PROBABILITY]		= { .reg = 0, .bits = 3, .shifts = 1 },
	[CHORD_TYPE]		= { .reg = 0, .bits = 5, .shifts = 4 },
	[CHORD_VARIATION]	= { .reg = 0, .bits = 3, .shifts = 9 },
	[CHORD_INVERSION]	= { .reg = 0, .bits = 2, .shifts = 12 },
	[CHORD_ROOT_SHIFT]	= { .reg = 0, .bits = 3, .shifts = 14 },
	[VELOCITY]			= { .reg = 0, .bits = 7, .shifts = 17 },
	[GATE_LENGTH]		= { .reg = 0, .bits = 7, .shifts = 24 },
	[DELAY]				= { .reg = 1, .bits = 2, .shifts = 0 },
	[ARP_SPEED]			= { .reg = 1, .bits = 5, .shifts = 2 },
	[ARP_MODE]			= { .reg = 1, .bits = 4, .shifts = 7 },
	[ARP_GATE]			= { .reg = 1, .bits = 4, .shifts = 11 },
	[ARP_VELOCITY]		= { .reg = 1, .bits = 4, .shifts = 15 },
	[ARP_TRIGGER]		= { .reg = 1, .bits = 3, .shifts = 19 },
	[ARP_OFFSET]		= { .reg = 1, .bits = 3, .shifts = 22 },
};

// step items
SerialItem ChordTrack::step_item_[NUM_STEP_ITEMS] = {
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
	[CHORD_TYPE] = {
		.min_ = 0,
		.max_ = 21,
		.inc_shifted_ = 7,
		.init_value_ = 7,
		.mask_ = step_mask_[CHORD_TYPE]
	},
	[CHORD_VARIATION] = {
		.min_ = 0,
		.max_ = 7,
		.inc_shifted_ = 2,
		.init_value_ = 0,
		.mask_ = step_mask_[CHORD_VARIATION]
	},
	[CHORD_INVERSION] = {
		.min_ = 0,
		.max_ = 3,
		.inc_shifted_ = 2,
		.init_value_ = 0,
		.mask_ = step_mask_[CHORD_INVERSION]
	},
	[CHORD_ROOT_SHIFT] = {
		.min_ = 0,
		.max_ = 7,
		.inc_shifted_ = 2,
		.init_value_ = 0,
		.mask_ = step_mask_[CHORD_ROOT_SHIFT]
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
	[DELAY] = {
		.min_ = 0,
		.max_ = 3,
		.inc_shifted_ = 1,
		.init_value_ = 0,
		.mask_ = step_mask_[DELAY]
	},
	[ARP_SPEED] = {
		.min_ = 0,
		.max_ = 17,
		.inc_shifted_ = 3,
		.init_value_ = 13,
		.mask_ = step_mask_[ARP_SPEED]
	},
	[ARP_MODE] = {
		.min_ = 0,
		.max_ = 8,
		.inc_shifted_ = 1,
		.init_value_ = 0,
		.mask_ = step_mask_[ARP_MODE]
	},
	[ARP_GATE] = {
		.min_ = 0,
		.max_ = 8,
		.inc_shifted_ = 1,
		.init_value_ = 0,
		.mask_ = step_mask_[ARP_GATE]
	},
	[ARP_VELOCITY] = {
		.min_ = 0,
		.max_ = 8,
		.inc_shifted_ = 1,
		.init_value_ = 0,
		.mask_ = step_mask_[ARP_VELOCITY]
	},
	[ARP_TRIGGER] = {
		.min_ = 0,
		.max_ = 5,
		.inc_shifted_ = 1,
		.init_value_ = 5,
		.mask_ = step_mask_[ARP_TRIGGER]
	},
	[ARP_OFFSET] = {
		.min_ = 0,
		.max_ = 3,
		.inc_shifted_ = 1,
		.init_value_ = 0,
		.mask_ = step_mask_[ARP_OFFSET]
	},
};


// track masks
SerialMask ChordTrack::track_mask_[NUM_TRACK_ITEMS] = {
	[CHORD_OCT_OFFSET]	= { .reg = 0, .bits = 4, .shifts = 0 },
};

// track items
SerialItem ChordTrack::track_item_[NUM_TRACK_ITEMS] = {
	[CHORD_OCT_OFFSET] = {
		.min_ = 0,
		.max_ = 8,
		.inc_shifted_ = 1,
		.init_value_ = 4,
		.mask_ = track_mask_[CHORD_OCT_OFFSET]
	},
};
