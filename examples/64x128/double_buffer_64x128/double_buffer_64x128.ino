
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
//uint8_t listrgbpins[6] = {27, 26, 14, 12, 25, 15};
//GPxMatrix matrix(P_A, P_B, P_C, P_D, P_E, P_CLK, P_LAT, P_OE, true, 128, listrgbpins);

GPxMatrix matrix(P_A, P_B, P_C, P_D, P_E, P_CLK, P_LAT, P_OE, true, 128);

#define clear()          fillScreen(0)
#define show()           swapBuffers(true)


struct Text {
  char *text;
  uint16_t width, height;
  int16_t x, y;
  int16_t dx, dy;
} text = {"Hello", 0, 0, 0, 0, 1, 1};


uint16_t textColor = matrix.Color888(0, 0, 255);
uint16_t myBLACK = matrix.Color888(0, 0, 0);
uint16_t lineColor = matrix.Color888(255, 0, 0);
uint16_t backgroundColor = matrix.Color888(0, 255, 0);

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
  matrix.begin();
  matrix.setTextWrap(false);

  int16_t x1 = 0, y1 = 0;
  matrix.getTextBounds(text.text, 0, 0, &x1, &y1, &text.width, &text.height);
  text.width-=2;
  text.height-=2;
}

int16_t x=0, dx=1;

void loop() {
  matrix.clear();

  if(x+dx>=matrix.width() || x+dx<0)
    dx=-dx;
  x+=dx;
  matrix.drawLine(x,0, matrix.width()-x-1, matrix.height()-1, lineColor);

  if(text.x+text.dx+text.width>=matrix.width() || text.x+text.dx<0)
    text.dx=-text.dx;
  if(text.y+text.dy+text.height>=matrix.height() || text.y+text.dy<0)
    text.dy=-text.dy;
  text.x+=text.dx;
  text.y+=text.dy;
  matrix.setTextColor(textColor);
  matrix.setCursor(text.x, text.y);
  matrix.print(text.text);

  matrix.show();
  delay(50);
}
