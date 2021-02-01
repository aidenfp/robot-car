#include "SonicModule.h"

    SonicModule::SonicModule(int trigger_pin, int echo_pin) {
        TRIG = trigger_pin;
        ECHO = echo_pin;
        pulse_timer = micros();
        pulse_start = micros();
        is_trigger = false;
        is_echo = false;
        distance = 0.0;
        pinMode(TRIG, OUTPUT);
        pinMode(ECHO, INPUT);
    }

    void SonicModule::update() {
        if (!is_trigger) {
            pulse_timer = micros();
            is_trigger = true;
            digitalWrite(TRIG, HIGH);
        }
        if (micros() - pulse_timer > 10 && is_trigger) {
            digitalWrite(TRIG, LOW);
            is_trigger = false;
            if(!is_echo && digitalRead(ECHO) == HIGH) {
                pulse_start = micros();
                is_echo = true;
            }
            if (is_echo && digitalRead(ECHO) == LOW) {
                double duration = micros() - pulse_start;
                distance = duration * 0.017;
                is_trigger = false;
                is_echo = false;
            }
        }
    }

    double SonicModule::get_distance() {
        return distance;
    }
