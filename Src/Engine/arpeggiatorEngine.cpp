#include "arpeggiatorEngine.h"

Reciprocal<ArpeggiatorEngine::kMaxSize>ArpeggiatorEngine::reciprocal;

void ArpeggiatorEngine::reset(Clock *clock, uint8_t mode) {
	uint8_t offset_ = arpeggiator_->offset();

	switch (mode)
	{
	case Arpeggiator::FORWARD:
	case Arpeggiator::ALTERNATE:
	case Arpeggiator::PINGPONG:
		clock->phase = 0;
		clock->next = offset_;
		break;
	case Arpeggiator::ZIGZAG:
		clock->phase = offset_;
		clock->next = offset_ / 2;
		if (offset_ & 1) {
			clock->next = size_ - 1 - clock->next;
		}
		break;
	case Arpeggiator::REVERSE:
	case Arpeggiator::ALTERNATE_REVERSE:
	case Arpeggiator::PINGPONG_REVERSE:
		clock->phase = 1;
		clock->next = (size_ - 1) - offset_;
		break;
	case Arpeggiator::ZIGZAG_REVERSE:
		clock->phase = offset_;
		clock->next = offset_ / 2;
		if ((offset_ & 1) == 0) {
			clock->next = size_ - 1 - clock->next;
		}
		break;
	default:
		break;
	}
}

void ArpeggiatorEngine::tick(Clock *clock, uint8_t mode) {

	clock->curr = clock->next;

	if (clock->curr < 0 || size_ < 2) {
		clock->curr = 0;
	} else if (clock->curr >= size_) {
		clock->curr = size_ - 1;
	}

	switch (mode)
	{
	case Arpeggiator::FORWARD:
		++clock->next;
		if (clock->next >= size_) {
			clock->next = 0;
		}
		break;
	case Arpeggiator::REVERSE:
		--clock->next;
		if (clock->next < 0) {
			clock->next = size_ - 1;
		}
		break;
	case Arpeggiator::ALTERNATE:
	case Arpeggiator::ALTERNATE_REVERSE:
		if (clock->phase == 0) {
			++clock->next;
			if (clock->next >= size_) {
				clock->phase = 1;
				clock->next = size_ - 1;
			}
		} else {
			--clock->next;
			if (clock->next < 0) {
				clock->phase = 0;
				clock->next = 0;
			}
		}
		break;
	case Arpeggiator::PINGPONG:
	case Arpeggiator::PINGPONG_REVERSE:
		if (clock->phase == 0) {
			++clock->next;
			if (clock->next >= size_) {
				clock->phase = 1;
				clock->next = size_ - 2;
			}
		} else {
			--clock->next;
			if (clock->next < 0) {
				clock->phase = 0;
				clock->next = 1;
			}
		}
		break;
	case Arpeggiator::ZIGZAG:
		++clock->phase;
		if (clock->phase >= size_) {
			clock->phase = 0;
		}

		clock->next = clock->phase / 2;
		if (clock->phase & 1) {
			clock->next = size_ - 1 - clock->next;
		}
		break;
	case Arpeggiator::ZIGZAG_REVERSE:
		++clock->phase;
		if (clock->phase >= size_) {
			clock->phase = 0;
		}

		clock->next = clock->phase / 2;
		if ((clock->phase & 1) == 0) {
			clock->next = size_ - 1 - clock->next;
		}
		break;
	default:
		clock->next = 0;
		break;
	}
}
