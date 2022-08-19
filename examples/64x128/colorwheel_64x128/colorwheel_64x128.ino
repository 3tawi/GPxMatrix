// colorwheel demo for Adafruit RGBmatrixPanel library.
// Renders a nice circle of hues on our 32x32 RGB LED matrix:
// http://www.adafruit.com/products/607
// 32x32 MATRICES DO NOT WORK WITH ARDUINO UNO or METRO 328.

// Written by Limor Fried/Ladyada & Phil Burgess/PaintYourDragon
// for Adafruit Industries.
// BSD license, all text above must be included in any redistribution.


#include <GPxMatrix.h>

#define P_A    32
#define P_B    17
#define P_C    33
#define P_D    16
#define P_E    5
#define P_CLK  0 
#define P_LAT  2 
#define P_OE   4

//                        R1, G1, B1, R2, G2, B2  (RGB Pins)
uint8_t listrgbpins[6] = {27, 26, 14, 12, 25, 15};
GPxMatrix matrix(P_A, P_B, P_C, P_D, P_E, P_CLK, P_LAT, P_OE, false, 128, listrgbpins);
//RGBmatrixPanelGP matrix(P_A, P_B, P_C, P_D, P_E, P_CLK, P_LAT, P_OE, true, 128);

void setup() {
  int      x, y, hue;
  float    dx, dy, d;
  uint8_t  sat, val;
  uint16_t c;

  matrix.begin();

  for(x=0; x < matrix.width(); x++) {
    dx = 63.5 - (float)x;
    for(y=0; y < matrix.height(); y++) {
      dy = 31.5 - (float)y;
      d  = dx * dx + dy * dy;
      if(d <= (128.5 * 64.5)) { // Inside the circle(ish)?
        hue = (int)((atan2(dx, -dy) + PI) * 1536.0 / (PI * 2.0))*3;
        d = sqrt(d);
        if(d > 63.5) {
          // Do a little pseudo anti-aliasing along perimeter
          sat = 255;
          val = (int)((1.0 - (d - 63.5)) * 255.0 + 0.5);
        } else
        {
          // White at center
          sat = (int)(d / 63.5 * 255.0 + 0.5);
          val = 255;
        }
        c = matrix.ColorHSV(hue, sat, val, true);
      } else {
        c = 0;
      }
      matrix.drawPixel(x, y, c);
    }
  }
}

void loop() {
  // Do nothing -- image doesn't change
}
