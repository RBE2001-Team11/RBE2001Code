#include <Arduino.h>
#include <wpi-32u4-lib.h>
#include <RemoteConstants.h>
#include <IRdecoder.h>
#include <MyDrive.h>
#include <LineSensor.h>
#include <BlueMotor.h>
#include <MyUltraSonic.h>
#include <JawServo.h>

Chassis chassis;

MyDrive drive;

LineSensor lSense;

BlueMotor motor;

IRDecoder decoder(15);

MyUltraSonic ultra;

JawServo servo;

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

enum PickRoofState
{
  WAIT,
  CLOSE_GRIP,
  REMOVE_PANEL,
  BACKUP,
  TURN_AROUND,
  DRIVE_INTER

};

PickRoofState pickRoofState = WAIT;

void setup()
{

  chassis.init();
  motor.setup();
  decoder.init();
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
  switch (pickRoofState)
  {
  case WAIT:
    // flash the led //TODO
    if (decoder.getKeyCode(2)) // TODO
    {
      pickRoofState = CLOSE_GRIP;
    }
    break;
  case CLOSE_GRIP:
    // closeGripper();
    break;
  case REMOVE_PANEL:
    // 1.move the four-bar
    break;
  case BACKUP:
    // 1. back away from roof
    break;
  case TURN_AROUND:
    // 1.turn 180 degrees
    break;
  case DRIVE_INTER:
    //  1. follow line until intersection
    return true;
    break;
  }
  return false;
}

boolean placePlat(RobotSide s)
{
  return false;
}

boolean pickUpPlat(RobotSide s)
{
  return false;
}

boolean placeRoof(RobotSide s)
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
    if (drive.movePanelPickUp(robotSide, ultra.getDistanceIN()))
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
    if (placePlat(robotSide))
    {
      runState = PICKUP_PLAT;
    }

    break;
  case PICKUP_PLAT:
    if (pickUpPlat(robotSide))
    {
      runState = PLACE_ROOF;
    }
    break;
  case PLACE_ROOF:
    if (placeRoof(robotSide))
    {
      runState = CROSS_SIDE;
    }
  case CROSS_SIDE:
    if (drive.crossSide(robotSide))
    {
      runState = HOW_PICKUP;
      return true;
    }
    break;
  }

  return false;
}

void startRobot()
{
  switch (decoder.getKeyCode())
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
    if (decoder.getKeyCode(0)) // TODO
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