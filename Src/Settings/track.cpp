#include "track.h"
#include "fileWriter.h"
#include "fileReader.h"


void Track::save(FileWriter &fileWriter) {
	fileWriter.write(index_);
	fileWriter.write(data.type_);
	fileWriter.write(data.port_);
	fileWriter.write(data.channel_);
	fileWriter.write(data.clock_mode_);
	fileWriter.write(data.clock_speed_);
	fileWriter.write(data.delay_);
	fileWriter.write(data.swing_);
	fileWriter.write(data.humanise_);
	fileWriter.write(data.serial_[0]);
	fileWriter.write(data.serial_[1]);

	for (int i = 0; i < TrackData::kMaxLabelLength; ++i) {
		fileWriter.write(data.label_[i]);
	}

	fileWriter.write(data.pattern_list_);
}

void Track::load(FileReader &fileReader) {
	fileReader.read(index_);
	fileReader.read(data.type_);
	fileReader.read(data.port_);
	fileReader.read(data.channel_);
	fileReader.read(data.clock_mode_);
	fileReader.read(data.clock_speed_);
	fileReader.read(data.delay_);
	fileReader.read(data.swing_);
	fileReader.read(data.humanise_);
	fileReader.read(data.serial_[0]);
	fileReader.read(data.serial_[1]);

	for (int i = 0; i < TrackData::kMaxLabelLength; ++i) {
		fileReader.read(data.label_[i]);
	}

	fileReader.read(data.pattern_list_);
}
