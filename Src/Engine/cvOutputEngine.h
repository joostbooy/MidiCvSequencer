#ifndef CvOutputEngine_h
#define CvOutputEngine_h

#include "dac.h"
#include "gateIO.h"
#include "cvOutput.h"
#include "midiEvent.h"
#include "clockEngine.h"
#include "voltPerOctave.h"

class CvOutputEngine {

public:
	void init() {
		reset();

		calibrating_ = false;

		for (int i = 0; i < MidiEvent::NUM_SOURCES; ++i) {
			vel_value[i] = 0;
			cc_value[i] = 0;
			note_value[i] = 0;
			last_note_value[i] = 0;
		}
	}

	void reset() {
		is_running_ = false;
		trigger_flags = 0;

		for (int i = 0; i < MidiEvent::NUM_SOURCES; ++i) {
			legato_flag[i] = 0;
			note_count[i] = 0;
			slide_phase[i] = 0.f;
			bend_value[i] = 0.f;
			last_note_value[i] = note_value[i];
		}

		for (int i = 0; i < CvOutput::NUM_PORTS; ++i) {
			clock[i].reset();
			clock_ticks[i] = 0;
		}
	}

	void start() {
		is_running_ = true;
	}

	void start_calibration() {
		calibrating_ = true;
	}

	void stop_calibration() {
		calibrating_ = false;
	}

	void tick() {
		for (int i = 0; i < CvOutput::NUM_PORTS; ++i) {
			auto &cvOut = settings.cvOutput(i);

			// update clock
			clock[i].set_speed(cvOut.clock_speed());

			if (clock[i].tick()) {
				clock_ticks[i] = clock[i].step_duration() / 2;
			}

			if (clock_ticks[i] > 0) {
				--clock_ticks[i];
			}

			// Update gate
			bool gate_state = 0;
			int source = cvOut.gate_source();

			switch (cvOut.gate_mode())
			{
			case CvOutput::GATE:
				gate_state = note_count[source] > 0;
				break;
			case CvOutput::TRIGGER:
				gate_state = trigger_flags & (1 << source);
				break;
			case CvOutput::CLOCK_START:
				gate_state = is_running_;
				break;
			case CvOutput::CLOCK_STOP:
				gate_state = !is_running_;
				break;
			case CvOutput::CLOCK_PULSE:
				gate_state = clock_ticks[i] > 0;
				break;
			default:
				break;
			}

			// invert gate
			if (cvOut.gate_invert()) {
				gate_state = !gate_state;
			}

			gateIO.set_output(i, gate_state);

			// Update cv
			source = cvOut.cv_source();

			switch (cvOut.cv_mode())
			{
			case CvOutput::VELOCITY:
				dac.set(i, vel_value[source]);
				break;
			case CvOutput::CC:
				dac.set(i, cc_value[source]);
				break;
			case CvOutput::NOTE:
				if (cvOut.slide_mode() == CvOutput::ON || (cvOut.slide_mode() == CvOutput::LEGATO && legato_flag[source] == true)) {
					dac.set(i, next_slide_value(source, cvOut.slide_speed()));
				} else {
					dac.set(i, note_value[source]);
				}
				break;
			default:
				break;
			}
		}

		// clear flags
		trigger_flags = 0;
	}


	void send() {
		// overwrite dac values if calibrating
		if (calibrating_) {
			dac.set(0, settings.voltPerOctave.min());
			dac.set(1, settings.voltPerOctave.max());
		}

		dac.write();
		gateIO.write_output();
	}

	float current_value(uint8_t channel) {
		return (1.f / 65535.0f) * dac.value(channel);
	}

	bool current_gate(uint8_t channel) {
		return gateIO.read_output(channel);
	}

	void write_cc(MidiEvent::Event &event, int value_16_bit = -1) {
		if (value_16_bit >= 0) {
			cc_value[event.source] = settings.voltPerOctave.cv_to_value(value_16_bit);
		} else {
			cc_value[event.source] = settings.voltPerOctave.cv_to_value(event.data[1] << 9);
		}
	}

	void write_bend(MidiEvent::Event &event, int value_16_bit = -1) {
		if (value_16_bit >= 0) {
			bend_value[event.source] = (1.f / 65535.f) * value_16_bit;
		} else {
			bend_value[event.source] = (1.f / 16383.f) * MidiEvent::get_14bit_data(&event);
		}
	}

	void write_note_off(MidiEvent::Event &event) {
		if (note_count[event.source] > 0) {
			--note_count[event.source];
		}
	}

	void write_note_on(MidiEvent::Event &event) {
		uint8_t source = event.source;

		// update note count
		legato_flag[source] = note_count[source] > 0;
		++note_count[source];

		// reset slide
		slide_phase[source] = 0.0f;

		// set trigger
		trigger_flags |= (1 << source);

		// set note & vel
		last_note_value[source] = note_value[source];
		note_value[source] = settings.voltPerOctave.note_to_value(event.data[0]);
		vel_value[source] = settings.voltPerOctave.cv_to_value(event.data[1]);
	}

private:
	bool is_running_;
	bool calibrating_;

	uint32_t trigger_flags;

	bool legato_flag[MidiEvent::NUM_SOURCES];
	float slide_phase[MidiEvent::NUM_SOURCES];
	float bend_value[MidiEvent::NUM_SOURCES];
	uint16_t note_count[MidiEvent::NUM_SOURCES];
	uint16_t cc_value[MidiEvent::NUM_SOURCES];
	uint16_t vel_value[MidiEvent::NUM_SOURCES];
	uint16_t note_value[MidiEvent::NUM_SOURCES];
	uint16_t last_note_value[MidiEvent::NUM_SOURCES];

	ClockEngine clock[CvOutput::NUM_PORTS];
	uint32_t clock_ticks[CvOutput::NUM_PORTS];

	inline uint16_t next_slide_value(uint8_t source, uint8_t speed) {
		uint16_t value = Dsp::cross_fade(last_note_value[source], note_value[source], slide_phase[source]);

		slide_phase[source] += ClockEngine::step_duration_reciprocal(speed);
		if (slide_phase[source] >= 1.f) {
			slide_phase[source] = 1.f;
		}

		return value;
	}

	// unimplemented for now, might be to taxing
	inline uint16_t apply_bend(uint8_t source, uint16_t pitch) {
		const uint16_t whole_note = settings.voltPerOctave.note_to_value(2) - settings.voltPerOctave.note_to_value(0);

		uint16_t low = clip_min(0, pitch - whole_note);
		uint16_t high = clip_max(65535, pitch + whole_note);

		return Dsp::cross_fade(low, pitch, high, bend_value[source]);
	}
};

#endif
