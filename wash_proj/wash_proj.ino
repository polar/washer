
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Display.h>
#include <Cycle.h>
#include <StartButton.h>

  
Relay *r0 = new Relay("SPRAY",     3); // V-BU
Relay *r1 = new Relay("HI_MOTOR1", 4); // T-BU
Relay *r2 = new Relay("HI_MOTOR2", 5); // T-OR
Relay *r3 = new Relay("LO_MOTOR",  6); // V-T
Relay *r4 = new Relay("AGITATE1",  7); // R-W-BLK
Relay *r5 = new Relay("AGITATE2",  8); // Y-BU
Relay *r6 = new Relay("WASH",      9); // P-BR
Relay *r7 = new Relay("RINSE",    10); // P-Y-R
Relay *r8 = new Relay("SPIN1",    11); // R-BU
Relay *r9 = new Relay("SPIN2",    12); // Y-W-BLK
Relay *rA = new Relay("BYPASS",   13); // BLK-W-GY
Relay *rB = new Relay("FILL",     A0); // BK-W-V
Relay *rC = new Relay("ACTIVE",   A1); // BK-W-BLK

Relay *all[] = {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,rA,rB,rC};

int n_relays = 13;
//                   0  1  2  3  4  5  6  7  8  9  A  B  C
int PFill[]      = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 }; //  Fill
int PPreWash[]   = { 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1 }; // Fill Agitate, Wash HI
int PDrain1[]    = { 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1 }; // 1/2 Tub Drain, Drain HI
int PWashHi[]    = { 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1 }; // Agitate HI, Fill & Wash
int PWashLo[]    = { 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1 }; // Agitate LO, Fill & Wash
int PDrain2[]    = { 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1 }; //  Drain HI
int PSpinWash[]  = { 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1 }; //  Spin HI
int PRinse[]     = { 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1 }; // Fill Agitate, Rinse HI
int PDrain3[]    = { 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1 }; //  Drain HI
int PSpraySpin[] = { 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1 }; //  Spray Spin HI
int PSpinRinse[] = { 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1 }; //  Spin HI
int POff[]       = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; //  Off
int PWait[]      = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }; //  Wait

Phase *pFill      = new Phase("Fill",       5 * 60, all, PFill,      n_relays);
Phase *pHalfFill  = new Phase("Fill",       3 * 60, all, PFill,      n_relays);
Phase *pPreWash   = new Phase("PreWash",    4 * 60, all, PPreWash,   n_relays);
Phase *pDrain1    = new Phase("Drain",      1 * 60, all, PDrain1,    n_relays);
Phase *pWashHi    = new Phase("Wash Hi",    5 * 60, all, PWashHi,    n_relays);
Phase *pWashLo    = new Phase("Wash Lo",    5 * 60, all, PWashLo,    n_relays); 
Phase *pDrain2    = new Phase("Drain",      3 * 60, all, PDrain2,    n_relays);
Phase *pSpinWash  = new Phase("Spin Wash",  4 * 60, all, PSpinWash,  n_relays);
Phase *pRinse     = new Phase("Rinse",      4 * 60, all, PRinse,     n_relays);
Phase *pDrain3    = new Phase("Drain",      3 * 60, all, PDrain3,    n_relays);
Phase *pSpraySpin = new Phase("Spin/Spray", 2 * 60, all, PSpraySpin, n_relays);
Phase *pSpinRinse = new Phase("Spin Rinse" ,4 * 60, all, PSpinRinse, n_relays);
Phase *pOff       = new Phase("Off",            20, all, POff,       n_relays);
Phase *pWait      = new Phase("Wait",            6, all, PWait,      n_relays);

int n_phases = 22; 

Phase *regularWashPhases[] = {
  pFill, pPreWash, pWait, pDrain1, pWait, pHalfFill, pWashHi, pWashLo, pWait,
  pDrain2, pWait, pSpinWash, pWait, pFill, pRinse, pWait, pDrain3, 
  pWait, pSpraySpin, pSpinRinse, pWait, pOff
};

Cycle *regularWashCycle = new Cycle("Regular Wash", regularWashPhases, n_phases);

Cycle *currentCycle;

StartButton *startButton;

void setup() {
    Serial.begin(9600);
    Display->begin(16,2);
    Display->backlight();
    Display->clear();
    Display->setCursor(0,0);
    Display->print("Setup");
    startButton = new StartButton(2, 2000);
    startButton->begin();
    for(int i = 0; i < n_relays; i++) {
        all[i]->begin();
    }
    Display->print(" Done");
    currentCycle = regularWashCycle;
    delay(1000);
}
    
void loop() {
    startButton->check();
    if (startButton->wasClicked()) {
        Serial.println("Button was clicked");
        startButton->reset(1000);
        if (!currentCycle->isRunning()) {
            currentCycle->start();
        } else {
          currentCycle->advance();
        }
    }
    if (startButton->wasHeld()) {
        Serial.println("Button was held");
        currentCycle->stop();
        startButton->reset(5000);
    }
    currentCycle->process();
    if (!currentCycle->isRunning()) {
        Display->clear();
        Display->print("Ready");
    }
    delay(100);
}
    

