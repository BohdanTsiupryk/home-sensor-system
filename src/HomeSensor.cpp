#include <Arduino.h>
#include <HomeSensor.h>

HomeSensor::HomeSensor(String sensorName, int8_t pin, bool defaultStatus) {
			_name = sensorName;
			_pin = pin;
			_defaultStatus = defaultStatus;
			_status = !defaultStatus;
            pinMode(pin, INPUT);
		}

bool HomeSensor::checkSensor() {
    return digitalRead(_pin) == _defaultStatus;
}

String HomeSensor::getName() {
    return _name;
}

bool HomeSensor::getStatus() {
	return _status;
}

void HomeSensor::worked() {
	_status = !_defaultStatus;
}