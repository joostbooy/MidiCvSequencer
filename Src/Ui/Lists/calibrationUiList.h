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
		case PLUS_5V:	return UiText::str.write(settings.calibration.min());
		case MIN_5V:	return UiText::str.write(settings.calibration.max());
		default:
			return nullptr;
		}
	};

	void edit(int item, int inc, bool shifted) override {
		switch (item)
		{
		case PLUS_5V:
			settings.calibration.set_min(settings.calibration.min() + (shifted ? inc * 50 : inc));
			break;
		case MIN_5V:
			settings.calibration.set_max(settings.calibration.max() + (shifted ? inc * 50 : inc));
			break;
		default:
			break;
		}
	}

private:
	//
};

#endif
