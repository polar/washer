#include "Arduino.h"

#define RELAY_OFF 0
#define RELAY_ON  1

class Relay {
    int _pin;
    int _status;
    char *_name;
public: 
    Relay(char *name, int i);
    void begin();
    void on();
    void off();
    int status();
    void set(int x);
};