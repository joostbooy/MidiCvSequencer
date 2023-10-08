#ifndef CalibrationUiList_h
#define CalibrationUiList_h

#include "uiText.h"
#include "settings.h"

class CalibrationUiList : public UiList {

public:

	enum Item {
		PLUS_5V,
		MIN_5V,

		NUM_ITEMS
	};

	const int num_items() override {
		return NUM_ITEMS;
	}


	const char* item_text(int item) override {
		switch (item)
		{
		case PLUS_5V:	return "+5V";
		case MIN_5V:	return "-5V";
		default:
			return nullptr;
		}
	};

	const char* value_text(int item) override {
		switch (item)
		{
		case PLUS_5V:	return UiText::str.write(settings.voltPerOctave.min());
		case MIN_5V:	return UiText::str.write(settings.voltPerOctave.max());
		default:
			return nullptr;
		}
	};

	void edit(int item, int inc, bool shifted) override {
		switch (item)
		{
		case PLUS_5V:
			settings.voltPerOctave.set_min(settings.voltPerOctave.min() + (shifted ? inc * 50 : inc));
			break;
		case MIN_5V:
			settings.voltPerOctave.set_max(settings.voltPerOctave.max() + (shifted ? inc * 50 : inc));
			break;
		default:
			break;
		}
	}

private:
	//
};

#endif
