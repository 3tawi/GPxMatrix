/* GPxMatrix
 * Adapted by 3tawi for 32x16, 32x32, 64x32, 128x32, 64x64, 128x64 RGB LED matrix panels
 * which can be found at https://github.com/3tawi/GPxMatrix
 * based on the adafruit RGB-matrix-panel library which can be found at https://github.com/adafruit/RGB-matrix-Panel
*/

#ifndef GPXMATRIX_H
#define GPXMATRIX_H

#include "Arduino.h"
#include "Adafruit_GFX.h"

typedef uint32_t PortType; // Formerly 'RwReg' but interfered w/CMCIS header

class GPxMatrix : public Adafruit_GFX {

public:
  GPxMatrix(uint8_t a, uint8_t b, uint8_t c, uint8_t clk, uint8_t lat,
                 uint8_t oe, boolean dbuf, uint8_t *pinlist = NULL);

  GPxMatrix(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t clk,
                 uint8_t lat, uint8_t oe, boolean dbuf, uint8_t width = 32, uint8_t *pinlist = NULL);

  GPxMatrix(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t clk,
                 uint8_t lat, uint8_t oe, boolean dbuf, uint8_t width = 32, uint8_t *pinlist = NULL);

  void begin(void);

  void drawPixel(int16_t x, int16_t y, uint16_t c);

  void xLine(int x0, int x1, int y, uint16_t color);

  void ellipse(int x, int y, int rx, int ry, uint16_t color);

  void fillEllipse(int x, int y, int rx, int ry, uint16_t color);

  void fillScreen(uint16_t c);

  void updateDisplay(void);

  void swapBuffers(boolean);

  void dumpMatrix(void);

  uint8_t *backBuffer(void);

  uint16_t Color333(uint8_t r, uint8_t g, uint8_t b);

  uint16_t Color444(uint8_t r, uint8_t g, uint8_t b);

  uint16_t Color888(uint8_t r, uint8_t g, uint8_t b);

  uint16_t Color888(uint8_t r, uint8_t g, uint8_t b, boolean gflag);

  uint16_t ColorHSV(long hue, uint8_t sat, uint8_t val, boolean gflag);


private:
  uint8_t *matrixbuff[2];     ///< Buffer pointers for double-buffering
  uint8_t nRows;              ///< Number of rows (derived from A/B/C/D pins)
  volatile uint8_t backindex; ///< Index (0-1) of back buffer
  volatile boolean swapflag;  ///< if true, swap on next vsync

  // Init/alloc code common to both constructors:
  void init(uint8_t rows, uint8_t a, uint8_t b, uint8_t c, uint8_t clk,
            uint8_t lat, uint8_t oe, boolean dbuf, uint8_t width, uint8_t *rgbpins);

  uint8_t _clk;       ///< RGB clock pin number
  uint8_t _lat;       ///< RGB latch pin number
  uint8_t _oe;        ///< Output enable pin number
  uint8_t _a;         ///< Address/row-select A pin number
  uint8_t _b;         ///< Address/row-select B pin number
  uint8_t _c;         ///< Address/row-select C pin number
  uint8_t _d;         ///< Address/row-select D pin number
  uint8_t _e;         ///< Address/row-select E pin number
  PortType clkmask;   ///< RGB clock pin bitmask
  PortType latmask;   ///< RGB latch pin bitmask
  PortType oemask;    ///< Output enable pin bitmask
  PortType addramask; ///< Address/row-select A pin bitmask
  PortType addrbmask; ///< Address/row-select B pin bitmask
  PortType addrcmask; ///< Address/row-select C pin bitmask
  PortType addrdmask; ///< Address/row-select D pin bitmask
  PortType addremask; ///< Address/row-select E pin bitmask
  // PORT register pointers (CLKPORT is hardcoded on AVR)
  volatile PortType *latport;   ///< RGB latch PORT register
  volatile PortType *oeport;    ///< Output enable PORT register
  volatile PortType *addraport; ///< Address/row-select A PORT register
  volatile PortType *addrbport; ///< Address/row-select B PORT register
  volatile PortType *addrcport; ///< Address/row-select C PORT register
  volatile PortType *addrdport; ///< Address/row-select D PORT register
  volatile PortType *addreport; ///< Address/row-select E PORT register

#if defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_ESP32)
  uint8_t rgbpins[6];           ///< Pin numbers for 2x R,G,B bits
  volatile PortType *outsetreg; ///< RGB PORT bit set register
  volatile PortType *outclrreg; ///< RGB PORT bit clear register
  PortType rgbclkmask;          ///< Mask of all RGB bits + CLK
  PortType expand[256];         ///< 6-to-32 bit converter table
#endif

  volatile uint8_t row;      ///< Row counter for interrupt handler
  volatile uint8_t plane;    ///< Bitplane counter for interrupt handler
  volatile uint8_t *buffptr; ///< Current RGB pointer for interrupt handler
};

#endif // GPxMatrix_H
