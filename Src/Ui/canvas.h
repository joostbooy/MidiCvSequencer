#ifndef Canvas_h
#define Canvas_h

#include "stmf4lib.h"
#include "frameBuffer.h"
#include "font.h"
#include "bitmap.h"
#include "stringUtils.h"
#include "dsp.h"

class Canvas {

public:

	enum Color {
		WHITE,
		LIGHT_GRAY,
		GRAY,
		DARK_GRAY,
		BLACK,
		INVERTED,
		SUBTRACTED,
		MULTIPLIED,
		NONE
	};

	enum Allign {
		LEFT,
		RIGHT,
		CENTER,
		TOP,
		BOTTOM,
		NO_ALLIGNMENT
	};

	//Frame buffer
	static const uint16_t width_ = 256;
	static const uint16_t height_ = 64;
	const uint16_t width() { return Canvas::width_; }
	const uint16_t height() { return Canvas::height_; }

	FrameBuffer <width_, height_> buffer;


	void draw_pixel(uint16_t x, uint16_t y, Color color) {
		if (buffer.inside(x, y) == false) {
			return;
		}

		switch (color)
		{
		case WHITE:
			buffer.write(x, y, 0x00);
			break;
		case LIGHT_GRAY:
			buffer.write(x, y, 0x04);
			break;
		case GRAY:
			buffer.write(x, y, 0x07);
			break;
		case DARK_GRAY:
			buffer.write(x, y, 0x0C);
			break;
		case BLACK:
			buffer.write(x, y, 0x0F);
			break;
		case INVERTED:
			buffer.write(x, y, buffer.read(x, y) ^ 0xFF);
			break;
		case SUBTRACTED:
			buffer.write(x, y, buffer.read(x, y) / 3);
			break;
		case MULTIPLIED:
			buffer.write(x, y, clip_max(0xF, buffer.read(x, y) * 3));
			break;
		case NONE:
		default:
			break;
		}
	}

	void horizontal_line(uint16_t x, uint16_t y, uint16_t w, Color color) {
		uint16_t x2 = x + w;
		for (uint16_t xi = x; xi < x2; ++xi) {
			draw_pixel(xi, y, color);
		}
	}

	void vertical_line(uint16_t x, uint16_t y, uint16_t h, Color color) {
		uint16_t y2 = y + h;
		for (uint16_t yi = y; yi < y2; ++yi) {
			draw_pixel(x, yi, color);
		}
	}

	void fill(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Color color) {
		uint16_t y2 = y + h;
		for (uint16_t yi = y; yi < y2; ++yi) {
			horizontal_line(x, yi, w, color);
		}
	}

	void frame(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Color color, uint16_t border = 1) {
		uint16_t x2 = clip_min(0, x + w - 1);
		uint16_t y2 = clip_min(0, y + h - 1);

		for (uint16_t i = 0; i < border; ++i) {
			horizontal_line(x, y + i, w, color);
			horizontal_line(x, y2 - i, w, color);
			vertical_line(x + i, y, h, color);
			vertical_line(x2 - i, y, h, color);
		}
	}

	void box(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Color frame_clr, Color fill_clr, uint16_t border = 1){
		if (frame_clr != Color::NONE) {
			frame(x, y, w, h, frame_clr, border);
		}

		if (fill_clr != Color::NONE) {
			fill(x + border, y + border, w - (border * 2), h - (border * 2), fill_clr);
		}
	}

	void get_xy_allignment(int *x, int *y, int w, int h, int frame_w, int frame_h, Allign x_allign, Allign y_allign) {
		int x_ = *x;
		int y_ = *y;

		switch (x_allign)
		{
		case LEFT:
			break;
		case CENTER:
			x_ += (w - frame_w) / 2;
			break;
		case RIGHT:
			x_ += (w - frame_w);
			break;
		default:
			break;
		}

		switch (y_allign)
		{
		case TOP:
			break;
		case CENTER:
			y_ += (h - frame_h) / 2;
			break;
		case BOTTOM:
			y_ += (h - frame_h);
			break;
		default:
			break;
		}

		*x = clip(0, width() - frame_w, x_);
		*y = clip(0, height() - frame_h, y_);
	}

	// bitmap
	void set_bitmap(Bitmap::Id id, Color color = BLACK) {
		bitmap_color = color;
		bitmap.set(id);
	}

	// text
	void set_font(Font::Type type, Color color = BLACK) {
		font_color = color;
		font.set_type(type);
	}

	uint16_t text_cursor(){
		return text_cursor_;
	}

	void draw_text(uint16_t x, uint16_t y, const char *str, Color color = BLACK) {
		font_color = color;
		while (*str) {
			draw_char(x, y, *str++);
			x = text_cursor();
		}
	}

	void draw_text(int x, int y, int w, int h, const char *str, Allign x_allign, Allign y_allign, Color color = BLACK) {
		int x_ = x;
		int y_ = y;
		get_xy_allignment(&x_, &y_, w, h, font.string_width(str), font.height(), x_allign, y_allign);
		draw_text(x_, y_, str, color);
	}

	void draw_bitmap(int x, int y, int w, int h, Bitmap::Id id, Allign x_allign, Allign y_allign) {
		int x_ = x;
		int y_ = y;
		set_bitmap(id);
		get_xy_allignment(&x_, &y_, w, h, bitmap.width, bitmap.height, x_allign, y_allign);
		draw_bitmap(x_, y_, id);
	}

	void draw_char(uint16_t x, uint16_t y, char c);
	void draw_bitmap(uint16_t x, uint16_t y, Bitmap::Id id, uint16_t frame = 0);

private:
	uint16_t text_cursor_ = 0;
	Color font_color = BLACK;
	Color bitmap_color = BLACK;
};

extern Canvas canvas;

#endif
