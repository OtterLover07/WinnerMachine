/*
* Name: "Vem Vinner?"-Maskinen
* Author: Melker Willforss
* Date: 2024-12-19
* Description:
* Randomly selects two contestants (from [winners]) and one contest (from [contests]) each day, and displays it on a screen.
* Then collects votes from buttons during the day, and selects a winner based on the votes at the end of the day.
*/

// SD stuff didn't work, keeping in case I want to try to fix it later.

// Include Libraries
#include <RTClib.h>
#include <Wire.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_GFX.h>
#include <Adafruit_HX8357.h>
// #include <SPI.h>
// #include <SD.h>

// Init constants
const int winnerAmount = 14;
const int contestAmount = 11;
// const int chipSelect = 4;

// Init master arrays
// "åäö" = "<=>"
const String winners[winnerAmount]{ "Gargamel", "Bolibompadraken", "Dr Doofenshmirtz", "Musse Pigg", "Snurre Spr=tt", "Bamse", "Hulken", "Batman", "Donald J. Trump", "Donkey Kong", "Waluigi", "Mr. Bean", "Perry the Platypus", "Stalin" };
const String contests[contestAmount]{ "Schack", "Monopol", "Kubb", "Ett Maraton", "Kurrag>mma", "L=ngdhopp", "Paintball", "Stirrt=vling", "Korv=tning", "Boxning", "Presidentval" };
const int vote1 = 6, vote2 = 7;

// Init global variables
int charVotes[winnerAmount]{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int tempVotes[2]{ 0, 0 };
int current[3]{ 0, 0, 0 };
bool contestActive = false;

// construct objects
RTC_DS3231 rtc;
#define CLOCK_INTERRUPT_PIN 2

#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8

Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  // init communication
  Serial.begin(9600);

  // Init Hardware
  // SDSetup();
  pinMode(vote1, INPUT_PULLUP);
  pinMode(vote2, INPUT_PULLUP);
  randomSeed(analogRead(A0));
  tft.begin();
  tft.fillScreen(HX8357_BLACK);
  tft.setRotation(3);
  rtcSetup();

  Serial.println(F("Setup Complete!"));
  tft.setCursor(0, 0);
  tft.println(F("Setup Complete!"));

  // Funny loading bar
  delay(1000);
  tft.println("");
  tft.print(F("Loading"));
  while (!contestActive) {
    tft.print(".");
    delay(1000);
  }
}

void loop() {
  while (!contestActive) {
    checkAlarm();
  }  //Wait until a contest begins

  while (contestActive) {
    voteCheck();
    checkAlarm();
  }
}