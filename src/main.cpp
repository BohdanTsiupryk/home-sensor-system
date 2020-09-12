#include <Arduino.h>
#include <SoftwareSerial.h>
#include <HomeSensor.h>
#include <RF24.h>
#include <SPI.h>

#define ARRAY_SIZE 4
#define TRANSMITED_DATA_SIZE 32
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
byte notTramsmitedData[TRANSMITED_DATA_SIZE];

bool signOn = false;
bool alarm = false;
const byte DEVICE_ID = 2;

HomeSensor SR501_1("SR501_1", 10, true);
HomeSensor SR501_2("SR501_2", 11, true);
HomeSensor DOOR_1("DOOR_1", 4, true);
HomeSensor DOOR_2("DOOR_2", 3, true);
HomeSensor sensors[ARRAY_SIZE] = {SR501_1, SR501_2, DOOR_1, DOOR_2};

void checkRfid() {
	if (rfid.available() && rfid.find(key[0])) {
		Serial.println("SIGNALIATION OFF");
		alarm = false;
		digitalWrite(LED, LOW);
		rfid.end();
	}
}

void checkSensors() {
	for (size_t i = 0; i < ARRAY_SIZE; i++) {
		if (sensors[i].checkSensor()) {
			sensors[i].worked();
			alarm = true;	
		}
	}
}

void radioSetup() {
	radio.begin();
 	radio.setAutoAck(1);         
 	radio.setRetries(1, 15);    
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

void sendData(byte data[]) {
	radio.stopListening();
	delay(20);
	radio.openWritingPipe(address[0]);
	radio.write(data, sizeof(data));
	radio.startListening();
	delay(30);
}

void formAndSendData() {
	byte data[6];
	data[0] = DEVICE_ID;
	data[1] = alarm;
	for (byte i = 0; i < ARRAY_SIZE; i++) {
		data[i + 2] = sensors[i].getStatus();
	}
	
	sendData(data);
}

void setup() {
	Serial.begin(9600);
	rfid.begin(9600);
	radioSetup();
	Serial.println("start");
}

void loop() {
	if (alarm) {
		checkRfid();
	} else {
		checkSensors();
	}
	formAndSendData();
}