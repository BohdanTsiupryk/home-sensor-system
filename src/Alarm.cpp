#include <Arduino.h>
#include <Alarm.h>

Alarm::Alarm(uint8_t pin) {
    Alarm::pin = pin;
    pinMode(pin, OUTPUT);
    flag = false;
    back = false;
    timeDelay = 10;
}

void Alarm::play() {
    if (flag && (millis() - time >= timeDelay)) {
        if (back) {
            analogWrite(pin, 255 - count);
        } else {
            analogWrite(pin, count);
        }        
        count++;
        time = millis();
    
        if (count == 255 || count == 0) {
            back = !back;
        }
    }
}

void Alarm::startPlay() {
    if (!flag) {
        flag = true;
        time = millis();
    }
}

void Alarm::stop() {
    if (flag) {
        flag = false;
        back = false;
        count = 0;
    }
}