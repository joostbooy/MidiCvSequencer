#ifndef DrumTrackUiList_h
#define DrumTrackUiList_h

#include "baseTrackUiList.h"

class DrumTrackUiList : public UiList {

public:

	enum Item {
		DRUM_NOTE = BaseTrackUiList::NUM_ITEMS,
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
		case DRUM_NOTE:		return "DRUM NOTE";
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
		case DRUM_NOTE:	 return UiText::str.write(drum_track.read(DrumTrack::DRUM_NOTE));
		default:
			return nullptr;
		}
	};

	void edit(int item, int inc, bool shifted) override {
		auto &drum_track = settings.selected_drum_track();

		if (item < BaseTrackUiList::NUM_ITEMS) {
			baseTrackUiList.edit(item, inc, shifted);

			if (item == BaseTrackUiList::CLOCK_SPEED) {
				settings.drumKit.update_clock_speed(drum_track.clock_speed());
			}
			return;
		}

		switch (item)
		{
		case DRUM_NOTE:
			drum_track.edit(DrumTrack::DRUM_NOTE, inc, shifted);
			break;
		default:
			break;
		}
	}

private:
	BaseTrackUiList baseTrackUiList;
};

#endif
