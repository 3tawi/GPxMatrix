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

public:
    DigitalClock4() {
        name = (char *)"DigitalClock4";
    }

    void start() {
    }

    unsigned int playframe() {
      effects.fillRect(0, 2, 128, 14, 0x0000);
      drawMesg(textmsg);
      if(millis() - lastTime >= 1000) {
        effects.fillRect(0, 17, 128, 5, 0x0000);
        effects.fillRect(61, 29, 7, 29, 0x0000);
        drawSTime();
        flasher = !flasher;
        lastTime = millis();
      }
      if (NewRTCm != rtc.getMinute()) {
        co3 = random(0, 25);
        effects.fillRect(0, 24, 128, 40, 0x0000);
        drawHMTime();
        flashSe = !flashSe;
        NewRTCm = rtc.getMinute();
      }
    show();
    return 0;
    }
    

void drawMesg(String text) {
  uint16_t text_length = text.length();
  effects.setFont(GPFont8x9); 
  effects.setCursor(xps, 2);
  co1 = co0;
  effects.print(text, co1);
  xps -= sp0;
  if (xps < -(text_length * 8)) {
    effects.getmesg();
  }
}

void drawSTime() {
     co2 = rtc.getTime("%S").toInt();
     int xx = flashSe ? (co2*2)+4 : 128-((co2*2)+4);
     co2 += co3;
     effects.fillEllipse(xx, 19, 4, 2, MyColor[co2%25]);
     String text = flasher ? ":" : ";";
     effects.setFont(NuFont); 
     effects.setCursor(52, 24);
     effects.print(text, co2);
    }
    
void drawHMTime() {
     String text = flasher ? rtc.getTime("%H:%M") : rtc.getTime("%H;%M");
     effects.setFont(NuFont); 
     effects.setCursor(4, 24);
     effects.print(text, co3);
    }
};

#endif
