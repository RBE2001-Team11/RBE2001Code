#include <Arduino.h>
#include <Romi32U4.h>
#include "BlueMotor.h"

class JawServo
{
private:
    // TODO get all of these
    const int linearPotPin = 0; // TODO
    const int stop = 1500;
    const int up = 1700;   // TODO
    const int down = 1300; // TODO
    const int delay = 540;
    const int openVADC = 600;
    const int closedVADC = 940;

public:
    JawServo();

    float getPotVADC();

    boolean openJaw();
    boolean closeJaw();
};
// set timer
