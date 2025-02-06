/*
* Contest Functions
* The big functions that contain the bulk of the program code.
* These mostly consist of displaying the information on the TFT display.
*/

/*
* This function begins a new contest by first randomizing contestants and the contest itself.
* It then displays it on the screen.
* Parameters: Void
* Returns: Void
*/
void beginContest() {
  // Randomize contestants and contest
  int temp1 = random(0, winnerAmount -1);
  int temp2 = random(0, winnerAmount -1);
  int temp3 = random(0, contestAmount -1);
  while (temp2 == temp1) {  // Make sure a contestant is not fighting themself
    temp1 = random(0, winnerAmount -1);
  }
  current[0] = temp1;
  current[1] = temp2;
  current[2] = temp3;

  displayContest();

  contestActive = true;
  Serial.println(F("Contest Begun"));
}

/*
* This function determines who won the contest by comparing the votes.
* It then displays the winner along with the total vote tally on the screen.
* It also logs the votes to the [charVotes] array, and resets [tempVotes].
* It also attempts to write data to the SD-card (DEACTIVATED).
* Parameters: Void
* Returns: Void
*/
void finishContest() {
  String winner;
  // Determine who won and set text color and string accordingly
  if (tempVotes[0] > tempVotes[1]) {
    winner = winners[current[0]];
    tft.setTextColor(HX8357_YELLOW);
  } else if (tempVotes[0] < tempVotes[1]) {
    winner = winners[current[1]];
    tft.setTextColor(HX8357_GREEN);
  } else {
    winner = "Ingen";
    tft.setTextColor(HX8357_MAGENTA);
  }

  // Display it all
  int y = 5;
  String tempString;
  tft.fillScreen(HX8357_BLACK);
  tft.setTextSize(6); 
  tft.setCursor(centerText(winner), y); 
  tft.println(winner); y += 55;
  tft.setTextColor(HX8357_MAGENTA);
  tft.setCursor(centerText(F(" Vinner!")), y);
  tft.println(F(" Vinner!")); y += 70;
  tft.setTextSize(4); tft.setTextColor(HX8357_YELLOW);
  tempString = (winners[current[0]] + ": " + String(tempVotes[0]));
  tft.setCursor(centerText(tempString), y);  y += 65;
  tft.print(tempString);
  tft.setTextColor(HX8357_GREEN);
  tempString = (winners[current[1]] + ": " + String(tempVotes[1]));
  tft.setCursor(centerText(tempString), y);  y += 55;
  tft.print(tempString);
  tft.setTextSize(6); tft.setTextColor(HX8357_WHITE);
  tft.setCursor(centerText(contests[current[2]]), y);
  tft.print(contests[current[2]]);

  Serial.print("Final Votes: "); Serial.print(tempVotes[0]); Serial.print(", "); Serial.println(tempVotes[1]);

  // Transfer votes from finished contest to total vote tally
  for (int i = 0; i < 2; i++) {
    charVotes[current[i]] += tempVotes[i];
    tempVotes[i] = 0;
  }
  // Log total vote tally to microSD
  // SDWrite();

  contestActive = false;
  Serial.println(F("Contest Finished"));
}

/*
* This function displays the current contest on the screen, as defined by [current].
* Parameters: Void
* Returns: Void
*/
void displayContest() {
  int y = 15;
  tft.fillScreen(HX8357_BLACK);
  tft.setTextSize(7); tft.setTextColor(HX8357_MAGENTA);
  tft.setCursor(centerText(F("Vem Vinner?")), y);   y += 75;
  tft.println(F("Vem Vinner?"));
  tft.setTextSize(5); tft.setTextColor(HX8357_YELLOW);
  tft.setCursor(centerText(winners[current[0]]), y);   y += 55;
  tft.print(winners[current[0]]);
  tft.setTextColor(HX8357_WHITE);
  tft.setCursor(centerText(F("VS")), y);   y += 45;
  tft.print(F("VS"));
  tft.setTextColor(HX8357_GREEN);
  tft.setCursor(centerText(winners[current[1]]), y);   y += 65;
  tft.print(winners[current[1]]);
  tft.setTextSize(6); tft.setTextColor(HX8357_WHITE);
  tft.setCursor(centerText(contests[current[2]]), y);
  tft.print(contests[current[2]]);
}