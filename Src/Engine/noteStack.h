#ifndef NoteStack_h
#define NoteStack_h

class NoteStack {

public:

	void init() {
		size_ = 0;
	}

	uint8_t size() {
		return size_;
	}

	bool readable() {
		return size_ > 0;
	}

	MidiEvent::Event& read(uint8_t slot) {
		return entry[slot].event;
	}

	bool pull(MidiEvent::Event &e) {
		if (size_) {
			e = entry[--size_].event;
			return true;
		}
		return false;
	}

	bool add_note(uint8_t key, MidiEvent::Event &event) {
		if (size_ >= kMax || excists(key, event)) {
			return false;
		}

		int i = size_;

		while (i > 0 && entry[i - 1].key >= key) {
			entry[i] = entry[i - 1];
			--i;
		}

		entry[i].key = key;
		entry[i].event = event;
		++size_;

		return true;
	}

	bool remove_note(uint8_t key, MidiEvent::Event &dest) {
		int8_t slot = key_to_slot(key);

		if (slot < 0) {
			return false;
		}

		MidiEvent::convert_to_note_off(&entry[slot].event);
		dest = entry[slot].event;

		remove_slot(slot);

		return true;
	}

private:
	uint8_t size_;
	static const uint8_t kMax = 8;

	struct Entry {
		uint8_t key;
		MidiEvent::Event event;
	}entry[kMax];


	void remove_slot(uint8_t slot) {
		for (int i = slot; i < size_; ++i) {
			entry[i] = entry[i + 1];
		}
		--size_;
	}

	int8_t key_to_slot(uint8_t key) {
		int8_t slot = -1;
		for (int i = 0; i < size_; ++i) {
			if (entry[i].key == key) {
				slot = i;
				break;
			}
		}
		return slot;
	}

	bool excists(uint8_t key, MidiEvent::Event &e) {
		for (int i = 0; i < size_; ++i) {
			if (entry[i].key == key || entry[i].event.data[0] == e.data[0]) {
				return true;
			}
		}
		return false;
	}

};

#endif
