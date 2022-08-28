#include <ArduinoJson.h>
#include <CmdMessenger.h>
#include <Countimer.h>
#include <jled.h>

#define REDPIN 5
#define GREENPIN 6
#define BLUEPIN 3
#define BUFFER_SIZE 500
#define BAUD_RATE 115200

bool freezetime_breath = 1;

Countimer tDown;
auto freezetime_sequence = JLed(BLUEPIN).Breathe(2000).Forever();
bool freezetime_sequence_run = 0;

auto breathRed_jled = JLed(REDPIN).Breathe(3000).Forever();
bool breathRed_run = 0;
auto breathGreen_jled = JLed(GREENPIN).Breathe(3000).Forever();
bool breathGreen_run = 0;
auto breathBlue_jled = JLed(BLUEPIN).Breathe(3000).Forever();
bool breathBlue_run = 0;

bool bombPlanted = false;
bool bombLight = false;

bool bombIsExploded = false;
bool bombIsDefused = false;

bool winCondition = false;
bool terrorWin = false;
bool winLight = true;

bool isFlashed = false;

bool fadeInProgress = false;
int fadeSpeed = 10;
int fadeSpeedMulti = 5;

int terroColor[3] = {255, 255, 0};    // Yellow
int counterColor[3] = {0, 0, 255};    // Blue
int bombColor[3] = {255, 0, 0};       // Red
int bombColorOff[3] = {0, 0, 0};
int explodedColor[3] = {255, 0, 0};   // Red
int defusedColor[3] = {255, 135, 0};  // Orange
int offColor[3] = {0, 0, 0};
int menuColor[3] = {255, 180, 255};     // Voilet
int flashedColor[3] = {0, 0, 0};
int burningColor[3] = {255, 180, 0};
int randomColor[3] = {0, 0, 0};
int roundoverColor[3] = {255, 255, 0};
int roundliveColor[3] = {0, 255, 0};
int freezetimeColor[3] = {0, 0, 255};   // Blue
int currentColor[3] = {0, 0, 0};
int fadeToColor[3] = {0, 0, 0};

int bombBeeps = 0;  //how many beeps have the bomb emitted
int nextBeep = 0; //the interval until next beep should occur
unsigned long bombLastBeep = 0; //when was the last beep of the bomb?
int state = 5;
int roundlive = 0;
int health = 100;
int health_fade;
bool multifade_seq = 0;

CmdMessenger cmdMessenger = CmdMessenger(Serial);

long explodedChangeInterval = 50;
long lastExplodedChange = 0;

long winChangeInterval = 250;
long lastWinChange = 0;

long fixedUpdateInterval = 35;
long lastUpdate = 0;

long currentTime = 0;
int bombtimeleft = 0;

float winIntensity = 1.0f;
int flashedState = 0;
int burnint = 255;

void attachCommandCallbacks()
{
  cmdMessenger.printLfCr();
  cmdMessenger.attach(1, onMenu);
  cmdMessenger.attach(2, flashed);
  cmdMessenger.attach(3, bomb);
  cmdMessenger.attach(4, bombExploded);
  cmdMessenger.attach(5, bombDefused);
  cmdMessenger.attach(6, defaultColor);
  cmdMessenger.attach(7, tWin);
  cmdMessenger.attach(8, ctWin);
  cmdMessenger.attach(9, burning);
  cmdMessenger.attach(10, freezetime);
  cmdMessenger.attach(11, live);
  cmdMessenger.attach(12, serverstopped);
  cmdMessenger.attach(13, healthupdate);

  cmdMessenger.attach(20, breathRed);
  cmdMessenger.attach(21, breathGreen);
  cmdMessenger.attach(22, breathBlue);
  cmdMessenger.attach(23, breathOff);
  cmdMessenger.attach(24, multifade);

  cmdMessenger.attach(unknownCmd);
}

void initSerial()
{
  Serial.begin(BAUD_RATE);
  attachCommandCallbacks();
}

void setup()
{
  //tDown.setCounter(0, 0, 40, tDown.COUNT_DOWN, tDownComplete);
  //tDown.setInterval(print_time2, 1000);
  randomSeed(analogRead(0));
  pinMode(REDPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);

  digitalWrite(REDPIN, HIGH);
  delay(1000);
  digitalWrite(REDPIN, LOW);
  digitalWrite(BLUEPIN, HIGH);
  delay(1000);
  digitalWrite(BLUEPIN, LOW);
  digitalWrite(GREENPIN, HIGH);
  delay(1000);

  setColor(offColor);
  initSerial();
}

void loop()
{
  //tDown.run();
  currentTime = millis();

  if (freezetime_sequence_run) {
    freezetime_sequence.Update();
  }

  if (breathRed_run) {
    breathRed_jled.Update();
  }

  if (breathGreen_run) {
    breathGreen_jled.Update();
  }

  if (breathBlue_run) {
    breathBlue_jled.Update();
  }

  if (multifade_seq == 1){
    run_multifade();
  }

  // Handle bomb blinking
  bombtimeleft = tDown.getCurrentSeconds();
  if (!winCondition && state == bombPlanted) {
    // calculate the next beep time
    nextBeep = (0.13 * sq(bombBeeps) - 20 * bombBeeps + 990); //if the bomb beep sequence changes we need to update this polynomial...
    if ((millis() - bombLastBeep) > nextBeep) {
      bombLastBeep = millis();
      blinkRed();
      bombBeeps++;
    }
    if (bombBeeps > 80) //after 80 beeps the bomb is exploding no matter what so we just light up the red LEDs
    {
      setColor(bombColor);
    }
  }

  // Handle bomb exploded
  if (bombIsExploded && currentTime - lastExplodedChange >= explodedChangeInterval) {
    setRandomColor();
    lastExplodedChange = currentTime;
  }

  // Handle wins
  if (winCondition && !bombIsExploded && !bombIsDefused && currentTime - lastWinChange >= winChangeInterval) {
    if (terrorWin) {
      // T won
      setFadeColorWithIntensity(terroColor, winIntensity, 20);
    }
    else {
      // CT won
      setFadeColorWithIntensity(counterColor, winIntensity, 20);
    }
    if (winLight) {
      winIntensity = 0.0f;
    }
    else {
      winIntensity = 1.0f;
    }
    winLight = !winLight;
    lastWinChange = currentTime;
  }

  // Handle fixed update
  if (currentTime - lastUpdate >= fixedUpdateInterval) {
    // Handle regular state updates
    // Handle color fades
    if (fadeInProgress) {
      fadeToColorStep(fadeToColor);
    }
    lastUpdate = currentTime;
  }

  // Read in new commands
  cmdMessenger.feedinSerialData();
}


void setCurrentColor(int color[3]) {
  currentColor[0] = color[0];
  currentColor[1] = color[1];
  currentColor[2] = color[2];
}

void setRandomColor() {
  randomColor[0] = random(256);
  randomColor[1] = random(256);
  randomColor[2] = random(256);
  fadeInProgress = false;
  setColor(randomColor);
}

bool isArrayEquals(int first[], int second[]) {
  if (first[0] == second[0] && first[1] == second[1] && first[2] == second[2]) {
    return true;
  }
  return false;
}

void defaultColor() {
  setColor(offColor);
  resetSpecialState();
  cmdMessenger.feedinSerialData();
}

void resetSpecialState() {
  bombPlanted = false;
  bombIsExploded = false;
  bombIsDefused = false;
  winCondition = false;
  isFlashed = false;
}
