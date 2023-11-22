#ifndef TrackData_h
#define TrackData_h

#include "stmf4lib.h"

class TrackData {

public:

	static const uint8_t kMaxPatterns = 255;
	static const uint8_t kMaxLabelLength = 8;
	static const uint8_t kMaxStepsPerPattern = 16;
	static const uint8_t kMaxStepItems = 16;

	uint8_t type_;
	uint8_t port_;
	uint8_t channel_;
	uint8_t clock_mode_;
	uint8_t clock_speed_;
	uint8_t delay_;
	uint8_t swing_;
	uint8_t humanise_;
	uint32_t serial_[2];
	uint16_t grouped_steps_;
	char label_[kMaxLabelLength];

	struct Step {
		uint32_t serial_[2];
	};

	struct Pattern {
		char label_[kMaxLabelLength];
		Step step_[kMaxStepsPerPattern];
		Step random_min_;
		Step random_max_;
		uint16_t random_mask_[kMaxStepItems];
	};

	struct List {
		uint8_t entry_[kMaxPatterns];
		uint8_t size_;
	};

	List pattern_list_;

	// all tracks use the same pattern memory space
	static List pattern_pool_;
	static Pattern pattern_[kMaxPatterns];
	static void init();
};

#endif
