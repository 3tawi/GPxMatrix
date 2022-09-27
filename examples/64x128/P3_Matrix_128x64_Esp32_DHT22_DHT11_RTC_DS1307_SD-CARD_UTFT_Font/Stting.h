
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

#ifndef Stting_H
#define Stting_H


class Stting {
public:

const char* Apssid = "Esp32-P3";
const char* Appassword = "GPxMatrix";
char user[30], pasw[30];

void setup() {
  readSttWifi();
  readSttspeed();
  updateTime();
  prepacard();
}

void prepacard() { 
    File root = SD.open("/Temhumi");
    if(!root.isDirectory()){
        SD.mkdir("/Temhumi");
    }
}

void updateTime()
{
  rtc.DSgetdatime(); 
  configTime(Tz * 3600, 0, "", "");
} 

void writeTime()
{
  configTime(0, 0, "", ""); 
  rtc.DSsetdatime(); 
  configTime(Tz * 3600, 0, "", "");
}

void getWifi() {
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
}

void getAPWifi()
{
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
    msg = 4, effects.getmesg(); 
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
    sp0 = 3;
    sp1 = 20;
    sp2 = 6;
    sp3 = 8000;
    Ci = 0;
    Modeauto = false;
    Message = "Great Projects wish you a very nice day. Hope you have a great time ahead.";
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
      else if (Line.startsWith("Cindex=")) {
        Line = Line.substring(7);
        if (Line.length() > 0) {
          Ci = Line.toInt();
        }
        Serial.println(Ci);
      }
      else if (Line.startsWith("mclock=")) {
        Line = Line.substring(7);
        if (Line.length() > 0) {
          Modeauto = Line.toInt();
        }
        Serial.println(Modeauto);
      }
      else if (Line.startsWith("Message=")) {
        Line = Line.substring(8);
        if (Line.length() > 0) {
          Message = Line;
        }
        Serial.println(Message);
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
             text += "\nCindex=";
             text += Ci;
             text += "\nmclock=";
             text += Modeauto;
             text += "\nMessage=";
             text += Message;
             text += "\n";
      file.print(text);
      file.close();
      Serial.println(text);
    }
}

};

#endif
