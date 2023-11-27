#include "settings.h"
#include "disk.h"
#include "fileWriter.h"
#include "fileReader.h"

Settings settings;
FileWriter fileWriter;
FileReader fileReader;


bool Settings::save(const char* new_path) {
	path.write(new_path);
	if (!save()) {
		path.clear();
		return false;
	}
	return true;
}

bool Settings::save() {
	if (!path.length()) {
		return false;
	}

	fileWriter.start(&disk.file, path.read(), current_version());

	if (!fileWriter.write_ok()) {
		fileWriter.stop();
		return false;
	}

	song.save(fileWriter);

	for (int i = 0; i < CvInput::NUM_PORTS; ++i) {
		cvInput(i).save(fileWriter);
	}

	for (int i = 0; i < CvOutput::NUM_PORTS; ++i) {
		cvOutput(i).save(fileWriter);
	}

	for (int i = 0; i < MidiPort::NUM_PORTS; ++i) {
		midiInput(i).save(fileWriter);
		midiOutput(i).save(fileWriter);
	}

	fileWriter.stop();

	if (!fileWriter.write_ok()) {
		return false;
	}

	return true;
};

bool Settings::load(const char* new_path) {
	init();	// also clears the path

	fileReader.start(&disk.file, new_path);

	if (!fileReader.read_ok()) {
		fileReader.stop();
		init();
		return false;
	}

	song.load(fileReader);

	for (int i = 0; i < CvInput::NUM_PORTS; ++i) {
		cvInput(i).load(fileReader);
	}

	for (int i = 0; i < CvOutput::NUM_PORTS; ++i) {
		cvOutput(i).load(fileReader);
	}

	for (int i = 0; i < MidiPort::NUM_PORTS; ++i) {
		midiInput(i).load(fileReader);
		midiOutput(i).load(fileReader);
	}

	fileReader.stop();

	if (!fileReader.read_ok()) {
		init();
		return false;
	}

	path.write(new_path);	// the new valid path
	return true;
};

bool Settings::save_calibration() {
	fileWriter.start(&disk.file, "0:/SETTINGS.CAL", current_version());
	calibration.save(fileWriter);
	fileWriter.stop();

	return fileWriter.write_ok();
}

bool Settings::load_calibration() {
	fileReader.start(&disk.file, "0:/SETTINGS.CAL");
	calibration.load(fileReader);
	fileReader.stop();

	if (fileReader.read_ok())  {
		calibration_loaded_ = true;
	} else {
		calibration.init();
		calibration_loaded_ = false;
	}

	return calibration_loaded_;
}
