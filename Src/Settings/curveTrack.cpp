#include "curveTrack.h"


// step masks
SerialMask CurveTrack::step_mask_[NUM_STEP_ITEMS] = {
	[TRIGGER]		= { .reg = 0, .bits = 1, .shifts = 0 },
	[PROBABILITY]	= { .reg = 0, .bits = 3, .shifts = 1 },
	[VALUE]			= { .reg = 0, .bits = 7, .shifts = 4 },
	[SHAPE]			= { .reg = 0, .bits = 4, .shifts = 11 },
	[SHIFT]			= { .reg = 0, .bits = 3, .shifts = 15 },
};

// step items
SerialItem CurveTrack::step_item_[NUM_STEP_ITEMS] = {
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
		.inc_shifted_ = 1,
		.init_value_ = 7,
		.mask_ = step_mask_[PROBABILITY]
	},
	[VALUE] = {
		.min_ = 0,
		.max_ = 127,
		.inc_shifted_ = 10,
		.init_value_ = 64,
		.mask_ = step_mask_[VALUE]
	},
	[SHAPE] = {
		.min_ = 0,
		.max_ = 9,
		.inc_shifted_ = 2,
		.init_value_ = 4,
		.mask_ = step_mask_[SHAPE]
	},
	[SHIFT] = {
		.min_ = 0,
		.max_ = 4,
		.inc_shifted_ = 1,
		.init_value_ = 4,
		.mask_ = step_mask_[SHIFT]
	},
};

// track masks
SerialMask CurveTrack::track_mask_[NUM_TRACK_ITEMS] = {
	[INIT_VALUE]	= { .reg = 0, .bits = 7, .shifts = 0 },
	[CC_NUMBER]		= { .reg = 0, .bits = 7, .shifts = 7 },
	[SEND_CC]		= { .reg = 0, .bits = 1, .shifts = 14 },
	[SEND_BEND]		= { .reg = 0, .bits = 1, .shifts = 15 },
};

// track items
SerialItem CurveTrack::track_item_[NUM_TRACK_ITEMS] = {
	[INIT_VALUE] = {
		.min_ = 0,
		.max_ = 127,
		.inc_shifted_ = 10,
		.init_value_ = 64,
		.mask_ = track_mask_[INIT_VALUE]
	},
	[CC_NUMBER] = {
		.min_ = 0,
		.max_ = 127,
		.inc_shifted_ = 10,
		.init_value_ = 0,
		.mask_ = track_mask_[CC_NUMBER]
	},
	[SEND_CC] = {
		.min_ = 0,
		.max_ = 1,
		.inc_shifted_ = 0,
		.init_value_ = 1,
		.mask_ = track_mask_[SEND_CC]
	},
	[SEND_BEND] = {
		.min_ = 0,
		.max_ = 1,
		.inc_shifted_ = 0,
		.init_value_ = 1,
		.mask_ = track_mask_[SEND_BEND]
	},
};
