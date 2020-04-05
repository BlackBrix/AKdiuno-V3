/*
See rights and use declaration in License.h
This library has been modified for the Maple Mini
*/

#ifndef _ADAFRUIT_ILI9481H_
#define _ADAFRUIT_ILI9481H_

#include "Arduino.h"
#include "Print.h"
#include <Adafruit_GFX.h>
//#include <Adafruit_GFX_AS.h>
#include <avr/pgmspace.h>

#define ILI9481_TFTWIDTH  320
#define ILI9481_TFTHEIGHT 480

#define ILI9481_NOP     0x00
#define ILI9481_SWRESET 0x01
#define ILI9481_RDDID   0xD3
#define ILI9481_RDDST   0x09

#define ILI9481_SLPIN   0x10
#define ILI9481_SLPOUT  0x11
#define ILI9481_PTLON   0x12
#define ILI9481_NORON   0x13

#define ILI9481_RDMODE  0x0A
#define ILI9481_RDMADCTL  0x0B
#define ILI9481_RDPIXFMT  0x0C
#define ILI9481_RDIMGFMT  0x0D
#define ILI9481_RDSELFDIAG  0x0F

#define ILI9481_INVOFF  0x20
#define ILI9481_INVON   0x21
#define ILI9481_GAMMASET 0x26
#define ILI9481_DISPOFF 0x28
#define ILI9481_DISPON  0x29

#define ILI9481_CASET   0x2A
#define ILI9481_PASET   0x2B
#define ILI9481_RAMWR   0x2C
#define ILI9481_RAMRD   0x2E

#define ILI9481_PTLAR   0x30
#define ILI9481_MADCTL  0x36
#define ILI9481_PIXFMT  0x3A

#define ILI9481_FRMCTR1 0xB1
#define ILI9481_FRMCTR2 0xB2
#define ILI9481_FRMCTR3 0xB3
#define ILI9481_INVCTR  0xB4
#define ILI9481_DFUNCTR 0xB6

#define ILI9481_PWCTR1  0xC0
#define ILI9481_PWCTR2  0xC1
#define ILI9481_PWCTR3  0xC2
#define ILI9481_PWCTR4  0xC3
#define ILI9481_PWCTR5  0xC4
#define ILI9481_VMCTR1  0xC5
#define ILI9481_VMCTR2  0xC7

#define ILI9481_RDID1   0xDA
#define ILI9481_RDID2   0xDB
#define ILI9481_RDID3   0xDC
#define ILI9481_RDID4   0xDD

#define ILI9481_GMCTRP1 0xE0
#define ILI9481_GMCTRN1 0xE1
/*
#define ILI9481_PWCTR6  0xFC

*/
										//	 R	  G	   B
										//	 B	  G	   R
// Color definitions
#define ILI9481_BLACK       0x0000      /*   0,   0,   0 -*/
#define ILI9481_NAVY        0x7800      /*   0,   0, 128 -*/
#define ILI9481_DARKGREEN   0x03E0      /*   0, 128,   0 -*/
#define ILI9481_DARKCYAN    0x7BE0      /*   0, 128, 128 -*/
#define ILI9481_MAROON      0x000F      /* 128,   0,   0 -*/
#define ILI9481_PURPLE      0x780F      /* 128,   0, 128 -*/
#define ILI9481_OLIVE       0x03EF      /* 128, 128,   0 -*/
#define ILI9481_LIGHTGREY   0xC618      /* 192, 192, 192 -*/
#define ILI9481_DARKGREY    0x7BEF      /* 128, 128, 128 -*/
#define ILI9481_BLUE        0xF800      /*   0,   0, 255 -*/
#define ILI9481_GREEN       0x07E0      /*   0, 255,   0 -*/
#define ILI9481_CYAN        0xFFE0      /*   0, 255, 255 -*/
#define ILI9481_RED         0x001F      /* 255,   0,   0 -*/
#define ILI9481_MAGENTA     0xF81F      /* 255,   0, 255 -*/
#define ILI9481_YELLOW      0x07FF      /* 255, 255,   0 -*/
#define ILI9481_WHITE       0xFFFF      /* 255, 255, 255 -*/
#define ILI9481_ORANGE      0xFD20      /* 255, 165,   0 */
#define ILI9481_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define ILI9481_PINK        0xF81F

/*
Define pins and Output Data Registers
*/

//Port data |D7 |D6 |D5 |D4 |D3 |D2 |D1 |D0 |
//Pin stm32 |PA7|PA6|PA5|PA4|PA3|PA2|PC1|PA0|
//Control pins |RD |WR |RS |CS |RST|
//Pin stm32    |PB4|PB5|PB6|PB7|PB8|
#define TFT_CNTRL      GPIOB
#define TFT_DATA       GPIOA
#define TFT_RD         PB4
#define TFT_WR         PB5
#define TFT_RS         PB6
#define TFT_CS         PB7
#define TFT_RST        PB8
#define TFT_RD_MASK    digitalPinToBitMask(TFT_RD)
#define TFT_WR_MASK    digitalPinToBitMask(TFT_WR)
#define TFT_RS_MASK    digitalPinToBitMask(TFT_RS)
#define TFT_CS_MASK    digitalPinToBitMask(TFT_CS)

#define RD_ACTIVE    TFT_CNTRL->regs->BRR  = TFT_RD_MASK
#define RD_IDLE      TFT_CNTRL->regs->BSRR = TFT_RD_MASK
#define WR_ACTIVE    TFT_CNTRL->regs->BRR  = TFT_WR_MASK
#define WR_IDLE      TFT_CNTRL->regs->BSRR = TFT_WR_MASK
#define CD_COMMAND   TFT_CNTRL->regs->BRR  = TFT_RS_MASK
#define CD_DATA      TFT_CNTRL->regs->BSRR = TFT_RS_MASK
#define CS_ACTIVE    TFT_CNTRL->regs->BRR  = TFT_CS_MASK
#define CS_IDLE      TFT_CNTRL->regs->BSRR  = TFT_CS_MASK

#ifndef RD_STROBE
 #define RD_STROBE  {RD_ACTIVE; RD_IDLE;}
#endif
#define WR_STROBE { WR_ACTIVE; WR_IDLE; }
#define swap(a, b) { int16_t t = a; a = b; b = t; }

//Set pins to the 8 bit number
#define write8special(c) { TFT_DATA->regs->BSRR = ((~c)<<16) | (c); WR_STROBE; }


class Adafruit_ILI9481_8bit_STM : public Adafruit_GFX {

 public:

  Adafruit_ILI9481_8bit_STM(void);
  
  void     begin(void),
           setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1),
           pushColor(uint16_t color),
           fillScreen(uint16_t color),
		   #if defined (__STM32F1__)
		   drawLine(int16_t x0, int16_t y0,int16_t x1, int16_t y1, uint16_t color),
		   #endif
           drawPixel(int16_t x, int16_t y, uint16_t color),
           drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
           drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),
           fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
             uint16_t color),
           setRotation(uint8_t r),
           invertDisplay(boolean i);
  uint16_t color565(uint8_t r, uint8_t g, uint8_t b);

  /* These are not for current use, 8-bit protocol only! */
  //uint8_t  readdata(void),
   uint8_t readcommand8(uint8_t reg); 
   uint32_t readID(void);


 private:
  uint8_t  tabcolor;
  uint8_t  read8(void);
  void     setReadDataBus(void),
    setWriteDataBus(void),
    write8(uint8_t),
    writecommand(uint8_t c),
    writedata(uint8_t d),
    commandList(uint8_t *addr);




#if defined (__STM32F1__)
	 uint16_t lineBuffer[ILI9481_TFTHEIGHT]; // DMA buffer. 16bit color data per pixel
#endif
};

#endif //endif of the header file
