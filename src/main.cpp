#include <Arduino.h>
#include <SoftwareSerial.h>
#include <HomeSensor.h>

#define BUTTON 2
#define ARRAY_SIZE 4
#define LED LED_BUILTIN

SoftwareSerial rfid(7, 8); // RX, TX
char* key[] = {"5300AA56E04F", "5300AC75A62C"};
boolean signOn = false;
boolean alarm = false;

uint32_t timer;
uint32_t period = 1000;
uint32_t delayLed = 5000;
uint32_t startLed;

HomeSensor SR501_1("SR501_1", 10, true);
HomeSensor SR501_2("SR501_2", 11, true);
HomeSensor DOOR_1("DOOR_1", 4, true);
HomeSensor DOOR_2("DOOR_2", 3, true);
HomeSensor sensors[ARRAY_SIZE] = {SR501_1, SR501_2, DOOR_1, DOOR_2};

void setup() {
	Serial.begin(9600);
	rfid.begin(9600);
	Serial.println("start");
}

void checkRfid() {
	if (!signOn) {
		if (rfid.available() && rfid.find(key[0])) {
			Serial.println("SIGNALIATION OFF");
			signOn = true;
			alarm = false;
			digitalWrite(LED, LOW);
			rfid.end();
		}
	}
}

void checkSensors() {
	for (size_t i = 0; i < ARRAY_SIZE; i++) {
		if (sensors[i].checkSensor()) {
			Serial.println(sensors[i].getName());
		}
	}
	
}

void loop() {
	checkSensors();
	delay(1000);
}