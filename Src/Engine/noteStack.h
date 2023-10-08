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
		if (size_ >= kMax || find_key(key) >= 0) {
			return false;
		}

		entry[size_].key = key;
		entry[size_].event = event;
		++size_;

		return true;
	}

	bool remove_note(uint8_t key, MidiEvent::Event &dest) {
		int8_t slot = find_key(key);

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

	int8_t find_key(uint8_t key) {
		int8_t slot = -1;
		for (int i = 0; i < size_; ++i) {
			if (entry[i].key == key) {
				slot = i;
				break;
			}
		}
		return slot;
	}

};

#endif
