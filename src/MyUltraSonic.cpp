#include <MyUltraSonic.h>

Rangefinder rangefinder(17, 3);

MyUltraSonic::MyUltraSonic()
{
}

void MyUltraSonic::initI()
{
    rangefinder.init();
}

/**
 * returns the distance in centimeters
 */
float MyUltraSonic::getDistanceCM()
{
    return rangefinder.getDistance();
}

/**
 * returns the distance in inches
 */
float MyUltraSonic::getDistanceIN()
{
    return rangefinder.getDistance() * CENTI_CONV;
}
