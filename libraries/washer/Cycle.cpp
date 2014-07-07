#include <Cycle.h>

Cycle::Cycle(char *name, Phase *phases[], int n_phases) {
    _phases = phases;
    _n_phases = n_phases;
    _name = name;
    _currentPhase = -1;
    _startTime = 0;
}

void Cycle::reset() {
    Serial.print("Resetting Cycle ");
    Serial.print(_name);
    Serial.print(" at ");
    Serial.print(millis() - _startTime);
    Serial.println(" after start");
}

boolean Cycle::isRunning() {
    return _currentPhase > -1;
}

void Cycle::start() {
    if (_startTime > 0) {
        reset();
    }
    _startTime = millis();
    Serial.print("Starting Cycle ");
    Serial.print(_name);
    Serial.print(" at ");
    Serial.println(_startTime);
    for(int i = 0; i < _n_phases; i++) {
        _phases[i]->reset();
    }
    _currentPhase = 0;
    _phases[_currentPhase]->start();
}

Phase *Cycle::currentPhase() {
    if (0 <= _currentPhase && _currentPhase < _n_phases) {
        return _phases[_currentPhase];
    } else {
        return (Phase *)NULL;
    }
}

Phase *Cycle::next() {
    if (-1 <= _currentPhase && _currentPhase < _n_phases - 1) {
        return _phases[++_currentPhase];
    } else {
        _currentPhase = -1;
        return (Phase *)NULL;
    }
}

void Cycle::stop() {
    // The last phase should stop it all.
    _phases[_n_phases-1]->start();
    _currentPhase = _n_phases-1;
}

void Cycle::process() {
    if (currentPhase() != (Phase *)NULL) {
       if (currentPhase()->isDone()) {
           Phase *n = next();
           if (n != (Phase *)NULL) {
               n->start();
           }
       }
    } else {
#ifdef DEBUG
        Serial.println("No Phases Left");
#endif
    }
}
