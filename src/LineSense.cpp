#include <wpi-32u4-lib.h>

class LineSense
{

private:
    //  ESP32AnalogRead rightSensor;

    //port for left sensor
    const int leftPort = A2; //TODO
    //port for right sensor
    const int rightPort = A0; //TODO

public:
    /**
    *  attaches the line sensor. Must call in main class
    * //TODO maybe dont need
    */
    void attach()
    {
        // leftSensor.attach(leftPort);
        // rightSensor.attach(rightPort);
    }

    /**
     * returns the left sensor voltage, less is more light
     */
    float getLeft()
    {
        return analogRead(leftPort);
        // return leftSensor.readVoltage();
    }

    /**
     * returns the right sensor voltage, less is more light
     */
    float getRight()
    {
        return analogRead(rightPort);
        //return rightSensor.readVoltage();
    }

    /**
     * returns the differance between the right and left sensors
     */
    float getDifference()
    {
        return getRight() - getLeft();
    }
};