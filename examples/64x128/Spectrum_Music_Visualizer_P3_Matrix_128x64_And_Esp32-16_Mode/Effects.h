
/*
 * Adapted by 3tawi
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 *
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef Effects_H
#define Effects_H



#define SWAPint(X,Y) { \
        int temp = X ; \
        X = Y ; \
        Y = temp ; \
    }

class Effects {
public:

const char* Apssid = "Esp32-P3";
const char* Appassword = "GPxMatrix";

  void Setup() {
  }
  
  void ClearFrame() {
    clear();
  }
  
  void drawRGBPixel(int16_t x, int16_t y, int color) {
    if ((x < 0) || (y < 0) || (x >= MATRIX_WIDTH) || (y >= MATRIX_HEIGHT))
      return;
    matrix.drawPixel(x, y, color);
  }
  
  void drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h) {
    for (int16_t j = 0; j < h; j++, y++) {
      for (int16_t i = 0; i < w; i++){
        int color = pgm_read_word(&bitmap[j * w + i]);
        drawRGBPixel(x + i, y, color);
      }
    }
  }
  
  void drawTRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h) {
    for (int16_t j = 0; j < h; j++, y++) {
      for (int16_t i = 0; i < w; i++){
        int color = pgm_read_word(&bitmap[j * w + i]);
        if (color != 0) drawRGBPixel(x + i, y, color);
      }
    }
  }

  void ShowFrame() {
    show();
  }

  void drawLine(int x0, int y0, int x1, int y1, int color)
  {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;
    for (;;) {
      drawRGBPixel(x0, y0, color);
      if (x0 == x1 && y0 == y1) break;
      e2 = 2 * err;
      if (e2 > dy) {
        err += dy;
        x0 += sx;
      }
      if (e2 < dx) {
        err += dx;
        y0 += sy;
      }
    }
  }
void drawCircle(int16_t x0, int16_t y0, int16_t r, int color) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  drawRGBPixel(x0, y0 + r, color);
  drawRGBPixel(x0, y0 - r, color);
  drawRGBPixel(x0 + r, y0, color);
  drawRGBPixel(x0 - r, y0, color);

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    drawRGBPixel(x0 + x, y0 + y, color);
    drawRGBPixel(x0 - x, y0 + y, color);
    drawRGBPixel(x0 + x, y0 - y, color);
    drawRGBPixel(x0 - x, y0 - y, color);
    
    drawRGBPixel(x0 + y, y0 + x, color);
    drawRGBPixel(x0 - y, y0 + x, color);
    drawRGBPixel(x0 + y, y0 - x, color);
    drawRGBPixel(x0 - y, y0 - x, color);
  }
}

void fillCircle(int16_t x0, int16_t y0, int16_t r, int color) {
  drawFastVLine(x0, y0 - r, 2 * r + 1, color);
  fillCircleHelper(x0, y0, r, 3, 0, color);
}

void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, int color) {

  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;
  int16_t px = x;
  int16_t py = y;

  delta++; // Avoid some +1's in the loop

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    // These checks avoid double-drawing certain lines, important
    // for the SSD1306 library which has an INVERT drawing mode.
    if (x < (y + 1)) {
      if (corners & 1)
        drawFastVLine(x0 + x, y0 - y, 2 * y + delta, color);
      if (corners & 2)
        drawFastVLine(x0 - x, y0 - y, 2 * y + delta, color);
    }
    if (y != py) {
      if (corners & 1)
        drawFastVLine(x0 + py, y0 - px, 2 * px + delta, color);
      if (corners & 2)
        drawFastVLine(x0 - py, y0 - px, 2 * px + delta, color);
      py = y;
    }
    px = x;
  }
}

void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int color) {
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
}
void xLine(int x0, int x1, int y, int color)
  {
    if (x0 > x1)
    {
      int xb = x0;
      x0 = x1;
      x1 = xb;
    }
    if (x0 < 0)
      x0 = 0;
    if (x1 > MATRIX_WIDTH)
      x1 = MATRIX_WIDTH;
    for (int x = x0; x < x1; x++)
    drawRGBPixel(x, y, color);
  }
  
void ellipse(int x, int y, int rx, int ry, int color)
  {
    if(ry == 0)
      return;
    int oxr = rx;
    float f = float(rx) / ry;
    f *= f;
    for(int i = 0; i < ry + 1; i++)
    {
      float s = rx * rx - i * i * f;
      int xr = (int)sqrt(s <= 0 ? 0 : s);
      xLine(x - oxr, x - xr + 1, y + i, color);
      xLine(x + xr, x + oxr + 1, y + i, color);
      if(i) 
      {
        xLine(x - oxr, x - xr + 1, y - i, color);
        xLine(x + xr, x + oxr + 1, y - i, color);
      }
      oxr = xr;
    }
  }

void fillEllipse(int x, int y, int rx, int ry, int color)
  {
    if(ry == 0)
      return;
    float f = float(rx) / ry;
    f *= f;   
    for(int i = 0; i < ry + 1; i++)
    {
      float s = rx * rx - i * i * f;
      int xr = (int)sqrt(s <= 0 ? 0 : s);
      xLine(x - xr, x + xr + 1, y + i, color);
      if(i) 
        xLine(x - xr, x + xr + 1, y - i, color);
    }
  }

  void fillRect(int x, int y, int w, int h, int color)
  {
    if (x < 0)
    {
      w += x;
      x = 0;
    }
    if (y < 0)
    {
      h += y;
      y = 0;
    }
    if (x + w > MATRIX_WIDTH)
      w = MATRIX_WIDTH - x;
    if (y + h > MATRIX_HEIGHT)
      h = MATRIX_HEIGHT - y;
    for (int j = y; j < y + h; j++)
      for (int i = x; i < x + w; i++)
      drawRGBPixel(i, j, color);
  }
void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int color) {

  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    SWAPint(y0, y1);
    SWAPint(x0, x1);
  }
  if (y1 > y2) {
    SWAPint(y2, y1);
    SWAPint(x2, x1);
  }
  if (y0 > y1) {
    SWAPint(y0, y1);
    SWAPint(x0, x1);
  }

  if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if (x1 < a)
      a = x1;
    else if (x1 > b)
      b = x1;
    if (x2 < a)
      a = x2;
    else if (x2 > b)
      b = x2;
    drawFastHLine(a, y0, b - a + 1, color);
    return;
  }

  int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
          dx12 = x2 - x1, dy12 = y2 - y1;
  int32_t sa = 0, sb = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if (y1 == y2)
    last = y1; // Include y1 scanline
  else
    last = y1 - 1; // Skip it

  for (y = y0; y <= last; y++) {
    a = x0 + sa / dy01;
    b = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if (a > b)
      SWAPint(a, b);
    drawFastHLine(a, y, b - a + 1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = (int32_t)dx12 * (y - y1);
  sb = (int32_t)dx02 * (y - y0);
  for (; y <= y2; y++) {
    a = x1 + sa / dy12;
    b = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if (a > b)
      SWAPint(a, b);
    drawFastHLine(a, y, b - a + 1, color);
  }
}
void drawFastHLine(int16_t x, int16_t y, int16_t w, int color) {
  drawLine(x, y, x + w - 1, y, color);
}
void drawFastVLine(int16_t x, int16_t y, int16_t h, int color) {
  drawLine(x, y, x, y + h - 1, color);
}

void getAPWifi() {
  WiFi.softAPdisconnect(true);
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(Apssid, Appassword);
  textip = "Local Wifi : ";
  textip += Apssid;
  textip += " Password: ";
  textip += Appassword;
  textip += " IP Address: http://";
  textip += WiFi.softAPIP().toString().c_str();
  wifiapok = true;
  Serial.println("");
  Serial.println(textip);
}

void readSttWifi() {
  File file = SPIFFS.open("/Stt-wifi.txt");
  if(file) {
    String textwifi = "";
    while (file.available()) {
      int savednum = file.read();
      textwifi += (char)savednum;
     }
    file.close();
    readsource(textwifi);
  }
}

void readSttspeed() {
  File file = SPIFFS.open("/Sttingsp.txt");
  if(file) {
    String text = "";
    while (file.available()) {
      int savednum = file.read();
      text += (char)savednum;
     }
    file.close();
    readsource(text);
  } else {
    sp0 = 15;
    sp1 = 50;
    sp2 = 2;
    sp3 = 8000;
    sp4 = 750;
    Ci = 0;
    ModeAuto = true;
  }
}

void readsource(String source) {
  String line = "";
  int k = source.length();
  for(int j=0; j<k; j++) {
    if (source[j] == '\n') {
      readLine(line);
      line = "";
      } else
    line += source[j];
  }
}
void readLine(String Line) {
        Serial.println(Line);
      if (Line.startsWith("speed0=")) {
        Line = Line.substring(7);
        if (Line.length() > 0) {
          sp0 = Line.toInt();
        }
        Serial.println(sp0);
      }
      else if (Line.startsWith("speed1=")) {
        Line = Line.substring(7);
        if (Line.length() > 0) {
          sp1 = Line.toInt();
        }
        Serial.println(sp1);
      }
      else if (Line.startsWith("speed2=")) {
        Line = Line.substring(7);
        if (Line.length() > 0) {
          sp2 = Line.toInt();
        }
        Serial.println(sp2);
      }
      else if (Line.startsWith("speed3=")) {
        Line = Line.substring(7);
        if (Line.length() > 0) {
          sp3 = Line.toInt();
        }
        Serial.println(sp3);
      }
      else if (Line.startsWith("speed4=")) {
        Line = Line.substring(7);
        if (Line.length() > 0) {
          sp4 = Line.toInt();
        }
        Serial.println(sp4);
      }
      else if (Line.startsWith("Cindex=")) {
        Line = Line.substring(7);
        if (Line.length() > 0) {
          Ci = Line.toInt();
        }
        Serial.println(Ci);
      }
      else if (Line.startsWith("mAuto=")) {
        Line = Line.substring(6);
        if (Line.length() > 0) {
          ModeAuto = Line.toInt();
        }
        Serial.println(ModeAuto);
      }
      else if (Line.startsWith("Ssid=")) {
        Line = Line.substring(5);
        if (Line.length() > 0) {
          Ssid = Line;
        }
        Serial.println(Ssid);
      }
      else if (Line.startsWith("Pass=")) {
        Line = Line.substring(5);
        if (Line.length() > 0) {
          Pass = Line;
        }
        Serial.println(Pass);
      }
  }
  
void Savewifi() {
    File file = SPIFFS.open("/Stt-wifi.txt", FILE_WRITE);
    if (file) {
      String textw =  "\n";
             textw += "Ssid=";
             textw += Ssid;
             textw += "\nPass=";
             textw += Pass;
             textw += "\n";
      file.print(textw);
      file.close();
      Serial.println(textw);
    }
}
  
void SaveStting() {
    File file = SPIFFS.open("/Sttingsp.txt", FILE_WRITE);
    if (file) {
      String text =  "speed0=";
             text += sp0;
             text += "\nspeed1=";
             text += sp1;
             text += "\nspeed2=";
             text += sp2;
             text += "\nspeed3=";
             text += sp3;
             text += "\nspeed4=";
             text += sp4;
             text += "\nCindex=";
             text += Ci;
             text += "\nmAuto=";
             text += ModeAuto;
             text += "\n";
      file.print(text);
      file.close();
      Serial.println(text);
    }
}
};

#endif
