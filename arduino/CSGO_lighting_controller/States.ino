void unknownCmd() {
  defaultColor();
  cmdMessenger.feedinSerialData();
}

void onMenu() {
  if (roundlive == 0) {
    setColor(menuColor);
    resetSpecialState();
    cmdMessenger.feedinSerialData();
  }
}

void flashed() {
  if (!bombPlanted && !bombIsDefused && !bombIsExploded && !winCondition) {
    flashedState = cmdMessenger.readInt16Arg();
    flashedColor[0] = flashedState;
    flashedColor[1] = flashedState;
    flashedColor[2] = flashedState;
    setFadeColor(flashedColor, 10);
    isFlashed = true;
  }
  if (flashedState < 10) {
    setColor(roundliveColor);
    isFlashed = false;
  }
  cmdMessenger.feedinSerialData();
}

void burning() {
  if (!isFlashed && !bombPlanted && !bombIsDefused && !bombIsExploded && !winCondition) {
    setFadeColor(burningColor, 10);
    burnint = cmdMessenger.readInt16Arg();
  }
  if (burnint == 0) {
    setColor(roundliveColor);
  }
  cmdMessenger.feedinSerialData();
}

void bomb() {
  bombPlanted = true;
  bombIsExploded = false;
  bombIsDefused = false;
  if (state != bombPlanted) { //if we're already active don't set the state again
    state = bombPlanted;
    bombBeeps = 0; //we have had 0 beeps so far
    bombLastBeep = millis(); //set the time of activating the bomb
    delay(50);             //this delay is used to match up the timing with the first beep.
    blinkRed();   //blink the first time
    //tDown.start();
  }
}

void bombExploded() {
  setRandomColor();
  bombPlanted = false;
  bombIsExploded = false;
  bombIsExploded = true;
  roundlive = 0;
  //tDown.stop();
  cmdMessenger.feedinSerialData();
}

void bombDefused() {
  setColor(defusedColor);
  bombPlanted = false;
  bombIsExploded = false;
  bombIsDefused = true;
  roundlive = 0;
  //tDown.stop();
  cmdMessenger.feedinSerialData();
}

void tWin() {
  winCondition = true;
  terrorWin = true;
  roundlive = 0;
}

void ctWin() {
  winCondition = true;
  terrorWin = false;
  roundlive = 0;
}

void freezetime() {
  setColor(freezetimeColor);
  resetSpecialState();
  bombBeeps = 0;
  state = 5;
  roundlive = 0;
}

void live() {
  if (bombPlanted == 0) {
    setColor(roundliveColor);
    resetSpecialState();
  }
  roundlive = 1;
}

void healthupdate() {
  health = cmdMessenger.readInt16Arg();
  //map(value, fromLow, fromHigh, toLow, toHigh)
  health = map(health,1,100,20,255);
  analogWrite(GREENPIN, health);
  //setColorhealth(roundliveColor);
}

void serverstopped() {
  setColor(offColor);
  resetSpecialState();
}