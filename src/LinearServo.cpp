#include <LinearServo.h>

LinearServo::LinearServo()
{
}

// Get feedback position
float LinearServo::getPotVADC()
{
    return analogRead(linearPotPin);
}

// Get whether the Servo is moving
boolean LinearServo::getMovingDown()
{
    // Serial.print(prevVADC);
    // Serial.print(" ");
    //  Serial.println(getPotVADC());

    // compares current pot value to prior value
    // if it moves less than the tolerance, then it isn't moving
    if (((getPotVADC() - prevVADC) > 0 && (getPotVADC() - prevVADC) > TOLERANCE_DOWN) || ((getPotVADC() - prevVADC) < 0 && (getPotVADC() - prevVADC) < -TOLERANCE_DOWN))
    {
        return true;
    }
    return false;
}

// Get whether the Servo is moving
boolean LinearServo::getMovingUp()
{
    // Serial.print(prevVADC);
    //  Serial.print(" ");
    // Serial.println(getPotVADC());

    // compares current pot value to prior value
    // if it moves less than the tolerance, then it isn't moving
    if (((getPotVADC() - prevVADC) > 0 && (getPotVADC() - prevVADC) > TOLERANCE_UP) || ((getPotVADC() - prevVADC) < 0 && (getPotVADC() - prevVADC) < -TOLERANCE_UP))
    {
        return true;
    }
    return false;
}

// Open Jaw - Stop opening if not moving
boolean LinearServo::openJaw()
{
    prevVADC = getPotVADC();
    servo.writeMicroseconds(jawDown);
    // If you hit target
    if (getPotVADC() <= jawOpenPotVoltageADC)
    {
        stop();
        return true;
    }
    delay(1000);

    // If you aren't moving, open
    if (!getMovingUp())
    {
        servo.writeMicroseconds(jawUp);
        delay(1000);
        return false;
    }
    return false;
}

// Close Jaw - Stop closing if not moving
boolean LinearServo::closeJaw()
{
    prevVADC = getPotVADC();
    servo.writeMicroseconds(jawUp);

    // Hit target
    if (getPotVADC() >= jawClosedPotVoltageADC)
    {
        stop();
        return true;
    }
    delay(1000);

    // Not moving but should be, reset
    if (!getMovingDown())
    {
        servo.writeMicroseconds(jawUp);
        delay(1000);
        return false;
    }
    return false;
}

// Stop
void LinearServo::stop()
{
    servo.writeMicroseconds(servoStop);
}