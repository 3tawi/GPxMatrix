/* GPxMatrix
 * Adapted by 3tawi for 32x16, 32x32, 64x32, 128x32, 64x64, 128x64 RGB LED matrix panels
 * which can be found at https://github.com/3tawi/GPxMatrix
 * based on the adafruit RGB-matrix-panel library which can be found at https://github.com/adafruit/RGB-matrix-Panel
 * RGBmatrixPanel Arduino library for Adafruit 16x32 and 32x32 RGB LED
 * matrix panels.  Pick one up at:
 * http://www.adafruit.com/products/420
 * http://www.adafruit.com/products/607
 *
 * A few notes on the implementation:
 *
 * - To control LED brightness, traditional PWM is eschewed in favor of
 *   Binary Code Modulation (aka Bit Angle Modulation), which operates
 *   through a succession of periods each twice the length of the preceeding
 *   one (rather than a direct linear count a la PWM).
 *   <a href="http://www.batsocks.co.uk/readme/art_bcm_1.htm">It's explained
 *   well here.</a>
 *   I was initially skeptical, but it works exceedingly well in practice!
 *   And this uses considerably fewer CPU cycles than software PWM.
 *
 * - Although many control pins are software-configurable in the user's
 *   code, a couple things are tied to specific PORT registers.  It's just
 *   a lot faster this way -- port lookups take time.  Please see the notes
 *   later regarding wiring on "alternative" Arduino boards.
 *
 * - A tiny bit of inline assembly language is used in the most speed-
 *   critical section.  The C++ compiler wasn't making optimal use of the
 *   instruction set in what seemed like an obvious chunk of code.  Since
 *   it's only a few short instructions, this loop is also "unrolled" --
 *   each iteration is stated explicitly, not through a control loop.
 *
 * - The library is stuck with some decisions that were made in the heyday
 *   of 2K RAM AVR microcontrollers. For newer projects on SAMD, ESP32 and
 *   other 32-bit devices, consider using
 *   <a
 * href="https://github.com/adafruit/Adafruit_Protomatter">Adafruit_Protomatter</a>
 *   instead, which offers more flexible support of size and color depth.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * @section dependencies Dependencies
 *
 * This library depends on <a
 * href="https://github.com/adafruit/Adafruit-GFX-Library"> Adafruit_GFX</a>
 * being present on your system. Please make sure you have installed the latest
 * version before using this library.
 *
 * @section author Author
 *
 * Written by Limor Fried/Ladyada & Phil Burgess/PaintYourDragon for
 * Adafruit Industries.
 *
 * @section license License
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */

#include "GPxMatrix.h"
#include "gamma.h"

#include "driver/timer.h"
#include "freertos/FreeRTOS.h"
#include <string.h>

#ifndef _swap_int16_t
#define _swap_int16_t(a, b)                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  } ///< 16-bit var swap
#endif
// A full PORT register is required for the data lines, though only the
// top 6 output bits are used.  For performance reasons, the port # cannot
// be changed via library calls, only by changing constants in the library.
// For similar reasons, the clock pin is only semi-configurable...it can
// be specified as any pin within a specific PORT register stated below.


#define nPlanes 4 ///< Bit depth per R,G,B (4 = (2^4)^3 = 4096 colors)

// The fact that the display driver interrupt stuff is tied to the
// singular Timer1 doesn't really take well to object orientation with
// multiple GPxMatrix instances.  The solution at present is to
// allow instances, but only one is active at any given time, via its
// begin() method.  The implementation is still incomplete in parts;
// the prior active panel really should be gracefully disabled, and a
// stop() method should perhaps be added...assuming multiple instances
// are even an actual need.
static GPxMatrix *activePanel = NULL; ///< Active RGB panel object

// Code common to both the 16x32 and 32x32 constructors:
void GPxMatrix::init(uint8_t rows, uint8_t a, uint8_t b, uint8_t c,
                          uint8_t clk, uint8_t lat, uint8_t oe, boolean dbuf,
                          uint8_t width, uint8_t *pinlist
) {
  // R1, G1, B1, R2, G2, B2 
  static const uint8_t defaultrgbpins[] = {27, 26, 14, 12, 25, 15};
  memcpy(rgbpins, pinlist ? pinlist : defaultrgbpins, sizeof rgbpins);

  nRows = rows; // Number of multiplexed rows; actual height is 2X this

  // Allocate and initialize matrix buffer:
  int buffsize = width * nRows * 3, // x3 = 3 bytes holds 4 planes "packed"
      allocsize = (dbuf == true) ? (buffsize * 2) : buffsize;
  if (NULL == (matrixbuff[0] = (uint8_t *)malloc(allocsize)))
    return;
  memset(matrixbuff[0], 0, allocsize);
  // If not double-buffered, both buffers then point to the same address:
  matrixbuff[1] = (dbuf == true) ? &matrixbuff[0][buffsize] : matrixbuff[0];

  // Save pin numbers for use by begin() method later.
  _a = a;
  _b = b;
  _c = c;
  _clk = clk;
  _lat = lat;
  _oe = oe;

  // Look up port registers and pin masks ahead of time,
  // avoids many slow digitalWrite() calls later.
  clkmask = digitalPinToBitMask(clk);
  latport = portOutputRegister(digitalPinToPort(lat));
  latmask = digitalPinToBitMask(lat);
  oeport = portOutputRegister(digitalPinToPort(oe));
  oemask = digitalPinToBitMask(oe);
  addraport = portOutputRegister(digitalPinToPort(a));
  addramask = digitalPinToBitMask(a);
  addrbport = portOutputRegister(digitalPinToPort(b));
  addrbmask = digitalPinToBitMask(b);
  addrcport = portOutputRegister(digitalPinToPort(c));
  addrcmask = digitalPinToBitMask(c);
  plane = nPlanes - 1;
  row = nRows - 1;
  swapflag = false;
  backindex = 0; // Array index of back buffer
}

// Constructor for 32x32 or 32x64 panel:
GPxMatrix::GPxMatrix(uint8_t a, uint8_t b, uint8_t c, uint8_t d,
                               uint8_t clk, uint8_t lat, uint8_t oe,
                               boolean dbuf, uint8_t width, uint8_t *pinlist)
    : Adafruit_GFX(width, 32) {

  init(16, a, b, c, clk, lat, oe, dbuf, width, pinlist);
  _d = d;
}

// Constructor for 64x64 or 32x64 panel:
GPxMatrix::GPxMatrix(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e,
                               uint8_t clk, uint8_t lat, uint8_t oe,
                               boolean dbuf, uint8_t width, uint8_t *pinlist)
    : Adafruit_GFX(width, 64) {

  init(32, a, b, c, clk, lat, oe, dbuf, width, pinlist);
  _d = d;
  _e = e;
}

IRAM_ATTR void IRQ_HANDLER(void *);

void GPxMatrix::begin(void) {

  backindex = 0;                       // Back buffer
  buffptr = matrixbuff[1 - backindex]; // -> front buffer
  activePanel = this;                  // For interrupt hander

  // Enable all comm & address pins as outputs, set default states:
  pinMode(_clk, OUTPUT);
  digitalWrite(_clk, LOW); // Low
  pinMode(_lat, OUTPUT);
  *latport &= ~latmask; // Low
  pinMode(_oe, OUTPUT);
  *oeport |= oemask; // High (disable output)
  pinMode(_a, OUTPUT);
  *addraport &= ~addramask; // Low
  pinMode(_b, OUTPUT);
  *addrbport &= ~addrbmask; // Low
  pinMode(_c, OUTPUT);
  *addrcport &= ~addrcmask; // Low
  if (nRows > 8) {
  addrdport = portOutputRegister(digitalPinToPort(_d));
  addrdmask = digitalPinToBitMask(_d);
    pinMode(_d, OUTPUT);
    *addrdport &= ~addrdmask; // Low
  }
  if (nRows > 16) {
  addreport = portOutputRegister(digitalPinToPort(_e));
  addremask = digitalPinToBitMask(_e);
    pinMode(_e, OUTPUT);
    *addreport &= ~addremask; // Low
  }
    outsetreg = &GPIO.out_w1ts;
    outclrreg = &GPIO.out_w1tc;

  PortType rgbmask[6];
  clkmask = rgbclkmask = digitalPinToBitMask(_clk);
  for (uint8_t i = 0; i < 6; i++) {
    pinMode(rgbpins[i], OUTPUT);
    rgbmask[i] = digitalPinToBitMask(rgbpins[i]); // Pin bit mask
    rgbclkmask |= rgbmask[i];                     // Add to RGB+CLK bit mask
  }
  for (int i = 0; i < 256; i++) {
    expand[i] = 0;
    if (i & 0x04)
      expand[i] |= rgbmask[0];
    if (i & 0x08)
      expand[i] |= rgbmask[1];
    if (i & 0x10)
      expand[i] |= rgbmask[2];
    if (i & 0x20)
      expand[i] |= rgbmask[3];
    if (i & 0x40)
      expand[i] |= rgbmask[4];
    if (i & 0x80)
      expand[i] |= rgbmask[5];
  }

  timer_config_t tim_config;
  tim_config.divider = 2; // Run Timer at 40 MHz
  tim_config.counter_dir = TIMER_COUNT_UP;
  tim_config.counter_en = TIMER_PAUSE;
  tim_config.alarm_en = TIMER_ALARM_EN;
  tim_config.auto_reload = TIMER_AUTORELOAD_EN;
  tim_config.intr_type = TIMER_INTR_LEVEL;

  timer_init(TIMER_GROUP_1, TIMER_0, &tim_config);
  /* Timer's counter will initially start from value below.
       Also, if auto_reload is set, this value will be automatically reload on
     alarm */
  timer_set_counter_value(TIMER_GROUP_1, TIMER_0, 0x00000000ULL);
  /* Configure the alarm value and the interrupt on alarm. */
  timer_set_alarm_value(TIMER_GROUP_1, TIMER_0, 10000);
  timer_enable_intr(TIMER_GROUP_1, TIMER_0);
  timer_isr_register(TIMER_GROUP_1, TIMER_0, IRQ_HANDLER, (void *)TIMER_0,
                     ESP_INTR_FLAG_IRAM, NULL);

  timer_start(TIMER_GROUP_1, TIMER_0);
}

// Original GPxMatrix library used 3/3/3 color.  Later version used
// 4/4/4.  Then Adafruit_GFX (core library used across all Adafruit
// display devices now) standardized on 5/6/5.  The matrix still operates
// internally on 4/4/4 color, but all the graphics functions are written
// to expect 5/6/5...the matrix lib will truncate the color components as
// needed when drawing.  These next functions are mostly here for the
// benefit of older code using one of the original color formats.


// Promote 3/3/3 RGB to Adafruit_GFX 5/6/5
uint16_t GPxMatrix::Color333(uint8_t r, uint8_t g, uint8_t b) {
  // RRRrrGGGgggBBBbb
  return r,g,b;
}

// Promote 4/4/4 RGB to Adafruit_GFX 5/6/5
uint16_t GPxMatrix::Color444(uint8_t r, uint8_t g, uint8_t b) {
  // RRRRrGGGGggBBBBb
  return ((r & 0xF) << 12) | ((r & 0x8) << 8) | ((g & 0xF) << 7) |
         ((g & 0xC) << 3) | ((b & 0xF) << 1) | ((b & 0x8) >> 3);
}

// Demote 8/8/8 to Adafruit_GFX 5/6/5
// If no gamma flag passed, assume linear color
uint16_t GPxMatrix::Color888(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint16_t)(r & 0xF8) << 8) | ((uint16_t)(g & 0xFC) << 3) | (b >> 3);
}

// 8/8/8 -> gamma -> 5/6/5
uint16_t GPxMatrix::Color888(uint8_t r, uint8_t g, uint8_t b,
                                  boolean gflag) {
  if (gflag) {                          // Gamma-corrected color?
    r = pgm_read_byte(&gamma_table[r]); // Gamma correction table maps
    g = pgm_read_byte(&gamma_table[g]); // 8-bit input to 4-bit output
    b = pgm_read_byte(&gamma_table[b]);
    return ((uint16_t)r << 12) | ((uint16_t)(r & 0x8) << 8) | // 4/4/4->5/6/5
           ((uint16_t)g << 7) | ((uint16_t)(g & 0xC) << 3) | (b << 1) |
           (b >> 3);
  } // else linear (uncorrected) color
  return ((uint16_t)(r & 0xF8) << 8) | ((uint16_t)(g & 0xFC) << 3) | (b >> 3);
}

uint16_t GPxMatrix::ColorHSV(long hue, uint8_t sat, uint8_t val,
                                  boolean gflag) {

  uint8_t r, g, b, lo;
  uint16_t s1, v1;

  // Hue
  hue %= 1536; // -1535 to +1535
  if (hue < 0)
    hue += 1536;      //     0 to +1535
  lo = hue & 255;     // Low byte  = primary/secondary color mix
  switch (hue >> 8) { // High byte = sextant of colorwheel
  case 0:
    r = 255;
    g = lo;
    b = 0;
    break; // R to Y
  case 1:
    r = 255 - lo;
    g = 255;
    b = 0;
    break; // Y to G
  case 2:
    r = 0;
    g = 255;
    b = lo;
    break; // G to C
  case 3:
    r = 0;
    g = 255 - lo;
    b = 255;
    break; // C to B
  case 4:
    r = lo;
    g = 0;
    b = 255;
    break; // B to M
  default:
    r = 255;
    g = 0;
    b = 255 - lo;
    break; // M to R
  }

  // Saturation: add 1 so range is 1 to 256, allowig a quick shift operation
  // on the result rather than a costly divide, while the type upgrade to int
  // avoids repeated type conversions in both directions.
  s1 = sat + 1;
  r = 255 - (((255 - r) * s1) >> 8);
  g = 255 - (((255 - g) * s1) >> 8);
  b = 255 - (((255 - b) * s1) >> 8);

  // Value (brightness) & 16-bit color reduction: similar to above, add 1
  // to allow shifts, and upgrade to int makes other conversions implicit.
  v1 = val + 1; 
  if(gflag) { // Gamma-corrected color?
		r = pgm_read_byte(&gamma_table[(r * v1) >> 8]); // Gamma correction table maps
		g = pgm_read_byte(&gamma_table[(g * v1) >> 8]); // 8-bit input to 4-bit output
		b = pgm_read_byte(&gamma_table[(b * v1) >> 8]);
	}  else {              // linear (uncorrected) color
    r = (r * v1) >> 12; // 4-bit results
    g = (g * v1) >> 12;
    b = (b * v1) >> 12;
  }
  return (r << 12) | ((r & 0x8) << 8) | // 4/4/4 -> 5/6/5
         (g << 7) | ((g & 0xC) << 3) | (b << 1) | (b >> 3);
}

void GPxMatrix::xLine(int x0, int x1, int y, uint16_t color)
  {
    if (y < 0 || y >= _height)
      return;
    if (x0 > x1)
    {
      int xb = x0;
      x0 = x1;
      x1 = xb;
    }
    if (x0 < 0)
      x0 = 0;
    if (x1 > _width)
      x1 = _width;
    for (int x = x0; x < x1; x++)
      drawPixel(x, y, color);
  }
  
void GPxMatrix::ellipse(int x, int y, int rx, int ry, uint16_t color)
  {
    if(ry == 0)
      return;
    int oxr = rx;
    float f = float(rx) / ry;
    f *= f;
    for(int i = 0; i < ry + 1; i++)
    {
      float s = rx * rx - i * i * f;
      int xr = (int)sqrt(s <= 0 ? 0 : s);
      xLine(x - oxr, x - xr + 1, y + i, color);
      xLine(x + xr, x + oxr + 1, y + i, color);
      if(i) 
      {
        xLine(x - oxr, x - xr + 1, y - i, color);
        xLine(x + xr, x + oxr + 1, y - i, color);
      }
      oxr = xr;
    }
  }

void GPxMatrix::fillEllipse(int x, int y, int rx, int ry, uint16_t color)
  {
    if(ry == 0)
      return;
    float f = float(rx) / ry;
    f *= f;   
    for(int i = 0; i < ry + 1; i++)
    {
      float s = rx * rx - i * i * f;
      int xr = (int)sqrt(s <= 0 ? 0 : s);
      xLine(x - xr, x + xr + 1, y + i, color);
      if(i) 
        xLine(x - xr, x + xr + 1, y - i, color);
    }
  }


void GPxMatrix::drawPixel(int16_t x, int16_t y, uint16_t c) {
  uint8_t r, g, b, bit, limit, *ptr;

  if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height))
    return;

  switch (rotation) {
  case 1:
    _swap_int16_t(x, y);
    x = WIDTH - 1 - x;
    break;
  case 2:
    x = WIDTH - 1 - x;
    y = HEIGHT - 1 - y;
    break;
  case 3:
    _swap_int16_t(x, y);
    y = HEIGHT - 1 - y;
    break;
  }

  // Adafruit_GFX uses 16-bit color in 5/6/5 format, while matrix needs
  // 4/4/4.  Pluck out relevant bits while separating into R,G,B:
  r = c >> 12;        // RRRRrggggggbbbbb
  g = (c >> 7) & 0xF; // rrrrrGGGGggbbbbb
  b = (c >> 1) & 0xF; // rrrrrggggggBBBBb

  // Loop counter stuff
  bit = 4;
  limit = 1 << nPlanes;

  if (y < nRows) {
    // Data for the upper half of the display is stored in the lower
    // bits of each byte.
    ptr = &matrixbuff[backindex][y * WIDTH * (nPlanes - 1) + x]; // Base addr
    // Plane 0 is a tricky case -- its data is spread about,
    // stored in least two bits not used by the other planes.
    ptr[WIDTH * 2] &= ~B00000011; // Plane 0 R,G mask out in one op
    if (r & 1)
      ptr[WIDTH * 2] |= B00000001; // Plane 0 R: 64 bytes ahead, bit 0
    if (g & 1)
      ptr[WIDTH * 2] |= B00000010; // Plane 0 G: 64 bytes ahead, bit 1
    if (b & 1)
      ptr[WIDTH] |= B00000001; // Plane 0 B: 32 bytes ahead, bit 0
    else
      ptr[WIDTH] &= ~B00000001; // Plane 0 B unset; mask out
    // The remaining three image planes are more normal-ish.
    // Data is stored in the high 6 bits so it can be quickly
    // copied to the DATAPORT register w/6 output lines.
    for (; bit < limit; bit <<= 1) {
      *ptr &= ~B00011100; // Mask out R,G,B in one op
      if (r & bit)
        *ptr |= B00000100; // Plane N R: bit 2
      if (g & bit)
        *ptr |= B00001000; // Plane N G: bit 3
      if (b & bit)
        *ptr |= B00010000; // Plane N B: bit 4
      ptr += WIDTH;        // Advance to next bit plane
    }
  } else {
    // Data for the lower half of the display is stored in the upper
    // bits, except for the plane 0 stuff, using 2 least bits.
    ptr = &matrixbuff[backindex][(y - nRows) * WIDTH * (nPlanes - 1) + x];
    *ptr &= ~B00000011; // Plane 0 G,B mask out in one op
    if (r & 1)
      ptr[WIDTH] |= B00000010; // Plane 0 R: 32 bytes ahead, bit 1
    else
      ptr[WIDTH] &= ~B00000010; // Plane 0 R unset; mask out
    if (g & 1)
      *ptr |= B00000001; // Plane 0 G: bit 0
    if (b & 1)
      *ptr |= B00000010; // Plane 0 B: bit 0
    for (; bit < limit; bit <<= 1) {
      *ptr &= ~B11100000; // Mask out R,G,B in one op
      if (r & bit)
        *ptr |= B00100000; // Plane N R: bit 5
      if (g & bit)
        *ptr |= B01000000; // Plane N G: bit 6
      if (b & bit)
        *ptr |= B10000000; // Plane N B: bit 7
      ptr += WIDTH;        // Advance to next bit plane
    }
  }
}

void GPxMatrix::fillScreen(uint16_t c) {
  if ((c == 0x0000) || (c == 0xffff)) {
    // For black or white, all bits in frame buffer will be identically
    // set or unset (regardless of weird bit packing), so it's OK to just
    // quickly memset the whole thing:
    memset(matrixbuff[backindex], c, WIDTH * nRows * 3);
  } else {
    // Otherwise, need to handle it the long way:
    Adafruit_GFX::fillScreen(c);
  }
}

// Return address of back buffer -- can then load/store data directly
uint8_t *GPxMatrix::backBuffer() { return matrixbuff[backindex]; }

// For smooth animation -- drawing always takes place in the "back" buffer;
// this method pushes it to the "front" for display.  Passing "true", the
// updated display contents are then copied to the new back buffer and can
// be incrementally modified.  If "false", the back buffer then contains
// the old front buffer contents -- your code can either clear this or
// draw over every pixel.  (No effect if double-buffering is not enabled.)
void GPxMatrix::swapBuffers(boolean copy) {
  if (matrixbuff[0] != matrixbuff[1]) {
    // To avoid 'tearing' display, actual swap takes place in the interrupt
    // handler, at the end of a complete screen refresh cycle.
    swapflag = true; // Set flag here, then...
    while (swapflag == true)
      delay(1); // wait for interrupt to clear it
    if (copy == true)
      memcpy(matrixbuff[backindex], matrixbuff[1 - backindex],
             WIDTH * nRows * 3);
  }
}

// Dump display contents to the Serial Monitor, adding some formatting to
// simplify copy-and-paste of data as a PROGMEM-embedded image for another
// sketch.  If using multiple dumps this way, you'll need to edit the
// output to change the 'img' name for each.  Data can then be loaded
// back into the display using a pgm_read_byte() loop.
void GPxMatrix::dumpMatrix(void) {

  int i, buffsize = WIDTH * nRows * 3;

  Serial.print(F("\n\n"
                 "#include <avr/pgmspace.h>\n\n"
                 "static const uint8_t PROGMEM img[] = {\n  "));

  for (i = 0; i < buffsize; i++) {
    Serial.print(F("0x"));
    if (matrixbuff[backindex][i] < 0x10)
      Serial.write('0');
    Serial.print(matrixbuff[backindex][i], HEX);
    if (i < (buffsize - 1)) {
      if ((i & 7) == 7)
        Serial.print(F(",\n  "));
      else
        Serial.write(',');
    }
  }
  Serial.println(F("\n};"));
}

// -------------------- Interrupt handler stuff --------------------

IRAM_ATTR void IRQ_HANDLER(void *arg) {
  int timer_idx = (int)arg;
  /* Retrieve the interrupt status and the counter value
           from the timer that reported the interrupt */
  uint32_t intr_status = TIMERG1.int_st_timers.val;
  activePanel->updateDisplay(); // Call refresh func for active display
  /* Clear the interrupt
                   and update the alarm time for the timer with without reload
   */
  if ((intr_status & BIT(timer_idx)) && timer_idx == TIMER_0) {
    TIMERG1.int_clr_timers.t0 = 1;
  }
  /* After the alarm has been triggered
   we need enable it again, so it is triggered the next time */
  TIMERG1.hw_timer[timer_idx].config.alarm_en = TIMER_ALARM_EN;
}


// Two constants are used in timing each successive BCM interval.
// These were found empirically, by checking the value of TCNT1 at
// certain positions in the interrupt code.
// CALLOVERHEAD is the number of CPU 'ticks' from the timer overflow
// condition (triggering the interrupt) to the first line in the
// updateDisplay() method.  It's then assumed (maybe not entirely 100%
// accurately, but close enough) that a similar amount of time will be
// needed at the opposite end, restoring regular program flow.
// LOOPTIME is the number of 'ticks' spent inside the shortest data-
// issuing loop (not actually a 'loop' because it's unrolled, but eh).
// Both numbers are rounded up slightly to allow a little wiggle room
// should different compilers produce slightly different results.

#define CALLOVERHEAD 30 // Actual = 25
#define LOOPTIME 400    // Actual = 1563 / 4

// The "on" time for bitplane 0 (with the shortest BCM interval) can
// then be estimated as LOOPTIME + CALLOVERHEAD * 2.  Each successive
// bitplane then doubles the prior amount of time.  We can then
// estimate refresh rates from this:
// 4 bitplanes = 320 + 640 + 1280 + 2560 = 4800 ticks per row.
// 4800 ticks * 16 rows (for 32x32 matrix) = 76800 ticks/frame.
// 16M CPU ticks/sec / 76800 ticks/frame = 208.33 Hz.
// Actual frame rate will be slightly less due to work being done
// during the brief "LEDs off" interval...it's reasonable to say
// "about 200 Hz."  The 16x32 matrix only has to scan half as many
// rows...so we could either double the refresh rate (keeping the CPU
// load the same), or keep the same refresh rate but halve the CPU
// load.  We opted for the latter.
// Can also estimate CPU use: bitplanes 1-3 all use 320 ticks to
// issue data (the increasing gaps in the timing invervals are then
// available to other code), and bitplane 0 takes 920 ticks out of
// the 2560 tick interval.
// 320 * 3 + 920 = 1880 ticks spent in interrupt code, per row.
// From prior calculations, about 4800 ticks happen per row.
// CPU use = 1880 / 4800 = ~39% (actual use will be very slightly
// higher, again due to code used in the LEDs off interval).
// 16x32 matrix uses about half that CPU load.  CPU time could be
// further adjusted by padding the LOOPTIME value, but refresh rates
// will decrease proportionally, and 200 Hz is a decent target.

// The flow of the interrupt can be awkward to grasp, because data is
// being issued to the LED matrix for the *next* bitplane and/or row
// while the *current* plane/row is being shown.  As a result, the
// counter variables change between past/present/future tense in mid-
// function...hopefully tenses are sufficiently commented.

IRAM_ATTR void GPxMatrix::updateDisplay(void) {
  uint8_t i, tick, tock, *ptr;
  uint16_t t, duration;

  *oeport |= oemask;   // Disable LED output during row/plane switchover
  *latport |= latmask; // Latch data loaded during *prior* interrupt

  // Calculate time to next interrupt BEFORE incrementing plane #.
  // This is because duration is the display time for the data loaded
  // on the PRIOR interrupt.  CALLOVERHEAD is subtracted from the
  // result because that time is implicit between the timer overflow
  // (interrupt triggered) and the initial LEDs-off line at the start
  // of this method.
	if(nRows > 16)
	t = LOOPTIME>>1;
	else 
	t = (nRows > 8) ? LOOPTIME : (LOOPTIME * 2);
  duration = ((t + CALLOVERHEAD * 2) << plane) - CALLOVERHEAD;

  // Borrowing a technique here from Ray's Logic:
  // www.rayslogic.com/propeller/Programming/AdafruitRGB/AdafruitRGB.htm
  // This code cycles through all four planes for each scanline before
  // advancing to the next line.  While it might seem beneficial to
  // advance lines every time and interleave the planes to reduce
  // vertical scanning artifacts, in practice with this panel it causes
  // a green 'ghosting' effect on black pixels, a much worse artifact.

  if (++plane >= nPlanes) {   // Advance plane counter.  Maxed out?
    plane = 0;                // Yes, reset to plane 0, and
    if (++row >= nRows) {     // advance row counter.  Maxed out?
      row = 0;                // Yes, reset row counter, then...
      if (swapflag == true) { // Swap front/back buffers if requested
        backindex = 1 - backindex;
        swapflag = false;
      }
      buffptr = matrixbuff[1 - backindex]; // Reset into front buffer
    }
  } else if (plane == 1) {
    // Plane 0 was loaded on prior interrupt invocation and is about to
    // latch now, so update the row address lines before we do that:
    if (row & 0x1)
      *addraport |= addramask;
    else
      *addraport &= ~addramask;
    // MYSTERY: certain matrices REQUIRE these delays ???
    delayMicroseconds(10);
    if (row & 0x2)
      *addrbport |= addrbmask;
    else
      *addrbport &= ~addrbmask;
    delayMicroseconds(10);
    if (row & 0x4)
      *addrcport |= addrcmask;
    else
      *addrcport &= ~addrcmask;
    delayMicroseconds(10);
    if (nRows > 8) {
      if (row & 0x8)
        *addrdport |= addrdmask;
      else
        *addrdport &= ~addrdmask;
      delayMicroseconds(10);
    }
    if (nRows > 16) {
      if(row & 0x10)
        *addreport |= addremask;
      else
        *addreport &= ~addremask;
      delayMicroseconds(10);
    }
  }

  // buffptr, being 'volatile' type, doesn't take well to optimization.
  // A local register copy can speed some things up:
  ptr = (uint8_t *)buffptr;

static timg_dev_t *TG[2] = {&TIMERG0, &TIMERG1};
static portMUX_TYPE timer_spinlock[TIMER_GROUP_MAX] = {
    portMUX_INITIALIZER_UNLOCKED, portMUX_INITIALIZER_UNLOCKED};
portENTER_CRITICAL(&timer_spinlock[TIMER_GROUP_1]);
TG[TIMER_GROUP_1]->hw_timer[TIMER_0].alarm_high = 0;
TG[TIMER_GROUP_1]->hw_timer[TIMER_0].alarm_low = (uint32_t)duration;
portEXIT_CRITICAL(&timer_spinlock[TIMER_GROUP_1]);
  *oeport &= ~oemask;   // Re-enable output
  *latport &= ~latmask; // Latch down

  // Record current state of CLKPORT register, as well as a second
  // copy with the clock bit set.  This makes the innnermost data-
  // pushing loops faster, as they can just set the PORT state and
  // not have to load/modify/store bits every single time.  It's a
  // somewhat rude trick that ONLY works because the interrupt
  // handler is set ISR_BLOCK, halting any other interrupts that
  // might otherwise also be twiddling the port at the same time
  // (else this would clobber them). only needed for AVR's where you
  // cannot set one bit in a single instruction

  if (plane > 0) { // 188 ticks from TCNT1=0 (above) to end of function

    // Planes 1-3 copy bytes directly from RAM to PORT without unpacking.
    // The least 2 bits (used for plane 0 data) are presumed masked out
    // by the port direction bits.


#define pew                                                                    \
  *outclrreg = rgbclkmask;                                                     \
  *outsetreg = expand[*ptr++];                                                 \
  *outsetreg = clkmask;


    // Loop is unrolled for speed:
    pew pew pew pew pew pew pew pew pew pew pew pew pew pew pew pew pew pew pew
        pew pew pew pew pew pew pew pew pew pew pew pew pew

        if (WIDTH == 64) {
      pew pew pew pew pew pew pew pew pew pew pew pew pew pew pew pew pew pew
          pew pew pew pew pew pew pew pew pew pew pew pew pew pew
    }
        if (WIDTH == 128) {
      pew pew pew pew pew pew pew pew pew pew pew pew pew pew pew pew pew pew
          pew pew pew pew pew pew pew pew pew pew pew pew pew pew
      pew pew pew pew pew pew pew pew pew pew pew pew pew pew pew pew pew pew
          pew pew pew pew pew pew pew pew pew pew pew pew pew pew
      pew pew pew pew pew pew pew pew pew pew pew pew pew pew pew pew pew pew
          pew pew pew pew pew pew pew pew pew pew pew pew pew pew
    }

    *outclrreg = clkmask; // Set clock low

    buffptr = ptr; //+= 32;

  } else { // 920 ticks from TCNT1=0 (above) to end of function

    for (int i = 0; i < WIDTH; i++) {
      byte b = (ptr[i] << 6) | ((ptr[i + WIDTH] << 4) & 0x30) |
               ((ptr[i + WIDTH * 2] << 2) & 0x0C);

      *outclrreg = rgbclkmask; // Clear all data and clock bits together
      *outsetreg = expand[b];  // Set new data bits
      *outsetreg = clkmask;    // Set clock high
    }
    *outclrreg = clkmask; // Set clock low
  }
}
