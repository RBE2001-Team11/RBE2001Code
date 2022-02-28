#include <JawServo.h>

JawServo::JawServo()
{
}

float JawServo::getPotVADC()
{
    return analogRead(linearPotPin);
}

boolean JawServo::getMoving()
{
    if (((getPotVADC() - prevVADC) > 0 && (getPotVADC() - prevVADC) > TOLERANCE) || ((getPotVADC() - prevVADC) < 0 && (getPotVADC() - prevVADC) < -TOLERANCE))
    {
        return true;
    }
    return false;
}

boolean JawServo::openJaw()
{
    servo.writeMicroseconds(open);
    if (getPotVADC() <= openVADC || getMoving())
    {
        stopServo();
        return true;
    }
    return false;
}

boolean JawServo::closeJaw()
{
    servo.writeMicroseconds(close);
    if (getPotVADC() >= closedVADC || getMoving())
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
