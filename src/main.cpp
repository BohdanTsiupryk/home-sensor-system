#include <Arduino.h>
#include <SoftwareSerial.h>
#include <HomeSensor.h>
#include <RF24.h>
#include <SPI.h>
#include <Alarm.h> 
#include <GyverButton.h>

//INITIALIZATION BASE DEFINE-------------------------------------------------------------
#define ARRAY_SIZE 4

#define RED_LED 10
#define BLUE_LED 11
#define BUTTON 5
#define BUZZER 12
#define SENSOR_1 A1
#define SENSOR_2 A2
#define SENSOR_3 A3
#define SENSOR_4 A4
//END INITIALIZATION-------------------------------------------------------------

//VARIABLES -------------------------------------------------------------
SoftwareSerial rfid(7, 8); // RX, TX
GButton button(BUTTON);

char* key[] = {"5300AA56E04F", "5300AC75A62C"};

bool alarmWork = false;
bool alarmStatus = false;
bool alarm = false;

int64_t timer = 10000;
int64_t lastAlarm = 0;

Alarm alarmPlayer(BUZZER);

HomeSensor SR501_1("SR501_1", SENSOR_1, false);
HomeSensor SR501_2("SR501_2", SENSOR_2, false);
HomeSensor DOOR_1("DOOR_1", SENSOR_3, true);
HomeSensor DOOR_2("DOOR_2", SENSOR_4, true);
HomeSensor sensors[ARRAY_SIZE] = {SR501_1, SR501_2, DOOR_1, DOOR_2};
//END VARIABLES -------------------------------------------------------------

void checkRfid() {
	if (rfid.available() && 
			(rfid.find(key[0]) || rfid.find(key[1]))) {
		alarmWork = false;
		alarmPlayer.stop();
		rfid.end();
	}
}

void checkSensors() {
	for (size_t i = 0; i < ARRAY_SIZE; i++) {
		if (sensors[i].checkSensor()) {
			sensors[i].worked();
			alarmStatus = true;	
			lastAlarm = millis();
		}
	}
}

void alarmed() {
	if (alarmStatus) {
		if (millis() - lastAlarm > timer) {
			alarmStatus = false;
			alarm = true;	
			alarmPlayer.startPlay();
		}
	}
	
}

void ledControl() {
	if (alarmWork) {
		digitalWrite(RED_LED, HIGH);
		digitalWrite(BLUE_LED, LOW);
	} else {
		digitalWrite(RED_LED, LOW);
		digitalWrite(BLUE_LED, HIGH);
	}
}

void setup() {
	rfid.begin(9600);
	pinMode(RED_LED, OUTPUT);
	pinMode(BLUE_LED, OUTPUT);
	pinMode(BUTTON, INPUT_PULLUP);
}

void loop() {
	if (alarmWork) {
		checkRfid();
		checkSensors();
		alarmed();
	} else {
		if (button.isStep()) {
			alarmWork = true;
		}	
	}

	ledControl();
	alarmPlayer.play();
}