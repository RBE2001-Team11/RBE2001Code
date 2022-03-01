#include <wpi-32u4-lib.h>
#include <Rangefinder.h>

#ifndef MYULTRA_H
#define MYULTRA_H

class MyUltraSonic
{
private:
    // conversion from cm to in
    const float CENTI_CONV = 2.54f;

public:
    MyUltraSonic();
    /**
     * returns the distance in centimeters
     */
    float getDistanceCM();

    /**
     * returns the distance in inches
     */
    float getDistanceIN();

    void initI();
};
#endif