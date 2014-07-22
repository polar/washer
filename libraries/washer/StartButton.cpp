#include "StartButton.h"

StartButton::StartButton(int pin, int threshold) {
  _threshold = threshold;
  _pin = pin;
  _held = false;
  _clicked = false;
  _holdReported = false;
  _wasClosed = false;
}

void StartButton::begin() {
    pinMode(_pin, INPUT);
    reset(1000);
}

void StartButton::reset(unsigned int activate) {
    _holdReported = false;
    _wasClosed = false;
    _held = false;
    _clicked = false;
    _activateTime = millis() + activate;
}
    
void StartButton::check() {
    unsigned long currentTimeMillis = millis();
    if (currentTimeMillis >= _activateTime) {
        int sig = digitalRead(_pin);
        boolean isClosed = (sig == HIGH);
            Serial.print("StartButton ");
            Serial.print(sig);
            Serial.print(" isClosed ");
            Serial.print(isClosed);
            Serial.print(" _wasClosed ");
            Serial.print(_wasClosed);
        if (_closeTimeMillis > 0) {
            Serial.print(" time ");
            Serial.print(currentTimeMillis - _closeTimeMillis);
        }
        
        if (isClosed != _wasClosed) {
            if (isClosed) {
                _closeTimeMillis = currentTimeMillis;
            } else {
                _holdReported = false;
            }
        }
        _clicked = _held = false;
        boolean overHoldThreshold =
            (currentTimeMillis - _closeTimeMillis) >= _threshold;
        if (isClosed && overHoldThreshold) {
            _held = true;
        } else if (!isClosed && _wasClosed && !overHoldThreshold) {
            _clicked = true;
        }
        if (_held) 
            Serial.print(" held ");
        if (_clicked)
            Serial.print(" clicked");
        Serial.println();
        digitalWrite(_pin, LOW);
        _wasClosed = isClosed;
    } else {
        Serial.print("StartButton active in ");
        Serial.print(_activateTime - currentTimeMillis);
        Serial.println(" millisecs");
    }
}

boolean StartButton::wasClicked() {
    return _clicked;
}

boolean StartButton::wasHeld() {
    if (_held && !_holdReported) {
        _holdReported = true;
        return _held;
    } else {
        return false;
    }
}
