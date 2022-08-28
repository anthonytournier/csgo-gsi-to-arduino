void breathRed() {
  if (breathGreen_run) {
    breathGreen_jled.Stop();
    breathGreen_run = 0;
  }
  if (breathBlue_run) {
    breathBlue_jled.Stop();
    breathBlue_run = 0;
  }
  multifade_seq = 0;
  breathRed_jled.Reset();
  breathRed_run = 1;
}

void breathGreen() {
  if (breathRed_run) {
    breathRed_jled.Stop();
    breathRed_run = 0;
  }
  if (breathBlue_run) {
    breathBlue_jled.Stop();
    breathBlue_run = 0;
  }
  multifade_seq = 0;
  breathGreen_jled.Reset();
  breathGreen_run = 1;
}

void breathBlue() {
  if (breathGreen_run) {
    breathGreen_jled.Stop();
    breathGreen_run = 0;
  }
  if (breathRed_run) {
    breathRed_jled.Stop();
    breathRed_run = 0;
  }
  multifade_seq = 0;
  breathBlue_jled.Reset();
  breathBlue_run = 1;
}

void breathOff() {
  breathRed_jled.Stop();
  breathRed_run = 0;
  breathGreen_jled.Stop();
  breathGreen_run = 0;
  breathBlue_jled.Stop();
  breathBlue_run = 0;
  multifade_seq = 0;
}

void multifade() {
  breathRed_jled.Stop();
  breathRed_run = 0;
  breathGreen_jled.Stop();
  breathGreen_run = 0;
  breathBlue_jled.Stop();
  breathBlue_run = 0;
  multifade_seq = 1;
}

void run_multifade() {
  int r, g, b;

  // fade from blue to violet
  for (r = 0; r < 256; r++) {
    analogWrite(REDPIN, r);
    delay(fadeSpeedMulti);
  }
  // fade from violet to red
  for (b = 255; b > 0; b--) {
    analogWrite(BLUEPIN, b);
    delay(fadeSpeedMulti);
  }
  // fade from red to yellow
  for (g = 0; g < 256; g++) {
    analogWrite(GREENPIN, g);
    delay(fadeSpeedMulti);
  }
  // fade from yellow to green
  for (r = 255; r > 0; r--) {
    analogWrite(REDPIN, r);
    delay(fadeSpeedMulti);
  }
  // fade from green to teal
  for (b = 0; b < 256; b++) {
    analogWrite(BLUEPIN, b);
    delay(fadeSpeedMulti);
  }
  // fade from teal to blue
  for (g = 255; g > 0; g--) {
    analogWrite(GREENPIN, g);
    delay(fadeSpeedMulti);
  }
}
