#ifndef CvInputUiList_h
#define CvInputUiList_h

#include "uiText.h"
#include "settings.h"

class CvInputUiList : public UiList {

public:

	enum Item {
		PORT,
		CV_MODE,
		FORWARD_PORT,
		FORWARD_CHANNEL,

		NUM_ITEMS
	};

	const int num_items() override {
		return NUM_ITEMS;
	}

	int selected_port() {
		return port_;
	}

	const char* item_text(int item) override {
		switch (item)
		{
		case PORT:				return "PORT";
		case CV_MODE:			return "CV MODE";
		case FORWARD_PORT:		return "FORWARD PORT";
		case FORWARD_CHANNEL:	return "FORWARD CHANNEL";
		default:
			return nullptr;
		}
	};

	const char* value_text(int item) override {
		CvInput &cvInput = settings.cvInput(port_);

		switch (item)
		{
		case PORT:				return CvInput::port_text(port_);
		case CV_MODE:			return cvInput.cv_mode_text();
		case FORWARD_PORT:		return cvInput.forward_port_text();
		case FORWARD_CHANNEL:	return cvInput.forward_channel_text();
		default:
			return nullptr;
		}
	};

	void edit(int item, int inc, bool shifted) override {
		CvInput &cvInput = settings.cvInput(port_);

		switch (item)
		{
		case PORT:
			port_ = stmlib::clip(0, CvInput::NUM_PORTS - 1, port_ + inc);
			break;
		case CV_MODE:
			cvInput.set_cv_mode(cvInput.cv_mode() + inc);
			break;
		case FORWARD_PORT:
			cvInput.set_forward_port(cvInput.forward_port() + inc);
			break;
		case FORWARD_CHANNEL:
			cvInput.set_forward_channel(cvInput.forward_channel() + inc);
			break;
		default:
			break;
		}
	}

private:
	int port_ = 0;
};

#endif
