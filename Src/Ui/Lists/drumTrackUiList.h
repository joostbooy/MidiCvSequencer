#ifndef DrumTrackUiList_h
#define DrumTrackUiList_h

#include "baseTrackUiList.h"

class DrumTrackUiList : public UiList {

public:

	enum Item {
		DRUM_NOTE_1 = BaseTrackUiList::NUM_ITEMS,
		DRUM_NOTE_2,
		DRUM_NOTE_3,
		DRUM_NOTE_4,

		NUM_ITEMS
	};

	const int num_items() override {
		return NUM_ITEMS;
	}

	const char* item_text(int item) override {
		if (item < BaseTrackUiList::NUM_ITEMS) {
			return baseTrackUiList.item_text(item);
		}

		switch (item)
		{
		case DRUM_NOTE_1:		return "DRUM NOTE 1";
		case DRUM_NOTE_2:		return "DRUM NOTE 2";
		case DRUM_NOTE_3:		return "DRUM NOTE 3";
		case DRUM_NOTE_4:		return "DRUM NOTE 4";
		default:
			return nullptr;
		}
	};

	const char* value_text(int item) override {
		if (item < BaseTrackUiList::NUM_ITEMS) {
			return baseTrackUiList.value_text(item);
		}

		auto &drum_track = settings.selected_drum_track();

		switch (item)
		{
		case DRUM_NOTE_1:	 return UiText::str.write(drum_track.read(DrumTrack::DRUM_NOTE_1));
		case DRUM_NOTE_2:	 return UiText::str.write(drum_track.read(DrumTrack::DRUM_NOTE_2));
		case DRUM_NOTE_3:	 return UiText::str.write(drum_track.read(DrumTrack::DRUM_NOTE_3));
		case DRUM_NOTE_4:	 return UiText::str.write(drum_track.read(DrumTrack::DRUM_NOTE_4));
		default:
			return nullptr;
		}
	};

	void edit(int item, int inc, bool shifted) override {
		if (item < BaseTrackUiList::NUM_ITEMS) {
			baseTrackUiList.edit(item, inc, shifted);
			return;
		}

		auto &drum_track = settings.selected_drum_track();

		switch (item)
		{
		case DRUM_NOTE_1:
			drum_track.edit(DrumTrack::DRUM_NOTE_1, inc, shifted);
			break;
		case DRUM_NOTE_2:
			drum_track.edit(DrumTrack::DRUM_NOTE_2, inc, shifted);
			break;
		case DRUM_NOTE_3:
			drum_track.edit(DrumTrack::DRUM_NOTE_3, inc, shifted);
			break;
		case DRUM_NOTE_4:
			drum_track.edit(DrumTrack::DRUM_NOTE_4, inc, shifted);
			break;
		default:
			break;
		}
	}

private:
	BaseTrackUiList baseTrackUiList;
};

#endif
