#include <wpi-32u4-lib.h>

#ifndef LINESENSE_H
#define LINESENSE_H
class LineSensor
{
private:
    //port for left sensor
    const int leftPort = A3; //TODO
    //port for right sensor
    const int rightPort = A4; //TODO
public:
    LineSensor();
    void attach();

    float getLeft();

    float getRight();

    float getDifference();
};
#endif