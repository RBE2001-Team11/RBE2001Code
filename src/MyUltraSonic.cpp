#include <MyUltraSonic.h>

Rangefinder rangeFinder(2, 3);

MyUltraSonic::MyUltraSonic()
{
}

/**
 * returns the distance in centimeters
 */
float MyUltraSonic::getDistanceCM()
{
    return rangeFinder.getDistance();
}

/**
 * returns the distance in inches
 */
float MyUltraSonic::getDistanceIN()
{
    return rangeFinder.getDistance() * CENTI_CONV;
}
