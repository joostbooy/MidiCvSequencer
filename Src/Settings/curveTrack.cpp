#include "curveTrack.h"


// step masks
SerialMask CurveTrack::step_mask_[NUM_STEP_ITEMS] = {
	[TRIGGER]		= { .reg = 0, .bits = 1, .shifts = 0 },
	[PROBABILITY]	= { .reg = 0, .bits = 3, .shifts = 1 },
	[CC_VALUE]		= { .reg = 0, .bits = 7, .shifts = 4 },
	[SHAPE]			= { .reg = 0, .bits = 4, .shifts = 11 },
	[SKEW]			= { .reg = 0, .bits = 4, .shifts = 15 },
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
	[CC_VALUE] = {
		.min_ = 0,
		.max_ = 127,
		.inc_shifted_ = 10,
		.init_value_ = 63,
		.mask_ = step_mask_[CC_VALUE]
	},
	[SHAPE] = {
		.min_ = 0,
		.max_ = 9,
		.inc_shifted_ = 2,
		.init_value_ = 4,
		.mask_ = step_mask_[SHAPE]
	},
	[SKEW] = {
		.min_ = 0,
		.max_ = 9,
		.inc_shifted_ = 2,
		.init_value_ = 4,
		.mask_ = step_mask_[SKEW]
	},
};


// track masks
SerialMask CurveTrack::track_mask_[NUM_TRACK_ITEMS] = {
	[CC_NUMBER]		= { .reg = 0, .bits = 7, .shifts = 0 },
	[INIT_CC_VALUE]	= { .reg = 0, .bits = 7, .shifts = 7 },
};

// track items
SerialItem CurveTrack::track_item_[NUM_TRACK_ITEMS] = {
	[CC_NUMBER] = {
		.min_ = 0,
		.max_ = 127,
		.inc_shifted_ = 10,
		.init_value_ = 0,
		.mask_ = track_mask_[CC_NUMBER]
	},
	[INIT_CC_VALUE] = {
		.min_ = 0,
		.max_ = 127,
		.inc_shifted_ = 10,
		.init_value_ = 0,
		.mask_ = track_mask_[INIT_CC_VALUE]
	},
};
