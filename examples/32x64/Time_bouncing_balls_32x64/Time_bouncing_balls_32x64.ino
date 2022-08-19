//This example shows how a custom matrix resolution can be created for one of the base modes
//By bitluni 


#include <PicoEspTime.h>
#include <GPxMatrix.h>

#define P_A    32
#define P_B    17
#define P_C    33
#define P_D    16
#define P_E    5
#define P_CLK  0 
#define P_LAT  2 
#define P_OE   4

//                          R1, G1, B1, R2, G2, B2  (RGB Pins)
//uint8_t listrgbpins[6] = {27, 26, 14, 12, 25, 15};
//GPxMatrix matrix(P_A, P_B, P_C, P_D, P_CLK, P_LAT, P_OE, true, 64, listrgbpins);

GPxMatrix matrix(P_A, P_B, P_C, P_D, P_CLK, P_LAT, P_OE, true, 64);
PicoEspTime rtc;

uint32_t lastTime; 
#define clear()  fillScreen(0)
#define show()   swapBuffers(true)
int16_t          hue = 0;
bool flasher = true;

void setup()
{
  rtc.adjust(5, 3, 20, 2022, 7, 20); // 01:26:21 03 Jun 2022
  matrix.begin();
  matrix.setTextWrap(false); // Allow text to run off right edge
  matrix.setTextSize(2);
}

///draws a bouncing balls
void balls()
{
  //some basic gravity physics
  static const uint16_t c[4] = {matrix.Color888(0, 255, 0), matrix.Color888(0, 255, 255), matrix.Color888(255, 0, 255), matrix.Color888(255, 255, 0)};
  static float y[4] = {16, 16, 16, 16};
  static float x[4] = {32, 32, 32, 32};
  static float vx[4] = {.02, -0.14, .10, -.06};
  static float vy[4] = {0, 2, 4, 6};
  static unsigned long lastT = 0;
  unsigned long t = millis();
  float dt = (t - lastT) * 0.001f;
  lastT = t;
  const int r = 8;
  for (int i = 0; i < 4; i++)
  {
    int rx = r;
    int ry = r;
    vy[i] += -9.81f * dt * 100;
    x[i] += vx[i];
    y[i] += vy[i] * dt;
    //check for boundaries and bounce back
    if (y[i] < r && vy[i] < 0)
    {
      vy[i] = 200 + i * 12;
      ry = y[i];
    }
    if (x[i] < r && vx[i] < 0)
    {
      vx[i] = -vx[i];
      rx = x[i];
    }
    if (x[i] >= matrix.width() - r && vx[i] > 0)
    {
      vx[i] = -vx[i];
      rx = matrix.width() - x[i];
    }
    //draw a filled ellipse
    matrix.fillEllipse(x[i], matrix.height() - y[i] - 1, rx, ry, c[i]);
    matrix.ellipse(x[i], matrix.height() - y[i] - 1, rx, ry, matrix.ColorHSV(hue*(i+1), 255, 255, true));
  }
}

//mainloop
void loop()
{
   if(millis() - lastTime >= 1000) {
    rtc.read(); 
    flasher = !flasher;
    lastTime = millis();
    }
  matrix.clear();
  //text position
  matrix.setCursor(17, 4);
  //black text color no background color
  matrix.setTextColor(matrix.ColorHSV(hue, 255, 255, true));
  matrix.print(flasher ? rtc.getTime("%H:%M:%S") : rtc.getTime("%H %M:%S"));   // (String) returns time with specified format
  //draw bouncing balls
  balls();
  //show the backbuffer (only needed when using backbuffering)
  matrix.show();
  hue++;
  if(hue >= 1536) hue -= 1536;
}
