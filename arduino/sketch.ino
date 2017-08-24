#include <ArduinoJson.h>
 
#define REDPIN 5
#define GREENPIN 6
#define BLUEPIN 3
#define BUFFER_SIZE 500
#define BAUD_RATE 9600

void setup() {
	initSerial();
	pinMode(REDPIN, OUTPUT);
	pinMode(GREENPIN, OUTPUT);
	pinMode(BLUEPIN, OUTPUT);
}

void loop() {
	/* Get that for later :) */
/* 	StaticJsonBuffer<200> jsonBuffer;
	char json[] = "{\"round\": {\"phase\": \"live\",\"bomb\": \"planted\"}}";
	JsonObject& root = jsonBuffer.parseObject(json);
	
	if (!root.success()) {
		Serial.println("parseObject() failed");
		return;
	}

	const char* bombStatus = root["round"]["bomb"];
	if(bombStatus == "planted") {
	}
	Serial.println(bombStatus); */
}

void initSerial() {
	Serial.begin(BAUD_RATE);
	while (!Serial) {
	  ; 
	}
	Serial.println("Serial ready");
}