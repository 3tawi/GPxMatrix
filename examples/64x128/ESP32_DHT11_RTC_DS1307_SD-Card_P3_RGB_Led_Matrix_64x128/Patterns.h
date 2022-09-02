/*
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

#ifndef Patterns_H
#define Patterns_H


class Patterns : public Playlist {
  public:

  
    typedef void (Patterns::*PatternFunctionPointer)(void);
    PatternFunctionPointer currentItem;
    static const int PATTERN_COUNT = 2;
    PatternFunctionPointer shuffledItems[PATTERN_COUNT];
    PatternFunctionPointer items[PATTERN_COUNT] = {
      &Patterns::Clock2,
      &Patterns::Clock1
    };

uint16_t MyColor[25] = {0xF800, 0xFA00, 0xFC00, 0xFE00, 0xFFE0, 0xC7E0, 0x87E0, 0x47E0, 0x07E0, 0x07E8,
                        0x07F0, 0x07F8, 0x07FF, 0x061F, 0x041F, 0x021F, 0x001F, 0x401F, 0x801F, 0xC01F,
                        0xF81F, 0xF818, 0xF810, 0xF808, 0xF800};

void drawMesg() {
  uint16_t text_length = textmsg.length();
  int co = sp4;
  matrix.fillRect(0, 0, 128, 22, 0x0000); 
  matrix.setFont(&atawi11x7g);
  matrix.setCursor(xps, 0);
  co1 = co0;
      for (int t = 0; t < text_length; t++) {
        matrix.setTextColor(MyColor[co1%25]);
        matrix.print(textmsg[t]);
        co1 += sp2;
      }
  matrix.setFont();
  xps -= sp0;
  if (xps < -(text_length * 14)) {
    effects.getmesg();
  }
}
    
 void Clock1() {
  if(millis() - lastTime >= 1000) {
      flasher = !flasher; 
      lastTime = millis();
    }
  clear();
  text = flasher ? rtc.getTime("%H:%M") : rtc.getTime("%H %M");
  if(x+dx>=matrix.width() || x+dx<0)
   dx=-dx;
  x+=dx;
  if(texte.x+texte.dx+texte.width>=matrix.width() || texte.x+texte.dx<0)
    texte.dx=-texte.dx;
  if(texte.y+texte.dy+texte.height>=matrix.height() || texte.y+texte.dy<0)
    texte.dy=-texte.dy;
  texte.x+=texte.dx;
  texte.y+=texte.dy;
  matrix.setFont(&atawi19x11c);
  matrix.setCursor(texte.x, texte.y);
  co3 = co2;
  for (int x = 0; x < 5; x++) {
    matrix.setTextColor(MyColor[co3%25]);
    matrix.print(text[x]);
    co3 += sp2;
  }
  matrix.setFont();
  show();
  if (NewRTCm != rtc.getMinute()) {
     co2 = random(0, 24);
     effects.redtemphumi();
     NewRTCm = rtc.getMinute();
  }
  delay(sp1);
  }

  void Clock2() {
    drawMesg();
    if(millis() - lastTime >= 1000) {
      co3 += sp2;
      co3 %= 25;
      matrix.fillRect(100, 24, 28, 40, 0x0000);
      matrix.setTextColor(MyColor[co3]);
      matrix.setFont(&atawi19x11c);
      matrix.setCursor(104, 25);
      if (flasher) { matrix.print("$"); }
      matrix.setFont();
      matrix.setFont(&atawi11x7g);
      matrix.setTextColor(MyColor[24-co3]);
      matrix.setCursor(102, 42);
      matrix.print(rtc.getTime("%S"));
      matrix.setFont();
      flasher = !flasher;  
      matrix.setTextColor(MyColor[co3]);
      text = flasher ? ":" : " ";
      matrix.fillRect(48, 24, 4, 40, 0x0000); 
      matrix.setFont(&atawi19x11c);
      matrix.setCursor(48, 24);
      matrix.print(text);
      matrix.setFont();
    if (NewRTCm != rtc.getMinute()) {
      text = flasher ? rtc.getTime("%H:%M") : rtc.getTime("%H %M");
      matrix.fillRect(0, 24, 100, 40, 0x0000);
      matrix.setFont(&atawi19x11c);
      matrix.setCursor(0, 24);
      for (int x = 0; x < 5; x++) {
        matrix.setTextColor(MyColor[(co3*x)%24]);
        matrix.print(text[x]);
      }
      matrix.setFont();
      NewRTCm = rtc.getMinute();
    }
    lastTime = millis();
  } 
  show();
  }

    int currentIndex = 0;

    int getCurrentIndex() {
      return currentIndex;
    }

    Patterns() {
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
        PatternFunctionPointer temp = shuffledItems[a];
        shuffledItems[a] = shuffledItems[r];
        shuffledItems[r] = temp;
      }
    }

    char* Drawable::name = (char *)"Patterns";

    void move(int step) {
      moveTo(currentIndex + step);
    }

    void moveTo(int index) {
      currentIndex = index;

      if (currentIndex >= PATTERN_COUNT) currentIndex = 0;
      else if (currentIndex < 0) currentIndex = PATTERN_COUNT - 1;


      currentItem = items[currentIndex];
    }

    unsigned int drawFrame() {
      (this->*currentItem)();
      return 0;
    }
    
    void listPatterns() {
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

    bool setPattern(String name) {
      for (int i = 0; i < PATTERN_COUNT; i++) {
        if (name.toInt() == i) {
          moveTo(i);
          return true;
        }
      }

      return false;
    }

    bool setPattern(int index) {
      if (index >= PATTERN_COUNT || index < 0)
        return false;

      moveTo(index);

      return true;
    }
};

#endif
