#include <Arduino.h>
#include <wpi-32u4-lib.h>
#include <MyDrive.h>
#include <LineSensor.h>

Chassis chassis;

MyDrive drive;

LineSensor lSense;

void setup()
{

  chassis.init();
  // put your setup code here, to run once:
}

void loop()
{
  drive.followLine(lSense.getDifference());
}