#include <Arduino.h>
#include <wpi-32u4-lib.h>
#include <MyDrive.h>
#include <LineSensor.h>

Chassis chassis;

MyDrive drive;

LineSensor lSense;

// START sensor value variables
float leftSense;
float rightSense;
float error;

// ultrasonic
float curDistIN;
float curDistCM;

boolean allowRun = true;

void setup()
{

  chassis.init();
  // put your setup code here, to run once:
}

void updateValues()
{
  leftSense = lSense.getLeft();
  rightSense = lSense.getRight();
  error = lSense.getDifference();

  // ultrasonic
  // curDistIN = ultra.getDistanceIN();
  // curDistCM = ultra.getDistanceCM();
}

boolean run(boolean side)
{

  return false;
}

void loop()
{

  while (allowRun == true)
  {
    if (run(true) == true)
    {
      allowRun = false;
    }
  }

  drive.followLine(lSense.getDifference());
}