#include "Phase.h"

Phase::Phase(char *name, long duration, Relay *relays[], int states[], int n_relays) {
    _name = name;
    _relays = relays;
    _relayStates = states;
    _n_relays = n_relays;
    _duration = duration * 1000; //minutes
}

void Phase::reset() {
    _startTime = 0;
    _started = false;
}

boolean Phase::hasStarted() {
    return _started;
}

void Phase::start() {
    _startTime = millis();
    _started = true;
#ifdef DEBUG
    Serial.print("Staring Phase ");
    Serial.print(_name);
    Serial.print(" at ");
    Serial.print(_startTime);
    Serial.print(" for ");
    Serial.print(_duration);
    Serial.println(" miliseconds");
#endif
    for(int i = 0; i < _n_relays; i++) {
        _relays[i]->set(_relayStates[i]);
    }
}

long Phase::timeRemaining() {
    long timeleft =  _duration - (millis() - _startTime);
    int secs, mins;
#ifdef DEBUG   
    Serial.print("Phase ");
    Serial.print(_name);
    Serial.print(" has ");
    Serial.print(timeleft);
    Serial.println(" miliseconds");
#else
    Serial.print(_name);
    Serial.print("  ");
    secs = timeleft/1000;
    mins = secs/60;
    secs = secs % 60;
    Serial.print(" ");Serial.print(mins);
    Serial.print(":");
    if (secs < 10) Serial.print("0");
    Serial.print(secs);
    Serial.print("\n");
#endif
    return timeleft;
}

boolean Phase::isDone() {
    return timeRemaining() <= 0;
}