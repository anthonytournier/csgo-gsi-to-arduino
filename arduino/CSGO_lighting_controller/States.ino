void unknownCmd() {
  defaultColor();
  cmdMessenger.feedinSerialData();
}

void onMenu() {
  winCondition = false;
  terrorWin = false;
  if (roundlive == 0) {
    freezetime_sequence.Stop();
    freezetime_sequence_run = 0;
    setColor(menuColor);
    cmdMessenger.feedinSerialData();
  }
}

void flashed() {
  if (!bombPlanted && !bombIsDefused && !bombIsExploded && !winCondition) {
    flashedState = cmdMessenger.readInt16Arg();
    flashedColor[0] = flashedState;
    flashedColor[1] = flashedState;
    flashedColor[2] = flashedState;
    if (alive_sequence_run) {
      alive_sequence.Stop();
      alive_sequence_run = 0;
    }
    setFadeColor(flashedColor, 10);
    if (flashedState > 10) {
      isFlashed = true;
    }
  }
  if (flashedState < 10) {
    setColor(offColor);
    isFlashed = false;
    if (alive_breath == 0) {
      setColor(roundliveColor);
    }
    else {
      if (!alive_sequence_run) {
        alive_sequence = JLed(GREENPIN).Breathe(3000).Forever().MaxBrightness(health_fade);
        alive_sequence_run = 1;
      }
    }
  }
  cmdMessenger.feedinSerialData();
}

void burning() {
  if (!isFlashed && !bombPlanted && !bombIsDefused && !bombIsExploded && !winCondition) {
    setFadeColor(burningColor, 10);
    burnint = cmdMessenger.readInt16Arg();
  }
  if (burnint == 0) {
    analogWrite(REDPIN, 0);
    analogWrite(BLUEPIN, 0);
    analogWrite(GREENPIN, health_fade);
  }
  cmdMessenger.feedinSerialData();
}

void bomb() {
  bombPlanted = true;
  bombIsExploded = false;
  bombIsDefused = false;
  alive_sequence.Stop();
  alive_sequence_run = 1;
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
  alive_sequence.Stop();
  alive_sequence_run = 0;
  winCondition = true;
  terrorWin = true;
  roundlive = 0;
}

void ctWin() {
  alive_sequence.Stop();
  alive_sequence_run = 0;
  winCondition = true;
  terrorWin = false;
  roundlive = 0;
}

void freezetime() {
  if (freezetime_breath) {
    setColor(offColor);
    freezetime_sequence.Reset();
    freezetime_sequence_run = 1;
  }
  else {
    setColor(freezetimeColor);
  }
  resetSpecialState();
  bombBeeps = 0;
  state = 5;
  roundlive = 0;
}

void live() {
  if (bombPlanted == 0) {
    freezetime_sequence.Stop();
    freezetime_sequence_run = 0;
    if (!isFlashed && alive_breath == 0) {
      setColor(roundliveColor);
    }
    else {
      alive_sequence.Reset();
      alive_sequence_run = 1;
    }
    resetSpecialState();
  }
  roundlive = 1;
}

void healthupdate() {
  if (!isFlashed && roundlive && !bombIsDefused && !bombIsExploded) {
    health = cmdMessenger.readInt16Arg();
    //map(value, fromLow, fromHigh, toLow, toHigh)
    health_fade = map(health, 1, 100, 20, 255);
    analogWrite(REDPIN, 0);
    if (alive_breath == 0) {
      analogWrite(GREENPIN, health_fade);
    }
    else {
      alive_sequence = JLed(GREENPIN).Breathe(3000).Forever().MaxBrightness(health_fade);
      alive_sequence_run = 1;
    }
    if (health == 0) {
      setColor(bombColor);
    }
    cmdMessenger.feedinSerialData();
    //setColorhealth(roundliveColor);
  }
}

void serverstopped() {
  setColor(offColor);
  resetSpecialState();
}
