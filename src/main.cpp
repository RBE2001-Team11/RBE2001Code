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

// TODO enumerate
boolean side = false;
boolean isFirst = false;

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

boolean run(boolean side, boolean isFirst)
{

  return false;
}

void loop()
{

  // stop if button hit
  while (allowRun)
  {
    /* if (stop button hit)
     {
       running = false setEfforts(0)
     }*/

    while (/*side = NULL, and isFirst = NULL*/)
    {
      // TODO
      // getButtonPress()
      // side = buttonPressed;
      // isFirst = buttonPressed;
    }

    run(side, isFirst);
  }
}