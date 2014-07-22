#ifndef __PHASE_H__
#define __PHASE_H__
#include <Arduino.h>
#include <Relay.h>
#include <Display.h>

class Phase {
    Relay **_relays;
    int *_relayStates;
    int _n_relays;
    long _duration;
    char *_name;
    unsigned long _startTime;
    boolean _started;
public:
    Phase(char *name, long duration, Relay *relays[], int states[], int n_relays);
    void start();
    void reset();
    long timeRemaining();
    void printTimeRemaining();
    long duration();
    boolean hasStarted();
    boolean isDone();
};
#endif
