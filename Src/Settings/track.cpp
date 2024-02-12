#include "track.h"
#include "fileWriter.h"
#include "fileReader.h"


void Track::save(FileWriter &fileWriter) {
	fileWriter.write(index_);
	fileWriter.write(data_.type_);
	fileWriter.write(data_.port_);
	fileWriter.write(data_.channel_);
	fileWriter.write(data_.clock_mode_);
	fileWriter.write(data_.clock_speed_);
	fileWriter.write(data_.delay_);
	fileWriter.write(data_.swing_);
	fileWriter.write(data_.humanise_);
	fileWriter.write(data_.serial_[0]);
	fileWriter.write(data_.serial_[1]);

	for (int i = 0; i < TrackData::kMaxLabelLength; ++i) {
		fileWriter.write(data_.label_[i]);
	}

	fileWriter.write(data_.pattern_list_);
}

void Track::load(FileReader &fileReader) {
	fileReader.read(index_);
	fileReader.read(data_.type_);
	fileReader.read(data_.port_);
	fileReader.read(data_.channel_);
	fileReader.read(data_.clock_mode_);
	fileReader.read(data_.clock_speed_);
	fileReader.read(data_.delay_);
	fileReader.read(data_.swing_);
	fileReader.read(data_.humanise_);
	fileReader.read(data_.serial_[0]);
	fileReader.read(data_.serial_[1]);

	for (int i = 0; i < TrackData::kMaxLabelLength; ++i) {
		fileReader.read(data_.label_[i]);
	}

	fileReader.read(data_.pattern_list_);
}
