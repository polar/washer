#ifndef __RELAY_H__
#define __RELAY_H__
#include "Arduino.h"


#define RELAY_OFF HIGH
#define RELAY_ON  LOW

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
#endif
