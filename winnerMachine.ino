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

// Init constants
const String winners[10]{ "Trazan o Banarne", "Bolibompadraken", "Dr Snuggles", "Johan Glans", "Robin Almström", "Bamse", "Fantomen", "Batman", "Donald J. Trump", "Mario och Luigi" };

// Init global variables

// construct objects
RTC_DS3231 rtc;
#define CLOCK_INTERRUPT_PIN 2

void setup() {
  // init communication
  Serial.begin(9600);

  // Init Hardware
  rtcSetup();
  randomSeed(analogRead(A5));
}

void loop() {
  checkAlarm();
  delay(1000);
}

void onAlarm() {
  Serial.println(winners[random(0, 9)]);
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