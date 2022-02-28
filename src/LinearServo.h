#include <Arduino.h>
#include <Romi32U4.h>
#include <servo32u4.h>

class LinearServo
{
private:
    Servo32U4 servo;
    const int linearPotPin = A11;

    const int jawDown = 1300;
    const int jawUp = 1700;
    const int servoStop = 1490;

    const int jawOpenPotVoltageADC = 600;
    const int jawClosedPotVoltageADC = 940;

    int prevVADC = 0;
    const int TOLERANCE_DOWN = 25;
    const int TOLERANCE_UP = 16;

public:
    LinearServo();

    float getPotVADC();
    boolean openJaw();
    boolean closeJaw();
    boolean getMovingDown();
    boolean getMovingUp();
    void stop();
};