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

long Phase::duration() {
    return _duration;
}

char * Phase::name() {
    return _name;
}
boolean Phase::hasStarted() {
    return _started;
}

void Phase::start() {
    _startTime = millis();
    _started = true;
    Serial.print("Staring Phase ");
    Serial.print(_name);
    Serial.print(" at ");
    Serial.print(_startTime);
    Serial.print(" for ");
    Serial.print(_duration);
    Serial.println(" miliseconds");
    for(int i = 0; i < _n_relays; i++) {
        _relays[i]->set(_relayStates[i]);
    }
}

long Phase::timeRemaining() {
    long timeleft =  _duration - (millis() - _startTime);
    if (timeleft < 0) return 0;
    return timeleft;
}

void Phase::printTimeRemaining() {
    long timeleft = timeRemaining();

    int secs, mins;
#ifdef DEBUG
    Serial.print("Phase ");
    Serial.print(_name);
    Serial.print(" has ");
    Serial.print(timeleft);
    Serial.println(" miliseconds");
#endif
    Display->setCursor(0,0);
    Display->print(_name);
    Display->print("              ");
    secs = timeleft/1000;
    mins = secs/60;
    secs = secs % 60;
    Display->setCursor(11,0);
    if (mins < 10) Display->print(" ");
    Display->print(mins);
    Display->print(":");
    if (secs < 10) Display->print("0");
    Display->print(secs);
}

boolean Phase::isDone() {
    return _started && timeRemaining() <= 0;
}

void Phase::finish() {
  _startTime = millis() - _duration - 1;
  _started = false;
}

