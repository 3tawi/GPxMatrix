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

#ifndef DigitalClock4_H
class DigitalClock4 : public Play {
private:
#define mw 128
#define mh 64
    int16_t xf = max(0, (mw-33)/2) << 4;
    int16_t yf = max(0, (mh-33)/2) << 4;
    // scroll speed in 1/16th
    int16_t xfc = 6;
    int16_t yfc = 3;
    // scroll down and right by moving upper left corner off screen
    // more up and left (which means negative numbers)
    int16_t xfdir = -1;
    int16_t yfdir = -1;

public:
    DigitalClock4() {
        name = (char *)"DigitalClock4";
    }

    void start() {
    }

    unsigned int playframe() {
      readtime();
      clear();
      display_Bitmap(bitmap, 33, 33);
      drawMesg(textmsg);
      drawSTime();
      drawHMTime();
      show();
      return 200;
    }
    
void display_Bitmap ( const uint16_t *bitmap, uint8_t w, uint8_t h) {
        bool updDir = false;
        // Get actual x/y by dividing by 16.
        int16_t x = xf >> 4;
        int16_t y = yf >> 4;

        effects.drawRGBBitmap(x, y, bitmap, w, h);

        // Only pan if the display size is smaller than the pixmap
        if (mw<w) {
            xf += xfc*xfdir;
            if (xf >= 0)                      { xfdir = -1; updDir = true ; };
            // we don't go negative past right corner, go back positive
            if (xf <= ((mw-w) << 4)) { xfdir = 1;  updDir = true ; };
        }
        if (mh<h) {
            yf += yfc*yfdir;
            // we shouldn't display past left corner, reverse direction.
            if (yf >= 0)                      { yfdir = -1; updDir = true ; };
            if (yf <= ((mh-h) << 4)) { yfdir = 1;  updDir = true ; };
        }
        // only bounce a pixmap if it's smaller than the display size
        if (mw>w) {
            xf += xfc*xfdir;
            // Deal with bouncing off the 'walls'
            if (xf >= (mw-w) << 4) { xfdir = -1; updDir = true ; };
            if (xf <= 0)                    { xfdir =  1; updDir = true ; };
        }
        if (mh>h) {
            yf += yfc*yfdir;
            if (yf >= (mh-h) << 4) { yfdir = -1; updDir = true ; };
            if (yf <= 0)                    { yfdir =  1; updDir = true ; };
        }

       if (updDir) {
            // Add -1, 0 or 1 but bind result to 1 to 1.
            // Let's take 3 is a minimum speed, otherwise it's too slow.
            xfc = constrain(xfc + random(-1, 2), 6, 24);
            yfc = constrain(xfc + random(-1, 2), 3, 12);
        }
}

    void readtime() {
      if(millis() - lastTime >= 1000) {
        flasher = !flasher;
        lastTime = millis();
      }
      if (NewRTCm != rtc.getMinute()) {
        co3 = random(0, 25);
        flashSe = !flashSe;
        NewRTCm = rtc.getMinute();
      }
    }

void drawMesg(String text) {
  uint16_t text_length = text.length();
  effects.setFont(MdumFont); 
  effects.setCursor(xps, 0);
  co1 = co0;
  effects.print(text, co1);
  xps -= sp0;
  if (xps < -(text_length * 8)) {
    effects.getmesg();
  }
}

void drawSTime() {
     co2 = rtc.getTime("%S").toInt();
     int xx = (co2*2)+4;
     xx = flashSe ? xx : 128-xx;
     co2 += co3;
     effects.fillEllipse(xx, 19, 4, 2, MyColor[co2%25]);
     String text = flasher ? ":" : ";";
     effects.setFont(NuFont); 
     effects.setCursor(52, 22);
     effects.setTextColor(MyColor[co2%25]);
     effects.print(text);
    }
    
void drawHMTime() {
     String text = flasher ? rtc.getTime("%H:%M") : rtc.getTime("%H;%M");
     effects.setFont(NuFont); 
     effects.setCursor(4, 22);
     effects.print(text, co3);
    }
};

#endif
