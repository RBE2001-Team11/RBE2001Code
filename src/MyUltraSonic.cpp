#include <MyUltraSonic.h>

MyUltraSonic::MyUltraSonic()
{
}

/**
     * returns the distance in centimeters
     */
float MyUltraSonic::getDistanceCM()
{
    return 0.0;
    //return ultra.getDistanceCM();
}

/**
     * returns the distance in inches
     */
float MyUltraSonic::getDistanceIN()
{
    return 0.0;
    //return ultra.getDistanceCM() * (1 / CENTI_CONV);
}
