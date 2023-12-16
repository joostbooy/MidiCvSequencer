#ifndef Usb_h
#define Usb_h

#include "stmf4lib.h"
#include "tusb.h"

class Usb {

public:

	void init();

	// only call from main, not safe to call in a ISR
	// https://github.com/hathach/tinyusb/issues/52
	void task() {
		tud_task();
	}

	bool read(uint8_t data[4]) {
		return tud_midi_packet_read(data);
	}

	void blocking_read(uint8_t data[4]) {
		while (!read(data));
	}

	bool write(uint8_t data[4]) {
		return tud_midi_packet_write(data);
	}

	void blocking_write(uint8_t data[4]) {
		while (!write(data));
	}

private:

};

extern Usb usb;

#endif
