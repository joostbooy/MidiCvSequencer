#ifndef SongMonitorPage_h
#define SongMonitorPage_h

#include "topPage.h"

namespace SongMonitorPage {
	//Declarations
	void init();
	void enter();
	void exit();
	void msTick(uint16_t ticks);
	void drawDisplay();
	void updateLeds();
	void onButton(uint8_t id, uint8_t value);
	void onEncoder(uint8_t id, int inc);
	const uint16_t targetFps();

	// Variables
	int selected_track;

	Window window = {
		.x = 0,
		.y = 12,
		.width = canvas.width() - 1,
		.height = 40,
		.collumns = 5,
		.rows = 4,
	};

	void init() {
		selected_track = 0;
		window.set_row_items_total(16);
		window.scroll_to_row(selected_track);
	}

	void enter() {

	}

	void exit() {

	}

	void onEncoder(uint8_t id, int inc) {
		selected_track = stmlib::clip(0, 15, selected_track + inc);
		window.scroll_to_row(selected_track);
	}

	void onButton(uint8_t id, uint8_t value) {

	}

	void drawLeds() {

	}

	void msTick(uint16_t ticks) {

	}

	// Bottom to top
	void drawDisplay() {
		WindowPainter::draw_header();

		canvas.set_font(Font::SMALL);

		int source;
		Canvas::Color color;
		MidiEvent::Event event;

		for (int i = window.row().first; i <= window.row().last; ++i) {
			color = (selected_track == i) ? Canvas::BLACK : Canvas::LIGHT_GRAY;

			source = MidiEvent::serialise_source(MidiEvent::TRACK, i);
			event = engine.midiOutputEngine.curr_event(source);

			WindowPainter::text(window.cell(0, i), settings.song.track(i).name(), Canvas::LEFT, Canvas::CENTER, color);
			WindowPainter::text(window.cell(1, i), UiText::midi_channel_text(event), Canvas::LEFT, Canvas::CENTER, color);
			WindowPainter::text(window.cell(2, i), UiText::midi_message_text(event), Canvas::LEFT, Canvas::CENTER, color);
			WindowPainter::text(window.cell(3, i), UiText::midi_data_text(event, 0), Canvas::LEFT, Canvas::CENTER, color);
			WindowPainter::text(window.cell(4, i), UiText::midi_data_text(event, 1), Canvas::LEFT, Canvas::CENTER, color);
		}

		WindowPainter::vertical_scrollbar(window);
	}

	const uint16_t targetFps() {
		return 1000 / 32;
	}

	const Pages::EventHandlers eventHandlers = {
		&init,
		&enter,
		&exit,
		&msTick,
		&drawLeds,
		&drawDisplay,
		&onEncoder,
		&onButton,
		&targetFps,
	};

};

#endif
