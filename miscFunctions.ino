/*
* Misc. Functions
* Smaller functions, doing simpler things. These contain the Setup scripts for specific modules,
* as well as utility functions for making the overall code cleaner.
*/

/*
* This function checks if one of the vote buttons have been pressed.
* If it has, it then adds a vote to the corresponding scoreboard, and waits for the button to be unpressed.
* It also displays a message on the screen to let the user know the vote went through.
* Parameters: Void
* Returns: Void
*/
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

/*
* This function runs when Alarm 1 is triggered by RTCLib.
* When it does, it either begins or finishes the contest,
* depending on wether it is active or not.
* Parameters: Void
* Returns: Void
*/
void onAlarm() {
  Serial.println("Alarm Triggered");
  if (!contestActive) {
    beginContest();
  } else {
    finishContest();
  }
}

/*
* This function calculates the length of the given String (in pixels),
* and returns the x-coordinate that the cursor should be positioned in order to make the text centered.
* Parameters: text - the text that should be centered (String)
* Returns: The desired x-coordinate (int)
*/
int centerText(const String text) {
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);  //calc width of new string
  return ((480 - w) / 2);
}

/*
* This function resets Alarm 1 if it has fired.
* The new alarm is set for a different time depending on wether or not a contest is active.
* Parameters: Void
* Returns: Void
*/
void checkAlarm() {
  if (rtc.alarmFired(1)) {
    rtc.clearAlarm(1);

    // Set different alarm lengths(times) depending on whether there's a contest active or not.
    if (contestActive) {
      rtc.setAlarm1(rtc.now() + TimeSpan(0, 0, 3, 0), DS3231_A1_Minute);  // 0 days, 0 hours, 3 minutes, 0 seconds
    } else {
      rtc.setAlarm1(rtc.now() + TimeSpan(0, 0, 1, 0), DS3231_A1_Minute);  // 0 days, 0 hours, 1 minutes, 0 seconds
    }
    Serial.println(F("Alarm Set"));
  }
}

/*
* This function sets up the RTC module for use with alarms.
* It is more or less stolen from the "alarm" example in RTClib.
* Parameters: Void
* Returns: Void
*/
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


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Below are functions that I tried to implement but was unsuccessful with. I've kept these around for further development.

/*
void SDSetup() {
  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("1. is a card inserted?");
    Serial.println("2. is your wiring correct?");
    Serial.println("3. did you change the chipSelect pin to match your shield or module?");
    Serial.println("Note: press reset button on the board and reopen this Serial Monitor after fixing your issue!");
    while (true);
  }

  Serial.println("initialization done.");
}

void SDWrite() {
  String dataString = "";

  // read three sensors and append to the string:
  for (int i = 0; i < winnerAmount; i++) {
    dataString += String(charVotes[i]);
    if (i < (winnerAmount - 1)) {
      dataString += ",";
    }
  }
  delay(500);

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}
*/