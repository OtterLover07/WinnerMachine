/*
* Name: "Vem Vinner?"-Maskinen
* Author: Melker Willforss
* Date: 2024-12-19
* Description: 
*/

// Include Libraries
#include <RTClib.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <Adafruit_HX8357.h>
// #include <U8g2_for_Adafruit_GFX.h>
// #include <u8g2_fonts.h>

// Init constants
// "<>=" = "åäö"
const String winners[13]{"Gargamel", "Bolibompadraken", "Dr Doofenshmirtz", "Musse Pigg", "Snurre Sprätt", "Bamse", "Hulken", "Batman", "Donald J. Trump", "Donkey Kong", "Waluigi", "Mr. Bean", "Perry the Platypus"};
const String contests[10]{"Schack", "Monopol", "Kubb", "Ett Maraton", "Kurrag<>=mma", "Längdhopp", "Paintball", "Stirrtävling", "Korvätning", "Boxning"};

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

Adafruit_HX8357 display = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);
// U8G2_FOR_ADAFRUIT_GFX u8g;


void setup() {
  // init communication
  Serial.begin(9600);

  // Init Hardware
  randomSeed(analogRead(A5));
  rtcSetup();
  lcdSetup();
}

void loop() {
  checkAlarm();
  delay(10000);
}

void onAlarm() {
  display.println(winners[random(0, 12)]);
}

void setAlarm() {
  if (rtc.alarmFired(1)) {
    rtc.clearAlarm(1);

    if (!rtc.setAlarm1(rtc.now() + TimeSpan(10), DS3231_A1_Second)) {
      Serial.println("Error, alarm wasn't set!");
    } else {
      Serial.println("");
    }
  }
}

void checkAlarm() {
  if (rtc.alarmFired(1)) {
    rtc.clearAlarm(1);

    if (!rtc.setAlarm1(rtc.now() + TimeSpan(2), DS3231_A1_Second)) {
      Serial.println("Error, alarm wasn't set!");
    } else {
      setAlarm();
    }
  }
}

void lcdSetup() {
  display.begin();
  display.setRotation(1);
  display.fillScreen(HX8357_BLACK);
  display.setCursor(0, 50);
  display.setTextColor(HX8357_WHITE); display.setTextSize(3);
  display.println(contests[4]);

  // display.begin();
  // display.fillScreen(0x0000);
  // u8g.begin(display);
  // u8g.setFontMode(1);
  // u8g.setFontDirection(1);
  // u8g.setFont(u8g2_font_helvR14_tf);
  // u8g.setForegroundColor(0xFFFF);
  // u8g.setCursor(0, 0);
  // u8g.print(contests[4]);
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

  setAlarm();
  Serial.println("Setup Complete!");
}