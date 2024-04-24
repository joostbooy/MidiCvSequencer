#ifndef MidiInput_h
#define MidiInput_h

#include "midiMonitor.h"
#include "arpeggiator.h"

class MidiInput {

public:

	MidiFilter filter;
	MidiMonitor monitor;
	Arpeggiator arpeggiator;

	void init() {
		filter.init();
		monitor.init(&filter);
		arpeggiator.init();

		set_channel_send(0);
		set_channel_receive(-1);
		set_cc_receive(-1);
		set_quantise_note(false);
		set_pitch_bend_receive(false);
	}

	// quantise note
	bool quantise_note() {
		return quantise_note_;
	}

	void set_quantise_note(bool state) {
		quantise_note_ = state;
	}

	const char* quantise_note_text() {
		return UiText::bool_to_on_off(quantise_note());
	}

	// pitch bend receive
	bool pitch_bend_receive() {
		return pitch_bend_receive_;
	}

	void set_pitch_bend_receive(bool state) {
		pitch_bend_receive_ = state;
	}

	const char* pitch_bend_receive_text() {
		return UiText::bool_to_on_off(pitch_bend_receive());
	}

	// channel send
	int channel_send() {
		return channel_send_;
	}

	void set_channel_send(int channel) {
		channel_send_ = stmlib::clip(0, 15, channel);
	}

	const char* channel_send_text() {
		return UiText::str.write(channel_send() + 1);
	}

	// channel receive
	int channel_receive() {
		return channel_receive_;
	}

	void set_channel_receive(int channel) {
		channel_receive_ = stmlib::clip(-1, 16, channel);
	}

	const char* channel_receive_text() {
		return UiText::midi_channel_text(channel_receive());
	}

	// cc receive
	int cc_receive() {
		return cc_receive_;
	}

	void set_cc_receive(int cc_number) {
		cc_receive_ = stmlib::clip(-1, 127, cc_number);
	}

	const char* cc_receive_text() {
		if (cc_receive() < 0) {
			return "NONE";
		}
		return UiText::str.write(cc_receive());
	}


	/***************
	*	Methods
	***************/

	bool cc_accepted(int cc_number) {
		return cc_receive() == cc_number; //&& filter.message_allowed(MidiFilter::CC);
	}

	bool channel_accepted(int channel) {
		return channel_receive() >= 16 || channel == channel_receive();
	}

	// storage
	void load(FileReader &fileReader) {
		fileReader.read(channel_receive_);
		fileReader.read(channel_send_);
		fileReader.read(cc_receive_);
		fileReader.read(quantise_note_);
		fileReader.read(pitch_bend_receive_);

		filter.load(fileReader);
		arpeggiator.load(fileReader);
	}

	void save(FileWriter &fileWriter) {
		fileWriter.write(channel_receive_);
		fileWriter.write(channel_send_);
		fileWriter.write(cc_receive_);
		fileWriter.write(quantise_note_);
		fileWriter.write(pitch_bend_receive_);

		filter.save(fileWriter);
		arpeggiator.save(fileWriter);
	}

	void paste(MidiInput *midiInput) {
		channel_receive_ = midiInput->channel_receive();
		channel_send_ = midiInput->channel_send();
		cc_receive_ = midiInput->cc_receive();
		quantise_note_ = midiInput->quantise_note();
		pitch_bend_receive_ = midiInput->pitch_bend_receive();

		arpeggiator.paste(&midiInput->arpeggiator);
	}

private:
	int8_t channel_receive_;
	int8_t channel_send_;
	int8_t cc_receive_;
	bool quantise_note_;
	bool pitch_bend_receive_;
};

#endif
