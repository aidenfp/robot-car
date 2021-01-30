#include <Arduino.h>
#pragma once

class SonicModule {
    int TRIG;
    int ECHO;
    bool is_trigger;
    bool is_echo;
    unsigned long pulse_timer;
    unsigned long pulse_start;
    double distance;

    public:
    SonicModule(int trigger_pin, int echo_pin);

    void update();

    double get_distance();
};