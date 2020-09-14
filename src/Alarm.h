#include <Arduino.h>

class Alarm {
    private:
        bool flag;
        bool back;
        uint8_t pin;
        uint64_t time;
        uint64_t timeDelay;
        uint8_t count = 0;

    public:
        Alarm(uint8_t pin);
        void startPlay();
        void play();
        void stop();
};