#include <Phase.h>

class Cycle {
    Phase **_phases;
    char *_name;
    int   _n_phases;
    int   _currentPhase;
    unsigned long _startTime;
public:
    Cycle(char *name, Phase *phases[], int n_phases);
    void start();
    void reset();
    void stop();
    boolean isRunning();
    Phase *currentPhase();
    Phase *next();
    void process();
};