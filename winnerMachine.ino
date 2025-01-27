/*
* Name: "Vem Vinner?"-Maskinen
* Author: Melker Willforss
* Date: 2024-12-19
* Description:
* Randomly selects two contestants (from [winners]) and one contest (from [contests]) each day, and displays it on a screen.
* Then collects votes from buttons during the day, and selects a winner based on the votes at the end of the day.
*/

// Include Libraries
#include <RTClib.h>
#include <Wire.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_GFX.h>
#include <Adafruit_HX8357.h>

// Init constants
const int winnerAmount = 13
const int contestAmount = 10
// "åäö" = "<=>"
const String winners[winnerAmount]{ "Gargamel", "Bolibompadraken", "Dr Doofenshmirtz", "Musse Pigg", "Snurre Spr=tt", "Bamse", "Hulken", "Batman", "Donald J. Trump", "Donkey Kong", "Waluigi", "Mr. Bean", "Perry the Platypus" };
const String contests[contestAmount]{ "Schack", "Monopol", "Kubb", "Ett Maraton", "Kurrag>mma", "L=ngdhopp", "Paintball", "Stirrt=vling", "Korv=tning", "Boxning" };
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
  pinMode(vote1, INPUT_PULLUP);
  pinMode(vote2, INPUT_PULLUP);
  randomSeed(analogRead(A5));
  tft.begin();
  tft.fillScreen(HX8357_BLACK);
  tft.setRotation(3);
  rtcSetup();

  Serial.println(F("Setup Complete!"));
  tft.setCursor(0, 0);
  tft.print(F("Setup Complete!"));
}

void loop() {
  while (!contestActive) {
    checkAlarm();
  }  //Wait until a contest begins

  while (contestActive) {
    if (digitalRead(vote1) == LOW) {
      tempVotes[0] += 1;
      while (digitalRead(vote1) == LOW) {
        delay(0);
      }
      Serial.println(F("Vote registered!"));
    } else if (digitalRead(vote2) == LOW) {
      tempVotes[1] += 1;
      while (digitalRead(vote2) == LOW) {
        delay(0);
      }
      Serial.println(F("Vote registered!"));
    }
    checkAlarm();
  }
}

void finishContest() {
  tft.fillScreen(HX8357_BLACK);
  Serial.print("Final Votes: "); Serial.print(tempVotes[0]); Serial.print(", "); Serial.println(tempVotes[1]);
  int winner;
  if (tempVotes[0] > tempVotes[1]) {
    winner = 0;
  } else if (tempVotes[0] < tempVotes[1]) {
    winner = 1;
  } else {
    winner = 1;
  }

  int y = 15;

  tft.fillScreen(HX8357_BLACK);
  tft.setTextSize(5); tft.setTextColor(HX8357_MAGENTA);
  tft.setCursor(centerText(winners[current[winner]] + F(" Vinner!")), y); y += 85;
  tft.println(winners[current[winner]] + F(" Vinner!"));
  // tft.setTextSize(5); tft.setTextColor(HX8357_YELLOW);
  // tft.setCursor(centerText(winners[temp1]), y);  y += 45;
  // tft.print(winners[temp1]);
  // tft.setTextColor(HX8357_WHITE);
  // tft.setCursor(centerText(F("VS")), y);  y += 40;
  // tft.print(F("VS"));
  // tft.setTextColor(HX8357_GREEN);
  // tft.setCursor(centerText(winners[temp2]), y);  y += 75;
  // tft.print(winners[temp2]);
  // tft.setTextSize(6); tft.setTextColor(HX8357_WHITE);
  // tft.setCursor(centerText(contests[temp3]), y);
  // tft.print(contests[temp3]);

  for (int i = 0; i < 2; i++) { // Transfer votes from finished contest to total vote tally
    charVotes[current[i]] += tempVotes[i];
    tempVotes[i] = 0;
  }
  contestActive = false;
  Serial.println(F("Contest Finished"));
}

void newContest() {
  // Randomize contestants and contest
  int temp1 = random(0, winnnerAmount -1);
  int temp2 = random(0, winnerAmount -1);
  int temp3 = random(0, contestAmount -1);
  while (temp2 == temp1) {  // Make sure a contestant is not fighting themself
    temp1 = random(0, winnerAmount -1);
  }
  current[0] = temp1;
  current[1] = temp2;
  current[2] = temp3;

  int y = 15;

  tft.fillScreen(HX8357_BLACK);
  tft.setTextSize(7); tft.setTextColor(HX8357_MAGENTA);
  tft.setCursor(centerText(F("Vem Vinner?")), y); y += 85;
  tft.println(F("Vem Vinner?"));
  tft.setTextSize(5); tft.setTextColor(HX8357_YELLOW);
  tft.setCursor(centerText(winners[temp1]), y);  y += 45;
  tft.print(winners[temp1]);
  tft.setTextColor(HX8357_WHITE);
  tft.setCursor(centerText(F("VS")), y);  y += 40;
  tft.print(F("VS"));
  tft.setTextColor(HX8357_GREEN);
  tft.setCursor(centerText(winners[temp2]), y);  y += 75;
  tft.print(winners[temp2]);
  tft.setTextSize(6); tft.setTextColor(HX8357_WHITE);
  tft.setCursor(centerText(contests[temp3]), y);
  tft.print(contests[temp3]);

  contestActive = true;
}

void onAlarm() {
  Serial.println("Alarm!");
  if (!contestActive) {
    newContest();
  } else {
    finishContest();
  }
}

int centerText(const String text) {
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);  //calc width of new string
  Serial.println((480 - w) / 2);
  return ((480 - w) / 2);
}

void checkAlarm() {
  if (rtc.alarmFired(1)) {
    rtc.clearAlarm(1);

    if (!rtc.setAlarm1(rtc.now() + TimeSpan(60), DS3231_A1_Second)) {
      Serial.println("Error, alarm wasn't set!");
    } else {
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

  checkAlarm();
}