#include "chapter.h"

#define CHAPTER_SIZE(chapter_pages) (sizeof(chapter_pages) / sizeof(ChapterPage))

ChapterPage song_pages[] = {
	{ .name = "SETTINGS", 	.id = Pages::SONG_PAGE,			.icon = Bitmap::SONG_PAGE },
	{ .name = "MONITOR", 	.id = Pages::SONG_MONITOR_PAGE,	.icon = Bitmap::SONG_MONITOR_PAGE },
};

ChapterPage track_pages[] = {
	{ .name = "SETTINGS", 	.id = Pages::TRACK_PAGE,		.icon = Bitmap::SETTINGS_PAGE },
};

ChapterPage pattern_pages[] = {
	{ .name = "PATTERN", 	.id = Pages::PATTERN_PAGE,		.icon = Bitmap::PATTERN_PAGE },
};

ChapterPage midi_pages[] = {
	{ .name = "INPUT", 		.id = Pages::MIDI_INPUT_PAGE,	.icon = Bitmap::MIDI_IN_PAGE },
	{ .name = "OUTPUT", 	.id = Pages::MIDI_OUTPUT_PAGE,	.icon = Bitmap::MIDI_OUT_PAGE },
};

ChapterPage cv_pages[] = {
	{ .name = "INPUT", 		.id = Pages::CV_INPUT_PAGE,		.icon = Bitmap::CV_IN_PAGE },
	{ .name = "OUTPUT", 	.id = Pages::CV_OUTPUT_PAGE,	.icon = Bitmap::CV_OUT_PAGE },
};

ChapterPage system_pages[] = {
	{ .name = "SAVE", 		.id = Pages::SAVE_PAGE,			.icon = Bitmap::PROJECT_SAVE_PAGE },
	{ .name = "LOAD", 		.id = Pages::LOAD_PAGE,			.icon = Bitmap::PROJECT_LOAD_PAGE },
	{ .name = "SETTINGS",	.id = Pages::SYSTEM_PAGE,		.icon = Bitmap::SETTINGS_PAGE },
};

ChapterEntry<CHAPTER_SIZE(song_pages), song_pages> song_chapter;
ChapterEntry<CHAPTER_SIZE(track_pages), track_pages> track_chapter;
ChapterEntry<CHAPTER_SIZE(pattern_pages), pattern_pages> pattern_chapter;
ChapterEntry<CHAPTER_SIZE(midi_pages), midi_pages> midi_chapter;
ChapterEntry<CHAPTER_SIZE(cv_pages), cv_pages> cv_chapter;
ChapterEntry<CHAPTER_SIZE(system_pages), system_pages> system_chapter;

void Chapter::init() {
	active_ = SONG;
	chapter_ = &song_chapter;
	open_page(0);
}

void Chapter::open(Id id) {

	Id last_active = active_;

	switch (id)
	{
	case SONG:
		chapter_ = &song_chapter;
		active_ = SONG;
		break;
	case TRACK:
		chapter_ = &track_chapter;
		active_ = TRACK;
		break;
	case PATTERN:
		chapter_ = &pattern_chapter;
		active_ = PATTERN;
		break;
	case MIDI:
		chapter_ = &midi_chapter;
		active_ = MIDI;
		break;
	case CV:
		chapter_ = &cv_chapter;
		active_ = CV;
		break;
	case SYSTEM:
		chapter_ = &system_chapter;
		active_ = SYSTEM;
		break;
	default:
		break;
	}

	if (active_ != last_active) {
		pages.close_all();
		open_page(chapter_->curr_page());
	}
}

Chapter chapter;
