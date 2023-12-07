#ifndef Chapter_h
#define Chapter_h

#include "pages.h"
#include "bitmap.h"


// Page
class ChapterPage {
public:
	const char* name;
	Pages::Id id;
	Bitmap::Id icon;
};

// Chapter entry
class ChapterEntryBase {
public:
	virtual uint8_t num_pages();
	virtual void open_page(int index);
	virtual uint8_t curr_page();
	virtual const char* page_name(int index);
	virtual Bitmap::Id page_icon(int index);
};

template<const uint8_t size_, ChapterPage* page_>
class ChapterEntry : public ChapterEntryBase {

public:

	uint8_t curr_page() override {
		return curr_page_;
	}

	uint8_t num_pages() override {
		return size_;
	}

	void open_page(int index) override {
		if (inside(index) == false || pages.is_open(page[index].id) == true) {
			return;
		}

		pages.close_all();
		pages.open(page[index].id);
		curr_page_ = index;
	}

	const char* page_name(int index) override {
		index = stmlib::clip(0, size_ - 1, index);
		return page[index].name;
	}

	Bitmap::Id page_icon(int index) override {
		index = stmlib::clip(0, size_ - 1, index);
		return page[index].icon;
	}

private:
	ChapterPage* page = page_;
	uint8_t size = size_;
	int curr_page_ = 0;

	bool inside(int index) {
		return (index >= 0) && (index < size);
	}
};

// Chapters
class Chapter {

public:

	enum Id {
		SONG,
		TRACK,
		PATTERN,
		MIDI,
		CV,
		SYSTEM,

		NUM_CHAPTERS
	};

	void init();
	void open(Id id);

	const char* name() {
		switch (active_)
		{
		case SONG:		return "SONG";
		case TRACK:		return "TRACK";
		case PATTERN:	return "PATTERN";
		case MIDI:		return "MIDI";
		case CV:		return "CV";
		case SYSTEM:	return "SYSTEM";
		default:		return nullptr;
			break;
		}
	}

	Id active() {
		return active_;
	}

	uint8_t num_pages() {
		return chapter_->num_pages();
	}

	uint8_t curr_page() {
		return chapter_->curr_page();
	}

	void open_page(int index) {
		chapter_->open_page(index);
	}

	Bitmap::Id page_icon(int index) {
		return chapter_->page_icon(index);
	}

	const char* page_name(int index) {
		return chapter_->page_name(index);
	}

	const char* page_name() {
		return page_name(curr_page());
	}

private:
	Id active_ = SONG;
	ChapterEntryBase* chapter_ = nullptr;
};

extern Chapter chapter;

#endif
