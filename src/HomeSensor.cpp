#include <Arduino.h>
#include <HomeSensor.h>

HomeSensor::HomeSensor(String sensorName, int8_t pin, bool dafaultStatus) {
			_name = sensorName;
			_pin = pin;
			_defaultStatus = dafaultStatus;
            pinMode(pin, INPUT);
		}

bool HomeSensor::checkSensor() {
    return digitalRead(_pin) == _defaultStatus;
}

String HomeSensor::getName() {
    return _name;
}