#ifndef MidiScheduler_h
#define MidiScheduler_h

#include "midiEvent.h"

template <const size_t kMaxEvents>
class MidiScheduler {

public:

	inline void init() {
		size = 0;
		head = 0;									// 0 is the tail
		head_free_slots = 1;						// Cannot be the same as head
		for (size_t i = 1; i < kMaxEvents; ++i) {
			entry[i].next = i + 1;
		}
	}

	inline bool writeable() {
		return size < kMaxEvents;
	}

	inline bool readable() {
		return size > 0;
	}

	inline void add(MidiEvent::Event &data, uint32_t when) {
		size_t slot = head_free_slots;
		head_free_slots = entry[head_free_slots].next;

		entry[slot].data = data;
		entry[slot].when = when;
		++size;

		// sort slot
		if (head == 0 || when < entry[head].when) {
			entry[slot].when = when;
			entry[slot].next = head;
			head = slot;
		} else {
			size_t prev = head;
			size_t curr = entry[prev].next;

			while ((curr) && (when >= entry[curr].when)) {
				prev = entry[prev].next;
				curr = entry[prev].next;
			}
			entry[prev].next = slot;
			entry[slot].next = curr;
		}
	}

	inline bool pull() {
		if (!head) {
			return false;
		}

		size_t free_slot = head;
		head = entry[head].next;

		entry[free_slot].next = head_free_slots;
		head_free_slots = free_slot;

		--size;
		return true;
	}

	inline bool pull_event(MidiEvent::Event &data) {
		data = entry[head].data;
		return pull();
	}

	inline bool tick(MidiEvent::Event &data) {
		if (entry[head].when == 0 && pull_event(data) == true) {
			return true;
		}

		size_t slot = head;
		while (slot) {
			--entry[slot].when;
			slot = entry[slot].next;
		}
		return false;
	}

	inline bool find_and_remove(MidiEvent::Event &data, MidiEvent::Event &dest) {
		if (!head) {
			return false;
		}

		if (match(entry[head].data, data)) {
			pull_event(dest);
			return true;
		}

		size_t prev = head;
		size_t curr = entry[prev].next;

		while (curr && !match(entry[curr].data, data)) {
			prev = entry[prev].next;
			curr = entry[prev].next;
		}

		if (!curr) {
			return false;
		}

		dest = entry[curr].data;

		entry[prev].next = entry[curr].next;
		entry[curr].next = head_free_slots;
		head_free_slots = curr;

		--size;
		return true;
	}

private:

	struct Entry {
		MidiEvent::Event data;
		uint32_t when;
		size_t next;
	} entry[kMaxEvents + 1]; // +tail

	size_t size;
	size_t head;
	size_t head_free_slots;

	bool match(MidiEvent::Event &a, MidiEvent::Event &b) {
		return (a.message == b.message) && (a.data[0] == b.data[0]);
	}

};

#endif
