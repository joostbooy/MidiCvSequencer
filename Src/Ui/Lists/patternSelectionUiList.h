#ifndef PatternSelectionUiList_h
#define PatternSelectionUiList_h

#include "uiText.h"
#include "settings.h"


class PatternSelectionUiList : public UiList {

public:

	enum Item {
		TRACK,
		PATTERN,
		STEP_ITEM,
		FOLLOW_PATTERN,

		NUM_ITEMS
	};

	const int num_items() override {
		return NUM_ITEMS;
	}

	const char* item_text(int item) override {
		switch (item)
		{
		case TRACK:				return "";
		case PATTERN:			return "PAT";
		case STEP_ITEM:			return "";
		case FOLLOW_PATTERN:	return "FOLLOW";
		default:
			return nullptr;
		}
	};

	const char* value_text(int item) override {
		Track &track = settings.selected_track();

		switch (item)
		{
		case TRACK:				return track.name();
		case PATTERN:			return track.pattern.label(settings.selected_pattern());
		case STEP_ITEM:			return step_item_text(track.type(), settings.selected_step_item());
		case FOLLOW_PATTERN:	return UiText::bool_to_on_off(settings.follow_pattern());
		default:
			return nullptr;
		}
	};

	void edit(int item, int inc, bool shifted = false) override {
		switch (item)
		{
		case TRACK:
			settings.select_track_index(settings.selected_track_index() + inc);
			break;
		case PATTERN:
			settings.select_pattern(settings.selected_pattern() + inc);
			break;
		case STEP_ITEM:
			settings.select_step_item(settings.selected_step_item() + inc);
			break;
		case FOLLOW_PATTERN:
			settings.set_follow_pattern(inc > 0);
			break;
		default:
			break;
		}
	}

private:

	const char *step_item_text(int track_type, int step_item) {
		switch (track_type)
		{
		case BaseTrack::NOTE_TRACK:		return NoteTrack::step_item_text(NoteTrack::StepItem(step_item));
		case BaseTrack::CHORD_TRACK:	return ChordTrack::step_item_text(ChordTrack::StepItem(step_item));
		case BaseTrack::DRUM_TRACK:		return DrumTrack::step_item_text(DrumTrack::StepItem(step_item));
		case BaseTrack::CURVE_TRACK:	return CurveTrack::step_item_text(CurveTrack::StepItem(step_item));
		default:
			break;
		}
		return nullptr;
	}
};

#endif
