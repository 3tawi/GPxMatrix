
/*
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


#define fontbyte(x) Font[x] 

uint16_t XY( uint8_t x, uint8_t y) 
{
    if( x >= MATRIX_WIDTH || x < 0) return 0;
    if( y >= MATRIX_HEIGHT || y < 0) return 0;  
  
    return (y * MATRIX_WIDTH) + x + 1; 
}

#define SWAPint(X,Y) { \
        int temp = X ; \
        X = Y ; \
        Y = temp ; \
    }

class Effects {
public:

  
  void ClearFrame() {
    matrix.fillScreen(0);
  }
  
  void drawRGBPixel(int16_t x, int16_t y, uint16_t color) {
    if ((x < 0) || (y < 0) || (x >= MATRIX_WIDTH) || (y >= MATRIX_HEIGHT))
      return;
    matrix.drawPixel(x, y, color);
  }
  
  void drawHuePixel(int x, int y, long colorIndex) {
    uint16_t color = matrix.ColorHSV(colorIndex, 255, 255, true);
    matrix.drawPixel(x, y, color);
  }
  
  void drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h) {
    for (int16_t j = 0; j < h; j++, y++) {
      for (int16_t i = 0; i < w; i++){
        uint16_t color = pgm_read_word(&bitmap[j * w + i]);
        drawRGBPixel(x + i, y, color);
      }
    }
  }
  
  void drawTRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h) {
    for (int16_t j = 0; j < h; j++, y++) {
      for (int16_t i = 0; i < w; i++){
        uint16_t color = pgm_read_word(&bitmap[j * w + i]);
        if (color != 0) drawRGBPixel(x + i, y, color);
      }
    }
  }

  void ShowFrame() {
    show();
  }

  void drawLine(int x0, int y0, int x1, int y1, uint16_t color)
  {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;
    for (;;) {
      matrix.drawPixel(x0, y0, color);
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
void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  matrix.drawPixel(x0, y0 + r, color);
  matrix.drawPixel(x0, y0 - r, color);
  matrix.drawPixel(x0 + r, y0, color);
  matrix.drawPixel(x0 - r, y0, color);

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    matrix.drawPixel(x0 + x, y0 + y, color);
    matrix.drawPixel(x0 - x, y0 + y, color);
    matrix.drawPixel(x0 + x, y0 - y, color);
    matrix.drawPixel(x0 - x, y0 - y, color);
    
    matrix.drawPixel(x0 + y, y0 + x, color);
    matrix.drawPixel(x0 - y, y0 + x, color);
    matrix.drawPixel(x0 + y, y0 - x, color);
    matrix.drawPixel(x0 - y, y0 - x, color);
  }
}

void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
  drawFastVLine(x0, y0 - r, 2 * r + 1, color);
  fillCircleHelper(x0, y0, r, 3, 0, color);
}

void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, uint16_t color) {

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

void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t color) {
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
}
void xLine(int x0, int x1, int y, int16_t color)
  {
    if (y < 0 || y >= MATRIX_HEIGHT)
      return;
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
    matrix.drawPixel(x, y, color);
  }
  
void ellipse(int x, int y, int rx, int ry, int16_t color)
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

void fillEllipse(int x, int y, int rx, int ry, int16_t color)
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

  void fillRect(int x, int y, int w, int h, int16_t color)
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
      matrix.drawPixel(i, j, color);
  }
void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t color) {

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
void drawFastHLine(int16_t x, int16_t y, int16_t w, int16_t color) {
  drawLine(x, y, x + w - 1, y, color);
}
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
  drawLine(x, y, x, y + h - 1, color);
}

void printhue(char *st, int c)
{
  int x = cursor_x;
  int y = cursor_y;
  int stl = strlen(st);
  for (int cnt=0; cnt<stl; cnt++){
    int16_t color = MyColor[c%25];
    _print_char(*st++, x + (cnt*(x_size)), y, color);
    c+=sp2;
  }
}
void print(char *st)
{
  int x = cursor_x;
  int y = cursor_y;
  int16_t color = textcolor;
  int stl = strlen(st);
  for (int cnt=0; cnt<stl; cnt++)
    _print_char(*st++, x + (cnt*(x_size)), y, color);
}
void printbg(char *st)
{
  int x = cursor_x;
  int y = cursor_y;
  int stl = strlen(st);
  for (int cnt=0; cnt<stl; cnt++)
    _print_bgchar(*st++, x + (cnt*(x_size)), y);
}

void printbg(String st)
{
  char buf[st.length()+1];
  st.toCharArray(buf, st.length()+1);
  printbg(buf);
}

void print(String st)
{
  char buf[st.length()+1];
  st.toCharArray(buf, st.length()+1);
  print(buf);
}
void print(String st, int c)
{
  char buf[st.length()+1];
  st.toCharArray(buf, st.length()+1);
  printhue(buf, c);
}

void _print_char(unsigned char c, int x, int y, int16_t color)
{
   int font_idx = ((c - offset)*((x_size*y_size/8)))+4;
    int cbyte=fontbyte(font_idx);
    int cbit=7;
    for (int cy=0; cy<y_size; cy++)
    {
      for (int cx=0; cx<x_size; cx++)
      {
        if ((cbyte & (1<<cbit)) != 0)
          matrix.drawPixel(x+cx, y+cy, color);
        cbit--;
        if (cbit<0)
        {
          cbit=7;
          font_idx++;
          cbyte=fontbyte(font_idx);
        }
      }
    }
}
void _print_bgchar(unsigned char c, int x, int y)
{
   int font_idx = ((c - offset)*((x_size*y_size/8)))+4;
    int cbyte=fontbyte(font_idx);
    int cbit=7;
    for (int cy=0; cy<y_size; cy++)
    {
      for (int cx=0; cx<x_size; cx++)
      {
        if ((cbyte & (1<<cbit)) == 0)
          matrix.drawPixel(x+cx, y+cy, 0x0);
        cbit--;
        if (cbit<0)
        {
          cbit=7;
          font_idx++;
          cbyte=fontbyte(font_idx);
        }
      }
    }
}

void setFont(uint8_t* font_c)
{
  Font=font_c;
  x_size=fontbyte(0);
  y_size=fontbyte(1);
  offset=fontbyte(2);
  numchars=fontbyte(3);
}

void setTextColor(uint16_t c) 
{
    textcolor = c;
}

void setCursor(int16_t x, int16_t y) 
{
    cursor_x = x;
    cursor_y = y;
}  

void getmesg()
{
    xps = 128;
    co0 = random(0, 24);
    switch(msg)
    {
    case 0 :
        redtemphumi();
        textmsg = Message;
        msg++;
        break;
    case 1 :
        textmsg = "Temperature : ";
        textmsg += NewTemp;
        textmsg += " ";
        textmsg += (char)127;
        msg++;
        break;
    case 2 :
        textmsg = "Humidity : ";
        textmsg += NewHumi;
        textmsg += " H%";
        msg++;
        break;
    case 3 :
        textmsg = rtc.getDate(true);
        msg = 0;
        break;
    case 4 :
        textmsg = textip;
        msg = 0;
        break;
    case 5 :
        textmsg = "Time Saved";
        msg = 3;
        break;
    }
}
void redtemphumi() {
  sensor.read();
  if (Temp != NewTemp || Humi != NewHumi) {
    Temp = NewTemp;
    Humi = NewHumi;
    wirttemphu();
  }
}
void wirttemphu() {
    ddate = "/Temhumi/";
    ddate += rtc.getTime("%F");
    ddate += ".txt";
    text = rtc.getTime("%T");
    text += " Temperature : ";
    text += Temp;
    text += " *C - Humidity : ";
    text += Humi;
    text += " H%";
    writeFileSD(ddate, text);
}
void writeFileSD(const String path, const String mesg){
    File fileth = SD.open(path, FILE_APPEND);
     if(fileth){ fileth.println(mesg); } 
    fileth.close();
}

protected:
  uint8_t* Font;
  uint8_t x_size;
  uint8_t y_size;
  uint8_t offset;
  uint8_t numchars;
  int16_t cursor_x;     ///< x location to start print()ing text
  int16_t cursor_y;     ///< y location to start print()ing text
  uint16_t textcolor;   ///< 16-bit background color for print()
};

#endif
