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

#ifndef plasmaClock_H
class plasmaClock : public Play {
private:


public:
const float radius1 =16.3, radius2 =23.0, radius3 =40.8, radius4 =44.2,
            centerx1=16.1, centerx2=11.6, centerx3=23.4, centerx4= 4.1,
            centery1= 8.7, centery2= 6.5, centery3=14.0, centery4=-2.9;
float       angle1  = 0.0, angle2  = 0.0, angle3  = 0.0, angle4  = 0.0;
long        hueShift= 0;

    plasmaClock() {
        name = (char *)"plasmaClock";
    }

    unsigned int playframe() {
      clear();
      drawplasma();
      drawMesg(textmsg);
      drawSTime();
      drawHMTime();
      if(millis() - lastTime >= 1000) {
        flasher = !flasher;
        lastTime = millis();
      }
      if (NewRTCm != rtc.getMinute()) {
        co3 = random(0, 24);
        flashSe = !flashSe;
        NewRTCm = rtc.getMinute();
      }
      show();
      return 0;
    }
 
void drawplasma() {
  int           x1, x2, x3, x4, y1, y2, y3, y4, sx1, sx2, sx3, sx4;
  unsigned char x, y;
  long          value;

  sx1 = (int)(cos(angle1) * radius1 + centerx1);
  sx2 = (int)(cos(angle2) * radius2 + centerx2);
  sx3 = (int)(cos(angle3) * radius3 + centerx3);
  sx4 = (int)(cos(angle4) * radius4 + centerx4);
  y1  = (int)(sin(angle1) * radius1 + centery1);
  y2  = (int)(sin(angle2) * radius2 + centery2);
  y3  = (int)(sin(angle3) * radius3 + centery3);
  y4  = (int)(sin(angle4) * radius4 + centery4);

  for(y=0; y<MATRIX_HEIGHT; y++) {
    x1 = sx1; x2 = sx2; x3 = sx3; x4 = sx4;
    for(x=0; x<MATRIX_WIDTH; x++) {
      value = hueShift
        + (int8_t)pgm_read_byte(sinetab + (uint8_t)((x1 * x1 + y1 * y1) >> 3))
        + (int8_t)pgm_read_byte(sinetab + (uint8_t)((x2 * x2 + y2 * y2) >> 3))
        + (int8_t)pgm_read_byte(sinetab + (uint8_t)((x3 * x3 + y3 * y3) >> 4))
        + (int8_t)pgm_read_byte(sinetab + (uint8_t)((x4 * x4 + y4 * y4) >> 4));
      matrix.drawPixel(x, y, matrix.ColorHSV(value * 3, 255, 255, true));
      x1--; x2--; x3--; x4--;
    }
    y1--; y2--; y3--; y4--;
  }
  angle1 += 0.03;
  angle2 -= 0.07;
  angle3 += 0.13;
  angle4 -= 0.15;
  hueShift += 2;
}

void drawMesg(String text) {
  uint16_t text_length = text.length();
  effects.setFont(nadianne); 
  effects.setCursor(xps, 0);
  effects.printbg(text);
  xps -= sp0;
  if (xps < -(text_length * 16)) {
    effects.getmesg();
  }
}

void drawSTime() {
     co2 = rtc.getTime("%S").toInt();
     int xx = flashSe ? (co2*2)+4 : 128-((co2*2)+4);
     co2 += co3;
     effects.fillEllipse(xx, 17, 4, 2, 0x0);
     effects.fillEllipse(xx, 17, 2, 1, MyColor[co2%25]);
     String text = flasher ? ":" : "";
     effects.setFont(NuFont); 
     effects.setCursor(52, 19);
     effects.printbg(text);
    }
    
void drawHMTime() {
     String text = flasher ? rtc.getTime("%H:%M") : rtc.getTime("%H;%M");
     effects.setFont(NuFont); 
     effects.setCursor(4, 19);
     effects.printbg(text);
    }
};

#endif
