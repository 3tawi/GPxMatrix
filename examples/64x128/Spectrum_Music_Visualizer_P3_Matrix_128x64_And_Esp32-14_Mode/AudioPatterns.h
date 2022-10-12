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
    static const int PATTERN_COUNT = 14;
    AudioPatternFunctionPointer shuffledItems[PATTERN_COUNT];
    AudioPatternFunctionPointer items[PATTERN_COUNT] = {
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
      &AudioPatterns::SpMuAudio14
    };
    void SpMuAudio1() {
      drawAudio1();
    }
    void SpMuAudio2() {
      drawAudio2();
    }
    void SpMuAudio3() {
      drawAudio3();
    }
    void SpMuAudio4() {
      drawAudio4();
    }
    void SpMuAudio5() {
      drawAudio5();
    }
    void SpMuAudio6() {
      drawAudio6();
    }
    void SpMuAudio7() {
      drawAudio7(128, 64);
    }
    void SpMuAudio8() {
      drawAudio8(128, 64);
    }
    void SpMuAudio9() {
      drawAudio9(128, 64);
    }
    void SpMuAudio10() {
      drawAudio10(128, 64);
    }
    void SpMuAudio11() {
      drawAudio11(128, 64);
    }
    void SpMuAudio12() {
      drawAudio12(128, 64);
    }
    void SpMuAudio13() {
      drawAudio13();
    }
    void SpMuAudio14() {
      analyzerLinesByIntensity(128, 64);
    }

    void drawAudio1() {
      float degreesPerLine = 360 / bandCount;
      float angle = totalevels;
      for (int band = 0; band < bandCount; band++) {
        int level = levels[band];
        int nextLevel = levels[band + 1];
        if (band == 15) nextLevel = levels[0];
        uint16_t color1 = MyColor[level % 26];
        float radians = radians(angle);
        int x1 = CENTRE_X + level * 2 * sin(radians);
        int y1 = CENTRE_Y + level * cos(radians);
        radians = radians(angle - degreesPerLine);
        int x2 = CENTRE_X + nextLevel * 2 * sin(radians);
        int y2 = CENTRE_Y + nextLevel * cos(radians);
        radians = radians(angle + degreesPerLine);
        int x3 = CENTRE_X + nextLevel * 2 * sin(radians);
        int y3 = CENTRE_Y + nextLevel * cos(radians);
        effects.fillTriangle(x1, y1, x2, y2, x3, y3, color1);
        angle -= degreesPerLine;
      }
    }

    void drawAudio2() {
      float degreesPerLine = 360 / bandCount;
      float angle = totalevels;
      for (int band = 0; band < bandCount; band++) {
        int level = levels[band];
        int nextLevel = levels[band + 1];
        if (band == 15) nextLevel = levels[0];
        uint16_t color1 = MyColor[level % 26];
        uint16_t color2 = MyColor[nextLevel % 26];
        float radians = radians(angle);
        int x1 = CENTRE_X + level * 2 * sin(radians);
        int y1 = CENTRE_Y + level * cos(radians);
        radians = radians(angle - degreesPerLine);
        int x2 = CENTRE_X + nextLevel * 2 * sin(radians);
        int y2 = CENTRE_Y + nextLevel * cos(radians);
        radians = radians(angle + degreesPerLine);
        int x3 = CENTRE_X + nextLevel * 2 * sin(radians);
        int y3 = CENTRE_Y + nextLevel * cos(radians);
        effects.drawTriangle(CENTRE_X, CENTRE_Y, x1, y1, x2, y2, color1);
        effects.fillTriangle(x1, y1, x2, y2, x3, y3, color2);
        angle -= degreesPerLine;
      }
    }

    void drawAudio3() {
      float degreesPerLine = 360 / bandCount;
      float angle = totalevels;
      for (int band = 0; band < bandCount; band++) {
        int level = levels[band];
        int nextLevel = levels[band + 1];
        if (band == 15) nextLevel = levels[0];
        uint16_t color1 = MyColor[level % 26];
        uint16_t color2 = MyColor[nextLevel % 26];
        float radians = radians(angle);
        int x1 = CENTRE_X + level * 2 * sin(radians);
        int y1 = CENTRE_Y + level * cos(radians);
        radians = radians(angle - degreesPerLine);
        int x2 = CENTRE_X + nextLevel * 2 * sin(radians);
        int y2 = CENTRE_Y + nextLevel * cos(radians);
        radians = radians(angle + degreesPerLine);
        int x3 = CENTRE_X + nextLevel * 2 * sin(radians);
        int y3 = CENTRE_Y + nextLevel * cos(radians);
        effects.fillTriangle(CENTRE_X, CENTRE_Y, x1, y1, x2, y2, color1);
        effects.fillTriangle(x1, y1, x2, y2, x3, y3, color2);
        angle -= degreesPerLine;
      }
    }

    void drawAudio4() {
      float degreesPerLine = 360 / bandCount;
      float angle = totalevels;
      for (int band = 0; band < bandCount; band++) {
        int level = levels[band];
        int nextLevel = levels[band + 1];
        if (band == 15) nextLevel = levels[0];
        uint16_t color1 = MyColor[level % 26];
        uint16_t color2 = MyColor[nextLevel % 26];
        float radians = radians(angle);
        int x1 = CENTRE_X + level * 2 * sin(radians);
        int y1 = CENTRE_Y + level * cos(radians);
        radians = radians(angle - degreesPerLine);
        int x2 = CENTRE_X + nextLevel * 2 * sin(radians);
        int y2 = CENTRE_Y + nextLevel * cos(radians);
        effects.fillTriangle(CENTRE_X, CENTRE_Y, x1, y1, x2, y2, color1);
        angle -= degreesPerLine;
      }
    }

    void drawAudio5() {
      float degreesPerLine = 360 / bandCount;
      float angle = totalevels;
      for (int band = 0; band < bandCount; band++) {
        int level = levels[band];
        int nextLevel = levels[band + 1];
        if (band == 15) nextLevel = levels[0];
        uint16_t color1 = MyColor[level % 26];
        uint16_t color2 = MyColor[nextLevel % 26];
        float radians = radians(angle);
        int x1 = CENTRE_X + level * 2 * sin(radians);
        int y1 = CENTRE_Y + level * cos(radians);
        radians = radians(angle - degreesPerLine);
        int x2 = CENTRE_X + nextLevel * 2 * sin(radians);
        int y2 = CENTRE_Y + nextLevel * cos(radians);
        effects.drawLine(x1, y1, x2, y2, color1);
        angle -= degreesPerLine;
      }
    }

    void drawAudio6() {
      float degreesPerLine = 360 / bandCount;
      float angle = totalevels;
      for (int band = 0; band < bandCount; band++) {
        int level = levels[band];
        int nextLevel = levels[band + 1];
        if (band == 15) nextLevel = levels[0];
        uint16_t color1 = MyColor[level % 26];
        uint16_t color2 = MyColor[nextLevel % 26];
        float radians = radians(angle);
        int x1 = CENTRE_X + level * 2 * sin(radians);
        int y1 = CENTRE_Y + level * cos(radians);
        radians = radians(angle + degreesPerLine);
        int x2 = CENTRE_X + nextLevel * 2 * sin(radians);
        int y2 = CENTRE_Y + nextLevel * cos(radians);
        effects.drawLine(x1, y1, x2, y2, color1);
        angle -= degreesPerLine;
      }
    }

    void drawAudio7(int Wx, int Hy) {
        int dx = Wx / bandCount;
        int dy = MATRIX_HEIGHT / Hy;
      for (int i = 0; i < bandCount; i++) {
        int level = levels[i] * 2;
        int nextLevel = levels[i + 1] * 2;
        if (i == 15) nextLevel = levels[0] * 2;

        uint16_t color1 = MyColor[level % 26];

        uint8_t y = Hy - 1 - (level * dy);
        uint8_t nextY = Hy - 1 - (nextLevel * dy);

        y = y >= Hy ? Hy - 1 : y;
        nextY = nextY >= Hy ? Hy - 1 : nextY;

        int x1 = i * dx, y1 = y;
        int x2 = i * dx + dx, y2 = nextY;
        effects.fillTriangle(x1, y1, x2, y2, x2, y1, color1);
      }
    }

    void drawAudio8(int Wx, int Hy) {
        int dx = Wx / bandCount;
        int dy = MATRIX_HEIGHT / Hy;
      for (int i = 0; i < bandCount; i++) {
        int level = levels[i] * 2;
        int nextLevel = levels[i + 1] * 2;
        if (i == 15) nextLevel = levels[0] * 2;

        uint16_t color1 = MyColor[level % 26];

        uint8_t y = Hy - 1 - (level * dy);
        uint8_t nextY = Hy - 1 - (nextLevel * dy);

        y = y >= Hy ? Hy - 1 : y;
        nextY = nextY >= Hy ? Hy - 1 : nextY;

        int x1 = i * dx, y1 = y;
        int x2 = i * dx + dx, y2 = nextY;
        effects.fillTriangle(x1, y1, x2, y2, x2, y1, color1);
        effects.fillTriangle(x1, y1, x2, y2, x1, y2, color1);
      }
    }

    void drawAudio9(int Wx, int Hy) {
        int dx = Wx / bandCount;
        int dy = MATRIX_HEIGHT / Hy;
      for (int i = 0; i < bandCount; i++) {
        int level = levels[i] * 2;
        int nextLevel = levels[i + 1] * 2;
        if (i == 15) nextLevel = levels[0] * 2;

        uint16_t color1 = MyColor[level % 26];

        uint8_t y = Hy - 1 - (level * dy);
        uint8_t nextY = Hy - 1 - (nextLevel * dy);

        y = y >= Hy ? Hy - 1 : y;
        nextY = nextY >= Hy ? Hy - 1 : nextY;

        int x1 = i * dx, y1 = y;
        int x2 = i * dx + dx, y2 = nextY;
        effects.drawLine(x1, y1, x2, y1, color1);
        effects.drawLine(x1, y1, x1, y2, color1);
        effects.drawLine(x2, y2, x1, y2, color1);
        effects.drawLine(x2, y2, x2, y1, color1);
      }
    }

    void drawAudio10(int Wx, int Hy) {
        int dx = Wx / bandCount;
        int dy = MATRIX_HEIGHT / Hy;
      for (int i = 0; i < bandCount; i++) {
        int level = levels[i] * 2;
        int nextLevel = levels[i + 1] * 2;
        if (i == 15) nextLevel = levels[0] * 2;

        uint16_t color1 = MyColor[level % 26];

        uint8_t y = Hy - 1 - (level * dy);
        uint8_t nextY = Hy - 1 - (nextLevel * dy);

        y = y >= Hy ? Hy - 1 : y;
        nextY = nextY >= Hy ? Hy - 1 : nextY;

        int x1 = i * dx, y1 = y;
        int x2 = i * dx + dx, y2 = nextY;
        effects.drawLine(x1, y1, x2, y2, color1);
        effects.drawLine(x2, y1, x1, y2, color1);
      }
    }

    void drawAudio11(int Wx, int Hy) {
        int dx = Wx / bandCount;
        int dy = MATRIX_HEIGHT / Hy;
      for (int i = 0; i < bandCount; i++) {
        int level = levels[i] * 2;
        int nextLevel = levels[i + 1] * 2;
        if (i == 15) nextLevel = levels[0] * 2;

        uint16_t color1 = MyColor[level % 26];

        uint8_t y = Hy - 1 - (level * dy);
        uint8_t nextY = Hy - 1 - (nextLevel * dy);

        y = y >= Hy ? Hy - 1 : y;
        nextY = nextY >= Hy ? Hy - 1 : nextY;

        int x1 = i * dx, y1 = y;
        int x2 = i * dx + dx, y2 = nextY;
        effects.fillTriangle(x1, y1, x2, y2, x1, 63, color1);
        effects.fillTriangle(x1, 63, x2, 63, x2, y2, color1);
      }
    }

    void drawAudio12(int Wx, int Hy) {
        int dx = Wx / bandCount;
        int dy = MATRIX_HEIGHT / Hy;
      for (int i = 0; i < bandCount; i++) {
        int level = levels[i] * 2;
        int nextLevel = levels[i + 1] * 2;
        if (i == 15) nextLevel = levels[0] * 2;

        uint16_t color1 = MyColor[level % 26];

        uint8_t y = Hy - 1 - (level * dy);
        uint8_t nextY = Hy - 1 - (nextLevel * dy);

        y = y >= Hy ? Hy - 1 : y;
        nextY = nextY >= Hy ? Hy - 1 : nextY;

        int x1 = i * dx, y1 = y;
        int x2 = i * dx + dx, y2 = nextY;
        effects.fillTriangle(CENTRE_X, 63, x1, y1, x2, y2, color1);
      }
    }

    void drawAudio13() {
      float degreesPerLine = 180 / bandCount;
      float angle = degreesPerLine - 90;
      for (int band = 0; band < bandCount; band++) {
        int level = levels[band] * 2;
        int nextLevel = levels[band + 1] * 2;
        if (band == 15) nextLevel = levels[0];
        uint16_t color1 = MyColor[level % 26];
        float radians = radians(angle);
        int x1 = CENTRE_X + level * sin(radians);
        int y1 = 63 - level * cos(radians);
        radians = radians(angle - degreesPerLine);
        int x2 = CENTRE_X + nextLevel * sin(radians);
        int y2 = 63 - nextLevel * cos(radians);
        radians = radians(angle + degreesPerLine);
        int x3 = CENTRE_X + nextLevel * sin(radians);
        int y3 = 63 - nextLevel * cos(radians);
        effects.fillTriangle(x1, y1, x2, y2, x3, y3, color1);
        angle += degreesPerLine;
        if (angle > 180) angle -= degreesPerLine - 90;
      }
    }

    void analyzerLinesByIntensity(int Wx, int Hy) {
        int dx = Wx / bandCount;
        int dy = MATRIX_HEIGHT / Hy;
      for (int i = 0; i < bandCount; i++) {
        int level = levels[i] * 2;
        int nextLevel = levels[i + 1] * 2;
        if (i == 15) nextLevel = levels[0] * 2;

        uint16_t color1 = MyColor[level % 26];

        uint8_t y = Hy - 1 - (level * dy);
        uint8_t nextY = Hy - 1 - (nextLevel * dy);

        y = y >= Hy ? Hy - 1 : y;
        nextY = nextY >= Hy ? Hy - 1 : nextY;

        effects.drawLine(i * dx, y, i * dx + dx, nextY, color1);
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
      ReadAudio();
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
