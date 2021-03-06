#ifndef MICROVIEW_H
#define MICROVIEW_H

#include <stdio.h>
#include <Arduino.h>
#include <avr/pgmspace.h>

#define swap(a, b) { uint8_t t = a; a = b; b = t; }

#define DC		8
#define RESET	12

// SS, SCK, MOSI already defined by original pins_arduino.h
//#define CS 		10
//#define SCK		13
//#define MOSI	11

#define BLACK 0
#define WHITE 1

#define LCDWIDTH			64
#define LCDHEIGHT			48
#define FONTHEADERSIZE		6

#define NORM				0
#define XOR					1

#define PAGE				0
#define ALL					1

#define WIDGETSTYLE0			0
#define WIDGETSTYLE1			1
#define WIDGETSTYLE2			2

#define SETCONTRAST 		0x81
#define DISPLAYALLONRESUME 	0xA4
#define DISPLAYALLON 		0xA5
#define NORMALDISPLAY 		0xA6
#define INVERTDISPLAY 		0xA7
#define DISPLAYOFF 			0xAE
#define DISPLAYON 			0xAF
#define SETDISPLAYOFFSET 	0xD3
#define SETCOMPINS 			0xDA
#define SETVCOMDESELECT		0xDB
#define SETDISPLAYCLOCKDIV 	0xD5
#define SETPRECHARGE 		0xD9
#define SETMULTIPLEX 		0xA8
#define SETLOWCOLUMN 		0x00
#define SETHIGHCOLUMN 		0x10
#define SETSTARTLINE 		0x40
#define MEMORYMODE 			0x20
#define COMSCANINC 			0xC0
#define COMSCANDEC 			0xC8
#define SEGREMAP 			0xA0
#define CHARGEPUMP 			0x8D
#define EXTERNALVCC 		0x01
#define SWITCHCAPVCC 		0x02

// Scroll
#define ACTIVATESCROLL 					0x2F
#define DEACTIVATESCROLL 				0x2E
#define SETVERTICALSCROLLAREA 			0xA3
#define RIGHTHORIZONTALSCROLL 			0x26
#define LEFT_HORIZONTALSCROLL 			0x27
#define VERTICALRIGHTHORIZONTALSCROLL	0x29
#define VERTICALLEFTHORIZONTALSCROLL	0x2A

typedef enum CMD {
	CMD_CLEAR,			//0
	CMD_INVERT,			//1
	CMD_CONTRAST,		//2
	CMD_DISPLAY,		//3
	CMD_SETCURSOR,		//4
	CMD_PIXEL,			//5
	CMD_LINE,			//6
	CMD_LINEH,			//7
	CMD_LINEV,			//8
	CMD_RECT,			//9
	CMD_RECTFILL,		//10
	CMD_CIRCLE,			//11
	CMD_CIRCLEFILL,		//12
	CMD_DRAWCHAR,		//13
	CMD_DRAWBITMAP,		//14
	CMD_GETLCDWIDTH,	//15
	CMD_GETLCDHEIGHT,	//16
	CMD_SETCOLOR,		//17
	CMD_SETDRAWMODE		//18
} commCommand_t;

class MicroView : public Print{
public:
	MicroView(void) {};
	void begin(void);

#if ARDUINO >= 100
	virtual size_t write(uint8_t);
#else
	virtual void   write(uint8_t);
#endif

	// RAW LCD functions
	void command(uint8_t c);
	void data(uint8_t c);
	void setColumnAddress(uint8_t add);
	void setPageAddress(uint8_t add);
	
	// LCD Draw functions
	void clear(uint8_t mode);
	void clear(uint8_t mode, uint8_t c);
	void invert(boolean inv);
	void contrast(uint8_t contrast);
	void display(void);
	void setCursor(uint8_t x, uint8_t y);
	void pixel(uint8_t x, uint8_t y);
	void pixel(uint8_t x, uint8_t y, uint8_t color, uint8_t mode);
	void line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
	void line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color, uint8_t mode);
	void lineH(uint8_t x, uint8_t y, uint8_t width);
	void lineH(uint8_t x, uint8_t y, uint8_t width, uint8_t color, uint8_t mode);
	void lineV(uint8_t x, uint8_t y, uint8_t height);
	void lineV(uint8_t x, uint8_t y, uint8_t height, uint8_t color, uint8_t mode);
	void rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
	void rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color , uint8_t mode);
	void rectFill(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
	void rectFill(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color , uint8_t mode);
	void circle(uint8_t x, uint8_t y, uint8_t radius);
	void circle(uint8_t x, uint8_t y, uint8_t radius, uint8_t color, uint8_t mode);
	void circleFill(uint8_t x0, uint8_t y0, uint8_t radius);
	void circleFill(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t color, uint8_t mode);
	void drawChar(uint8_t x, uint8_t y, uint8_t c);
	void drawChar(uint8_t x, uint8_t y, uint8_t c, uint8_t color, uint8_t mode);
	void drawBitmap(void);
	uint8_t getLCDWidth(void);
	uint8_t getLCDHeight(void);
	void setColor(uint8_t color);
	void setDrawMode(uint8_t mode);

	// Font functions
	uint8_t getFontWidth(void);
	uint8_t getFontHeight(void);
	uint8_t getTotalFonts(void);
	uint8_t getFontType(void);
	uint8_t setFontType(uint8_t type);
	uint8_t getFontStartChar(void);
	uint8_t getFontTotalChar(void);

	// LCD Rotate Scroll functions	
	void scrollRight(uint8_t start, uint8_t stop);
	void scrollLeft(uint8_t start, uint8_t stop);
	void scrollVertRight(uint8_t start, uint8_t stop);
	void scrollVertLeft(uint8_t start, uint8_t stop);
	void scrollStop(void);
	void flipVertical(boolean flip);
	void flipHorizontal(boolean flip);
	
	// Communication
	void checkComm(void);
	void doCmd(uint8_t index);
	
private:
	//uint8_t cs;
	//volatile uint8_t *mosiport, *sckport;
	volatile uint8_t *ssport, *dcport, *ssreg, *dcreg;	// use volatile because these are fixed location port address
	uint8_t mosipinmask, sckpinmask, sspinmask, dcpinmask;
	uint8_t foreColor,drawMode,fontWidth, fontHeight, fontType, fontStartChar, fontTotalChar, cursorX, cursorY;
	uint16_t fontMapWidth;
	//unsigned char *fontsPointer[TOTALFONTS];
	static const unsigned char *fontsPointer[];
	
	int readSerial(void);
};

class MicroViewWidget {
public:
	MicroViewWidget(uint8_t newx, uint8_t newy, int16_t min, int16_t max);
	uint8_t getX();
	uint8_t getY();
	void setX(uint8_t newx);
	void setY(uint8_t newy);
	
	int16_t getMinValue();
	int16_t getMaxValue();
	int16_t getValue();
	void setMaxValue(int16_t max);
	void setMinValue(int16_t max);
	void setValue(int16_t val);
	virtual void draw(){};
	virtual void drawFace(){};
	
private:
	uint8_t x;
	uint8_t y;
	int16_t maxValue;
	int16_t minValue;
	int16_t value;
};

class MicroViewSlider: public MicroViewWidget{
public:
	MicroViewSlider(uint8_t newx, uint8_t newy, int16_t min, int16_t max);
	MicroViewSlider(uint8_t newx, uint8_t newy, int16_t min, int16_t max, uint8_t sty);
	void draw();
	void drawFace();
private:
	uint8_t totalTicks, style;
	bool needFirstDraw;
	int16_t prevValue;
};

class MicroViewGauge: public MicroViewWidget{
public:
	MicroViewGauge(uint8_t newx, uint8_t newy, int16_t min, int16_t max);
	MicroViewGauge(uint8_t newx, uint8_t newy, int16_t min, int16_t max, uint8_t sty);
	void draw();
	void drawFace();
private:
	uint8_t radius, style;
	bool needFirstDraw;
	int16_t prevValue;
};

#define SPI_CLOCK_DIV4 0x00
#define SPI_CLOCK_DIV16 0x01
#define SPI_CLOCK_DIV64 0x02
#define SPI_CLOCK_DIV128 0x03
#define SPI_CLOCK_DIV2 0x04
#define SPI_CLOCK_DIV8 0x05
#define SPI_CLOCK_DIV32 0x06
//#define SPI_CLOCK_DIV64 0x07

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

#define SPI_MODE_MASK 0x0C  // CPOL = bit 3, CPHA = bit 2 on SPCR
#define SPI_CLOCK_MASK 0x03  // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK 0x01  // SPI2X = bit 0 on SPSR

class MVSPIClass {
public:
  inline static byte transfer(byte _data);

  // SPI Configuration methods

  inline static void attachInterrupt();
  inline static void detachInterrupt(); // Default

  static void begin(); // Default
  static void end();

  static void setBitOrder(uint8_t);
  static void setDataMode(uint8_t);
  static void setClockDivider(uint8_t);
};

extern MVSPIClass MVSPI;

byte MVSPIClass::transfer(byte _data) {
  SPDR = _data;
  while (!(SPSR & _BV(SPIF)))
    ;
  return SPDR;
}

void MVSPIClass::attachInterrupt() {
  SPCR |= _BV(SPIE);
}

void MVSPIClass::detachInterrupt() {
  SPCR &= ~_BV(SPIE);
}


extern MicroView uView;
#endif