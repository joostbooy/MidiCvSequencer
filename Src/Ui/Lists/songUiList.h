#ifndef SongUiList_h
#define SongUiList_h

#include "uiText.h"
#include "settings.h"

class SongUiList : public UiList {

public:

	enum Item {
		BPM,
		SYNC_PORT,
		SCALE_ROOT,
		SCALE_TYPE,

		NUM_ITEMS
	};

	const int num_items() override {
		return NUM_ITEMS;
	}

	const char* item_text(int item) override {
		switch (item)
		{
		case BPM:			return "BPM";
		case SYNC_PORT:		return "SYNC_PORT";
		case SCALE_ROOT:	return "SCALE ROOT";
		case SCALE_TYPE:	return "SCALE TYPE";
		default:
			return nullptr;
		}
	};

	const char* value_text(int item) override {
		switch (item)
		{
		case BPM:			return midiClock.bpm_text();
		case SYNC_PORT:		return midiClock.sync_port_text();
		case SCALE_ROOT:	return scale.root_text();
		case SCALE_TYPE:	return scale.type_text();
		default:
			return nullptr;
		}
	};

	void edit(int item, int inc, bool shifted) override {
		switch (item)
		{
		case BPM:
			if (!shifted) {
				midiClock.set_bpm(midiClock.bpm() + inc);
			} else {
				midiClock.set_bpm_fractional(midiClock.bpm_fractional() + inc);
			}
			break;
		case SYNC_PORT:
			if (engine.state() != Engine::STOPPED) {
				engine.add_request_wait(Engine::STOP);
			}
			midiClock.set_sync_port(midiClock.sync_port() + inc);
			break;
		case SCALE_ROOT:
			engine.suspend();
			scale.set_root(scale.root() + inc);
			engine.resume();
			break;
		case SCALE_TYPE:
			engine.suspend();
			scale.set_type(scale.type() + inc);
			engine.resume();
			break;
		default:
			break;
		}
	}

private:
	Scale &scale = settings.song.scale;
	MidiClock &midiClock = settings.song.midiClock;
};

#endif
