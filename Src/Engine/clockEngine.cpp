#include "clockEngine.h"

const uint32_t ClockEngine::step_duration_[NUM_SIGNATURES] = {
	uint32_t(PPQN * 4 * 1.5),	// 1/1.
	uint32_t(PPQN * 4),			// 1/1
	uint32_t(PPQN * 4 / 1.5),	// 1/1T

	uint32_t(PPQN * 2 * 1.5),	// 1/2.
	uint32_t(PPQN * 2),			// 1/2
	uint32_t(PPQN * 2 / 1.5),	// 1/2T

	uint32_t(PPQN * 1 * 1.5),	// 1/4.
	uint32_t(PPQN * 1),			// 1/4
	uint32_t(PPQN * 1 / 1.5),	// 1/4T

	uint32_t(PPQN / 2 * 1.5),	// 1/8.
	uint32_t(PPQN / 2),			// 1/8
	uint32_t(PPQN / 2 / 1.5),	// 1/8T

	uint32_t(PPQN / 4 * 1.5),	// 1/16.
	uint32_t(PPQN / 4),			// 1/16
	uint32_t(PPQN / 4 / 1.5),	// 1/16T

	uint32_t(PPQN / 8 * 1.5),	// 1/32.
	uint32_t(PPQN / 8),			// 1/32
	uint32_t(PPQN / 8 / 1.5),	// 1/32T
};

const float ClockEngine::step_duration_reciprocal_[NUM_SIGNATURES] = {
	1.f / step_duration_[0],	// 1/1.
	1.f / step_duration_[1],	// 1/1
	1.f / step_duration_[2],	// 1/1T

	1.f / step_duration_[3],	// 1/2.
	1.f / step_duration_[4],	// 1/2
	1.f / step_duration_[5],	// 1/2T

	1.f / step_duration_[6],	// 1/4.
	1.f / step_duration_[7],	// 1/4
	1.f / step_duration_[8],	// 1/4T

	1.f / step_duration_[9],	// 1/8.
	1.f / step_duration_[10],	// 1/8
	1.f / step_duration_[11],	// 1/8T

	1.f / step_duration_[12],	// 1/16.
	1.f / step_duration_[13],	// 1/16
	1.f / step_duration_[14],	// 1/16T

	1.f / step_duration_[15],	// 1/32.
	1.f / step_duration_[16],	// 1/32
	1.f / step_duration_[17],	// 1/32T
};
