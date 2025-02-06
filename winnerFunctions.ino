void voteCheck() {
  if (digitalRead(vote1) == LOW) {
    tempVotes[0] += 1;
    while (digitalRead(vote1) == LOW) {
      delay(1);
    }

    tft.fillScreen(HX8357_BLACK);
    tft.setTextSize(5);
    tft.setTextColor(HX8357_YELLOW);
    tft.setCursor(centerText(F("Vote registered!")), 100);
    tft.println(F("Vote registered!"));
    Serial.println(tempVotes[0]);
    delay(500);
    displayContest();
  } 
  else if (digitalRead(vote2) == LOW) {
    tempVotes[1] += 1;
    while (digitalRead(vote2) == LOW) {
      delay(1);
    }

    tft.fillScreen(HX8357_BLACK);
    tft.setTextSize(5);
    tft.setTextColor(HX8357_GREEN);
    tft.setCursor(centerText(F("Vote registered!")), 100);
    tft.println(F("Vote registered!"));
    Serial.println(tempVotes[1]);
    delay(500);
    displayContest();
  }
}

void onAlarm() {
  Serial.println("Alarm Triggered");
  if (!contestActive) {
    beginContest();
  } else {
    finishContest();
  }
}

int centerText(const String text) {
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);  //calc width of new string
  return ((480 - w) / 2);
}

void checkAlarm() {
  if (rtc.alarmFired(1)) {
    rtc.clearAlarm(1);
    delay(10);

    // Set different alarm lengths(times) depending on whether there's a contest active or not.
    if (contestActive) {
      rtc.setAlarm1(rtc.now() + TimeSpan(0, 0, 3, 0), DS3231_A1_Minute);  // 0 days, 0 hours, 3 minutes, 0 seconds
    } else {
      rtc.setAlarm1(rtc.now() + TimeSpan(0, 0, 1, 0), DS3231_A1_Minute);  // 0 days, 0 hours, 1 minutes, 0 seconds
    }
    Serial.println(F("Alarm Set"));
  }
}

// More or less stolen from alarm example in RTClib
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
  rtc.setAlarm1(rtc.now() + TimeSpan(0, 0, 1, 0), DS3231_A1_Minute);
}

// void SDSetup() {
//   Serial.print("Initializing SD card...");

//   if (!SD.begin(chipSelect)) {
//     Serial.println("initialization failed. Things to check:");
//     Serial.println("1. is a card inserted?");
//     Serial.println("2. is your wiring correct?");
//     Serial.println("3. did you change the chipSelect pin to match your shield or module?");
//     Serial.println("Note: press reset button on the board and reopen this Serial Monitor after fixing your issue!");
//     while (true);
//   }

//   Serial.println("initialization done.");
// }

// void SDWrite() {
//   String dataString = "";

//   // read three sensors and append to the string:
//   for (int i = 0; i < winnerAmount; i++) {
//     dataString += String(charVotes[i]);
//     if (i < (winnerAmount - 1)) {
//       dataString += ",";
//     }
//   }
//   delay(500);

//   // open the file. note that only one file can be open at a time,
//   // so you have to close this one before opening another.
//   File dataFile = SD.open("datalog.txt", FILE_WRITE);

//   // if the file is available, write to it:
//   if (dataFile) {
//     dataFile.println(dataString);
//     dataFile.close();
//     // print to the serial port too:
//     Serial.println(dataString);
//   }
//   // if the file isn't open, pop up an error:
//   else {
//     Serial.println("error opening datalog.txt");
//   }
// }