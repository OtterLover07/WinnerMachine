/*
* Name: "Vem Vinner?"-Maskinen
* Author: Melker Willforss
* Date: 2024-12-19
* Description: 
*/

// Include Libraries
#include <RTClib.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_HX8357.h>
// #include <U8g2_for_Adafruit_GFX.h> //U8g2-addon
// #include <u8g2_fonts.h> //U8g2-addon

// Init constants
// "åäö" = "<=>"
const String winners[] {"Gargamel", "Bolibompadraken", "Dr Doofenshmirtz", "Musse Pigg", "Snurre Spr=tt", "Bamse", "Hulken", "Batman", "Donald J. Trump", "Donkey Kong", "Waluigi", "Mr. Bean", "Perry the Platypus"};
const String contests[] {"Schack", "Monopol", "Kubb", "Ett Maraton", "Kurrag>mma", "Längdhopp", "Paintball", "Stirrt=vling", "Korv=tning", "Boxning"};

// Init global variables
int charVotes[13]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int tempVotes[2]{0, 0};
int contest[3]{0, 0, 0};

// construct objects
RTC_DS3231 rtc;
#define CLOCK_INTERRUPT_PIN 2

#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8

Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);
// U8G2_FOR_ADAFRUIT_GFX u8g; //U8g2-addon


void setup() {
  // init communication
  Serial.begin(9600);

  // Init Hardware
  randomSeed(analogRead(A5));
  rtcSetup();
  lcdSetup();

  Serial.println("Setup Complete!");
}

void loop() {
  checkAlarm();
}

void newContest(){
  blabla
}

void onAlarm() {
  newContest();
  Serial.println("Alarm!");
}

void checkAlarm() {
  if (rtc.alarmFired(1)) {
    rtc.clearAlarm(1);

    if (!rtc.setAlarm1(rtc.now() + TimeSpan(10), DS3231_A1_Second)) {
      Serial.println("Error, alarm wasn't set!");
    } else {
      Serial.println("");
    }
  }
}

void lcdSetup() {
  tft.begin();
  tft.fillScreen(HX8357_BLACK);
  tft.setRotation(1);
}

void rtcSetup() {
  Serial.println("Initiate RTC Setup...");

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC!");
    Serial.flush();
    while (1) delay(10);
  }
  if (rtc.lostPower()) {
    // this will adjust to the date and time at compilation
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  rtc.disable32K();
  pinMode(CLOCK_INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(CLOCK_INTERRUPT_PIN), onAlarm, FALLING);
  rtc.clearAlarm(1);
  rtc.clearAlarm(2);
  rtc.writeSqwPinMode(DS3231_OFF);
  rtc.disableAlarm(2);

  checkAlarm();
}