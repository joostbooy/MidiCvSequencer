#ifndef CurveTrackUiList_h
#define CurveTrackUiList_h

#include "baseTrackUiList.h"

class CurveTrackUiList : public UiList {

public:

	enum Item {
		CC_NUMBER = BaseTrackUiList::NUM_ITEMS,
		INIT_CC_VALUE,

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
		case CC_NUMBER:		return "CC NUMBER";
		case INIT_CC_VALUE:	return "INITIAL CC VALUE";
		default:
			return nullptr;
		}
	};

	const char* value_text(int item) override {
		if (item < BaseTrackUiList::NUM_ITEMS) {
			return baseTrackUiList.value_text(item);
		}

		auto &curve_track = settings.selected_curve_track();

		switch (item)
		{
		case CC_NUMBER:		return UiText::str.write(curve_track.read(CurveTrack::CC_NUMBER));
		case INIT_CC_VALUE:	return UiText::str.write(curve_track.read(CurveTrack::INIT_CC_VALUE));
		default:
			return nullptr;
		}
	};

	void edit(int item, int inc, bool shifted) override {
		if (item < BaseTrackUiList::NUM_ITEMS) {
			baseTrackUiList.edit(item, inc, shifted);
			return;
		}

		auto &curve_track = settings.selected_curve_track();

		switch (item)
		{
		case CC_NUMBER:
			curve_track.edit(CurveTrack::CC_NUMBER, inc, shifted);
			break;
		case INIT_CC_VALUE:
			curve_track.edit(CurveTrack::INIT_CC_VALUE, inc, shifted);
			break;
		default:
			break;
		}
	}

private:
	BaseTrackUiList baseTrackUiList;
};

#endif
