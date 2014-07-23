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
    Serial.println("Cycle Stopped!");
    // The last phase should stop it all.
    _phases[_n_phases-1]->start();
    _currentPhase = _n_phases-1;
}

void Cycle::printProgress() {
    double prog = progress();
    int n = (int) (16*prog);
#define DEBUG 1
#ifdef DEBUG
    Serial.print("Progress ");
    Serial.print(prog);
    Serial.print(" = ");
    Serial.println(n);
#endif
    Display->setCursor(0,1);
    for(int i = 0; i < n; i++) {
        Display->print("#");
    }
}

void Cycle::advance() {
    if (currentPhase() != (Phase *)NULL) {
       if (currentPhase()->hasStarted()) {
	   currentPhase()->finish();
           Phase *n = next();
           if (n != (Phase *)NULL) {
               n->start();
           }
       }
       // The above will always print a line with Phase and the Time.
       printProgress();
    }
}
  

void Cycle::process() {
    if (currentPhase() != (Phase *)NULL) {
       currentPhase()->printTimeRemaining();
       if (currentPhase()->isDone()) {
           Phase *n = next();
           if (n != (Phase *)NULL) {
               n->start();
           }
       }
       // The above will always print a line with Phase and the Time.
       printProgress();
    } else {
#ifdef DEBUG
        Serial.println("No Phases Left");
#endif
    }
}
    
unsigned long Cycle::totalTime() {
    unsigned long time;
    for(int i = 0; i < _n_phases; i++) {
        time += _phases[i]->duration();
    }
    return time;
}

double Cycle::progress() {
    unsigned long time = totalTime();
    unsigned long performed = 0;
    for(int i = 0; i < _n_phases; i++) {
        if (_phases[i]->hasStarted()) {
            if (_phases[i]->isDone()) {
                performed += _phases[i]->duration();
            } else {
                performed += _phases[i]->duration() - _phases[i]->timeRemaining();
                break;
            }
        } else {
            break;
        }
    }
    return (double) performed / (double) time;
}
