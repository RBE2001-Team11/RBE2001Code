#include <LineSensor.h>

LineSensor::LineSensor()
{
}

/**
 * returns the left sensor voltage, less is more light
 */
float LineSensor::getLeft()
{
    return analogRead(leftPort);
    // return leftSensor.readVoltage();
}

/**
 * returns the right sensor voltage, less is more light
 */
float LineSensor::getRight()
{
    return analogRead(rightPort);
    // return rightSensor.readVoltage();
}

/**
 * returns the differance between the right and left sensors
 */
float LineSensor::getDifference()
{
    return getRight() - getLeft();
}
