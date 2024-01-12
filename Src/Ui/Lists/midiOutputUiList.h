#ifndef MidiOutputUiList_h
#define MidiOutputUiList_h

#include "uiText.h"
#include "settings.h"

class MidiOutputUiList : public UiList {

public:

	enum Item {
		PORT,
		SEND_CLOCK,
		SEND_CLOCK_CONTINUES,

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
		case PORT:					return "PORT";
		case SEND_CLOCK:			return "SEND CLOCK";
		case SEND_CLOCK_CONTINUES:	return "SEND CLOCK CONTINUES";
		default:					return nullptr;
			break;
		}
	}

	const char* value_text(int item) override {
		MidiOutput &output = settings.midiOutput(port_);

		switch (item)
		{
		case PORT:					return MidiPort::port_text(port_);
		case SEND_CLOCK:			return output.send_clock_text();
		case SEND_CLOCK_CONTINUES:	return output.send_clock_continues_text();
		default:					return nullptr;
			break;
		}
	}

	void edit(int item, int inc, bool shifted) override {
		MidiOutput &output = settings.midiOutput(port_);

		switch (item)
		{
		case PORT:
			port_ = stmlib::clip(0, MidiPort::NUM_PORTS - 1, port_ + inc);
			break;
		case SEND_CLOCK:
			output.set_send_clock(inc > 0);
			break;
		case SEND_CLOCK_CONTINUES:
			output.set_send_clock_continues(inc > 0);
			break;
		default:
			break;
		}
	}

private:
	int port_ = 0;
};

#endif
