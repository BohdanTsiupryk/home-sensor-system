#include <Arduino.h>

class HomeSensor {
    private:
        String _name;
		int8_t   _pin;
		bool _defaultStatus;
	public:
		HomeSensor(String sensorName, int8_t pin, bool dafaultStatus);
        bool checkSensor();
        String getName();
};