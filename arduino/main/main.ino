#include <ArduinoJson.h>
#include <CmdMessenger.h>

#define REDPIN 5
#define GREENPIN 6
#define BLUEPIN 3
#define BUFFER_SIZE 500
#define BAUD_RATE 9600

#define BOMB_START_BLINK_INTV 500
#define BOMB_MIN_BLINK_INTV 50
#define BOMB_BLINK_INTV_DEC 1

bool bombPlanted = false;
bool bombLight = false;

bool bombIsExploded = false;
bool bombIsDefused = false;

bool winCondition = false;
bool terrorWin = false;
bool winLight = true;

int terroColor[3] = {255, 100, 0};
int counterColor[3] = {20, 36, 214};
int bombColor[3] = {255, 0, 0};
int bombColorOff[3] = {0, 0, 0};
int explodedColor[3] = {255, 0, 0};
int defusedColor[3] = {0, 255, 55};
int offColor[3] = {255, 255, 255};
int menuColor[3] = {0, 255, 0};
int currentColor[3] = {0, 0, 0};
int flashedColor[3] = {0, 0, 0};
int randomColor[3] = {0, 0, 0};
CmdMessenger cmdMessenger = CmdMessenger(Serial);

long bombBlinkInterval = BOMB_START_BLINK_INTV;
long lastBlinkTime = 0;

long explodedChangeInterval = 50;
long lastExplodedChange = 0;

long winChangeInterval = 250;
long lastWinChange = 0;

long fixedUpdateInterval = 100;
long lastUpdate = 0;

long currentTime = 0;

float winIntensity = 1.0f;

void attachCommandCallbacks()
{
	cmdMessenger.printLfCr();
	cmdMessenger.attach(1, onMenu);
	cmdMessenger.attach(2, teamT);
	cmdMessenger.attach(3, teamCT);
	cmdMessenger.attach(4, flashed);
	cmdMessenger.attach(5, bomb);
	cmdMessenger.attach(6, bombExploded);
	cmdMessenger.attach(7, bombDefused);
	cmdMessenger.attach(8, defaultColor);
	cmdMessenger.attach(9, tWin);
	cmdMessenger.attach(10, ctWin);
	cmdMessenger.attach(unknownCmd);
}

void initSerial()
{
	Serial.begin(BAUD_RATE);
	attachCommandCallbacks();
}

void setup()
{
	randomSeed(analogRead(0));
	pinMode(REDPIN, OUTPUT);
	pinMode(BLUEPIN, OUTPUT);
	pinMode(GREENPIN, OUTPUT);
	setColor(offColor);
	initSerial();
}

void loop()
{
	currentTime = millis();

	// Handle bomb blinking
	if (!winCondition && bombPlanted && currentTime - lastBlinkTime >= bombBlinkInterval)
	{
		bombLight = !bombLight;
		if (bombLight)
		{
			setColor(bombColor);
		}
		else
		{
			setColor(bombColorOff);
		}

		lastBlinkTime = currentTime;
	}

	// Handle bomb exploded
	if (bombIsExploded && currentTime - lastExplodedChange >= explodedChangeInterval)
	{
		setRandomColor();
		lastExplodedChange = currentTime;
	}

	// Handle wins
	if (winCondition && !bombIsExploded && !bombIsDefused && currentTime - lastWinChange >= winChangeInterval)
	{
		if (terrorWin)
		{
			// T won
			setColorWithIntensity(terroColor, winIntensity);
		}
		else
		{
			// CT won
			setColorWithIntensity(counterColor, winIntensity);
		}
		if (winLight)
		{
			winIntensity = 0.6f;
		}
		else
		{
			winIntensity = 1.0f;
		}
		winLight = !winLight;
		lastWinChange = currentTime;
	}

	// Handle fixed update
	if (currentTime - lastUpdate >= fixedUpdateInterval)
	{
		// Handle regular state updates

		// Shorten bomb blink interval
		if (bombPlanted && bombBlinkInterval > BOMB_MIN_BLINK_INTV)
		{
			bombBlinkInterval -= BOMB_BLINK_INTV_DEC;
		}
		lastUpdate = currentTime;
	}

	// Read in new commands
	cmdMessenger.feedinSerialData();
}

void fadeTo(int color[])
{
	while (isArrayEquals(currentColor, color) == false)
	{
		if (currentColor[0] > color[0])
		{
			currentColor[0]--;
		}
		else if (currentColor[0] < color[0])
		{
			currentColor[0]++;
		}

		if (currentColor[1] > color[1])
		{
			currentColor[1]--;
		}
		else if (currentColor[1] < color[1])
		{
			currentColor[1]++;
		}

		if (currentColor[2] > color[2])
		{
			currentColor[2]--;
		}
		else if (currentColor[2] < color[2])
		{
			currentColor[2]++;
		}

		delay(5);
		setColor(currentColor);
	}
}

void setColor(int color[3])
{
	analogWrite(REDPIN, color[0]);
	analogWrite(GREENPIN, color[1]);
	analogWrite(BLUEPIN, color[2]);
	setCurrentColor(color);
}

void setColorWithIntensity(int color[3], float intensity)
{
	currentColor[0] = (int)(color[0] * intensity);
	currentColor[1] = (int)(color[1] * intensity);
	currentColor[2] = (int)(color[2] * intensity);
	currentColor[0] = constrain(currentColor[0], 0, 255);
	currentColor[1] = constrain(currentColor[1], 0, 255);
	currentColor[2] = constrain(currentColor[2], 0, 255);
	analogWrite(REDPIN, currentColor[0]);
	analogWrite(GREENPIN, currentColor[1]);
	analogWrite(BLUEPIN, currentColor[2]);
}

void setCurrentColor(int color[3])
{
	currentColor[0] = color[0];
	currentColor[1] = color[1];
	currentColor[2] = color[2];
}

void setRandomColor()
{
	randomColor[0] = random(256);
	randomColor[1] = random(256);
	randomColor[2] = random(256);
	setColor(randomColor);
}

bool isArrayEquals(int first[], int second[])
{
	if (first[0] == second[0] && first[1] == second[1] && first[2] == second[2])
	{
		return true;
	}
	return false;
}

void unknownCmd()
{
	defaultColor();
	cmdMessenger.feedinSerialData();
}

void onMenu()
{
	setColor(menuColor);
	resetSpecialState();
	cmdMessenger.feedinSerialData();
}

void teamCT()
{
	setColor(counterColor);
	resetSpecialState();
	bombBlinkInterval = BOMB_START_BLINK_INTV;
	cmdMessenger.feedinSerialData();
}

void teamT()
{
	setColor(terroColor);
	resetSpecialState();
	bombBlinkInterval = BOMB_START_BLINK_INTV;
	cmdMessenger.feedinSerialData();
}

void flashed()
{
	if (!bombPlanted && !bombIsDefused && !bombIsExploded && !winCondition)
	{
		int flashedState = cmdMessenger.readInt16Arg();
		flashedColor[0] = flashedState;
		flashedColor[1] = flashedState;
		flashedColor[2] = flashedState;
		setColor(flashedColor);
	}
	cmdMessenger.feedinSerialData();
}

void bomb()
{
	bombPlanted = true;
	bombIsExploded = false;
	bombIsDefused = false;
}

void bombExploded()
{
	setRandomColor();
	bombPlanted = false;
	bombIsExploded = false;
	bombIsExploded = true;
	cmdMessenger.feedinSerialData();
}

void bombDefused()
{
	setColor(defusedColor);
	bombPlanted = false;
	bombIsExploded = false;
	bombIsDefused = true;
	cmdMessenger.feedinSerialData();
}

void tWin()
{
	winCondition = true;
	terrorWin = true;
}

void ctWin()
{
	winCondition = true;
	terrorWin = false;
}

void defaultColor()
{
	setColor(offColor);
	resetSpecialState();
	cmdMessenger.feedinSerialData();
}

void resetSpecialState()
{
	bombPlanted = false;
	bombIsExploded = false;
	bombIsDefused = false;
	winCondition = false;
}
