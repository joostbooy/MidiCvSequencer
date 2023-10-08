#ifndef NotePatternUiList_h
#define NotePatternUiList_h

#include "uiText.h"
//#include "settings.h"

class NotePatternUiList : public UiList {

public:

	const int num_items() override {
		return NoteTrack::NUM_STEP_ITEMS;
	}

	void set_track(int track) {
		track_ = track;
	}

	void set_pattern(int pattern) {
		pattern_ = pattern;
	}

	void set_step(int step) {
		step_ = step;
	}

	void set_item(int item) {
		item_ = item;
	}

	const char* item_text(int item) override {
		switch (item)
		{
		case NoteTrack::TRIGGER:		return "TRIGGER";
		case NoteTrack::NOTE:			return "NOTE";
		case NoteTrack::VELOCITY:		return "VELOCITY";
		case NoteTrack::GATE_LENGTH:	return "GATE_LENGTH";
		case NoteTrack::REPEAT:			return "REPEAT";
		case NoteTrack::DELAY:			return "DELAY";
		case NoteTrack::PROBABILITY:	return "PROBABILITY";
		default:
			break;
		}
		return nullptr;
	}

	const char* value_text(int item) override {
		NoteTrack &track = settings.song.track(track_).note;
		int data = track.read_step(pattern_, step_, item);

		switch (item)
		{
		case NoteTrack::TRIGGER:		return UiText::bool_to_on_off(data);
		case NoteTrack::NOTE:			return Scale::note_text(data);
		case NoteTrack::VELOCITY:		return UiText::unsigned_int_to_text(data);
		case NoteTrack::GATE_LENGTH:	return SequencerClock::gate_duration_text(data);
		case NoteTrack::REPEAT:			return SequencerRepeat::repeat_text(data);
		case NoteTrack::DELAY:			return SequencerClock::gate_duration_text(data);
		case NoteTrack::PROBABILITY:	return UiText::percentage_text(data, 9);
		default:
			break;
		}
		return nullptr;
	}


	void edit(int item, int inc, bool shifted) override {
		NoteTrack &track = settings.song.track(track_).note;
		int inc_ = track.step_item(item).inc(inc > 0, shifted);
		int data = track.read_step(pattern_, step_, item_);

		switch (item)
		{
		case NoteTrack::TRIGGER:
			track.write_step(pattern_, step_, NoteTrack::TRIGGER, data + inc_);
			break;
		case NoteTrack::NOTE:
			track.write_step(pattern_, step_, NoteTrack::NOTE, data + inc_);
			break;
		case NoteTrack::VELOCITY:
			track.write_step(pattern_, step_, NoteTrack::VELOCITY, data + inc_);
			break;
		case NoteTrack::GATE_LENGTH:
			track.write_step(pattern_, step_, NoteTrack::GATE_LENGTH, data + inc_);
			break;
		case NoteTrack::REPEAT:
			track.write_step(pattern_, step_, NoteTrack::REPEAT, data + inc_);
			break;
		case NoteTrack::DELAY:
			track.write_step(pattern_, step_, NoteTrack::DELAY, data + inc_);
			break;
		case NoteTrack::PROBABILITY:
			track.write_step(pattern_, step_, NoteTrack::PROBABILITY, data + inc_);
			break;
		default:
			break;
		}
		return nullptr;
	}

private:
	int track_;
	int pattern_;
	int step_;
	NoteTrack::StepItem item_;
};

#endif
