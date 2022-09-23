/* REQUIRES the following Arduino libraries:
// - Lien vidéo: https://youtu.be/EvmB_jVc5IA
// - Diagram: https://youtu.be/l33WRd2h6gM
// - GPxMatrix Library: https://github.com/3tawi/GPxMatrix
// - Adafruit_GFX Library: https://github.com/adafruit/Adafruit-GFX-Library
// - DS1307ESP Library: https://github.com/3tawi/DS1307ESPTime
// - DHT Sensor Library: https://github.com/3tawi/esp32DHT
// - Getting Started ESP32 / ESP32S : https://www.youtube.com/watch?v=9b0Txt-yF7E
// Find All "Great Projects" Videos : https://www.youtube.com/c/GreatProjects
*/


#include <WiFi.h>
#include <WebServer.h>
#include <GPxMatrix.h>
#include <DS1307ESPTime.h>
#include <esp32DHT.h>
#include <SD.h>
#include "SPIFFS.h"
#include "BigFont.h"
#include "morocco.h"
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
const uint16_t NUM_LEDS = (MATRIX_WIDTH * MATRIX_HEIGHT) + 1;

String  Ssid, Pass;
IPAddress ip(192,168,1,69);
IPAddress gateway(192, 168,1,1);
IPAddress gatewayap(192,168,4,1);
IPAddress subnet(255, 255, 255, 0);

int xps = 128, msg = 0, fon;
int co0, co1, co2, co3;
int sp0, sp1, sp2, sp3, sp4, Ci;
String Message = "Great Projects wish you a very nice day. Hope you have a great time ahead.";
int Tz = 1; // Time Zone
uint32_t lastTime, prevTime, prevTim; 
String textmsg, textip, text;
String datee, datetxt, ddate;
float Temp, Humi;
float NewTemp, NewHumi;
int NewRTCh = 24;
int NewRTCm = 60;
int NewRTCs = 60;
bool flasher = false;
bool flashSe = false;
bool Modeauto = false;
bool wifiok = false;
bool wifiapok = false;
uint16_t MyColor[26] = {0xF000, 0xF800, 0xFA00, 0xFC00, 0xFE00, 0xFFE0, 0xC7E0, 0x87E0, 0x47E0, 0x07E0, 0x07E8,
                        0x07F0, 0x07F8, 0x07FF, 0x061F, 0x041F, 0x021F, 0x001F, 0x401F, 0x801F, 0xC01F,
                        0xF81F, 0xF818, 0xF810, 0xF808, 0xF800};
                    

#include "Effects.h"
Effects effects;
#include "Stting.h"
Stting stting;
#include "Play.h"
#include "Modeclock.h"
Modeclock Mode;

void(* resetFunc) (void) = 0;//declare reset function at address 0


void setup() {
  Serial.begin(115200);
  SPIFFS.begin();
  rtc.DSbegin();
  SD.begin(3);
  stting.setup();
  sensor.setup(13);  // pin 13 is DATA
  matrix.begin();
  Mode.setModeclock(Ci);
  Mode.start();
  handleserver();
  server.begin();
  stting.getWifi();
  if (!wifiok) { stting.getAPWifi(); }
}

void loop() {
  server.handleClient(); 
  if(millis() - prevTime >= sp1) {
    Mode.playframe();
    prevTime = millis();
  }
  if(millis() - prevTim >= sp3 && Modeauto) {
  effects.ClearFrame();
  movePatterns(1);
  NewRTCm = 60;
  prevTim = millis();
  }
}

void movePatterns(int i) {
    flashSe = !flashSe;
    Mode.stop();  
    Mode.move(i);
    Mode.start(); 
    Ci = Mode.getModeIndex();
    stting.SaveStting();
}

void handleserver() {
  sensor.onData([](float humidity, float temperature) {
  NewHumi = humidity;
  NewTemp = temperature;
  }); 
  server.on("/", handleRoot);
  server.on("/myMesg", handleMesg); 
  server.on("/modema", handleModeam);
  server.on("/modepre", handleModePre);
  server.on("/modenex", handleModeNex);
  server.on("/MesSpeed0", handleSpe0);
  server.on("/MesSpeed1", handleSpe1);
  server.on("/MesSpeed2", handleSpe2);
  server.on("/MesSpeed3", handleSpe3);
  server.on("/mytimezon", handlezoneTime); 
  server.on("/localdatime", handlelocaltime);
  server.on("/restime", handleMyTime);
  server.on("/readMesge", handleMesge); 
  server.on("/readtemhu", handlestate);
  server.on("/sendate", handlesendate);
  server.on("/readtext", handletxt);
  server.on("/Mywifi", handleWifi);
  server.on("/restesp", handleRestesp);
}

void handleModePre() { 
  handleRoot();
  effects.ClearFrame();
  movePatterns(-1);
  NewRTCm = 60;
}

void handleModeNex() { 
  handleRoot();
  effects.ClearFrame();
  movePatterns(1);
  NewRTCm = 60;
}
void handleRestesp() {  
    handleRoot();
    delay(1000);     
    resetFunc();
}
void handleRoot() {
 server.send(200, "text/html", MAIN_page); //Send web page
}
void sendatetxt() {
    datetxt = rtc.getTime("%F");
    datee = "/Temhumi/";
    datee += datetxt;
    datee += ".txt";
}
void handlesendate() {
    handleRoot();
    datetxt = server.arg("bday");
    datee = "/Temhumi/";
    datee += datetxt;
    datee += ".txt";
}
void handletxt() {
  if (datetxt == "") { sendatetxt(); }
  File file = SD.open(datee);
  if (file) {
    size_t sent = server.streamFile(file, "text/plain");
  } else {
    server.send(200, "text/plain", "file not fond");
  }
    file.close();
}
void handleModeam() {
    handleRoot();
    Modeauto = !Modeauto;
    stting.SaveStting();
}
void handleMesg() { 
    handleRoot();
    Message = server.arg("Message");
    stting.SaveStting();
    msg = 0, effects.getmesg(); 
}
void handleSpe0() {
    handleRoot();
    sp0 = server.arg("Speed0").toInt();
    stting.SaveStting();
}
void handleSpe1() {
    handleRoot();
    sp1 = server.arg("Speed1").toInt();
    stting.SaveStting();
}
void handleSpe2() {
    handleRoot();
    sp2 = server.arg("Speed2").toInt();
    stting.SaveStting();
}
void handleSpe3() {
    handleRoot();
    sp3 = server.arg("Speed3").toInt();
    sp3 *= 1000;
    stting.SaveStting();
}
void handlezoneTime() {
    handleRoot();
    Tz = server.arg("locatz").toInt(); 
    configTime(Tz * 3600, 0, "", "");
    stting.SaveStting();
    msg = 3, effects.getmesg(); 
    NewRTCm = 60;
}
void handlelocaltime() { 
    handleRoot();
    rtc.setTime(server.arg("locadt").toInt());
    stting.writeTime();
    msg = 5, effects.getmesg();  
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
    stting.writeTime();
    msg = 5, effects.getmesg(); 
    NewRTCm = 60;
}
void handleWifi() {  
  handleRoot();
  wifiapok = false;
  delay(1000);
  Ssid = server.arg("usname");
  Pass = server.arg("pssw");
  stting.getWifi();
  stting.Savewifi();
  if (!wifiok) { stting.getAPWifi(); } 
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
  content += Ci+1;
  content += "</analog><analog>";
  content += Modeauto;
  content += "</analog></inputs>";
  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "text/xml", content); //Send web page
}
