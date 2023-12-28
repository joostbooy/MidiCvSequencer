#ifndef Track_h
#define Track_h

#include "stmf4lib.h"
#include "trackData.h"
#include "noteTrack.h"
#include "chordTrack.h"
#include "drumTrack.h"
#include "curveTrack.h"

class Track : public BaseTrack {

public:

	NoteTrack note;
	ChordTrack chord;
	DrumTrack drum;
	CurveTrack curve;

	void init() {
		note.init(&data);
		chord.init(&data);
		drum.init(&data);
		curve.init(&data);

		BaseTrack::init(&data);
	}

	void set_index(int index) {
		index_ = index;
	}

	int index() {
		return index_;
	}

	const char* name() {
		UiText::str.write("T", index_ + 1, "-", BaseTrack::type_text());
		if (*BaseTrack::label() != '\0') {
			UiText::str.append("-");
			UiText::str.append(BaseTrack::label());
		}
		return UiText::str.read();
	}

	const char* name_small() {
		UiText::str.write("T", index_ + 1, "-", BaseTrack::label());
		UiText::str.truncate(6);
		return UiText::str.read();
	}

	// Only call through song.create_track() or drumKit.create_track()
	void create(uint8_t track_type) {
		BaseTrack::set_type(track_type);

		switch (track_type)
		{
		case BaseTrack::NOTE_TRACK:
			note.clear();
			break;
		case BaseTrack::CHORD_TRACK:
			chord.clear();
			break;
		case BaseTrack::CURVE_TRACK:
			curve.clear();
			break;
		case BaseTrack::DRUM_TRACK:
			drum.clear();
			break;
		default:
			break;
		}
	}

	int num_step_items() {
		switch (BaseTrack::type())
		{
		case BaseTrack::NOTE_TRACK:		return NoteTrack::NUM_STEP_ITEMS;
		case BaseTrack::CHORD_TRACK:	return ChordTrack::NUM_STEP_ITEMS;
		case BaseTrack::DRUM_TRACK:		return DrumTrack::NUM_STEP_ITEMS;
		case BaseTrack::CURVE_TRACK:	return CurveTrack::NUM_STEP_ITEMS;
		default:
			return 0;
		}
	}

	int num_track_items() {
		switch (BaseTrack::type())
		{
		case BaseTrack::NOTE_TRACK:		return NoteTrack::NUM_TRACK_ITEMS;
		case BaseTrack::CHORD_TRACK:	return ChordTrack::NUM_TRACK_ITEMS;
		case BaseTrack::DRUM_TRACK:		return DrumTrack::NUM_TRACK_ITEMS;
		case BaseTrack::CURVE_TRACK:	return CurveTrack::NUM_TRACK_ITEMS;
		default:
			return 0;
		}
	}

	void save(FileWriter&);
	void load(FileReader&);

private:
	int index_;
	TrackData data;				// All track types use the same memory space
};

#endif
