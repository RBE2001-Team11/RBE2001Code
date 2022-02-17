#include <JawServo.h>

JawServo::JawServo()
{
}

float JawServo::getPotVADC()
{
    return analogRead(linearPotPin);
}

boolean JawServo::openJaw()
{
    servo.writeMicroseconds(open);
    if (getPotVADC() <= openVADC)
    {
        stopServo();
        return true;
    }
    return false;
}

boolean JawServo::closeJaw()
{
    servo.writeMicroseconds(close);
    if (getPotVADC() >= closedVADC)
    {
        stopServo();
        return true;
    }
    return false;
}

void JawServo::stopServo()
{
    servo.writeMicroseconds(stop);
}
