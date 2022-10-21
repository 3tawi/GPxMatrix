/*
 * Adapted by 3tawi
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 *
 * http://www.stefan-petrick.de/wordpress_beta
 * Copyright (c) 2014 Stefan Petrick
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef AudioPatterns_H
#define AudioPatterns_H

class AudioPatterns : public Playlist {
  private:
  const int CENTRE_X = 64;
  const int CENTRE_Y = 32;
  public:
    typedef void (AudioPatterns::*AudioPatternFunctionPointer)(void);
    AudioPatternFunctionPointer currentItem;
    static const int PATTERN_COUNT = 52;
    AudioPatternFunctionPointer shuffledItems[PATTERN_COUNT];
    AudioPatternFunctionPointer items[PATTERN_COUNT] = {
      &AudioPatterns::drawClock1,
      &AudioPatterns::drawClock2,
      &AudioPatterns::drawClock3,
      &AudioPatterns::drawClock4,
      &AudioPatterns::drawClock5,
      &AudioPatterns::drawClock6,
      &AudioPatterns::drawClock7,
      &AudioPatterns::drawClock8,
      &AudioPatterns::SpMuAudio1,
      &AudioPatterns::SpMuAudio2,
      &AudioPatterns::SpMuAudio3,
      &AudioPatterns::SpMuAudio4,
      &AudioPatterns::SpMuAudio5,
      &AudioPatterns::SpMuAudio6,
      &AudioPatterns::SpMuAudio7,
      &AudioPatterns::SpMuAudio8,
      &AudioPatterns::SpMuAudio9,
      &AudioPatterns::SpMuAudio10,
      &AudioPatterns::SpMuAudio11,
      &AudioPatterns::SpMuAudio12,
      &AudioPatterns::SpMuAudio13,
      &AudioPatterns::SpMuAudio14,
      &AudioPatterns::SpMuAudio15,
      &AudioPatterns::SpMuAudio16,
      &AudioPatterns::SpMuAudio17,
      &AudioPatterns::SpMuAudio18,
      &AudioPatterns::SpMuAudio19,
      &AudioPatterns::SpMuAudio20,
      &AudioPatterns::SpMuAudio21,
      &AudioPatterns::SpMuAudio22,
      &AudioPatterns::SpMuAudio23,
      &AudioPatterns::SpMuAudio24,
      &AudioPatterns::SpMuAudio25,
      &AudioPatterns::SpMuAudio26,
      &AudioPatterns::SpMuAudio27,
      &AudioPatterns::SpMuAudio28,
      &AudioPatterns::SpMuAudio29,
      &AudioPatterns::SpMuAudio30,
      &AudioPatterns::SpMuAudio31,
      &AudioPatterns::SpMuAudio32,
      &AudioPatterns::SpMuAudio33,
      &AudioPatterns::SpMuAudio34,
      &AudioPatterns::SpMuAudio35,
      &AudioPatterns::SpMuAudio36,
      &AudioPatterns::SpMuAudio37,
      &AudioPatterns::SpMuAudio38,
      &AudioPatterns::SpMuAudio39,
      &AudioPatterns::SpMuAudio40,
      &AudioPatterns::SpMuAudio41,
      &AudioPatterns::SpMuAudio42,
      &AudioPatterns::SpMuAudio43,
      &AudioPatterns::SpMuAudio44
    };
    
    void drawClock1() {
      readtime();
      drawHMTime();
      drawMesg(textmsg, GPFont16x24, 16, 0);
    }
    void drawClock2() {
      readtime();
      drawHMTime();
      drawMesg(textmsg, GPFont16x16, 16, 6);
    }
    void drawClock3() {
      readtime1();
      drawclock();
      drawtime();
    }
    void drawClock4() {
      readtime1();
      drawclockfp();
      drawtimefp();
    }
    void drawClock5() {
      ReadAudio();
      readtime1();
      drawclockfp1();
      drawtimefp1();
    }
    void drawClock6() {
      ReadAudio();
      readtime1();
      drawclock1();
      drawtime1();
    }
    void drawClock7() {
      ReadAudio();
      readtime();
      drawHMTime();
      drawMesg(textmsg, GPFont16x24, 16, 0);
    }
    void drawClock8() {
      ReadAudio();
      readtime();
      drawHMTime();
      drawMesg(textmsg, GPFont16x16, 16, 6);
    }
    
    void SpMuAudio1() {
      ReadAudio();
      drawAudio1(0);
    }
    void SpMuAudio2() {
      ReadAudio();
      drawAudio1(1);
    }
    void SpMuAudio3() {
      ReadAudio();
      drawAudio1(2);
    }
    void SpMuAudio4() {
      ReadAudio();
      drawAudio1(3);
    }
    void SpMuAudio5() {
      ReadAudio();
      drawAudio1(4);
    }
    void SpMuAudio6() {
      ReadAudio();
      drawAudio1(5);
    }
    void SpMuAudio7() {
      ReadAudio();
      drawAudio1(6);
    }
    void SpMuAudio8() {
      ReadAudio();
      drawAudio1(7);
    }
    void SpMuAudio9() {
      ReadAudio();
      drawAudio1(8);
    }
    void SpMuAudio10() {
      ReadAudio();
      drawAudio1(9);
    }
    void SpMuAudio11() {
      ReadAudio();
      drawAudio1(10);
    }
    void SpMuAudio12() {
      ReadAudio();
      drawAudio2(0);
    }
    void SpMuAudio13() {
      ReadAudio();
      drawAudio2(1);
    }
    void SpMuAudio14() {
      ReadAudio();
      drawAudio2(2);
    }
    void SpMuAudio15() {
      ReadAudio();
      drawAudio2(3);
    }
    void SpMuAudio16() {
      ReadAudio();
      drawAudio2(4);
    }
    void SpMuAudio17() {
      ReadAudio();
      drawAudio2(5);
    }
    void SpMuAudio18() {
      ReadAudio();
      drawAudio2(6);
    }
    void SpMuAudio19() {
      ReadAudio();
      drawAudio2(11);
    }
    void SpMuAudio20() {
      ReadAudio();
      drawAudio2(8);
    }
    void SpMuAudio21() {
      ReadAudio();
      drawAudio2(9);
    }
    void SpMuAudio22() {
      ReadAudio();
      drawAudio2(10);
    }
    void SpMuAudio23() {
      ReadAudio();
      drawAudio3(0);
    }
    void SpMuAudio24() {
      ReadAudio();
      drawAudio3(1);
    }
    void SpMuAudio25() {
      ReadAudio();
      drawAudio3(2);
    }
    void SpMuAudio26() {
      ReadAudio();
      drawAudio3(3);
    }
    void SpMuAudio27() {
      ReadAudio();
      drawAudio3(4);
    }
    void SpMuAudio28() {
      ReadAudio();
      drawAudio3(5);
    }
    void SpMuAudio29() {
      ReadAudio();
      drawAudio3(6);
    }
    void SpMuAudio30() {
      ReadAudio();
      drawAudio3(7);
    }
    void SpMuAudio31() {
      ReadAudio();
      drawAudio3(8);
    }
    void SpMuAudio32() {
      ReadAudio();
      drawAudio3(9);
    }
    void SpMuAudio33() {
      ReadAudio();
      drawAudio3(10);
    }
    void SpMuAudio34() {
      ReadAudio();
      drawAudio3(11);
    }
    void SpMuAudio35() {
      ReadAudio();
      drawAudio3(12);
    }
    void SpMuAudio36() {
      ReadAudio();
      drawAudio2(11);
    }
    void SpMuAudio37() {
      ReadAudio();
      drawAudio4(0);
    }
    void SpMuAudio38() {
      ReadAudio();
      drawAudio4(1);
    }
    void SpMuAudio39() {
      ReadAudio();
      drawAudio4(2);
    }
    void SpMuAudio40() {
      ReadAudio();
      drawAudio4(3);
    }
    void SpMuAudio41() {
      ReadAudio();
      drawAudio4(4);
    }
    void SpMuAudio42() {
      ReadAudio();
      drawAudio4(5);
    }
    void SpMuAudio43() {
      ReadAudio();
      drawAudio4(6);
    }
    void SpMuAudio44() {
      ReadAudio();
      drawAudio4(7);
    }


    void readtime() {
      if(millis() - lastTim >= 1000) {
        co0 = random(0, 25);
        flasher = !flasher;
        lastTim = millis();
      }
      if (NewRTCm != rtc.getMinute()) {
        flashSe = !flashSe;
        co1 = random(0, 25);
        NewRTCm = rtc.getMinute();
      }
    }
    void readtime1() {
      if(millis() - lastTim >= 1000) {
        co0 = random(0, 25);
        flasher = !flasher;
        lastTim = millis();
      }
      if (NewRTCm != rtc.getMinute()) {
        effects.redtemphumi();
        co1 = random(0, 25);
        NewRTCm = rtc.getMinute();
      }
    }
    void readtime2() {
      if (NewRTCm != rtc.getMinute()) {
        effects.redtemphumi();
        co1 = random(0, 25);
        NewRTCm = rtc.getMinute();
      }
    }
    
void drawclockfp() {
      float radians, angle;
      int x0;
      int y0;
      int x1[12] = {60, 31, 17, 10, 17, 33, 59, 86, 102, 108, 99, 87};
      int y1[12] = {6, 10, 18, 30, 41, 49, 54, 49, 41, 29, 17, 10};
      for (int i = 0; i < 8; i++) {
        uint16_t color = MyColor[(co0+i)%26];
        effects.ellipse(CENTRE_X, CENTRE_Y, 56+i, 28+(i/2), color);
      }
      uint16_t color = MyColor[co1];
      for (int i = 0; i < 60; i+=15) {
        angle = i * 6;
        radians = radians(angle);
        x0 = CENTRE_X + 60.0F * sin(radians);
        y0 = CENTRE_Y + 30.0F * cos(radians);
        effects.fillEllipse(x0, y0, 4, 2, color);
     }
      for (int i = 5; i < 65; i+=5) {
        angle = i * 6;
        radians = radians(angle);
        x0 = CENTRE_X + 60.0F * sin(radians);
        y0 = CENTRE_Y + 30.0F * cos(radians);
        effects.fillEllipse(x0, y0, 2, 1, 0);
     }
        effects.setFont(GPFont8x5);
      for (int i = 0; i < 12; i++) {
        effects.setCursor(x1[i],y1[i]);
        uint16_t color = MyColor[(co1+i+sp6)%26];
        effects.setTextColor(color);
        text = (char)(i+48);
        effects.print(text);
      }
}
void drawclockfp1() {
      float radians, angle;
      int x0;
      int y0;
      int x1[12] = {60, 31, 17, 10, 17, 33, 59, 86, 102, 108, 99, 87};
      int y1[12] = {6, 10, 18, 30, 41, 49, 54, 49, 41, 29, 17, 10};
      for (int i = 0; i < 8; i++) {
        uint16_t color = MyColor[barcolor[i]];
        effects.ellipse(CENTRE_X, CENTRE_Y, 56+i, 28+i, color);
      }
      uint16_t color = MyColor[totalcolor];
      for (int i = 0; i < 60; i+=15) {
        angle = i * 6;
        radians = radians(angle);
        x0 = CENTRE_X + 60.0F * sin(radians);
        y0 = CENTRE_Y + 30.0F * cos(radians);
        effects.fillEllipse(x0, y0, 4, 2, color);
     }
      for (int i = 5; i < 65; i+=5) {
        angle = i * 6;
        radians = radians(angle);
        x0 = CENTRE_X + 60.0F * sin(radians);
        y0 = CENTRE_Y + 30.0F * cos(radians);
        effects.fillEllipse(x0, y0, 2, 1, 0);
     }
        effects.setFont(GPFont8x5);
      for (int i = 0; i < 12; i++) {
        effects.setCursor(x1[i],y1[i]);
        uint16_t color = MyColor[barcolor[i]];
        effects.setTextColor(color);
        text = (char)(i+48);
        effects.print(text);
      }
}
    
void drawclock() {
      float radians, angle;
      int x0;
      int y0;
      int x1[12] = {60, 87, 99, 108, 102, 86, 59, 33, 17, 10, 19, 35};
      int y1[12] = {6, 10, 17, 29, 41, 49, 54, 49, 41, 30, 17, 10};
      for (int i = 0; i < 8; i++) {
        uint16_t color = MyColor[(co0+i)%26];
        effects.ellipse(CENTRE_X, CENTRE_Y, 56+i, 28+(i/2), color);
      }
      uint16_t color = MyColor[co1];
      for (int i = 0; i < 60; i+=15) {
        angle = i * 6;
        radians = radians(angle);
        x0 = CENTRE_X + 60.0F * sin(radians);
        y0 = CENTRE_Y + 30.0F * cos(radians);
        effects.fillEllipse(x0, y0, 4, 2, color);
     }
      for (int i = 5; i < 65; i+=5) {
        angle = i * 6;
        radians = radians(angle);
        x0 = CENTRE_X + 60.0F * sin(radians);
        y0 = CENTRE_Y + 30.0F * cos(radians);
        effects.fillEllipse(x0, y0, 2, 1, 0);
     }
        effects.setFont(GPFont8x5);
      for (int i = 0; i < 12; i++) {
        effects.setCursor(x1[i],y1[i]);
        uint16_t color = MyColor[(co1+i+sp6)%26];
        effects.setTextColor(color);
        text = (char)(i+48);
        effects.print(text);
      }
}
    
void drawclock1() {
      float radians, angle;
      int x0;
      int y0;
      int x1[12] = {60, 87, 99, 108, 102, 86, 59, 33, 17, 10, 19, 35};
      int y1[12] = {6, 10, 17, 29, 41, 49, 54, 49, 41, 30, 17, 10};
      for (int i = 0; i < 8; i++) {
        uint16_t color = MyColor[barcolor[i]];
        effects.ellipse(CENTRE_X, CENTRE_Y, 56+i, 28+i, color);
      }
      uint16_t color = MyColor[totalcolor];
      for (int i = 0; i < 60; i+=15) {
        angle = i * 6;
        radians = radians(angle);
        x0 = CENTRE_X + 60.0F * sin(radians);
        y0 = CENTRE_Y + 30.0F * cos(radians);
        effects.fillEllipse(x0, y0, 4, 2, color);
     }
      for (int i = 5; i < 65; i+=5) {
        angle = i * 6;
        radians = radians(angle);
        x0 = CENTRE_X + 60.0F * sin(radians);
        y0 = CENTRE_Y + 30.0F * cos(radians);
        effects.fillEllipse(x0, y0, 2, 1, 0);
     }
        effects.setFont(GPFont8x5);
      for (int i = 0; i < 12; i++) {
        effects.setCursor(x1[i],y1[i]);
        uint16_t color = MyColor[barcolor[i]];
        effects.setTextColor(color);
        text = (char)(i+48);
        effects.print(text);
      }
}

void drawHMS(float angle, int x0, int y0, int c0, int c1, int z1) {
      int x[6];
      int y[6];
      float radians = radians(angle);
      x[0] = CENTRE_X + x0 * sin(radians);
      y[0] = CENTRE_Y + y0 * cos(radians);
      radians = radians(angle+z1);
      x[1] = CENTRE_X + 8 * sin(radians);
      y[1] = CENTRE_Y + 4 * cos(radians);
      radians = radians(angle-z1);
      x[2] = CENTRE_X + 8 * sin(radians);
      y[2] = CENTRE_Y + 4 * cos(radians);
      effects.fillTriangle(CENTRE_X, CENTRE_Y, x[0], y[0], x[1], y[1], MyColor[c0]);
      effects.fillTriangle(CENTRE_X, CENTRE_Y, x[0], y[0], x[2], y[2], MyColor[c0]);
      effects.drawLine(x[0], y[0], x[1], y[1], MyColor[c1]);
      effects.drawLine(x[0], y[0], x[2], y[2], MyColor[c1]);
}

    void drawtime() {
      uint8_t second = rtc.getSecond();
      uint8_t minute = rtc.getMinute();
      uint8_t hour = rtc.getHour();
      if (hour > 12) hour -= 12;
      float angle = 180 - (30 * hour) - (minute / 2);
      drawHMS(angle, 34, 17, 0, 0, 30);
      angle = 180 - (6 * minute);
      drawHMS(angle, 42, 21, 9, 9, 25);
      angle =  180 - (second * 6);
      drawHMS(angle, 50, 25, co0, co1, 20);
      effects.fillEllipse(CENTRE_X,CENTRE_Y, 8, 4, MyColor[co1]);
      effects.fillEllipse(CENTRE_X,CENTRE_Y, 6, 3, MyColor[co0]);
      effects.ellipse(CENTRE_X,CENTRE_Y, 8, 4, MyColor[co1]);
    }

    void drawtime1() {
      uint8_t second = rtc.getSecond();
      uint8_t minute = rtc.getMinute();
      uint8_t hour = rtc.getHour();
      if (hour > 12) hour -= 12;
      float angle = 180 - (30 * hour) - (minute / 2);
      drawHMS(angle, 34, 17, 0, 0, 30);
      angle = 180 - (6 * minute);
      drawHMS(angle, 42, 21, 9, 9, 25);
      angle =  180 - (second * 6);
      uint16_t color1 = MyColor[barcolor[12]];
      uint16_t color2 = MyColor[barcolor[13]];
      drawHMS(angle, 50, 25, barcolor[14], barcolor[15], 20);
      effects.fillEllipse(CENTRE_X,CENTRE_Y, 8, 4, 0xffff);
      effects.fillEllipse(CENTRE_X,CENTRE_Y, 6, 3, color1);
      effects.ellipse(CENTRE_X,CENTRE_Y, 8, 4, color2);
    }

    void drawtimefp() {
      uint8_t second = rtc.getSecond();
      uint8_t minute = rtc.getMinute();
      uint8_t hour = rtc.getHour();
      if (hour > 12) hour -= 12;
      float angle = 180 + (30 * hour) + (minute / 2);
      drawHMS(angle, 34, 17, 0, 0, 30);
      angle = 180 + (6 * minute);
      drawHMS(angle, 42, 21, 9, 9, 25);
      angle =  180 + (second * 6);
      drawHMS(angle, 50, 25, co0, co1, 20);
      effects.fillEllipse(CENTRE_X,CENTRE_Y, 8, 4, MyColor[co1]);
      effects.fillEllipse(CENTRE_X,CENTRE_Y, 6, 3, MyColor[co0]);
      effects.ellipse(CENTRE_X,CENTRE_Y, 8, 4, MyColor[co1]);
    }
    void drawtimefp1() {
      uint8_t second = rtc.getSecond();
      uint8_t minute = rtc.getMinute();
      uint8_t hour = rtc.getHour();
      if (hour > 12) hour -= 12;
      float angle = 180 + (30 * hour) + (minute / 2);
      drawHMS(angle, 34, 17, 0, 0, 30);
      angle = 180 + (6 * minute);
      drawHMS(angle, 42, 21, 9, 9, 25);
      angle =  180 + (second * 6);
      uint16_t color1 = MyColor[barcolor[12]];
      uint16_t color2 = MyColor[barcolor[13]];
      drawHMS(angle, 50, 25, barcolor[14], barcolor[15], 20);
      effects.fillEllipse(CENTRE_X,CENTRE_Y, 8, 4, 0xffff);
      effects.fillEllipse(CENTRE_X,CENTRE_Y, 6, 3, color1);
      effects.ellipse(CENTRE_X,CENTRE_Y, 8, 4, color2);
    }
    
void drawHMTime() {
     String text = Mode24h ? rtc.getTime("%H;%M") : rtc.getTime("%I;%M");
     effects.setFont(NuFont); 
     effects.setCursor(6, 25);
     effects.print(text, co1);
     effects.setFont(GPFont8x9); 
     text = Mode24h ? " " : rtc.getAmPm();
     effects.setCursor(59, 24);
     effects.print(text, co1);
     effects.setFont(GPFont16x16B); 
     text = rtc.getTime("%S");
     effects.setCursor(51, 34);
     effects.print(text, co0);
     effects.setCursor(59, 48);
     text = flasher ? ":" : "";
     effects.print(text, co0);
     int xx = rtc.getSecond() * 2;
     xx = flashSe ? xx+3 : 123-xx;
     effects.fillEllipse(xx, 3, 4, 2, MyColor[co0]);
    }

void drawMesg(String text, uint8_t* font_gp, int len, int ym) {
  uint16_t text_length = text.length();
  effects.setFont(font_gp); 
  effects.setCursor(xps, ym);
  effects.print(text, co1);
  xps -= sp1;
  if (xps < -(text_length * len)) {
    effects.getmesg();
  }
  if (xps > 128) {
    effects.getmesg();
    xps = -128 - (text_length * len);
  }
}

    void drawMode(int x1, int x2, int x3, int y1, int y2, int y3, uint16_t color, int md) {
      switch(md) {
        case 0 :
        effects.fillTriangle(CENTRE_X, CENTRE_Y, x1, y1, x2, y2, color);
        break;
        case 1 :
        effects.fillTriangle(CENTRE_X, CENTRE_Y, x1, y1, x3, y3, color);
        break;
        case 2 :
        effects.fillTriangle(CENTRE_X, CENTRE_Y, x2, y2, x3, y3, color);
        break;
        case 3 :
        effects.fillTriangle(x1, y1, x2, y2, x3, y3, color);
        break;
        case 4 :
        effects.drawTriangle(CENTRE_X, CENTRE_Y, x1, y1, x2, y2, color);
        break;
        case 5 :
        effects.drawTriangle(CENTRE_X, CENTRE_Y, x1, y1, x3, y3, color);
        break;
        case 6 :
        effects.drawTriangle(CENTRE_X, CENTRE_Y, x2, y2, x3, y3, color);
        break;
        case 7 :
        effects.drawTriangle(x1, y1, x2, y2, x3, y3, color);
        break;
      }
    }

    void drawMode(int x1, int x2, int y1, int y2, uint16_t color, int md) {
      switch(md) {
        case 0 :
        effects.fillTriangle(x1, y1, x2, y2, x2, y1, color);
        effects.fillTriangle(x1, y1, x2, y2, x1, y2, color);
        break;
        case 1 :
        effects.fillTriangle(x1, y1, x2, y2, x1, y2, color);
        break;
        case 2 :
        effects.fillTriangle(x1, y1, x2, y2, x2, y1, color);
        break;
        case 3 :
        effects.drawTriangle(x1, y1, x2, y2, x1, y2, color);
        effects.drawTriangle(x1, y1, x2, y2, x2, y1, color);
        break;
        case 4 :
        effects.drawTriangle(x1, y1, x2, y2, x1, y2, color);
        break;
        case 5 :
        effects.drawTriangle(x1, y1, x2, y2, x2, y1, color);
        break;
        case 6 :
        effects.drawTriangle(CENTRE_X, 63, x1, y1, x2, y2, color);
        break;
        case 7 :
        effects.fillTriangle(CENTRE_X, 63, x1, y1, x2, y2, color);
        break;
        case 8 :
        effects.drawLine(x1, y1, x1, y2, color);
        effects.drawLine(x2, y2, x1, y2, color);
        break;
        case 9 :
        effects.drawLine(x1, y1, x2, y1, color);
        effects.drawLine(x1, y1, x1, y2, color);
        effects.drawLine(x2, y2, x1, y2, color);
        effects.drawLine(x2, y2, x2, y1, color);
        break;
        case 10 :
        effects.drawLine(x1, y1, x2, y2, color);
        break;
        case 11 :
        effects.fillTriangle(CENTRE_X, CENTRE_Y, x1, y1, x2, y2, color);
        break;
        case 12 :
        effects.fillTriangle(x1, y1, x2, y2, x1, 63, color);
        effects.fillTriangle(x1, 63, x2, 63, x2, y2, color);
        break;
      }
    }

    void drawAudio1(int mod) {
      float degreesPerLine = 180 / bandCount;
      float angle = degreesPerLine - 90;
      for (int band = 0; band < bandCount; band++) {
        int level = levels[band];
        int nextLevel = levels[band + 1];
        if (band == 15) nextLevel = levels[0];
        uint16_t color1 = MyColor[barcolor[band]];
        float radians = radians(angle - degreesPerLine);
        int x1 = CENTRE_X + level * sin(radians);
        int y1 = 63 - level * cos(radians);
        radians = radians(angle);
        int x2 = CENTRE_X + nextLevel * sin(radians);
        int y2 = 63 - nextLevel * cos(radians);
        drawMode(x1, x2, y1, y2, color1, mod);
        angle += degreesPerLine;
      }
    }

    void drawAudio2(int mod) {
      float degreesPerLine = 360 / bandCount;
      float angle = totalevels;
      for (int band = 0; band < bandCount; band++) {
        int level = levels[band] / 2;
        int nextLevel = levels[band + 1] / 2;
        if (band == 15) nextLevel = levels[0] / 2;
        uint16_t color1 = MyColor[barcolor[band]];
        float radians = radians(angle - degreesPerLine);
        int x1 = CENTRE_X + level * 2 * sin(radians);
        int y1 = CENTRE_Y + level * cos(radians);
        radians = radians(angle);
        int x2 = CENTRE_X + nextLevel * 2 * sin(radians);
        int y2 = CENTRE_Y + nextLevel * cos(radians);
        drawMode(x1, x2, y1, y2, color1, mod);
        angle += degreesPerLine;
      }
    }

    void drawAudio3(int mod) {
        int dx = MATRIX_WIDTH / bandCount;
      for (int i = 0; i < bandCount; i++) {
        int level = levels[i];
        int nextLevel = levels[i + 1];
        if (i == 15) nextLevel = levels[0];
        uint16_t color1 = MyColor[barcolor[i]];
        uint8_t y = MATRIX_HEIGHT - level - 1;
        uint8_t nextY = MATRIX_HEIGHT - nextLevel - 1;
        y = y >= MATRIX_HEIGHT ? MATRIX_HEIGHT - 1 : y;
        nextY = nextY >= MATRIX_HEIGHT ? MATRIX_HEIGHT - 1 : nextY;
        int x1 = i * dx, y1 = y;
        int x2 = i * dx + dx, y2 = nextY;
        drawMode(x1, x2, y1, y2, color1, mod);
      }
    }

    void drawAudio4(int mod) {
      float degreesPerLine = 360 / bandCount;
      float angle = totalevels;
      for (int band = 0; band < bandCount; band++) {
        int level = levels[band]/2;
        int nextLevel = levels[band + 1]/2;
        if (band == 15) nextLevel = levels[0]/2;
        uint16_t color1 = MyColor[barcolor[band]];
        float radians = radians(angle - degreesPerLine);
        int x1 = CENTRE_X + level * 2  * sin(radians);
        int y1 = CENTRE_Y + level * cos(radians);
        radians = radians(angle);
        int x2 = CENTRE_X + level * 2 * sin(radians);
        int y2 = CENTRE_Y + level * cos(radians);
        radians = radians(angle + degreesPerLine);
        int x3 = CENTRE_X + nextLevel * 2 * sin(radians);
        int y3 = CENTRE_Y + nextLevel * cos(radians);
        drawMode(x1, x2, x1, y1, y2, y3, color1, mod);
        angle -= degreesPerLine;
      }
    }

    int currentIndex = 0;

    int getCurrentIndex() {
      return currentIndex;
    }

    AudioPatterns() {
      this->currentItem = items[currentIndex];

      // add the items to the shuffledItems array
      for (int a = 0; a < PATTERN_COUNT; a++) {
        shuffledItems[a] = items[a];
      }

      shuffleItems();
    }

    void moveRandom(int step) {
      currentIndex += step;

      if (currentIndex >= PATTERN_COUNT) currentIndex = 0;
      else if (currentIndex < 0) currentIndex = PATTERN_COUNT - 1;


      currentItem = shuffledItems[currentIndex];
    }

    void shuffleItems() {
      for (int a = 0; a < PATTERN_COUNT; a++)
      {
        int r = random(a, PATTERN_COUNT);
        AudioPatternFunctionPointer temp = shuffledItems[a];
        shuffledItems[a] = shuffledItems[r];
        shuffledItems[r] = temp;
      }
    }

    char* Drawable::name = (char *)"Audio Patterns";

    void move(int step) {
      moveTo(currentIndex + step);
    }

    void moveTo(int index) {
      currentIndex = index;

      if (currentIndex >= PATTERN_COUNT) currentIndex = 0;
      else if (currentIndex < 0) currentIndex = PATTERN_COUNT - 1;


      currentItem = items[currentIndex];
      Serial.println(currentIndex);
    }

    unsigned int drawFrame() {
      clear();
      (this->*currentItem)();
      show();
      return 0;
    }
    
    void listAudioPatterns() {
      Serial.println(F("{"));
      Serial.print(F("  \"count\": "));
      Serial.print(PATTERN_COUNT);
      Serial.println(",");
      Serial.println(F("  \"results\": ["));

      for (int i = 0; i < PATTERN_COUNT; i++) {
        Serial.print(F("    \""));
        Serial.print(i);
        if (i == PATTERN_COUNT - 1)
          Serial.println(F("\""));
        else
          Serial.println(F("\","));
      }

      Serial.println("  ]");
      Serial.println("}");
    }

    bool setAudioPattern(String name) {
      for (int i = 0; i < PATTERN_COUNT; i++) {
        if (name.toInt() == i) {
          moveTo(i);
          return true;
        }
      }

      return false;
    }

    bool setAudioPattern(int index) {
      if (index >= PATTERN_COUNT || index < 0)
        return false;

      moveTo(index);

      return true;
    }
};

#endif
