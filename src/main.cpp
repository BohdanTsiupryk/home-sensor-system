#include <Arduino.h>
#include <SoftwareSerial.h>
#include <HomeSensor.h>
#include <RF24.h>
#include <SPI.h>

#define ARRAY_SIZE 4
#define CHANNEL 0x70

#define LED LED_BUILTIN
#define BUTTON 5
#define SENSOR_1 1
#define SENSOR_2 2
#define SENSOR_3 3
#define SENSOR_4 4

SoftwareSerial rfid(7, 8); // RX, TX
RF24 radio(9,10);

char* key[] = {"5300AA56E04F", "5300AC75A62C"};
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"};
boolean signOn = false;
boolean alarm = false;

HomeSensor SR501_1("SR501_1", 10, true);
HomeSensor SR501_2("SR501_2", 11, true);
HomeSensor DOOR_1("DOOR_1", 4, true);
HomeSensor DOOR_2("DOOR_2", 3, true);
HomeSensor sensors[ARRAY_SIZE] = {SR501_1, SR501_2, DOOR_1, DOOR_2};

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

void radioSetup() {
	radio.begin();
 	radio.setAutoAck(1);         
 	radio.setRetries(0, 15);    
 	radio.enableAckPayload();    
	radio.setPayloadSize(32);     

	radio.openReadingPipe(1, address[0]);   
	radio.setChannel(CHANNEL);  

	radio.setPALevel(RF24_PA_MAX);
	radio.setDataRate(RF24_250KBPS); 

	radio.powerUp();
	radio.startListening();

	Serial.println("Finished config nRF on chanel - " + CHANNEL);
}

void sendData() {

}

void setup() {
	Serial.begin(9600);
	rfid.begin(9600);

	Serial.println("start");
}

void loop() {
	if (alarm) {
		
	} else {
		checkSensors();
	}
	
	
	checkSensors();
}