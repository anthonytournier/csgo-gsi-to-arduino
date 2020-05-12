#include <ArduinoJson.h>
#include <CmdMessenger.h>

#define REDPIN 5
#define GREENPIN 6
#define BLUEPIN 3
#define BUFFER_SIZE 500
#define BAUD_RATE 9600

bool bombPlanted = false;
bool bombLight = false;

bool bombIsExploded = false;

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

long bombBlinkInterval = 500;
long lastBlinkTime = 0;

long explodedChangeInterval = 50;
long lastExplodedChange = 0;

long currentTime = 0;

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
	if (bombPlanted && currentTime - lastBlinkTime >= bombBlinkInterval)
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

void setColor(int color[])
{
	analogWrite(REDPIN, color[0]);
	analogWrite(GREENPIN, color[1]);
	analogWrite(BLUEPIN, color[2]);
	setCurrentColor(color);
}

void setCurrentColor(int color[])
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
	bombPlanted = false;
	bombIsExploded = false;
	cmdMessenger.feedinSerialData();
}

void teamCT()
{
	setColor(counterColor);
	bombPlanted = false;
	bombIsExploded = false;
	cmdMessenger.feedinSerialData();
}

void teamT()
{
	setColor(terroColor);
	bombPlanted = false;
	bombIsExploded = false;
	cmdMessenger.feedinSerialData();
}

void flashed()
{
	int flashedState = cmdMessenger.readInt16Arg();
	flashedColor[0] = flashedState;
	flashedColor[1] = flashedState;
	flashedColor[2] = flashedState;
	setColor(flashedColor);
	cmdMessenger.feedinSerialData();
}

void bomb()
{
	bombPlanted = true;
	bombIsExploded = false;
}

void bombExploded()
{
	setRandomColor();
	bombPlanted = false;
	bombIsExploded = true;
	cmdMessenger.feedinSerialData();
}

void bombDefused()
{
	setColor(defusedColor);
	bombPlanted = false;
	bombIsExploded = false;
	cmdMessenger.feedinSerialData();
}

void defaultColor()
{
	setColor(offColor);
	bombPlanted = false;
	bombIsExploded = false;
	cmdMessenger.feedinSerialData();
}
