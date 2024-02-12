#include "song.h"
#include "fileWriter.h"
#include "fileReader.h"

void Song::clear_list(List* list) {
	list->size_ = 0;
}

void Song::add_to_list(List* list, uint8_t track_index) {
	int index = list->size_;
	while (index > 0 && list->entry_[index - 1] >= track_index) {
		list->entry_[index] = list->entry_[index - 1];
		--index;
	}
	list->entry_[index] = track_index;
	++list->size_;
}

void Song::remove_from_list(List* list, uint8_t track_index) {
	for (int i = 0; i < list->size_; ++i) {
		if (list->entry_[i] == track_index) {
			for (int j = i; j < list->size_; ++j) {
				list->entry_[j] = list->entry_[j + 1];
			}
			--list->size_;
			return;
		}
	}
}

void Song::save(FileWriter &fileWriter) {
	fileWriter.write(mute_flags);
	fileWriter.write(solo_flags);

	scale.save(fileWriter);
	midiClock.save(fileWriter);

	// track lists
	for (int i = 0; i < kMaxTracks; ++i) {
		fileWriter.write(track_list_[i]);
	}

	for (int i = 0; i < Track::NUM_TYPES; ++i) {
		fileWriter.write(track_type_list_[i]);
	}

	// song name
	for (int i = 0; i < max_name_length(); ++i) {
		fileWriter.write(name_[i]);
	}

	// tracks
	for (int i = 0; i < max_tracks(); ++i) {
		track_[i].save(fileWriter);
	}

	// patterns
	fileWriter.write(TrackData::pattern_pool_);

	for (int i = 0; i < TrackData::kMaxPatterns; ++i) {
		fileWriter.write(TrackData::pattern_[i]);
	}

}

void Song::load(FileReader &fileReader) {
	fileReader.read(mute_flags);
	fileReader.read(solo_flags);

	scale.load(fileReader);
	midiClock.load(fileReader);

	// track lists
	for (int i = 0; i < kMaxTracks; ++i) {
		fileReader.read(track_list_[i]);
	}

	for (int i = 0; i < Track::NUM_TYPES; ++i) {
		fileReader.read(track_type_list_[i]);
	}

	// song name
	for (int i = 0; i < max_name_length(); ++i) {
		fileReader.read(name_[i]);
	}

	// tracks
	for (int i = 0; i < max_tracks(); ++i) {
		track_[i].load(fileReader);
	}

	// patterns
	fileReader.read(TrackData::pattern_pool_);

	for (int i = 0; i < TrackData::kMaxPatterns; ++i) {
		fileReader.read(TrackData::pattern_[i]);
	}
}
