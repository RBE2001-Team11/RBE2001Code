#include <Arduino.h>
#include <wpi-32u4-lib.h>
#include <Hardware.h>

Hardware robot;

void setup()
{

  robot.init();
  // put your setup code here, to run once:
}

void loop()
{
  robot.drive.setEffort(100);
}