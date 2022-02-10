#include <Arduino.h>
#include <wpi-32u4-lib.h>
#include <MyDrive.h>
#include <LineSensor.h>
#include <BlueMotor.h>

#include <ButtonHandler.cpp>

Chassis chassis;

MyDrive drive;

LineSensor lSense;

BlueMotor motor;

ButtonHandler buttons;

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

enum RobotSide
{
  RIGHT,
  LEFT,
  BRUH_WHY_CANT_I_SET_ENUM_TO_NULL_THE_FIRST
};

RobotSide robotSide = BRUH_WHY_CANT_I_SET_ENUM_TO_NULL_THE_FIRST;

enum RobotRun
{
  FIRST_ROBOT,
  SECOND_ROBOT,
  BRUH_WHY_CANT_I_SET_ENUM_TO_NULL
};

RobotRun robotRun = BRUH_WHY_CANT_I_SET_ENUM_TO_NULL;

enum RunState
{
  HOW_PICKUP,
  PICKUP_ROOF,
  MOVE_ROOF,
  PLACE_PLAT,
  PICKUP_PLAT,
  PLACE_ROOF,
  CROSS_SIDE

};

RunState runState = HOW_PICKUP;

void setup()
{

  chassis.init();
  motor.setup();
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

boolean pickUpPanelRoof(RobotSide s)
{
  return false;
}

boolean run()
{

  // starting near the intersection facing the roof
  switch (runState)
  {
  case HOW_PICKUP:
    // return to intersection facing away from roof
    if (robotRun == FIRST_ROBOT)
    {
      runState = PICKUP_ROOF;
    }
    else
    {
      runState = MOVE_ROOF;
    }
    break;
  case MOVE_ROOF:
    if (drive.movePanelPickUp(robotSide))
    {
      runState = PICKUP_ROOF;
    }
    break;
  case PICKUP_ROOF:
    if (pickUpPanelRoof(robotSide))
    {
      runState = PLACE_PLAT;
    }
    break;
  case PLACE_PLAT:

    break;
  case PICKUP_PLAT:
    // placePanelRoof(side);
    break;
  case PLACE_ROOF:
    // end in starting position
    //  driveToOtherSide(side);
    break;
  case CROSS_SIDE:
    break;
  }

  return false;
}

void startRobot()
{
  switch (buttons.getStartButton())
  {
  case 1:
    side = RIGHT;
    robotRun = FIRST_ROBOT;
    allowRun = true;
    break;
  case 2:
    side = RIGHT;
    robotRun = SECOND_ROBOT;
    allowRun = true;
    break;
  case 3:
    side = LEFT;
    robotRun = FIRST_ROBOT;
    allowRun = true;
    break;
  case 4:
    side = LEFT;
    robotRun = SECOND_ROBOT;
    allowRun = true;
    break;
  }
}

boolean didTheThing = false;

void loop()
{
  // stop if button hit
  while (allowRun == true)
  {
    if (buttons.getStopButton())
    {
      allowRun = false;
      drive.setEffort(0);
      motor.setEffort(0);
    }

    if (run())
    {
      allowRun = false;
    }
  }

  if (allowRun == false)
  {
    startRobot();
  }
}