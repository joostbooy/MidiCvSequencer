#ifndef NoteTrackUiList_h
#define NoteTrackUiList_h

#include "baseTrackUiList.h"

class NoteTrackUiList : public UiList {

public:

	enum Item {
		USE_SCALE = BaseTrackUiList::NUM_ITEMS,

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
		case USE_SCALE:		return "USE SCALE";
		default:
			return nullptr;
		}
	};

	const char* value_text(int item) override {
		if (item < BaseTrackUiList::NUM_ITEMS) {
			return baseTrackUiList.value_text(item);
		}

		auto &note_track = settings.selected_note_track();

		switch (item)
		{
		case USE_SCALE:	 return note_track.read(NoteTrack::USE_SCALE) ? "ON" : "OFF";
		default:
			return nullptr;
		}
	};

	void edit(int item, int inc, bool shifted) override {
		if (item < BaseTrackUiList::NUM_ITEMS) {
			return baseTrackUiList.edit(item, inc, shifted);
		}

		auto &note_track = settings.selected_note_track();

		switch (item)
		{
		case USE_SCALE:
			note_track.write(NoteTrack::USE_SCALE, inc > 0);
			break;
		default:
			break;
		}
	}

private:
	BaseTrackUiList baseTrackUiList;
};

#endif
