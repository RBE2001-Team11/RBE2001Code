#include <Arduino.h>
#include <Romi32U4.h>
#include <servo32u4.h>

class JawServo
{
private:
    Servo32U4 servo;
    // TODO get all of these
    const int linearPotPin = 18; // TODO
    const int stop = 1490;
    const int close = 1700; // TODO
    const int open = 1300;  // TODO
    const int delay = 540;  // ms
    const int openVADC = 600;
    const int closedVADC = 940;

public:
    JawServo();

    float getPotVADC();

    boolean openJaw();
    boolean closeJaw();
    void stopServo();
};
// set timer
