/*
- Lien vidéo: https://youtu.be/bQL0pm41uJY

MIT License

Copyright (c) 2022 3tawi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef AnalogClock1_H
class AnalogClock1 : public Play {
private:
#define WIDTH 128
#define HEIGHT 64
const byte CENTRE_X = WIDTH / 2;
const byte CENTRE_Y = HEIGHT / 2;

public:
    AnalogClock1() {
        name = (char *)"AnalogClock1";
    }

    unsigned int playframe() {
      if(millis() - lastTime >= 1000) {
        effects.drawRGBBitmap(0, 0, clockbitmap1, 128, 64);
        drawtime();
        effects.ShowFrame();
        lastTime = millis();
      }
      if (NewRTCm != rtc.getMinute()) {
        effects.redtemphumi();
        co3 = random(0, 24);
        NewRTCm = rtc.getMinute();
      }
    return 0;
    }
    

    void drawtime() {
      double radians;
      int x0, x1, x2;
      int y0, y1, y2;
      uint8_t hour = rtc.getHour();
      if (hour > 12) hour -= 12;
      uint8_t minute = rtc.getMinute();
      uint8_t second = rtc.getSecond();
      uint16_t color = random(0, 25);
      // hour
      radians = (90.0F - (((hour-3) * 30.0F) + ((minute * 29.0F) / 60.0F))) * M_PI / 180.0F;
      x0 = round(CENTRE_X + 2 * cos(radians));
      y0 = round(CENTRE_Y - 2 * sin(radians));
      radians = (90.0F - ((hour * 30.0F) + ((minute * 29.0F) / 60.0F))) * M_PI / 180.0F;
      x1 = round(CENTRE_X + 28 * cos(radians));
      y1 = round(CENTRE_Y - 14 * sin(radians));
      radians = (90.0F - (((hour+3) * 30.0F) + ((minute * 29.0F) / 60.0F))) * M_PI / 180.0F;
      x2 = round(CENTRE_X + 2 * cos(radians));
      y2 = round(CENTRE_Y - 2 * sin(radians));
      effects.fillTriangle(CENTRE_X, CENTRE_Y, x0, y0, x2, y2, 0xF000);
      effects.drawLine(x0, y0, x1, y1, 0xF800);
      effects.drawLine(x1, y1, x2, y2, 0xF800);
      // minute
      radians = (90.0F - ((minute-10) * 6.0F)) * M_PI / 180.0;
      x0 = round(CENTRE_X + 2 * cos(radians));
      y0 = round(CENTRE_Y - 2 * sin(radians));
      radians = (90.0F - (minute * 6.0F)) * M_PI / 180.0;
      x1 = round(CENTRE_X + 40 * cos(radians));
      y1 = round(CENTRE_Y - 20 * sin(radians));
      radians = (90.0F - ((minute+10) * 6.0F)) * M_PI / 180.0;
      x2 = round(CENTRE_X + 2 * cos(radians));
      y2 = round(CENTRE_Y - 2 * sin(radians));
      effects.fillTriangle(CENTRE_X, CENTRE_Y, x0, y0, x2, y2, 0x07E0);
      effects.drawLine(x0, y0, x1, y1, 0x07E0);
      effects.drawLine(x1, y1, x2, y2, 0x07E0);
      // second
      radians = (90.0F - ((second-5) * 6.0F)) * M_PI / 180.0F;
      x0 = round(CENTRE_X + 4 * cos(radians));
      y0 = round(CENTRE_Y - 4 * sin(radians));
      radians = (90.0F - (second * 6.0F)) * M_PI / 180.0F;
      x1 = round(CENTRE_X + 50 * cos(radians));
      y1 = round(CENTRE_Y - 25 * sin(radians));
      radians = (90.0F - ((second+5) * 6.0F)) * M_PI / 180.0F;
      x2 = round(CENTRE_X + 4 * cos(radians));
      y2 = round(CENTRE_Y - 4 * sin(radians));
      effects.fillTriangle(CENTRE_X, CENTRE_Y, x0, y0, x2, y2, MyColor[color]);
      effects.drawLine(x0, y0, x1, y1, MyColor[color]);
      effects.drawLine(x1, y1, x2, y2, MyColor[color]);
    }

};

#endif
