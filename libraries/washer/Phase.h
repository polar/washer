#include <Arduino.h>
#include <Relay.h>

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
    boolean hasStarted();
    boolean isDone();
};