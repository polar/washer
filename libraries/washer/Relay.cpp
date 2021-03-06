#include <Relay.h>

Relay::Relay(char *name, int pin) {
  _pin = pin;
  _name = name;
}

void Relay::begin() {
  pinMode(_pin, OUTPUT);
  off();
}

void Relay::off() {
  digitalWrite(_pin, RELAY_OFF);
  _status = RELAY_OFF;
  Serial.print("Relay ");
  Serial.print(_pin);
  Serial.println(" OFF");
}

void Relay::on() {
  digitalWrite(_pin, RELAY_ON);
  _status = RELAY_ON;
  Serial.print("Relay ");
  Serial.print(_pin);
  Serial.println(" ON");
}

void Relay::set(int x) {
  if (x == 0) {
    off();
  } else {
    on();
  }
}
