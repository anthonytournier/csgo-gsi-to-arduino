#include <ArduinoJson.h>
#include <CmdMessenger.h>
 
#define REDPIN 5
#define GREENPIN 6
#define BLUEPIN 3
#define BUFFER_SIZE 500
#define BAUD_RATE 9600

int terroColor[3] = {255, 100, 0};
int counterColor[3] = {20, 36, 214};
int deadColor[3] = {171, 3, 142};
int bombColor[3] = {255, 0, 0};
int explodedColor[3] = {255, 255, 255};
int offColor[3] = {0, 0, 0};
int menuColor[3] = {0, 255, 0};
int currentColor[3] = {0, 0, 0};
CmdMessenger cmdMessenger = CmdMessenger(Serial);

void attachCommandCallbacks() {
	cmdMessenger.printLfCr();
	cmdMessenger.attach(1, onMenu);
	cmdMessenger.attach(2, teamT);
	cmdMessenger.attach(3, teamCT);
	cmdMessenger.attach(unknownCmd);
}

void initSerial() {
	Serial.begin(9600);
	attachCommandCallbacks();
}

void setup() {
	pinMode(REDPIN, OUTPUT);
	pinMode(BLUEPIN, OUTPUT);
	pinMode(GREENPIN, OUTPUT);
	setColor(offColor);
	initSerial();
}

void loop() {
	cmdMessenger.feedinSerialData();
}

void fadeTo(int color[]) {
	while(isArrayEquals(currentColor, color) == false) {
		if(currentColor[0] > color[0]) {
			currentColor[0]--;
		} else if (currentColor[0] < color[0]) {
			currentColor[0]++;
		}
		
		if(currentColor[1] > color[1]) {
			currentColor[1]--;
		} else if (currentColor[1] < color[1]) {
			currentColor[1]++;
		}
		
		if(currentColor[2] > color[2]) {
			currentColor[2]--;
		} else if (currentColor[2] < color[2]) {
			currentColor[2]++;
		}

		delay(5);
		setColor(currentColor);
	}
}

void setColor(int color[]) {
	analogWrite(REDPIN, color[0]);
	analogWrite(GREENPIN, color[1]);
	analogWrite(BLUEPIN, color[2]);  
	setCurrentColor(color);
}

void setCurrentColor(int color[]) {
	currentColor[0] = color[0];
	currentColor[1] = color[1];
	currentColor[2] = color[2];
}

bool isArrayEquals(int first[], int second[]) {
	if(first[0] == second[0] && first[1] == second[1] && first[2] == second[2]) {
		return true;
	}
	return false;
}

void bombBeep() {
	setColor(offColor);
	delay(900);
	setColor(bombColor);
	delay(100);
}

void bombExploded() {
	while(millis() < 3000) {
		setColor(offColor);
		delay(50);
		setColor(explodedColor);
		delay(50);
	}
}

void unknownCmd() {
	onMenu();
	cmdMessenger.feedinSerialData();
}

void onMenu() {
	setColor(menuColor);
	cmdMessenger.feedinSerialData();
}

void teamCT() {
	setColor(counterColor);
	cmdMessenger.feedinSerialData();
}

void teamT() {
	setColor(terroColor);
	cmdMessenger.feedinSerialData();
}