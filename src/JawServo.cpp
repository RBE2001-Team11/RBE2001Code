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

    while (getPotVADC() > openVADC)
    {
    }
}

boolean JawServo::closeJaw()
{
    while (getPotVADC() < closedVADC)
    {
    }
}
