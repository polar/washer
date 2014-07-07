#include <Arduino.h>
    

class StartButton {
  int _pin;
  int _threshold;
  unsigned long _activateTime;
  unsigned long _closeTimeMillis;
  boolean _wasClosed;
  boolean _holdReported;
  boolean _held, _clicked;
public:
  StartButton(int pin, int threshold);
  void begin();
  void check();
  void reset(unsigned int activate_interval);
  boolean wasClicked();
  boolean wasHeld();
};