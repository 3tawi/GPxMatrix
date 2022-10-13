/* 
// - REQUIRES the following Arduino libraries:
// - Lien vidéo: https://youtu.be/Haiq1Ytl-lY
// - Diagram: https://youtu.be/l33WRd2h6gM
// - GPxMatrix Library: https://github.com/3tawi/GPxMatrix
// - Adafruit_GFX Library: https://github.com/adafruit/Adafruit-GFX-Library
// - Getting Started ESP32 / ESP32S : https://www.youtube.com/watch?v=9b0Txt-yF7E
// Find All "Great Projects" Videos : https://www.youtube.com/c/GreatProjects
*/
#include <WiFi.h>
#include <WebServer.h>
#include <GPxMatrix.h>
#include "SPIFFS.h"
#include "page.h"

#define A    32
#define B    17
#define C    33
#define D    16
#define CLK  0 
#define LAT  2 
#define OE   4


GPxMatrix matrix(A, B, C, D, CLK, LAT, OE, true, 64);

WebServer server(80);

#define clear()          matrix.fillScreen(0)
#define show()           matrix.swapBuffers(true)
#define MATRIX_HEIGHT 32
#define MATRIX_WIDTH 64
int sp0, sp1, sp2, sp3, sp4;
bool wifiok = false;
bool wifiapok = false;
bool ModeAuto = true;
int Ci;
String textip;
long lastTime, prevTime, prevTim; 
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
    } else { 
    wifiok = false;
    } 
  Serial.println("");
  Serial.println(textip);
}

void setup() {
  Serial.begin(115200);
  SPIFFS.begin();
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
    clear();
    audioPatterns.drawFrame();
    show();
    lastTime = millis();
  }
  if(millis() - prevTime >= sp1) {
    for (byte band = 0; band < bandCount; band++) {
        levels[band] -= sp2;
    }
    prevTime = millis();
  }
  if(millis() - prevTim >= sp3 && ModeAuto) {
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

void handleSpe0() {
    sp0 = server.arg("Speed0").toInt();
    handleRoot();
    effects.SaveStting();
}
void handleSpe1() {
    sp1 = server.arg("Speed1").toInt();
    handleRoot();
    effects.SaveStting();
}
void handleSpe2() {
    sp2 = server.arg("Speed2").toInt();
    handleRoot();
    effects.SaveStting();
}
void handleSpe3() {
    sp3 = server.arg("Speed3").toInt();
    sp3 *= 1000;
    handleRoot();
    effects.SaveStting();
}
void handleSpe4() {
    int sp04 = server.arg("Speed4").toInt();
    sp04 = 11 - sp04;
    sp4 = 102.3 * sp04;
    handleRoot();
    effects.SaveStting();
}

void handleModeAuto() {
    ModeAuto = !ModeAuto;
    handleRoot();
    effects.SaveStting();
}
void handleModePre() { 
  audioPatterns.stop();
  audioPatterns.move(-1);
  audioPatterns.start();
  Ci = audioPatterns.getCurrentIndex();
  handleRoot();
}
void handleModeNex() { 
  audioPatterns.stop();
  audioPatterns.move(1);
  audioPatterns.start();
  Ci = audioPatterns.getCurrentIndex();
  handleRoot();
}
void handleModePat() { 
  Ci = server.arg("Pattern").toInt();
  Ci -= 1;
  audioPatterns.stop();
  setAudioPattern(Ci);
  audioPatterns.start();
  handleRoot();
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
  server.on("/", handleRoot);
  server.on("/mySpeed0", handleSpe0);
  server.on("/mySpeed1", handleSpe1);
  server.on("/mySpeed2", handleSpe2);
  server.on("/mySpeed3", handleSpe3);
  server.on("/mySpeed4", handleSpe4);
  server.on("/modepre", handleModePre);
  server.on("/modenex", handleModeNex);
  server.on("/modeauto", handleModeAuto);
  server.on("/modepat", handleModePat);
  server.on("/readstat", handlestate);
  server.on("/mywifi", handleWifi);
  server.on("/restesp", handleRestesp);
  server.begin();
}
void handlestate() {
  String content = "<?xml version = \"1.0\" ?>";
  content += "<inputs><analog>";
  content += Ci+1;
  content += "</analog><analog>";
  content += ModeAuto;
  content += "</analog></inputs>";
  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "text/xml", content); //Send web page
}
