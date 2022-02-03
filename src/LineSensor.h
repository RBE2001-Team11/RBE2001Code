#include <wpi-32u4-lib.h>

class LineSensor
{
private:
    //port for left sensor
    const int leftPort = A2; //TODO
    //port for right sensor
    const int rightPort = A0; //TODO
public:
    void attach();

    float getLeft();

    float getRight();

    float getDifference();
};