#ifndef ChordTrackUiList_h
#define ChordTrackUiList_h

#include "baseTrackUiList.h"

class ChordTrackUiList : public UiList {

public:

	enum Item {
		CHORD_OFFSET = BaseTrackUiList::NUM_ITEMS,

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
		case CHORD_OFFSET:		return "CHORD OFFSET";
		default:
			return nullptr;
		}
	};

	const char* value_text(int item) override {
		if (item < BaseTrackUiList::NUM_ITEMS) {
			return baseTrackUiList.value_text(item);
		}

		auto &chord_track = settings.selected_chord_track();

		switch (item)
		{
		case CHORD_OFFSET:	 return UiText::str.write(chord_track.read(ChordTrack::CHORD_OCT_OFFSET));
		default:
			return nullptr;
		}
	};

	void edit(int item, int inc, bool shifted) override {
		if (item < BaseTrackUiList::NUM_ITEMS) {
			baseTrackUiList.edit(item, inc, shifted);
			return;
		}

		auto &chord_track = settings.selected_chord_track();

		switch (item)
		{
		case CHORD_OFFSET:
			chord_track.edit(ChordTrack::CHORD_OCT_OFFSET, inc, shifted);
			break;
		default:
			break;
		}
	}

private:
	BaseTrackUiList baseTrackUiList;
};

#endif
