#ifndef StepEditPage_h
#define StepEditPage_h

#include "topPage.h"
#include "stepEditor.h"


namespace StepEditPage {
	//Declarations
	void init();
	void enter();
	void exit();
	void msTick(uint16_t ticks);
	void drawDisplay();
	void updateLeds();
	void onButton(uint8_t id, uint8_t value);
	void onEncoder(uint8_t id, uint8_t state);
	const uint16_t targetFps();

	//variables
	StepEditor stepEditor;

	void init() {

	}

	void enter() {
		stepEditor.set_mode(StepEditor::EDIT);
	}

	void exit() {
		stepEditor.reset();
	}

	void onEncoder(uint8_t id, int inc) {
		int step = controller.encoder_to_step(id);
		if (step >= 0) {
			stepEditor.on_encoder(step, inc, controller.encoder_is_pressed(id));
		}
	}

	bool update_edit_mode() {
		bool shift = controller.is_pressed(Controller::SHIFT_BUTTON);
		bool group = controller.is_pressed(Controller::GROUP_BUTTON);
		bool solo = controller.is_pressed(Controller::SOLO_BUTTON);
		bool mute = controller.is_pressed(Controller::MUTE_BUTTON);

		switch (stepEditor.mode())
		{
		case StepEditor::EDIT:
			if (shift) {
				stepEditor.set_mode(StepEditor::SHIFT);
				return true;
			}

			if (group) {
				stepEditor.set_mode(StepEditor::GROUPED_EDIT);
				return true;
			}
			break;
		case StepEditor::SHIFT:
			if (!shift) {
				stepEditor.set_mode(StepEditor::EDIT);
				return true;
			}

			if (solo) {
				stepEditor.set_mode(StepEditor::AUDITION);
				return true;
			}

			if (group) {
				stepEditor.set_mode(StepEditor::GROUPED_SHIFT);
				return true;
			}

			if (mute) {
				stepEditor.reset();
				pages.open(Pages::PATTERN_RANDOM_PAGE);
				return true;
			}
			break;
		case StepEditor::AUDITION:
			if (!shift || !solo) {
				stepEditor.set_mode(StepEditor::EDIT);
				return true;
			}
			break;
		case StepEditor::GROUPED_SHIFT:
			if (!shift || !group) {
				stepEditor.set_mode(StepEditor::EDIT);
				return true;
			}
			break;
		case StepEditor::GROUPED_EDIT:
			if (!group) {
				stepEditor.set_mode(StepEditor::EDIT);
				return true;
			}
			break;
		default:
			break;
		}
		return false;

	}

	void onButton(uint8_t id, uint8_t state) {
		if (update_edit_mode()) {
			return;
		}

		int step_button = controller.button_to_step(id);
		if (step_button >= 0) {
			stepEditor.on_button(step_button, state);
			return;
		}

		int step_encoder = controller.encoder_push_to_step(id);
		if (step_encoder >= 0) {
			stepEditor.on_encoder_push(step_encoder, state);
			return;
		}
	}


	void drawLeds() {
		Track &track = settings.selected_track();

		// active steps
		int item = -1;
		switch (track.type())
		{
		case Track::NOTE_TRACK:
			item = NoteTrack::TRIGGER;
			break;
		case Track::CHORD_TRACK:
			item = ChordTrack::TRIGGER;
			break;
		case Track::DRUM_TRACK:
			item = DrumTrack::TRIGGER;
			break;
		case Track::CURVE_TRACK:
			item = CurveTrack::TRIGGER;
			break;
		default:
			break;
		}

		if (item >= 0) {
			for (int i = 0; i < 16; ++i) {
				LedPainter::set_step_button(i, stepEditor.read(i, item) ? Matrix::GREEN : Matrix::BLACK);
			}
		}

		// step mode
		LedPainter::paint_step_mode(stepEditor, track);
	}

	void msTick(uint16_t ticks) {

	}

	void drawDisplay() {

	}

	const uint16_t targetFps() {
		return 1000 / 16;
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
