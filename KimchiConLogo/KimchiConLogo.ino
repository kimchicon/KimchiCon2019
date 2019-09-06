#include <M5Stack.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include "WebServer.h"
#include <Preferences.h>

// Include the header files that contain the icons
#include "Music.h"
#include "Logo.h"

#define NOTE_D0 -1
#define NOTE_D1 294
#define NOTE_D2 330
#define NOTE_D3 350
#define NOTE_D4 393
#define NOTE_D5 441
#define NOTE_D6 495
#define NOTE_D7 556

#define NOTE_DL1 147
#define NOTE_DL2 165
#define NOTE_DL3 175
#define NOTE_DL4 196
#define NOTE_DL5 221
#define NOTE_DL6 248
#define NOTE_DL7 278

#define NOTE_DH1 589
#define NOTE_DH2 661
#define NOTE_DH3 700
#define NOTE_DH4 786
#define NOTE_DH5 882
#define NOTE_DH6 990
#define NOTE_DH7 112

const IPAddress apIP(192, 168, 4, 1);
char *letters = "abcdefghijklmnopqrstuvwxyz0123456789";
boolean settingMode;
String ssidList;
String wifi_ssid;
String wifi_password;

char Buffer00[10];
int Buffer01Length=10;

// DNSServer dnsServer;
WebServer webServer(80);

// wifi config store
Preferences preferences;

typedef String(* FunctionPointerType) ();

typedef struct
{
  FunctionPointerType functionPtr;
  String name;
  String description;
  char buffer[30];
} FunctionInformation;

FunctionInformation functionInformation;
char projectName[15];

extern const unsigned char m5stack_startup_music[];

void playMusic()
{
  static uint8_t brightness, pre_brightness;
  uint32_t length = strlen((char*)m5stack_startup_music);
  M5.Lcd.setBrightness(0);
  for(int i=0; i<length; i++) {
      dacWrite(SPEAKER_PIN, m5stack_startup_music[i]>>2);
      delayMicroseconds(40);
      brightness = (i/157);
      if(pre_brightness != brightness) {
          pre_brightness = brightness;
          M5.Lcd.setBrightness(brightness);
      }
  }

  M5.Power.setWakeupButton(BUTTON_A_PIN);
  if(!M5.Power.canControl()) {
    // M5.Lcd.printf("IP5306 is not i2c version\n");
  }  
}

void setup()
{
  M5.begin();
  // Swap the colour byte order when rendering
  M5.Lcd.setSwapBytes(true);
  preferences.begin("wifi-config");

  delay(10);
  if (restoreConfig()) {   
    if (checkConnection()) {
      settingMode = false;
      startWebServer();           
      return;
    }
  }
  settingMode = true;
  setupMode();  
}

void loop()
{
  if (settingMode) {
  }
  webServer.handleClient();
}

boolean restoreConfig() {
  wifi_ssid = preferences.getString("WIFI_SSID");
  wifi_password = preferences.getString("WIFI_PASSWD");
  Serial.print("WIFI-SSID: ");
  M5.Lcd.print("WIFI-SSID: ");
  Serial.println(wifi_ssid);
  M5.Lcd.println(wifi_ssid);
  WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());

  if(wifi_ssid.length() > 0) {
    return true;
} else {
    return false;
  }
}

boolean checkConnection() {
  int count = 0;
  Serial.print("Waiting for Wi-Fi connection");
  M5.Lcd.print("Waiting for Wi-Fi connection");
  while ( count < 30 ) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println();
      M5.Lcd.println();
      Serial.println("Connected!");
      M5.Lcd.println("Connected!");
      return (true);
    }
    delay(500);
    Serial.print(".");
    M5.Lcd.print(".");
    count++;
  }
  Serial.println("Timed out.");
  M5.Lcd.println("Timed out.");
  return false;
}

String Test()
{
    return "This is a Test";
}

String Backdoor()
{
    playMusic();
    M5.Lcd.fillScreen(TFT_BLACK);
    delay(100);  
    M5.Lcd.fillScreen(TFT_GREEN);
    delay(100);  
    M5.Lcd.fillScreen(TFT_RED);
    delay(100);  
    M5.Lcd.fillScreen(TFT_YELLOW);
    delay(100);
    M5.Lcd.pushImage(0, 0, infoWidth, infoHeight, logo);
    delay(2000);
    
    return "This is a Backdoor";
}

void startWebServer() {
  strcpy(projectName, "KimchiCon");

  functionInformation.functionPtr=Backdoor;
  Serial.print("Backdoor:" + int(functionInformation.functionPtr));

  functionInformation.functionPtr=Test;
  Serial.print("Test:" + int(functionInformation.functionPtr));
  
  Test();
  
  if (settingMode) {
    Serial.print("Starting Web Server at ");
    M5.Lcd.print("Starting Web Server at ");
    Serial.println(WiFi.softAPIP());
    M5.Lcd.println(WiFi.softAPIP());
    
    webServer.on("/settings", []() {
      String s = "<h1>Wi-Fi Settings</h1><p>Please enter your password by selecting the SSID.</p>";
      s += "<form method=\"get\" action=\"setap\"><label>SSID: </label><select name=\"ssid\">";
      s += ssidList;
      s += "</select><br>Password: <input name=\"pass\" length=64 type=\"password\"><input type=\"submit\"></form>";
      webServer.send(200, "text/html", makePage("Wi-Fi Settings", s));
    });
    webServer.on("/setap", []() {
      String ssid = urlDecode(webServer.arg("ssid"));
      Serial.print("SSID: ");
      M5.Lcd.print("SSID: ");
      Serial.println(ssid);
      M5.Lcd.println(ssid);
      String pass = urlDecode(webServer.arg("pass"));
      Serial.print("Password: ");
      M5.Lcd.print("Password: ");
      Serial.println(pass);
      M5.Lcd.println(pass);
      Serial.println("Writing SSID to EEPROM...");
      M5.Lcd.println("Writing SSID to EEPROM...");

      // Store wifi config
      Serial.println("Writing Password to nvr...");
      M5.Lcd.println("Writing Password to nvr...");
      preferences.putString("WIFI_SSID", ssid);
      preferences.putString("WIFI_PASSWD", pass);

      Serial.println("Write nvr done!");
      M5.Lcd.println("Write nvr done!");
      String s = "<h1>Setup complete.</h1><p>device will be connected to \"";
      s += ssid;
      s += "\" after the restart.";
      webServer.send(200, "text/html", makePage("Wi-Fi Settings", s));
      delay(3000);
      ESP.restart();
    });
    webServer.onNotFound([]() {
      String s = "<h1>AP mode</h1><p><a href=\"/settings\">Wi-Fi Settings</a></p>";
      webServer.send(200, "text/html", makePage("AP mode", s));
    });
  }
  else {
    Serial.print("Starting Web Server at ");
    M5.Lcd.print("Starting Web Server at ");
    Serial.println(WiFi.localIP());
    M5.Lcd.println(WiFi.localIP());
    M5.Lcd.println("Let's play a game...");

    webServer.on("/", []() {
      char buf[1];
      String s = "<h1>STA mode</h1><p><a href=\"/reset\">Reset Wi-Fi Settings</a></p> <p>";
      webServer.send(200, "text/html", makePage("STA mode", s));
    });

    webServer.on("/reset", []() {
      // reset the wifi config
      preferences.remove("WIFI_SSID");
      preferences.remove("WIFI_PASSWD");
      String s = "<h1>Wi-Fi settings was reset.</h1><p>Please reset device.</p>";
      webServer.send(200, "text/html", makePage("Reset Wi-Fi Settings", s));
      delay(3000);
      ESP.restart();
    });
  }

  /*
   * These are for debugging purposes
   */

  /*
  webServer.on("/read", []() {
    int *address = (int *) strtol(urlDecode(webServer.arg("address")).c_str(), (char **) '\0', 16);
    String hex_data = String(((int)(*address) & 0xff), HEX) + " "
             + String((((int)(*address) >> 8) & 0xff), HEX) + " "
             + String((((int)(*address) >> 16) & 0xff), HEX) + " "
             + String((((int)(*address) >> 24) & 0xff), HEX);

    webServer.send(200, "text/html", makePage("Read Memory Test", "<p>" + hex_data));
  });
  
  webServer.on("/write", []() {
    int *address = (int *) strtol(urlDecode(webServer.arg("address")).c_str(), (char **) '\0', 16);
    int value = (int) strtol(urlDecode(webServer.arg("value")).c_str(), (char **) '\0', 16);
    *address=value;
    webServer.send(200, "text/html", makePage("Write Memory Test", "Writing memory..."));
  });
  */

  webServer.on("/setName", []() {
    const char *name=urlDecode(webServer.arg("name")).c_str();
    strcpy(projectName, name);
    webServer.send(200, "text/html", makePage("Test mode", ""));
  });
     
  webServer.on("/getInfo", []() {
    String s = functionInformation.functionPtr();
    webServer.send(200, "text/html", makePage("Test mode", s));
  });

  webServer.on("/test", []() {
    String s = Test();
    webServer.send(200, "text/html", makePage("Test mode", s));
  });

  webServer.begin();
}

void setupMode() {
  WiFi.mode(WIFI_MODE_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  delay(100);
  Serial.println("");
  M5.Lcd.println("");
  for (int i = 0; i < n; ++i) {
    ssidList += "<option value=\"";
    ssidList += WiFi.SSID(i);
    ssidList += "\">";
    ssidList += WiFi.SSID(i);
    ssidList += "</option>";
  }
  delay(100);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

  String apSSID="M5Stack";
  for(int i=0; i<10; i++)
  {
    byte randomValue = random(0, 37);
    char letter = randomValue + 'a';
    if(randomValue > 26)
      letter = (randomValue - 26) + '0';

    apSSID+=letter;
  }
        
  WiFi.softAP(apSSID.c_str());
  WiFi.mode(WIFI_MODE_AP);
  // WiFi.softAPConfig(IPAddress local_ip, IPAddress gateway, IPAddress subnet);
  // WiFi.softAP(const char* ssid, const char* passphrase = NULL, int channel = 1, int ssid_hidden = 0);
  // dnsServer.start(53, "*", apIP);
  startWebServer();
  Serial.print("Starting Access Point at \"");
  M5.Lcd.print("Starting Access Point at \"");
  Serial.print(apSSID.c_str());
  M5.Lcd.print(apSSID.c_str());
  Serial.println("\"");
  M5.Lcd.println("\"");
}

String makePage(String title, String contents) {
  String s = "<!DOCTYPE html><html><head>";
  s += "<meta name=\"viewport\" content=\"width=device-width,user-scalable=0\">";
  s += "<title>";
  s += title;
  s += "</title></head><body>";
  s += contents;
  s += "</body></html>";
  return s;
}

String urlDecode(String input) {
  String s = input;
  s.replace("%20", " ");
  s.replace("+", " ");
  s.replace("%21", "!");
  s.replace("%22", "\"");
  s.replace("%23", "#");
  s.replace("%24", "$");
  s.replace("%25", "%");
  s.replace("%26", "&");
  s.replace("%27", "\'");
  s.replace("%28", "(");
  s.replace("%29", ")");
  s.replace("%30", "*");
  s.replace("%31", "+");
  s.replace("%2C", ",");
  s.replace("%2E", ".");
  s.replace("%2F", "/");
  s.replace("%2C", ",");
  s.replace("%3A", ":");
  s.replace("%3A", ";");
  s.replace("%3C", "<");
  s.replace("%3D", "=");
  s.replace("%3E", ">");
  s.replace("%3F", "?");
  s.replace("%40", "@");
  s.replace("%5B", "[");
  s.replace("%5C", "\\");
  s.replace("%5D", "]");
  s.replace("%5E", "^");
  s.replace("%5F", "-");
  s.replace("%60", "`");
  return s;
}
