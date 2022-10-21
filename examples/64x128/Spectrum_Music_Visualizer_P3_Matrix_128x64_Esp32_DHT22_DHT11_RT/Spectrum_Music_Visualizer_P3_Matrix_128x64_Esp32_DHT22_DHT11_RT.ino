/* REQUIRES the following Arduino libraries:
// - Lien vidéo: https://youtu.be/hlDggNHrKxA
// - Diagram: https://youtu.be/l33WRd2h6gM
// - GPxMatrix Library: https://github.com/3tawi/GPxMatrix
// - Adafruit_GFX Library: https://github.com/adafruit/Adafruit-GFX-Library
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
#include "GPFont.h"
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
DS1307ESPTime rtc;
DHT11 sensor;

WebServer server(80);

#define clear()          matrix.fillScreen(0)
#define show()           matrix.swapBuffers(true)
#define MATRIX_HEIGHT 64
#define MATRIX_WIDTH 128
int co0, co1, co2, co3;
int sp0, sp1, sp2, sp3, sp4, sp5, sp6;
bool wifiok = false;
bool wifiapok = false;
bool ModeAuto = true;
bool flasher = false;
bool flashSe = false;
bool Mode24h = true;
int Ci;
String textmsg, textip, text;
String datee, datetxt, ddate;
long lastTime, lastTim, prevTime, prevTim; 
float Temp, Humi;
float NewTemp, NewHumi;
int Tz = 1; // Time Zone
int NewRTCh = 24;
int NewRTCm = 60;
int NewRTCs = 60;
int xps = 128, msg = 0;
String Message = "Great Projects wish you a very nice day. Hope you have a great time ahead.";
uint16_t MyColor[26] = {0xF000, 0xF800, 0xFA00, 0xFC00, 0xFE00, 0xFFE0, 0xC7E0, 0x87E0, 0x47E0, 0x07E0, 0x07E8,
                        0x07F0, 0x07F8, 0x07FF, 0x061F, 0x041F, 0x021F, 0x001F, 0x401F, 0x801F, 0xC01F,
                        0xF81F, 0xF818, 0xF810, 0xF808, 0xF800};

String  Ssid, Pass;
IPAddress ip(192,168,1,31);
IPAddress gateway(192, 168,1,1);
IPAddress gatewayap(192,168,4,1);
IPAddress subnet(255, 255, 255, 0);

#include "AudioLogic.h"
#include "Effects.h"
Effects effects;
#include "Drawable.h"
#include "Playlist.h"
#include "AudioPatterns.h"
AudioPatterns audioPatterns;


void(* resetFunc) (void) = 0;//declare reset function at address 0


void getWifi() {
  char user[30], pasw[30];
  int lengthSsid = Ssid.length() + 1;
  int lengthPass = Pass.length() + 1;
  Ssid.toCharArray(user, lengthSsid);
  Pass.toCharArray(pasw, lengthPass);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(user, pasw);
  int xc = 0;
    Serial.println(" ");
  while (WiFi.status() != WL_CONNECTED && xc < 15) {
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
  Serial.println("");
  Serial.println(textip);
}

void setup() {
  Serial.begin(115200);
  SD.begin(3);
  SPIFFS.begin();
  rtc.DSbegin();
  sensor.setup(13);  // pin 13 is DATA
  effects.Setup();
  effects.readSttWifi();
  getWifi();
  if (!wifiok) { effects.getAPWifi(); }
  handleserver();
  server.begin();
  matrix.begin();
  effects.readSttspeed();
  listAudioPatterns();
  setAudioPattern(Ci); 
  audioPatterns.start();
}

void loop() {
  server.handleClient(); 
  if(millis() - lastTime >= sp0) {
    audioPatterns.drawFrame();
    lastTime = millis();
  }
  if(millis() - prevTime >= sp2) {
    for (byte band = 0; band < bandCount; band++) {
        levels[band] -= sp3;
    }
    prevTime = millis();
  }
  if(millis() - prevTim >= sp6 && ModeAuto) {
    audioPatterns.stop();
    audioPatterns.move(1);
    audioPatterns.start();
    prevTim = millis();
  }
}

void listAudioPatterns() {
  audioPatterns.listAudioPatterns();
}

bool setAudioPattern(String name) {
  if (audioPatterns.setAudioPattern(name)) {
    return true;
  }
  return false;
}

bool setAudioPattern(int index) {
  if (audioPatterns.setAudioPattern(index)) {
    return true;
  }
  return false;
}

void handleRestesp() {  
    handleRoot();
    delay(4000);     
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
  File fdate = SD.open(datee);
  if (fdate) {
    size_t sent = server.streamFile(fdate, "text/plain");
  } else {
    server.send(200, "text/plain", "file not fond");
  }
    fdate.close();
}

void handleSpe0() {
    handleRoot();
    sp0 = server.arg("Speed0").toInt();
    effects.SaveStting();
}
void handleSpe1() {
    handleRoot();
    sp1 = server.arg("Speed1").toInt();
    effects.SaveStting();
}
void handleSpe2() {
    handleRoot();
    sp2 = server.arg("Speed2").toInt();
    effects.SaveStting();
}
void handleSpe3() {
    handleRoot();
    sp3 = server.arg("Speed3").toInt();
    effects.SaveStting();
}
void handleSpe4() {
    handleRoot();
    sp4 = server.arg("Speed4").toInt();
    effects.SaveStting();
}
void handleSpe5() {
    handleRoot();
    sp5 = server.arg("Speed5").toInt();
    effects.SaveStting();
}
void handleSpe6() {
    handleRoot();
    sp6 = server.arg("Speed6").toInt();
    sp6 *= 1000;
    effects.SaveStting();
}

void handlezoneTime() {
    handleRoot();
    Tz = server.arg("locatz").toInt(); 
    configTime(Tz * 3600, 0, "", "");
    effects.SaveStting();
    msg = 3, effects.getmesg(); 
    NewRTCm = 60;
}
void handlelocaltime() { 
    handleRoot();
    rtc.setTime(server.arg("locadt").toInt());
    effects.writeTime();
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
    effects.writeTime();
    msg = 5, effects.getmesg(); 
    NewRTCm = 60;
}

void handleMode24h() {
    Mode24h = !Mode24h;
    handleRoot();
    effects.SaveStting();
}
void handleModeAuto() {
    ModeAuto = !ModeAuto;
    handleRoot();
    effects.SaveStting();
}
void handleModePre() { 
  handleRoot();
  audioPatterns.stop();
  audioPatterns.move(-1);
  audioPatterns.start();
  Ci = audioPatterns.getCurrentIndex();
  effects.SaveStting();
}
void handleModeNex() {
  handleRoot(); 
  audioPatterns.stop();
  audioPatterns.move(1);
  audioPatterns.start();
  Ci = audioPatterns.getCurrentIndex();
  effects.SaveStting();
}
void handleModePat() { 
  handleRoot();
  Ci = server.arg("Pattern").toInt();
  Ci -= 1;
  audioPatterns.stop();
  setAudioPattern(Ci);
  audioPatterns.start();
  effects.SaveStting();
}

void handleMesg() { 
    handleRoot();
    Message = server.arg("Message");
    msg = 0, effects.getmesg(); 
    effects.SaveStting();
}
void handleWifi() {  
  handleRoot();
  wifiapok = false;
  Ssid = server.arg("usname");
  Pass = server.arg("pssw");
  getWifi();
  effects.Savewifi();
  if (!wifiok) { effects.getAPWifi(); } 
}
void handleserver() {
  sensor.onData([](float humidity, float temperature) {
  NewHumi = humidity;
  NewTemp = temperature;
  }); 
  server.on("/", handleRoot);
  server.on("/mySpeed0", handleSpe0);
  server.on("/mySpeed1", handleSpe1);
  server.on("/mySpeed2", handleSpe2);
  server.on("/mySpeed3", handleSpe3);
  server.on("/mySpeed4", handleSpe4);
  server.on("/mySpeed5", handleSpe5);
  server.on("/mySpeed6", handleSpe6);
  server.on("/modepre", handleModePre);
  server.on("/modenex", handleModeNex);
  server.on("/modeauto", handleModeAuto);
  server.on("/modepat", handleModePat);
  server.on("/readstat", handlestate);
  server.on("/mytimezon", handlezoneTime); 
  server.on("/localdatime", handlelocaltime);
  server.on("/restime", handleMyTime);
  server.on("/myMesg", handleMesg); 
  server.on("/sendate", handlesendate);
  server.on("/readtext", handletxt);
  server.on("/readdata", handledata);
  server.on("/mode24h", handleMode24h);
  server.on("/mywifi", handleWifi);
  server.on("/restesp", handleRestesp);
  server.begin();
}
void handlestate() {
  String content = "<?xml version = \"1.0\" ?>";
  content += "<inputs><analog>";
  content += NewTemp;
  content += "</analog><analog>";
  content += NewHumi;
  content += "</analog><analog>";
  content += Tz;
  content += "</analog><analog>";
  content += Ci+1;
  content += "</analog><analog>";
  content += ModeAuto;
  content += "</analog><analog>";
  content += Mode24h;
  content += "</analog></inputs>";
  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "text/xml", content); //Send web page
}
void handledata() {
  String content = "<?xml version = \"1.0\" ?>";
  content += "<inputs><analog>";
  content += datetxt;
  content += "</analog></inputs>";
  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "text/xml", content); //Send web page
}
