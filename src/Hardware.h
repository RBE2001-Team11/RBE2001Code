#include <wpi-32u4-lib.h>
#include <MyDrive.h>
#include <LineSensor.h>
#include <BlueMotor.h>

class Hardware
{
private:
public:
    MyDrive drive;
    LineSensor lSensor;
    BlueMotor bMotor;
    void init();
};