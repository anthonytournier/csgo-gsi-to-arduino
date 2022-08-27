void breathRed() {
  if (breathGreen_run) {
    breathGreen_jled.Stop();
    breathGreen_run = 0;
  }
  if (breathBlue_run) {
    breathBlue_jled.Stop();
    breathBlue_run = 0;
  }
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
}
