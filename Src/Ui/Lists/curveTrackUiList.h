#ifndef CurveTrackUiList_h
#define CurveTrackUiList_h

#include "baseTrackUiList.h"

class CurveTrackUiList : public UiList {

public:

	enum Item {
		INIT_VALUE = BaseTrackUiList::NUM_ITEMS,
		CC_NUMBER,
		SEND_CC,
		SEND_BEND,

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
		case INIT_VALUE:	return "INITIAL VALUE";
		case CC_NUMBER:		return "CC NUMBER";
		case SEND_CC:		return "SEND CC";
		case SEND_BEND:		return "SEND BEND";
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
		case INIT_VALUE:	return UiText::str.write(curve_track.read(CurveTrack::INIT_VALUE));
		case CC_NUMBER:		return UiText::str.write(curve_track.read(CurveTrack::CC_NUMBER));
		case SEND_CC:		return UiText::bool_to_on_off(curve_track.read(CurveTrack::SEND_CC));
		case SEND_BEND:		return UiText::bool_to_on_off(curve_track.read(CurveTrack::SEND_BEND));
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
		case INIT_VALUE:
			curve_track.edit(CurveTrack::INIT_VALUE, inc, shifted);
			break;
		case CC_NUMBER:
			curve_track.edit(CurveTrack::CC_NUMBER, inc, shifted);
			break;
		case SEND_CC:
			curve_track.write(CurveTrack::SEND_CC, inc > 0);
			break;
		case SEND_BEND:
			curve_track.write(CurveTrack::SEND_BEND, inc > 0);
			break;
		default:
			break;
		}
	}

private:
	BaseTrackUiList baseTrackUiList;
};

#endif
