#include <JawServo.h>

JawServo::JawServo()
{
}

// Get feedback position
float JawServo::getPotVADC()
{
    return analogRead(linearPotPin);
}

// Get whether the Servo is moving
boolean JawServo::getMoving()
{
    // Serial.print(prevVADC);
    // Serial.print(" ");
    // Serial.println(getPotVADC());

    // compares current pot value to prior value
    // if it moves less than the tolerance, then it isn't moving
    if (((getPotVADC() - prevVADC) > 0 && (getPotVADC() - prevVADC) > TOLERANCE) || ((getPotVADC() - prevVADC) < 0 && (getPotVADC() - prevVADC) < -TOLERANCE))
    {
        return true;
    }
    return false;
}

// Open Jaw - Stop opening if not moving
boolean JawServo::openJaw()
{
    prevVADC = getPotVADC();
    servo.writeMicroseconds(open);
    delay(25);
    // If you hit target
    if (getPotVADC() <= openVADC)
    {
        return true;
    }

    // If you aren't moving, open
    if (!getMoving())
    {
        //  Serial.println("here");
        stopServoOpen();
        delay(200);
        return false;
    }
    return false;
}

// Close Jaw - Stop closing if not moving
boolean JawServo::closeJaw()
{
    prevVADC = getPotVADC();
    servo.writeMicroseconds(close);
    delay(25);

    // Hit target
    if (getPotVADC() >= closedVADC)
    {
        return true;
    }

    // Not moving but should be, reset
    if (!getMoving())
    {
        stopServoClose();
        delay(200);
        return false;
    }
    return false;
}

// Close no feedback
void JawServo::closeNoPot()
{
    servo.writeMicroseconds(close);
    // Serial.println(getPotVADC());
}

// Open no feedback
void JawServo::openNoPot()
{
    servo.writeMicroseconds(open);
}

// Stop
void JawServo::stopServoOpen()
{
    servo.writeMicroseconds(stopOpen);
}

// Stop
void JawServo::stopServoClose()
{
    servo.writeMicroseconds(stopClose);
}