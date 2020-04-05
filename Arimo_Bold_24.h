// Created by http://oleddisplay.squix.ch/ Consider a donation
// In case of problems make sure that you are using the font file with the correct version!
const uint8_t Arimo_Bold_24Bitmaps[] PROGMEM = {

	// Bitmap Data:
	0x00, // ' '
	0xFF,0xFF,0xFF,0xFF,0x80,0x3F,0xE0, // '!'
	0xE7,0xE7,0xE7,0xE7,0xE7,0xE7, // '"'
	0x04,0x30,0x61,0x03,0x18,0x18,0xC0,0xC6,0x3F,0xFD,0xFF,0xE3,0x18,0x18,0xC0,0xC6,0x04,0x31,0xFF,0xEF,0xFF,0x18,0xC0,0xC6,0x04,0x30,0x61,0x00, // '#'
	0x02,0x00,0xFE,0x0F,0xF8,0xFF,0xE7,0x27,0x39,0x3D,0xC8,0x0F,0x40,0x3F,0x80,0xFF,0x03,0xFC,0x05,0xF0,0x23,0x91,0x1D,0xC8,0xEF,0x4F,0x3F,0xF0,0xFE,0x00,0x80, // '$'
	0x3E,0x01,0x07,0xF0,0x30,0xF7,0x06,0x0E,0x38,0x40,0xE3,0x8C,0x0E,0x39,0x80,0xE3,0x9B,0xCE,0x3B,0x7E,0xE7,0x6E,0x77,0xF6,0xE7,0x3E,0xCE,0x70,0x18,0xE7,0x01,0x8E,0x70,0x30,0xE7,0x07,0x0E,0x70,0x60,0x7E,0x0C,0x03,0xC0, // '%'
	0x07,0xC0,0x0F,0xE0,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0xE0,0x0F,0xE0,0x0F,0x80,0x3E,0x08,0x7F,0x0C,0xF3,0x9C,0xE3,0x9C,0xE1,0xF8,0xE1,0xF8,0xF0,0xF8,0x7F,0xFF,0x1F,0x0F, // '&'
	0xFF,0xFF,0xC0, // '''
	0x1C,0x78,0xE3,0xC7,0x0E,0x3C,0x70,0xE1,0xC3,0x87,0x0E,0x1C,0x38,0x70,0xF0,0xE1,0xC3,0xC3,0x87,0x87,0x00, // '('
	0x70,0xF0,0xE1,0xE1,0xC3,0x87,0x07,0x0E,0x1C,0x38,0x70,0xE1,0xC3,0x87,0x1C,0x38,0x71,0xE3,0x8F,0x1C,0x00, // ')'
	0x0E,0x01,0xC1,0x39,0x3F,0xE7,0xFC,0x3E,0x0E,0xE3,0x8E,0x11,0x00, // '*'
	0x0E,0x00,0xE0,0x0E,0x00,0xE0,0x0E,0x0F,0xFF,0xFF,0xF0,0xE0,0x0E,0x00,0xE0,0x0E,0x00,0xE0, // '+'
	0xFF,0xF3,0x36,0xE0, // ','
	0xFF,0xFF,0xC0, // '-'
	0xFF,0x80, // '.'
	0x0C,0x38,0x70,0xE1,0xC3,0x0E,0x1C,0x38,0x70,0xC3,0x87,0x0E,0x1C,0x38,0x61,0xC0, // '/'
	0x1F,0x07,0xF1,0xC7,0x38,0xEE,0x0F,0xC1,0xF8,0x3F,0x07,0xE0,0xFC,0x1F,0x83,0xF0,0x7E,0x0E,0xC3,0x9C,0x71,0xFC,0x1F,0x00, // '0'
	0x1E,0x07,0xC1,0xF8,0x77,0x0C,0xE0,0x1C,0x03,0x80,0x70,0x0E,0x01,0xC0,0x38,0x07,0x00,0xE0,0x1C,0x03,0x87,0xFF,0xFF,0xE0, // '1'
	0x1F,0x07,0xFC,0x71,0xCF,0x0E,0x00,0xE0,0x0E,0x01,0xE0,0x3C,0x07,0xC0,0xF8,0x1F,0x03,0xE0,0x3C,0x07,0x80,0xF0,0x0F,0xFF,0xFF,0xF0, // '2'
	0x0F,0x81,0xFF,0x0E,0x3C,0xF0,0xE0,0x07,0x00,0x38,0x03,0x80,0xF8,0x07,0xE0,0x03,0x80,0x0E,0x00,0x77,0x03,0xBC,0x1D,0xE1,0xC7,0xFE,0x0F,0xC0, // '3'
	0x01,0xE0,0x1F,0x00,0xF8,0x0F,0xC0,0xEE,0x07,0x70,0x73,0x87,0x1C,0x38,0xE3,0x87,0x38,0x39,0xFF,0xFF,0xFF,0x80,0x70,0x03,0x80,0x1C,0x00,0xE0, // '4'
	0x7F,0xE7,0xFE,0x70,0x07,0x00,0x70,0x07,0x00,0x77,0x87,0xFE,0x78,0xE0,0x07,0x00,0x70,0x07,0x00,0x7F,0x07,0x78,0xE3,0xFC,0x1F,0x80, // '5'
	0x1F,0x07,0xF9,0xC7,0x38,0x6E,0x01,0xC0,0x3B,0xE7,0xFE,0xF1,0xDC,0x1F,0x83,0xF0,0x7E,0x0E,0xC1,0xDC,0x71,0xFE,0x1F,0x00, // '6'
	0xFF,0xFF,0xFC,0x03,0x80,0xE0,0x1C,0x07,0x00,0xE0,0x38,0x07,0x01,0xC0,0x38,0x07,0x00,0xC0,0x38,0x07,0x00,0xE0,0x1C,0x00, // '7'
	0x1F,0x83,0xFC,0x79,0xE7,0x0E,0x70,0xE7,0x0E,0x79,0xE3,0xFC,0x3F,0xC7,0x0E,0xE0,0x7E,0x07,0xE0,0x7E,0x07,0x70,0xE7,0xFE,0x1F,0x80, // '8'
	0x1F,0x83,0xFC,0x70,0xEE,0x0E,0xE0,0x7E,0x07,0xE0,0x7E,0x07,0x70,0xF7,0xFF,0x1F,0x70,0x07,0x00,0x6E,0x0E,0x71,0xE7,0xFC,0x1F,0x00, // '9'
	0xFF,0x80,0x00,0x1F,0xF0, // ':'
	0x77,0x70,0x00,0x00,0x07,0x77,0x33,0x66, // ';'
	0x00,0x00,0x07,0x01,0xF0,0xFE,0x3F,0x8F,0xC0,0xF0,0x0E,0x00,0xFC,0x03,0xF8,0x0F,0xE0,0x1F,0x00,0x70,0x00, // '<'
	0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF, // '='
	0x00,0x0E,0x00,0xF8,0x07,0xF0,0x1F,0xC0,0x3F,0x00,0xF0,0x07,0x03,0xF1,0xFE,0x7F,0x0F,0x80,0xE0,0x00,0x00, // '>'
	0x0F,0xC1,0xFF,0x8E,0x1C,0xF0,0x77,0x03,0x80,0x1C,0x01,0xE0,0x1E,0x01,0xE0,0x1E,0x01,0xE0,0x0E,0x00,0x00,0x00,0x00,0x1C,0x00,0xE0,0x07,0x00, // '?'
	0x00,0xFE,0x00,0x1F,0xFC,0x03,0xC0,0x78,0x38,0x00,0xE3,0x87,0x93,0x18,0xFE,0x9D,0x8E,0x3C,0x6C,0x60,0xE3,0xC7,0x07,0x1E,0x30,0x38,0xF1,0x81,0x87,0x8C,0x0C,0x7C,0x60,0xE3,0x63,0x8F,0x3B,0x0F,0xDF,0x8C,0x3C,0x70,0x70,0x00,0x01,0xC0,0x04,0x07,0x01,0xE0,0x1F,0xFE,0x00,0x3F,0x80,0x00, // '@'
	0x03,0xE0,0x01,0xF0,0x00,0xFC,0x00,0xEE,0x00,0x77,0x00,0x39,0xC0,0x38,0xE0,0x1C,0x70,0x0E,0x1C,0x0E,0x0E,0x07,0xFF,0x03,0xFF,0xC3,0x80,0xE1,0xC0,0x71,0xE0,0x1C,0xF0,0x0E,0x70,0x07,0x80, // 'A'
	0xFF,0xC3,0xFF,0xCE,0x07,0xB8,0x0E,0xE0,0x3B,0x80,0xEE,0x07,0x3F,0xF8,0xFF,0xF3,0x80,0xEE,0x01,0xF8,0x07,0xE0,0x1F,0x80,0x7E,0x03,0xBF,0xFE,0xFF,0xE0, // 'B'
	0x07,0xE0,0x1F,0xF8,0x3C,0x3C,0x70,0x1E,0x70,0x0C,0xE0,0x00,0xE0,0x00,0xE0,0x00,0xE0,0x00,0xE0,0x00,0xE0,0x00,0xE0,0x00,0x70,0x0E,0x70,0x1E,0x3C,0x3C,0x1F,0xF8,0x07,0xE0, // 'C'
	0xFF,0x83,0xFF,0x8E,0x0F,0x38,0x1E,0xE0,0x3B,0x80,0x7E,0x01,0xF8,0x07,0xE0,0x1F,0x80,0x7E,0x01,0xF8,0x07,0xE0,0x3B,0x81,0xEE,0x0F,0x3F,0xF8,0xFF,0x80, // 'D'
	0xFF,0xFB,0xFF,0xEE,0x00,0x38,0x00,0xE0,0x03,0x80,0x0E,0x00,0x38,0x00,0xFF,0xFB,0xFF,0xEE,0x00,0x38,0x00,0xE0,0x03,0x80,0x0E,0x00,0x3F,0xFF,0xFF,0xFC, // 'E'
	0xFF,0xFF,0xFF,0xE0,0x0E,0x00,0xE0,0x0E,0x00,0xE0,0x0E,0x00,0xE0,0x0F,0xFF,0xFF,0xFE,0x00,0xE0,0x0E,0x00,0xE0,0x0E,0x00,0xE0,0x00, // 'F'
	0x07,0xF0,0x1F,0xFC,0x3C,0x3E,0x70,0x0F,0x70,0x0C,0xE0,0x00,0xE0,0x00,0xE0,0x00,0xE0,0x7F,0xE0,0x7F,0xE0,0x07,0xE0,0x07,0x70,0x07,0x78,0x07,0x3C,0x1F,0x1F,0xFE,0x07,0xF0, // 'G'
	0xE0,0x1F,0x80,0x7E,0x01,0xF8,0x07,0xE0,0x1F,0x80,0x7E,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x01,0xF8,0x07,0xE0,0x1F,0x80,0x7E,0x01,0xF8,0x07,0xE0,0x1C, // 'H'
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE0, // 'I'
	0x07,0xF0,0x7F,0x00,0x70,0x07,0x00,0x70,0x07,0x00,0x70,0x07,0x00,0x70,0x07,0x00,0x70,0x07,0x30,0x77,0x87,0x78,0xE3,0xFE,0x0F,0x80, // 'J'
	0xE0,0x3C,0xE0,0x78,0xE0,0xF0,0xE1,0xE0,0xE3,0xC0,0xE7,0x80,0xEF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x80,0xE3,0xC0,0xE1,0xE0,0xE0,0xF0,0xE0,0xF0,0xE0,0x78,0xE0,0x3C,0xE0,0x1E, // 'K'
	0xE0,0x07,0x00,0x38,0x01,0xC0,0x0E,0x00,0x70,0x03,0x80,0x1C,0x00,0xE0,0x07,0x00,0x38,0x01,0xC0,0x0E,0x00,0x70,0x03,0x80,0x1F,0xFF,0xFF,0xF8, // 'L'
	0xF8,0x0F,0xFC,0x07,0xFE,0x03,0xFF,0x83,0xFF,0xC1,0xFF,0x60,0xDF,0xB8,0xEF,0xDC,0x77,0xE6,0x33,0xF3,0xB9,0xF9,0xDC,0xFC,0x6E,0x7E,0x36,0x3F,0x1F,0x1F,0x8F,0x8F,0xC3,0x87,0xE1,0xC3,0x80, // 'M'
	0xF0,0x1F,0xE0,0x7F,0x81,0xFF,0x07,0xFC,0x1F,0xB8,0x7E,0xE1,0xF9,0xC7,0xE7,0x9F,0x8E,0x7E,0x3D,0xF8,0x77,0xE0,0xFF,0x83,0xFE,0x07,0xF8,0x1F,0xE0,0x3C, // 'N'
	0x07,0xE0,0x1F,0xF8,0x3C,0x3C,0x70,0x0E,0x70,0x0E,0xE0,0x07,0xE0,0x07,0xE0,0x07,0xE0,0x07,0xE0,0x07,0xE0,0x07,0xE0,0x07,0x70,0x0E,0x78,0x1E,0x3C,0x3C,0x1F,0xF8,0x07,0xE0, // 'O'
	0xFF,0xC7,0xFF,0xB8,0x1D,0xC0,0x7E,0x03,0xF0,0x1F,0x80,0xFC,0x07,0xE0,0x77,0xFF,0x3F,0xF1,0xC0,0x0E,0x00,0x70,0x03,0x80,0x1C,0x00,0xE0,0x00, // 'P'
	0x07,0xE0,0x1F,0xF8,0x3C,0x3C,0x70,0x0E,0x70,0x0E,0xE0,0x07,0xE0,0x07,0xE0,0x07,0xE0,0x07,0xE0,0x07,0xE0,0x07,0xE0,0x07,0x70,0x0F,0x78,0x1E,0x3C,0x3E,0x1F,0xFC,0x0F,0xF8,0x01,0xE0,0x01,0xE0,0x00,0xE0,0x00,0xFE,0x00,0x3E, // 'Q'
	0xFF,0xE1,0xFF,0xF3,0x80,0xE7,0x00,0xEE,0x01,0xDC,0x03,0xB8,0x07,0x70,0x1C,0xFF,0xF9,0xFF,0xC3,0x83,0x87,0x07,0x8E,0x07,0x1C,0x0F,0x38,0x0E,0x70,0x0E,0xE0,0x1E, // 'R'
	0x0F,0xE0,0xFF,0xC3,0x87,0x9C,0x0F,0x70,0x01,0xE0,0x07,0xF0,0x0F,0xF8,0x1F,0xF8,0x1F,0xE0,0x07,0xC0,0x07,0x20,0x1F,0xC0,0x77,0x83,0x8F,0xFE,0x0F,0xE0, // 'S'
	0xFF,0xFF,0xFF,0xF0,0x38,0x00,0xE0,0x03,0x80,0x0E,0x00,0x38,0x00,0xE0,0x03,0x80,0x0E,0x00,0x38,0x00,0xE0,0x03,0x80,0x0E,0x00,0x38,0x00,0xE0,0x03,0x80, // 'T'
	0xE0,0x1F,0x80,0x7E,0x01,0xF8,0x07,0xE0,0x1F,0x80,0x7E,0x01,0xF8,0x07,0xE0,0x1F,0x80,0x7E,0x01,0xF8,0x07,0xE0,0x1D,0xC0,0xE7,0x87,0x8F,0xFC,0x0F,0xC0, // 'U'
	0xF0,0x0F,0x70,0x0E,0x78,0x1E,0x78,0x1E,0x38,0x1C,0x38,0x1C,0x1C,0x3C,0x1C,0x38,0x1C,0x38,0x0E,0x70,0x0E,0x70,0x0E,0x70,0x07,0xE0,0x07,0xE0,0x07,0xE0,0x03,0xC0,0x03,0xC0, // 'V'
	0xF0,0x78,0x1F,0xE0,0xF8,0x39,0xC1,0xF0,0x73,0x83,0xE1,0xE7,0x8F,0xC3,0xCF,0x1D,0xC7,0x0E,0x3B,0x8E,0x1C,0x77,0x1C,0x39,0xCE,0x78,0x7B,0x8C,0xE0,0x77,0x1D,0xC0,0xEE,0x3B,0x81,0xD8,0x7F,0x03,0xF0,0xFC,0x03,0xE0,0xF8,0x07,0xC1,0xF0,0x0F,0x03,0xE0, // 'W'
	0x78,0x1E,0x38,0x1C,0x3C,0x3C,0x1C,0x38,0x0E,0x78,0x0F,0xF0,0x07,0xE0,0x03,0xE0,0x03,0xE0,0x07,0xE0,0x0F,0xF0,0x0E,0x70,0x1E,0x78,0x3C,0x3C,0x38,0x1C,0x78,0x1E,0x70,0x0E, // 'X'
	0xF0,0x1E,0xF0,0x78,0xE0,0xE1,0xE3,0xC1,0xC7,0x03,0xDE,0x03,0xB8,0x03,0xE0,0x07,0xC0,0x07,0x00,0x0E,0x00,0x1C,0x00,0x38,0x00,0x70,0x00,0xE0,0x01,0xC0,0x03,0x80, // 'Y'
	0x7F,0xFB,0xFF,0xC0,0x1E,0x01,0xE0,0x1E,0x00,0xE0,0x0F,0x00,0xF0,0x07,0x00,0x78,0x07,0x80,0x38,0x03,0xC0,0x3C,0x03,0xC0,0x1F,0xFF,0xFF,0xF8, // 'Z'
	0xFF,0xFF,0x87,0x0E,0x1C,0x38,0x70,0xE1,0xC3,0x87,0x0E,0x1C,0x38,0x70,0xE1,0xC3,0x87,0x0E,0x1F,0xFF,0x80, // '['
	0xE0,0xC1,0xC3,0x87,0x0E,0x1C,0x18,0x38,0x70,0xE1,0xC1,0x83,0x87,0x0E,0x1C,0x38, // '\'
	0xFF,0xF1,0xC7,0x1C,0x71,0xC7,0x1C,0x71,0xC7,0x1C,0x71,0xC7,0x1C,0x71,0xC7,0x1F,0xFF,0xC0, // ']'
	0x07,0x80,0x3F,0x00,0xFC,0x03,0x30,0x1C,0xE0,0x61,0x83,0x87,0x0E,0x0C,0x70,0x39,0xC0,0xE6,0x01,0x80, // '^'
	0x7F,0xFE, // '_'
	0xF1,0xC1,0x83, // '`'
	0x1F,0x83,0xFE,0x1C,0x78,0xE1,0xC0,0x0E,0x1F,0xF1,0xFF,0x9E,0x1C,0xE0,0xE7,0x0F,0x38,0xF8,0xFE,0xF3,0xC7,0x80, // 'a'
	0x70,0x03,0x80,0x1C,0x00,0xE0,0x07,0x00,0x39,0xE1,0xFF,0xCF,0x0E,0x78,0x7B,0x81,0xDC,0x0E,0xE0,0x77,0x03,0xB8,0x1D,0xE1,0xEF,0x0E,0x7F,0xF3,0x9E,0x00, // 'b'
	0x1F,0x03,0xFC,0x71,0xEE,0x0E,0xE0,0x0E,0x00,0xE0,0x0E,0x00,0xE0,0x0E,0x0E,0x71,0xE3,0xFC,0x1F,0x00, // 'c'
	0x00,0x70,0x07,0x00,0x70,0x07,0x00,0x71,0xE7,0x7F,0xF7,0x0F,0xF0,0xFE,0x07,0xE0,0x7E,0x07,0xE0,0x7E,0x07,0xE0,0xF7,0x1F,0x7F,0xF1,0xE7, // 'd'
	0x1F,0x07,0xF1,0xC7,0x70,0x7E,0x0F,0xFF,0xFF,0xFF,0x00,0xE0,0x1C,0x11,0xC7,0x1F,0xE1,0xF0, // 'e'
	0x1F,0x3F,0x38,0x38,0x38,0xFF,0xFF,0x38,0x38,0x38,0x38,0x38,0x38,0x38,0x38,0x38,0x38,0x38, // 'f'
	0x1E,0x77,0xFF,0x70,0xFF,0x0F,0xE0,0x7E,0x07,0xE0,0x7E,0x07,0xE0,0x7F,0x0F,0x70,0xF7,0xFF,0x1E,0x70,0x07,0x20,0x77,0x0E,0x7F,0xE1,0xF8, // 'g'
	0xE0,0x1C,0x03,0x80,0x70,0x0E,0x01,0xCF,0x3F,0xF7,0x8F,0xF0,0xFC,0x1F,0x83,0xF0,0x7E,0x0F,0xC1,0xF8,0x3F,0x07,0xE0,0xFC,0x1C, // 'h'
	0xFC,0x01,0xFF,0xFF,0xFF,0xFF,0xFC, // 'i'
	0x39,0xC0,0x00,0x1C,0xE7,0x39,0xCE,0x73,0x9C,0xE7,0x39,0xCE,0x73,0xFF,0xC0, // 'j'
	0xE0,0x0E,0x00,0xE0,0x0E,0x00,0xE0,0x0E,0x1E,0xE3,0xCE,0x78,0xE7,0x0E,0xE0,0xFE,0x0F,0xE0,0xF7,0x0E,0x78,0xE3,0x8E,0x3C,0xE1,0xCE,0x0E, // 'k'
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC, // 'l'
	0xE7,0x87,0x9F,0xFB,0xFB,0xC7,0xC7,0xF8,0x70,0x7E,0x0E,0x0F,0xC1,0xC1,0xF8,0x38,0x3F,0x07,0x07,0xE0,0xE0,0xFC,0x1C,0x1F,0x83,0x83,0xF0,0x70,0x7E,0x0E,0x0E, // 'm'
	0xE7,0x9F,0xFB,0xC7,0xF8,0x7E,0x0F,0xC1,0xF8,0x3F,0x07,0xE0,0xFC,0x1F,0x83,0xF0,0x7E,0x0E, // 'n'
	0x0F,0x81,0xFF,0x1E,0x3C,0xE0,0xEE,0x03,0xF0,0x1F,0x80,0xFC,0x07,0xE0,0x3B,0x83,0x9E,0x3C,0x7F,0xC0,0xF8,0x00, // 'o'
	0x73,0xC3,0xFF,0x9E,0x1C,0xF0,0xF7,0x03,0xB8,0x1D,0xC0,0xEE,0x07,0x70,0x3B,0xC3,0xDE,0x1C,0xFF,0xE7,0x3C,0x38,0x01,0xC0,0x0E,0x00,0x70,0x03,0x80,0x00, // 'p'
	0x1E,0x77,0xFF,0x70,0xFF,0x0F,0xE0,0x7E,0x07,0xE0,0x7E,0x07,0xE0,0x7F,0x0F,0x70,0xF7,0xFF,0x1E,0x70,0x07,0x00,0x70,0x07,0x00,0x70,0x07, // 'q'
	0xE7,0xEF,0xFF,0xF0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0, // 'r'
	0x1F,0x83,0xFE,0x70,0xE7,0x04,0x78,0x07,0xF8,0x3F,0xE0,0x7F,0x00,0xF2,0x07,0x70,0xF7,0xFE,0x1F,0x80, // 's'
	0x18,0x30,0xE7,0xFF,0xE7,0x0E,0x1C,0x38,0x70,0xE1,0xC3,0x87,0x0F,0x8F, // 't'
	0xE0,0xEE,0x0E,0xE0,0xEE,0x0E,0xE0,0xEE,0x0E,0xE0,0xEE,0x0E,0xE0,0xEE,0x1E,0xF1,0xE7,0xFE,0x3C,0xE0, // 'u'
	0xF0,0x39,0xC1,0xE7,0x07,0x1E,0x1C,0x38,0xF0,0xE3,0x83,0xCE,0x07,0x78,0x1D,0xC0,0x77,0x00,0xF8,0x03,0xE0,0x0F,0x80, // 'v'
	0x70,0xF0,0x77,0x0F,0x0E,0x38,0xF8,0xE3,0x8D,0x8E,0x39,0xD8,0xE3,0x9D,0x9C,0x19,0x9D,0xC1,0xD8,0xDC,0x1F,0x8D,0xC1,0xF8,0xD8,0x0F,0x0F,0x80,0xF0,0x78,0x0F,0x07,0x80, // 'w'
	0x70,0x79,0xE3,0xC3,0xCE,0x07,0x70,0x0F,0xC0,0x3E,0x00,0x78,0x03,0xE0,0x1F,0xC0,0x77,0x83,0x8E,0x1E,0x1C,0x70,0x78, // 'x'
	0x78,0x3C,0xE0,0xE3,0x83,0x8F,0x1E,0x1C,0x70,0x71,0xC1,0xC7,0x03,0xB8,0x0E,0xE0,0x1B,0x00,0x7C,0x01,0xF0,0x03,0x80,0x0E,0x00,0x78,0x01,0xC0,0x3E,0x00,0xF0,0x00, // 'y'
	0x7F,0xDF,0xF0,0x3C,0x1E,0x07,0x03,0xC1,0xE0,0xF0,0x3C,0x1E,0x0F,0x03,0xFF,0xFF,0xC0, // 'z'
	0x07,0x87,0xC7,0x83,0x81,0xC0,0xE0,0x70,0x38,0x1C,0x0E,0x0F,0x1F,0x0F,0x81,0xE0,0x70,0x38,0x1C,0x0E,0x07,0x03,0x81,0xE0,0xFC,0x1E, // '{'
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8, // '|'
	0xF8,0x7C,0x0F,0x03,0x81,0xC0,0xE0,0x70,0x38,0x1C,0x0E,0x07,0x81,0xF0,0xF8,0xF0,0x70,0x38,0x1C,0x0E,0x07,0x03,0x83,0xC7,0xE3,0xE0 // '}'
};
const GFXglyph Arimo_Bold_24Glyphs[] PROGMEM = {
// bitmapOffset, width, height, xAdvance, xOffset, yOffset
	  {     0,   1,   1,   8,    0,    0 }, // ' '
	  {     1,   3,  17,   8,    2,  -17 }, // '!'
	  {     8,   8,   6,  13,    2,  -17 }, // '"'
	  {    14,  13,  17,  16,    1,  -17 }, // '#'
	  {    42,  13,  19,  15,    0,  -18 }, // '$'
	  {    73,  20,  17,  23,    1,  -17 }, // '%'
	  {   116,  16,  17,  18,    1,  -17 }, // '&'
	  {   150,   3,   6,   6,    1,  -17 }, // '''
	  {   153,   7,  23,   9,    1,  -18 }, // '('
	  {   174,   7,  23,   9,    0,  -18 }, // ')'
	  {   195,  11,   9,  11,    0,  -17 }, // '*'
	  {   208,  12,  12,  15,    1,  -14 }, // '+'
	  {   226,   4,   7,   8,    1,   -3 }, // ','
	  {   230,   6,   3,   9,    1,   -8 }, // '-'
	  {   233,   3,   3,   8,    2,   -3 }, // '.'
	  {   235,   7,  18,   8,    0,  -18 }, // '/'
	  {   251,  11,  17,  14,    1,  -17 }, // '0'
	  {   275,  11,  17,  15,    2,  -17 }, // '1'
	  {   299,  12,  17,  15,    1,  -17 }, // '2'
	  {   325,  13,  17,  15,    0,  -17 }, // '3'
	  {   353,  13,  17,  16,    1,  -17 }, // '4'
	  {   381,  12,  17,  14,    0,  -17 }, // '5'
	  {   407,  11,  17,  14,    1,  -17 }, // '6'
	  {   431,  11,  17,  14,    1,  -17 }, // '7'
	  {   455,  12,  17,  15,    1,  -17 }, // '8'
	  {   481,  12,  17,  15,    1,  -17 }, // '9'
	  {   507,   3,  12,   8,    2,  -12 }, // ':'
	  {   512,   4,  16,   8,    1,  -12 }, // ';'
	  {   520,  12,  14,  15,    1,  -15 }, // '<'
	  {   541,  12,   8,  15,    1,  -13 }, // '='
	  {   553,  12,  14,  15,    1,  -15 }, // '>'
	  {   574,  13,  17,  15,    0,  -17 }, // '?'
	  {   602,  21,  21,  24,    1,  -17 }, // '@'
	  {   658,  17,  17,  18,    0,  -17 }, // 'A'
	  {   695,  14,  17,  18,    2,  -17 }, // 'B'
	  {   725,  16,  17,  18,    1,  -17 }, // 'C'
	  {   759,  14,  17,  18,    2,  -17 }, // 'D'
	  {   789,  14,  17,  17,    2,  -17 }, // 'E'
	  {   819,  12,  17,  16,    2,  -17 }, // 'F'
	  {   845,  16,  17,  19,    1,  -17 }, // 'G'
	  {   879,  14,  17,  19,    2,  -17 }, // 'H'
	  {   909,   3,  17,   8,    2,  -17 }, // 'I'
	  {   916,  12,  17,  14,   -1,  -17 }, // 'J'
	  {   942,  16,  17,  18,    2,  -17 }, // 'K'
	  {   976,  13,  17,  16,    2,  -17 }, // 'L'
	  {  1004,  17,  17,  22,    2,  -17 }, // 'M'
	  {  1041,  14,  17,  19,    2,  -17 }, // 'N'
	  {  1071,  16,  17,  19,    1,  -17 }, // 'O'
	  {  1105,  13,  17,  17,    2,  -17 }, // 'P'
	  {  1133,  16,  22,  19,    1,  -17 }, // 'Q'
	  {  1177,  15,  17,  18,    2,  -17 }, // 'R'
	  {  1209,  14,  17,  16,    0,  -17 }, // 'S'
	  {  1239,  14,  17,  17,    1,  -17 }, // 'T'
	  {  1269,  14,  17,  17,    1,  -17 }, // 'U'
	  {  1299,  16,  17,  17,    0,  -17 }, // 'V'
	  {  1333,  23,  17,  24,    0,  -17 }, // 'W'
	  {  1382,  16,  17,  17,    0,  -17 }, // 'X'
	  {  1416,  15,  17,  16,    0,  -17 }, // 'Y'
	  {  1448,  13,  17,  16,    1,  -17 }, // 'Z'
	  {  1476,   7,  23,   9,    1,  -18 }, // '['
	  {  1497,   7,  18,   8,    0,  -18 }, // '\'
	  {  1513,   6,  23,   9,    1,  -18 }, // ']'
	  {  1531,  14,  11,  15,    0,  -17 }, // '^'
	  {  1551,  15,   1,  14,   -1,    2 }, // '_'
	  {  1553,   6,   4,   9,    1,  -18 }, // '`'
	  {  1556,  13,  13,  15,    1,  -13 }, // 'a'
	  {  1578,  13,  18,  16,    1,  -18 }, // 'b'
	  {  1608,  12,  13,  14,    1,  -13 }, // 'c'
	  {  1628,  12,  18,  16,    1,  -18 }, // 'd'
	  {  1655,  11,  13,  14,    1,  -13 }, // 'e'
	  {  1673,   8,  18,  10,    1,  -18 }, // 'f'
	  {  1691,  12,  18,  16,    1,  -13 }, // 'g'
	  {  1718,  11,  18,  15,    2,  -18 }, // 'h'
	  {  1743,   3,  18,   8,    2,  -18 }, // 'i'
	  {  1750,   5,  23,   8,    0,  -18 }, // 'j'
	  {  1765,  12,  18,  14,    2,  -18 }, // 'k'
	  {  1792,   3,  18,   8,    2,  -18 }, // 'l'
	  {  1799,  19,  13,  23,    2,  -13 }, // 'm'
	  {  1830,  11,  13,  15,    2,  -13 }, // 'n'
	  {  1848,  13,  13,  16,    1,  -13 }, // 'o'
	  {  1870,  13,  18,  16,    1,  -13 }, // 'p'
	  {  1900,  12,  18,  16,    1,  -13 }, // 'q'
	  {  1927,   8,  13,  11,    2,  -13 }, // 'r'
	  {  1940,  12,  13,  14,    0,  -13 }, // 's'
	  {  1960,   7,  16,  10,    1,  -16 }, // 't'
	  {  1974,  12,  13,  15,    1,  -13 }, // 'u'
	  {  1994,  14,  13,  14,    0,  -13 }, // 'v'
	  {  2017,  20,  13,  20,   -1,  -13 }, // 'w'
	  {  2050,  14,  13,  14,    0,  -13 }, // 'x'
	  {  2073,  14,  18,  14,   -1,  -13 }, // 'y'
	  {  2105,  10,  13,  13,    1,  -13 }, // 'z'
	  {  2122,   9,  23,  11,    0,  -18 }, // '{'
	  {  2148,   3,  23,   8,    2,  -18 }, // '|'
	  {  2157,   9,  23,  11,    0,  -18 } // '}'
};
const GFXfont Arimo_Bold_24 PROGMEM = {
(uint8_t  *)Arimo_Bold_24Bitmaps,(GFXglyph *)Arimo_Bold_24Glyphs,0x20, 0x7E, 28};
