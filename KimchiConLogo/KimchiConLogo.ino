// Icon images are stored in tabs ^ e.g. Alert.h etc above this line
// more than one icon can be in a header file

// Arrays containing FLASH images can be created with UTFT library tool:
// (libraries\UTFT\Tools\ImageConverter565.exe)
// Convert to .c format then copy into a new tab

/*
 This sketch demonstrates loading images from arrays stored in program (FLASH) memory.

 Works with TFT_eSPI library here:
 https://github.com/Bodmer/TFT_eSPI

 This sketch does not use/need any fonts at all...

 Code derived from ILI9341_due library example

 Make sure all the display driver and pin comnenctions are correct by
 editting the User_Setup.h file in the TFT_eSPI library folder.

 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################
*/

#include <M5Stack.h>

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
}

void loop()
{
  if(M5.BtnA.wasPressed()) {
    M5.Lcd.printf("A wasPressed \r\n");
    M5.Speaker.beep(); //beep
  }

  if(M5.BtnB.wasPressed())
  {
    M5.Lcd.printf("B wasPressed \r\n");
    M5.Speaker.tone(NOTE_DH2, 200); //frequency 3000, with a duration of 200ms
  }

  if(M5.BtnC.wasPressed())
  {
    M5.Lcd.printf("B wasPressed \r\n");
    M5.Speaker.tone(NOTE_DL3, 200); //frequency 3000, with a duration of 200ms
  }
/*
  delay(2500);
  // call this fun, power btn will Invalid
  // Restore power btn please call M5.Power.setPowerBoostKeepOn(false)
  M5.Power.deepSleep(0);  
*/
}
