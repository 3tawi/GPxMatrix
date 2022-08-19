// testshapes demo for RGBmatrixPanel library.
// Demonstrates the drawing abilities of the RGBmatrixPanel library.
// For 32x64 RGB LED matrix.

// WILL NOT FIT on ARDUINO UNO -- requires a Mega, M0 or M4 board

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

const uint16_t Wheelcolor[7] = {
  0x1082, // White=1
  0x0080, // Green=1
  0x0002, // Blue=1
  0x1000,  // Red=1
  0x1002, // Well=1
  0x1080, // Well=1
  0x0082, // Well=1
};
void setup() {

  matrix.begin();

  // draw a pixel in solid white
  matrix.drawPixel(0, 0, matrix.Color888(255, 255, 255));
  delay(500);

  // fix the screen with green
  matrix.fillRect(0, 0, matrix.width(), matrix.height(), matrix.Color888(0, 255, 0));
  delay(500);

  // draw a box in yellow
  matrix.drawRect(0, 0, matrix.width(), matrix.height(), matrix.Color888(255, 255, 0));
  delay(500);

  // draw an 'X' in red
  matrix.drawLine(0, 0, matrix.width()-1, matrix.height()-1, matrix.Color888(255, 0, 0));
  matrix.drawLine(matrix.width()-1, 0, 0, matrix.height()-1, matrix.Color888(255, 0, 0));
  delay(500);

  // draw a blue circle
  matrix.drawCircle(10, 10, 10, matrix.Color888(0, 0, 255));
  delay(500);

  // fill a violet circle
  matrix.fillCircle(40, 21, 10, matrix.Color888(255, 0, 255));
  delay(500);

  // draw a red circle
  matrix.ellipse(70, 31, 12, 20, matrix.Color888(255, 0, 0));
  delay(500);

  // fill a violet circle
  matrix.fillEllipse(100, 41, 12, 20, matrix.Color888(0, 255, 255));
  delay(2000);

  // fill the screen with 'black'
  matrix.fillScreen(0);

  // draw some text!
  matrix.setTextSize(2);     // size 1 == 8 pixels high
  matrix.setTextWrap(false); // Don't wrap at end of line - will do ourselves

  matrix.setCursor(8, 0);    // start at top left, with 8 pixel of spacing
  uint8_t w = 0;
  char *str = "AdafruitIndustries";
  for (w=0; w<8; w++) {
    matrix.setTextColor(Wheelcolor[w%7]);
    matrix.print(str[w]);
  }
  matrix.println();
  for (w=8; w<18; w++) {
    matrix.setTextColor(Wheelcolor[w%7]);
    matrix.print(str[w]);
  }
  matrix.println();
  //matrix.setTextColor(matrix.Color888(4,4,4));
  //matrix.println("Industries");
  matrix.setTextColor(matrix.Color888(255,255,255));
  matrix.println("LED MATRIX!");

  // print each letter with a rainbow color
  matrix.setTextColor(matrix.Color888(255,0,0));
  matrix.print('6');
  matrix.setTextColor(matrix.Color888(255,127,0));
  matrix.print('4');
  matrix.setTextColor(matrix.Color888(255,255,0));
  matrix.print('x');
  matrix.setTextColor(matrix.Color888(127,255,0));
  matrix.print('1');
  matrix.setTextColor(matrix.Color888(0,255,0));
  matrix.print('2');
  matrix.setTextColor(matrix.Color888(0,255,255));
  matrix.print('8');
  matrix.setTextColor(matrix.Color888(0,127,255));
  matrix.print('*');
  matrix.setTextColor(matrix.Color888(0,0,255));
  matrix.print('R');
  matrix.setTextColor(matrix.Color888(127,0,255));
  matrix.print('G');
  matrix.setTextColor(matrix.Color888(255,0,127));
  matrix.print('B');
  matrix.setTextColor(matrix.Color888(255,255,255));
  matrix.print('*');

  // whew!
}

void loop() {
  // Do nothing -- image doesn't change
}
