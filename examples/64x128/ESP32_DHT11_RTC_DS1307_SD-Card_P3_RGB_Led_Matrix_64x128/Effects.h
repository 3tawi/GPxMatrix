
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


class Effects {
public:

String ftemp[4] = {"Temperature : ", "Température : ", "Temperatur : ", "Temperatura : "}; 
String fhumi[4] = {"Humidity : ", "Humidité : ", "Feuchtigkeit : ", "Humedad : "}; 

const char* Apssid = "Esp32-P3";
const char* Appassword = "GPxMatrix";

  void Setup() {
    }



void getAPWifi()
{
    WiFi.softAPConfig(ip, gateway, subnet);
    WiFi.softAP(Apssid, Appassword);
    textip = "Local Wifi : ";
    textip += Apssid;
    textip += " Password: ";
    textip += Appassword;
    textip += " IP Address: http://";
    textip += WiFi.softAPIP().toString().c_str();
    wifiapok = true;
    msg = 4, getmesg(); 
}
int codchar(char source1) {
  int so1 = source1;
    switch(so1) {
    case 169 :
        so1 = 232;
        break;
    case 168 :
        so1 = 233;
        break;
    case 187 :
        so1 = 251;
        break;
    case 164 :
        so1 = 228;
        break;
    case 161 :
        so1 = 225;
        break;
    default :
        so1 = 195;
        break;
    }
    return (so1);
}
int gtchar(char source1) {
  int so1 = source1;
    return (so1);
}
String printTextMesg(String source) {
  int so;
  String sour="";
  int k = source.length();
  for(int j=0; j<k; j++) {
    so = gtchar(source[j]);
    if (so == 195) {
      so = codchar(source[j+1]);
    if (so != 195) { j++; }
      so = codchar(source[j]);
    }
    sour += char(so);
    }
    return (sour);
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
        textmsg = ftemp[fon];
        textmsg = printTextMesg(textmsg);
        textmsg += NewTemp;
        textmsg += (char)(1);
        msg++;
        break;
    case 2 :
        textmsg = fhumi[fon];
        textmsg = printTextMesg(textmsg);
        textmsg += NewHumi;
        textmsg += " %";
        msg++;
        break;
    case 3 :
        textmsg = printTextMesg(rtc.getDate(true));
        msg = 0;
        break;
    case 4 :
        textmsg = textip;
        msg = 1;
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
    text += " ";
    text += ftemp[fon];
    text += Temp;
    text += " *C ";
    text += fhumi[fon];
    text += Humi;
    text += " %";
    text = printTextMesg(text);
    writeFileSD(ddate, text);
}
void writeFileSD(const String path, const String mesg){
    File fileth = SD.open(path, FILE_APPEND);
     if(fileth){ fileth.println(mesg); } 
    fileth.close();
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
void SttingWifi() {
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
void SttingSpeed() {
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
    Modeclock = false;
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
          Modeclock = Line.toInt();
        }
        Serial.println(Modeclock);
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
};

#endif
