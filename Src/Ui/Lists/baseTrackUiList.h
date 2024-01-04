#ifndef BaseTrackUiList_h
#define BaseTrackUiList_h

#include "uiText.h"
#include "settings.h"

class BaseTrackUiList : public UiList {

public:

	enum Item {
		TRACK,	// keep this the first item
		PORT,
		CHANNEL,
		CLOCK_MODE,
		CLOCK_SPEED,
		DELAY,
		SWING,
		HUMANISE,
		MUTE,
		SOLO,

		NUM_ITEMS
	};

	const int num_items() override {
		return NUM_ITEMS;
	}

	const char* item_text(int item) override {
		switch (item)
		{
		case TRACK:			return "TRACK";
		case PORT:			return "PORT";
		case CHANNEL:		return "CHANNEL";
		case CLOCK_MODE:	return "CLOCK MODE";
		case CLOCK_SPEED:	return "CLOCK SPEED";
		case DELAY:			return "DELAY";
		case SWING:			return "SWING";
		case HUMANISE:		return "HUMANISE";
		case MUTE:			return "MUTE";
		case SOLO:			return "SOLO";
		default:			return nullptr;
			break;
		}
	}

	const char* value_text(int item) override {
		auto &track = settings.selected_track();

		switch (item)
		{
		case TRACK:			return track.name();
		case PORT:			return track.port_text();
		case CHANNEL:		return track.channel_text();
		case CLOCK_MODE:	return track.clock_mode_text();
		case CLOCK_SPEED:	return track.clock_speed_text();
		case DELAY:			return track.delay_text();
		case SWING:			return track.swing_text();
		case HUMANISE:		return track.humanise_text();
		case MUTE:			return settings.song.mute_text(track.index());
		case SOLO:			return settings.song.solo_text(track.index());
		default:			return nullptr;
			break;
		}
	};

	void edit(int item, int inc, bool shifted) override {
		auto &track = settings.selected_track();

		switch (item)
		{
		case TRACK:
			settings.select_track_index(track.index() + inc);
			break;
		case PORT:
			track.set_port(track.port() + inc);
			break;
		case CHANNEL:
			track.set_channel(track.channel() + inc);
			break;
		case CLOCK_MODE:
			track.set_clock_mode(track.clock_mode() + inc);
			break;
		case CLOCK_SPEED:
			track.set_clock_speed(track.clock_speed() + (shifted ? inc * 3 : inc));
			break;
		case DELAY:
			track.set_delay(track.delay() + inc);
			break;
		case SWING:
			track.set_swing(track.swing() + inc);
			break;
		case HUMANISE:
			track.set_humanise(track.humanise() + inc);
			break;
		case MUTE:
			settings.song.set_track_mute(track.index(), inc > 0);
			break;
		case SOLO:
			settings.song.set_track_solo(track.index(), inc > 0);
			break;
		default:
			break;
		}
	}

private:

};

#endif
