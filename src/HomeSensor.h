#include <Arduino.h>

class HomeSensor {
    private:
        String _name;
		int8_t   _pin;
		bool _defaultStatus;
		bool _status;
	public:
		HomeSensor(String sensorName, int8_t pin, bool defaultStatus);
        bool checkSensor();
        String getName();
        bool getStatus();
        void worked();
};