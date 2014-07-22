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
Relay *r7 = new Relay("RINSE",    10); // P-BR
Relay *r8 = new Relay("SPIN1",    11); // R-BU
Relay *r9 = new Relay("SPIN2",    12); // Y-W-BLK
Relay *rA = new Relay("BYPASS",   13); // BLK-W-GY
Relay *rB = new Relay("FILL",     A0); // BK-W-V
Relay *rC = new Relay("ACTIVE",   A1); // BK-W-BLK

Relay *all[] = {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,rA,rB,rC};

int n_relays = 13;

int Phase0[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 }; //  Fill
int Phase1[] = { 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1 }; // Fill Agitate, Wash HI
int Phase2[] = { 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1 }; // 1/2 Tub Drain, Drain HI
int Phase3[] = { 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1 }; // Agitate HI, Fill & Wash
int Phase4[] = { 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1 }; // Agitate LO, Fill & Wash
int Phase5[] = { 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1 }; //  Drain HI
int Phase6[] = { 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1 }; //  Spin HI
int Phase7[] = { 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1 }; // Fill Agitate, Rinse HI
int Phase8[] = { 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1 }; //  Drain HI
int Phase9[] = { 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1 }; //  Spray Spin HI
int PhaseA[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; //  Off

Phase *p0 = new Phase("Fill",  1 * 60, all, Phase0, n_relays);
Phase *p1 = new Phase("Wash",  3 * 60, all, Phase1, n_relays);
Phase *p2 = new Phase("Drain", 3 * 60, all, Phase2, n_relays);
Phase *p3 = new Phase("Wash", 10 * 60, all, Phase3, n_relays);
Phase *p4 = new Phase("Wash",  5 * 60, all, Phase4, n_relays);
Phase *p5 = new Phase("Drain", 5 * 60, all, Phase5, n_relays);
Phase *p6 = new Phase("Spin",  5 * 60, all, Phase6, n_relays);
Phase *p7 = new Phase("Rinse", 8 * 60, all, Phase7, n_relays);
Phase *p8 = new Phase("Drain", 5 * 60, all, Phase8, n_relays);
Phase *p9 = new Phase("Spin", 10 * 60, all, Phase9, n_relays);
Phase *pA = new Phase("Off",       20, all, PhaseA, n_relays);

int n_phases = 11;

Phase *cycle1[] = {p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,pA};

Cycle *c1 = new Cycle("Regular Wash", cycle1, n_phases);

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
    delay(1000);
}
    
void loop() {
    startButton->check();
    if (startButton->wasClicked()) {
        Serial.println("Button was clicked");
        startButton->reset(1000);
        if (!c1->isRunning()) {
            c1->start();
        }
    }
    if (startButton->wasHeld()) {
        Serial.println("Button was held");
        c1->stop();
        startButton->reset(5000);
    }
    c1->process();
    if (!c1->isRunning()) {
        Display->clear();
        Display->print("Ready");
    }
    delay(100);
}
    

