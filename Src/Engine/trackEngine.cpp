#include "trackEngine.h"

void TrackEngine::tick(uint8_t index) {
	switch (settings.song.track(index).type())
	{
	case Track::NOTE_TRACK:
		note_track[index].tick();
		break;
	case Track::CHORD_TRACK:
		chord_track[index].tick();
		break;
	case Track::DRUM_TRACK:
		drum_track[index].tick();
		break;
	case Track::CURVE_TRACK:
		curve_track[index].tick();
		break;
	default:
		break;
	}
}

void TrackEngine::reset(uint8_t index) {
	switch (settings.song.track(index).type())
	{
	case Track::NOTE_TRACK:
		note_track[index].reset();
		break;
	case Track::CHORD_TRACK:
		chord_track[index].reset();
		break;
	case Track::DRUM_TRACK:
		drum_track[index].reset();
		break;
	case Track::CURVE_TRACK:
		curve_track[index].reset();
		break;
	default:
		break;
	}
}

void TrackEngine::tick_step(uint8_t index) {
	switch (settings.song.track(index).type())
	{
	case Track::NOTE_TRACK:
		note_track[index].tick_step();
		break;
	case Track::CHORD_TRACK:
		chord_track[index].tick_step();
		break;
	case Track::DRUM_TRACK:
		drum_track[index].tick_step();
		break;
	case Track::CURVE_TRACK:
		curve_track[index].tick_step();
		break;
	default:
		break;
	}
}

void TrackEngine::send_step(uint8_t index) {
	switch (settings.song.track(index).type())
	{
	case Track::NOTE_TRACK:
		note_track[index].send_step();
		break;
	case Track::CHORD_TRACK:
		chord_track[index].send_step();
		break;
	case Track::DRUM_TRACK:
		drum_track[index].send_step();
		break;
	case Track::CURVE_TRACK:
		curve_track[index].send_step();
		break;
	default:
		break;
	}
}

void TrackEngine::process_step(uint8_t index, uint8_t pattern, uint8_t step) {
	switch (settings.song.track(index).type())
	{
	case Track::NOTE_TRACK:
		note_track[index].process_step(pattern, step);
		break;
	case Track::CHORD_TRACK:
		chord_track[index].process_step(pattern, step);
		break;
	case Track::DRUM_TRACK:
		drum_track[index].process_step(pattern, step);
		break;
	case Track::CURVE_TRACK:
		curve_track[index].process_step(pattern, step);
		break;
	default:
		break;
	}
}
