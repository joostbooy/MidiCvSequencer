#include "stepEditor.h"

int StepEditor::read(int step, int item) {
	Track &track = settings.selected_track();
	int pattern = settings.selected_pattern();

	switch (track.type())
	{
	case Track::NOTE_TRACK:
		return track.note.read_step(pattern, step, NoteTrack::StepItem(item));
	case Track::CHORD_TRACK:
		return track.chord.read_step(pattern, step, ChordTrack::StepItem(item));
	case Track::DRUM_TRACK:
		return track.drum.read_step(pattern, step, DrumTrack::StepItem(item));
	case Track::CURVE_TRACK:
		return track.curve.read_step(pattern, step, CurveTrack::StepItem(item));
	default:
		break;
	}
	return 0;
}

int StepEditor::read(int step) {
	return read(step, settings.selected_step_item());
}

void StepEditor::write(int step, int value) {
	Track &track = settings.selected_track();
	int pattern = settings.selected_pattern();
	int item = settings.selected_step_item();

	switch (track.type())
	{
	case Track::NOTE_TRACK:
		track.note.write_step(pattern, step, NoteTrack::StepItem(item), value);
		break;
	case Track::CHORD_TRACK:
		track.chord.write_step(pattern, step, ChordTrack::StepItem(item), value);
		break;
	case Track::DRUM_TRACK:
		track.drum.write_step(pattern, step, DrumTrack::StepItem(item), value);
		break;
	case Track::CURVE_TRACK:
		track.curve.write_step(pattern, step, CurveTrack::StepItem(item), value);
		break;
	default:
		break;
	}
}

void StepEditor::edit(int step, int inc, bool pressed) {
	Track &track = settings.selected_track();
	int item = settings.selected_step_item();
	int pattern = settings.selected_pattern();

	switch (track.type())
	{
	case Track::NOTE_TRACK:
		track.note.edit_step(pattern, step, NoteTrack::StepItem(item), inc, pressed);
		break;
	case Track::CHORD_TRACK:
		track.chord.edit_step(pattern, step, ChordTrack::StepItem(item), inc, pressed);
		break;
	case Track::DRUM_TRACK:
		track.drum.edit_step(pattern, step, DrumTrack::StepItem(item), inc, pressed);
		break;
	case Track::CURVE_TRACK:
		track.curve.edit_step(pattern, step, CurveTrack::StepItem(item), inc, pressed);
		break;
	default:
		break;
	}
}

void StepEditor::toggle(int step) {
	bool state;
	Track &track = settings.selected_track();
	int pattern = settings.selected_pattern();

	switch (track.type())
	{
	case Track::NOTE_TRACK:
		state = track.note.read_step(pattern, step, NoteTrack::TRIGGER);
		track.note.write_step(pattern, step, NoteTrack::TRIGGER, !state);
		break;
	case Track::CHORD_TRACK:
		state = track.chord.read_step(pattern, step, ChordTrack::TRIGGER);
		track.chord.write_step(pattern, step, ChordTrack::TRIGGER, !state);
		break;
	case Track::DRUM_TRACK:
		state = track.drum.read_step(pattern, step, DrumTrack::TRIGGER);
		track.drum.write_step(pattern, step, DrumTrack::TRIGGER, !state);
		break;
	case Track::CURVE_TRACK:
		state = track.curve.read_step(pattern, step, CurveTrack::TRIGGER);
		track.curve.write_step(pattern, step, CurveTrack::TRIGGER, !state);
		break;
	default:
		break;
	}
}
