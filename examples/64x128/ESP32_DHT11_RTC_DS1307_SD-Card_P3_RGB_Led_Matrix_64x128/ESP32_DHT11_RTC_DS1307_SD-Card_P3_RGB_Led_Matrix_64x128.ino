/* REQUIRES the following Arduino libraries:
// - Lien vidéo: https://youtu.be/fFUG7dE3uSk
// - Diagram: https://youtu.be/l33WRd2h6gM
// - GPxMatrix Library: https://github.com/3tawi/GPxMatrix
// - Adafruit_GFX Library: https://github.com/adafruit/Adafruit-GFX-Library
// - DS1307ESP Library: https://github.com/3tawi/DS1307ESP
// - DHT Sensor Library: https://github.com/3tawi/esp32DHT
// - Getting Started ESP32 / ESP32S : https://www.youtube.com/watch?v=9b0Txt-yF7E
// Find All "Great Projects" Videos : https://www.youtube.com/c/GreatProjects
*/

#include <WiFi.h>
#include <WebServer.h>
#include <DS1307ESPTime.h>
#include <GPxMatrix.h>
#include <atawi19x11c.h>
#include <atawi11x7g.h>
#include <esp32DHT.h>
#include <SD.h>
#include "SPIFFS.h"
#include "page.h"


#define P_A    32
#define P_B    17
#define P_C    33
#define P_D    16
#define P_E    5
#define P_CLK  0 
#define P_LAT  2 
#define P_OE   4

GPxMatrix matrix(P_A, P_B, P_C, P_D, P_E, P_CLK, P_LAT, P_OE, true, 128);
WebServer server(80);
DS1307ESPTime rtc;
DHT22 sensor;

#define clear()          matrix.fillScreen(0)
#define show()           matrix.swapBuffers(true)
#define MATRIX_HEIGHT 64
#define MATRIX_WIDTH 128

String Message = "Great Projects wish you a very nice day. Hope you have a great time ahead.";
int Tz = 1; // Time Zone
int NewRTCh = 24;
int NewRTCm = 60;
int NewRTCs = 60;
bool flasher = false;
bool wifiok = false;
bool wifiapok = false;
bool Modeclock = true;
int Ci;
uint32_t lastTime, prevTime, prevTim; 
String text, textmsg, textip;
String datee, datetxt, ddate;
float Temp, Humi;
float NewTemp, NewHumi;
int sp0, sp1, sp2, sp3, sp4, xps = 128;
int msg = 4;
int fon; // language 0 = EN  |  1 = FR  |  2 = GR | 3 = ES
String langfont[4] = {"EN", "FR", "GR", "ES"}; 
int co0, co1, co2, co3;

char user[30], pasw[30];
String  Ssid, Pass;
IPAddress ip(192,168,1,69);
IPAddress gateway(192, 168,1,1);
IPAddress gatewayap(192,168,4,1);
IPAddress subnet(255, 255, 255, 0);

struct Text {
 char *texte;
  uint16_t width, height;
  int16_t x, y;
  int16_t dx, dy;
} texte = {"22:22", 0, 0, 0, 0, 1, 1};
int16_t x=0, dx=1;

#include "Effects.h"
Effects effects;
#include "Drawable.h"
#include "Playlist.h"
#include "Patterns.h"
Patterns patterns;


void(* resetFunc) (void) = 0;//declare reset function at address 0

void getWifi() {
  if (wifiapok) { WiFi.softAPdisconnect(true); }
  int lengthSsid = Ssid.length() + 1;
  int lengthPass = Pass.length() + 1;
  Ssid.toCharArray(user, lengthSsid);
  Pass.toCharArray(pasw, lengthPass);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(user, pasw);
  int xc = 0;
    Serial.println(" ");
  while (WiFi.status() != WL_CONNECTED && xc < 25) {
    delay(1000);
    Serial.print(".");
    xc++;
  }
    Serial.println(" ");
  if (WiFi.status() == WL_CONNECTED) { 
    textip = "CONNECTED To: ";
    textip += Ssid;
    textip += " IP Address: http://";
    textip += WiFi.localIP().toString().c_str();
    wifiok = true;
    msg = 4, effects.getmesg(); 
    } else { 
    wifiok = false;
    }
}

void handleRoot() {
 server.send(200, "text/html", MAIN_page); //Send web page
}
void handlesendate() {
    handleRoot();
    datetxt = server.arg("bday");
  if (datetxt == "") { datetxt = rtc.getTime("%F"); }
    datee = "/Temhumi/";
    datee += datetxt;
    datee += ".txt";
}
void handletxt() {
  if (datetxt == "") { handlesendate(); }
  File file = SD.open(datee);
  if (file) {
    size_t sent = server.streamFile(file, "text/plain");
  } else {
    server.send(200, "text/plain", "file not fond");
  }
    file.close();
}
void handleMesg() { 
    handleRoot();
    Message = server.arg("Message");
    Message = effects.printTextMesg(Message);
    msg = 0, effects.getmesg(); 
  }
void handlezoneTime() {
    handleRoot();
    Tz = server.arg("locatz").toInt(); 
    configTime(Tz * 3600, 0, "", "");
    msg = 3, effects.getmesg(); 
    NewRTCm = 60;
}
void handlelocaltime() { 
    handleRoot();
    rtc.setTime(server.arg("locadt").toInt());
    effects.writeTime();
    msg = 3, effects.getmesg();  
    NewRTCm = 60;
}
void handleMyTime() { 
    handleRoot();
    String hbuf = server.arg("htmie"); 
    String dbuf = server.arg("ddate");
    text = hbuf[0];
    text += hbuf[1];
    int h = text.toInt(); 
    text = hbuf[3];
    text += hbuf[4];
    int m = text.toInt(); 
    text = hbuf[6];
    text += hbuf[7];
    int s = text.toInt();
    
    text = dbuf[0];
    text += dbuf[1];
    text += dbuf[2];
    text += dbuf[3];
    int yr = text.toInt(); 
    text = dbuf[5];
    text += dbuf[6];
    int mo = text.toInt();
    text = dbuf[8];
    text += dbuf[9];
    int dd = text.toInt();
    rtc.setTime(s, m, h, dd, mo, yr);
    effects.writeTime();
    msg = 3, effects.getmesg(); 
    NewRTCm = 60;
}
void handleModera() { 
  handleRoot();
  clear();
  NewRTCm = 60;
  patterns.stop();
  patterns.move(1);
  patterns.start();
  Ci = patterns.getCurrentIndex();
  Ci %= 2;
}
void handleChangefont() {  
    handleRoot();
    fon++;
    fon%=4;
    effects.getmesg();
}
void handleSpe0() {
    handleRoot();
    sp0 = server.arg("Speed0").toInt();
}
void handleSpe1() {
    handleRoot();
    sp1 = server.arg("Speed1").toInt();
}
void handleSpe2() {
    handleRoot();
    sp2 = server.arg("Speed2").toInt();
}
void handleSpe3() {
    handleRoot();
    sp3 = server.arg("Speed3").toInt();
    sp3 *= 1000;
}
void handleModeam() {
    handleRoot();
    Modeclock = !Modeclock;
}
void Savewifi() {
    handleRoot();
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
void handleSave() {
    handleRoot();
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
             text += "\nCindex=";
             text += Ci;
             text += "\nmclock=";
             text += Modeclock;
             text += "\nMessage=";
             text += Message;
             text += "\n";
      file.print(text);
      file.close();
      Serial.println(text);
    }
}
void handleRestesp() {  
    handleRoot();
    delay(4000);     
    resetFunc();
}
void handleWifi() {  
  handleRoot();
  if (wifiapok) { WiFi.softAPdisconnect(true); }
  wifiapok = false;
  delay(1000);
  Ssid = server.arg("usname");
  Pass = server.arg("pssw");
  getWifi();
  if (!wifiok) { effects.getAPWifi(); } 
  else { Savewifi(); }   
}
void prepacard() { 
    File root = SD.open("/Temhumi");
    if(!root.isDirectory()){
        SD.mkdir("/Temhumi");
    }
}
void setup() {
  Serial.begin(115200);
  SPIFFS.begin();
  effects.SttingWifi();
  getWifi();
  delay(2000);
  rtc.DSbegin();
  effects.updateTime();
  SD.begin(3);
  prepacard();
  server.on("/", handleRoot);
  server.on("/myMesg", handleMesg);  
  server.on("/mytimezon", handlezoneTime); 
  server.on("/localdatime", handlelocaltime);
  server.on("/restime", handleMyTime);
  server.on("/MesSpeed0", handleSpe0);
  server.on("/MesSpeed1", handleSpe1);
  server.on("/MesSpeed2", handleSpe2);
  server.on("/MesSpeed3", handleSpe3);
  server.on("/modema", handleModeam);
  server.on("/sendate", handlesendate);
  server.on("/readtext", handletxt);
  server.on("/readMesge", handleMesge); 
  server.on("/sendate", handlesendate);
  server.on("/readtemhu", handlestate);
  server.on("/modera", handleModera);
  server.on("/changefont", handleChangefont);
  server.on("/SttSave", handleSave);
  server.on("/Mywifi", handleWifi);
  server.on("/restesp", handleRestesp);
  server.begin();
  delay(500);
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setFont(&atawi19x11c); 
  int16_t x1 = 0, y1 = 0;
  matrix.setTextSize(2);
  matrix.getTextBounds(texte.texte, 0, 0, &x1, &y1, &texte.width, &texte.height);
  texte.width-=2;
  texte.height-=2;
  x=0, dx=1;
  delay(500);
  sensor.setup(13);  // pin 13 is DATA, RMT channel defaults to channel 0 and 1
  sensor.onData([](float humidity, float temperature) {
  NewHumi = humidity;
  NewTemp = temperature;
  });
  delay(500);
  effects.SttingSpeed();
  listPatterns();
  setPattern(Ci); 
  patterns.start();
  if (!wifiok) { effects.getAPWifi(); } 
}

void loop() {
  server.handleClient(); 
  patterns.drawFrame();
  if(millis() - prevTim >= sp3 && Modeclock) {
  clear();
  NewRTCm = 60;
  patterns.stop();
  patterns.move(1);
  patterns.start();
  prevTim = millis();
  }
}

void listPatterns() {
  patterns.listPatterns();
}

bool setPattern(String name) {
  if (patterns.setPattern(name)) {
    return true;
  }
  return false;
}

bool setPattern(int index) {
  if (patterns.setPattern(index)) {
    return true;
  }
  return false;
}
void handleMesge() {
  String content = "<?xml version = \"1.0\" ?>";
  content += "<inputs><analog>";
  content += datetxt;
  content += "</analog></inputs>";
  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "text/xml", content); //Send web page
}
void handlestate() {
  String content = "<?xml version = \"1.0\" ?>";
  content += "<inputs><analog>";
  content += Temp;
  content += "</analog><analog>";
  content += Humi;
  content += "</analog><analog>";
  content += Tz;
  content += "</analog><analog>";
  content += fon;
  content += "</analog><analog>";
  content += Ci+1;
  content += "</analog><analog>";
  content += Modeclock;
  content += "</analog></inputs>";
  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "text/xml", content); //Send web page
}
