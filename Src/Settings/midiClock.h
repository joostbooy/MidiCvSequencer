#ifndef MidiClock_h
#define MidiClock_h

#include "uiText.h"
#include "midiPort.h"
#include "fileWriter.h"
#include "fileReader.h"
#include "lookupTables.h"

class MidiClock {

public:

	void init() {
		set_sync_port(INTERNAL);	// needs to be first
		set_bpm(120);
		set_bpm_fractional(0);

		ext_bpm_ = 120;

		reset();
	}

	enum SyncPort {
		EXTERNAL = MidiPort::NUM_PORTS - 1,
		INTERNAL,

		NUM_SYNC_PORTS
	};

	const char* sync_port_text(int value) {
		if (value <= EXTERNAL) {
			return MidiPort::port_text(value);
		} else {
			return "INTERNAL";
		}
	}

	// bpm
	uint16_t bpm() {
		return bpm_;
	}

	void set_bpm(int value) {
		if (sync_port() == INTERNAL) {
			bpm_ = stmlib::clip(0, MAX_BPM - 1, value);
			update_inc(bpm(), bpm_fractional());
		}
	}

	// bpm fractional
	uint16_t bpm_fractional() {
		return bpm_fractional_;
	}

	void set_bpm_fractional(int value) {
		if (sync_port() == INTERNAL) {
			bpm_fractional_ = stmlib::clip(0, MAX_BPM_FRACTIONAL - 1, value);
			update_inc(bpm(), bpm_fractional());
		}
	}

	const char* bpm_text() {
		if (sync_port() == INTERNAL) {
			return UiText::str.write(bpm(), ".", bpm_fractional());
		}  else {
			return UiText::str.write("EXT ", ext_bpm_, ".0");
		}
	}

	// sync port
	uint8_t sync_port() {
		return sync_port_;
	}

	void set_sync_port(int value) {
		sync_port_ = stmlib::clip(0, NUM_SYNC_PORTS - 1, value);
	}

	const char* sync_port_text() {
		return sync_port_text(sync_port());
	}


	/***************
	*	Methods
	***************/

	inline void reset() {
		ticks = 0;
		isr_ticks = 0;
		isr_average = 0;
		tempo_phase = 0;
	}

	inline bool tick() {
		++isr_ticks;
		tempo_phase += tempo_inc;
		return tempo_phase < tempo_inc;
	}

	inline void sync_bpm() {
		isr_average += isr_ticks;

		if (((++num_readings % 4) == 0) && isr_average > 0) {
			ext_bpm_ = uint32_t(45000000UL * 60UL / 24UL / (kUpdatePeriod * (isr_average / 4)));
			update_inc(ext_bpm_, 0);
			isr_average = 0;
		}

		isr_ticks = 0;
	}

	void save(FileWriter &fileWriter) {
		fileWriter.write(bpm_);
		fileWriter.write(bpm_fractional_);
		fileWriter.write(sync_port_);
	}

	void load(FileReader &fileReader) {
		fileReader.read(bpm_);
		fileReader.read(bpm_fractional_);
		fileReader.read(sync_port_);
		update_inc(bpm_, bpm_fractional_);
	}

private:
	uint16_t bpm_ = 120;
	uint16_t bpm_fractional_ = 0;
	uint8_t sync_port_;

	volatile uint16_t ext_bpm_;
	volatile uint32_t tempo_inc, tempo_phase, ticks, isr_ticks, isr_average;
	volatile uint8_t num_readings = 0;
	static const uint32_t kUpdatePeriod = 45000000UL / CLOCK_ISR_FREQ;

	inline void update_inc(uint16_t bpm, uint8_t fractional) {
		tempo_inc = lut_bpm_inc[bpm] + lut_bpm_fractional_inc[fractional];
	}
};

#endif
