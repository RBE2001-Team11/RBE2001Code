#include <Arduino.h>
#include <wpi-32u4-lib.h>
#include <RemoteConstants.h>
#include <IRdecoder.h>
#include <MyDrive.h>
#include <LineSensor.h>
#include <BlueMotor.h>
#include <MyUltraSonic.h>
#include <JawServo.h>

// start Object declarations ++++++++++++++++++++++++++++++++++++++++++++++++
Chassis chassis;

MyDrive drive;

LineSensor lSense;

BlueMotor motor;

IRDecoder decoder(15);

MyUltraSonic ultra;

JawServo servo;

// end Object declarations ===================================================================

// START sensor value variables +++++++++++++++++++++++++++++++++++++++++++++

uint16_t keyPress;
float leftSense;
float rightSense;
float error;

// ultrasonic
float curDistIN;
float curDistCM;

// end sensor values ===================================================================

// Start function variables +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++==
boolean allowRun = true;

// TODO enumerate
boolean side = false;
boolean isFirst = false;

// end function variables ===================================================================

// START constants +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const uint16_t PICKUP_BUTTON = remoteUp; // TODO
const uint16_t STOP_BUTTON = remotePlayPause;
// TODO
const uint16_t S_RIGHT_FIRST = remote1;
const uint16_t S_RIGHT_SECOND = remote2;
const uint16_t S_LEFT_FIRST = remote3;
const uint16_t S_LEFT_SECOND = remote4;

// end constants ===================================================================

// START enums +++++++++++++++++++++++++++++++++++++++++++++++++++++

// TODO check sides
/**
 * @brief side of the field
 * Right = 25 degrees
 * Left = 45 degrees
 *
 */
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

// end enums ===================================================================

/**
 * @brief setup code for initilizing stuffs
 *
 */
void setup()
{

  chassis.init();
  motor.setup();
  decoder.init();
  Serial.begin(9600);
}

/**
 * @brief updates sensor values to local variables, should be constantly called in the loop() function
 *
 */
void updateValues()
{

  keyPress = decoder.getKeyCode();
  leftSense = lSense.getLeft();
  rightSense = lSense.getRight();
  error = lSense.getDifference();

  // ultrasonic
  curDistIN = ultra.getDistanceIN();
  curDistCM = ultra.getDistanceCM();
}

/**
 * @brief starting with the gripper opon on the panel pick up the panel
 * and drive to the intersection
 *
 * @param RobotSide
 * @return true if ran throught the state machine
 */
boolean pickUpPanelRoof(RobotSide s)
{
  switch (pickRoofState)
  {
  case WAIT:
    // flash the led //TODO
    if (keyPress == PICKUP_BUTTON) // TODO
    {
      pickRoofState = CLOSE_GRIP;
    }
    break;
  case CLOSE_GRIP:
    if (servo.closeJaw())
    {
      pickRoofState = REMOVE_PANEL;
    }
    break;
  case REMOVE_PANEL:
    if (s = RIGHT)
    {
      if (motor.moveTo(motor.Side25Prep))
      {
        pickRoofState = BACKUP;
      }
    }
    else
    {
      if (motor.moveTo(motor.Side45Prep))
      {
        pickRoofState = BACKUP;
      }
    }
    break;
  case BACKUP:
    if (drive.driveInches(-4, drive.DRIVE_SPEED_MED))
    {
      pickRoofState = TURN_AROUND;
    }
    break;
  case TURN_AROUND:
    if (drive.turn(180, drive.TURN_SPEED_MED))
    {
      pickRoofState = DRIVE_INTER;
    }
    break;
  case DRIVE_INTER:
    if (drive.driveTillLine(drive.DRIVE_SPEED_MED, leftSense, rightSense))
    {
      pickRoofState = WAIT;
      return true;
    }
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
  switch (keyPress)
  {
  case S_RIGHT_FIRST:
    side = RIGHT;
    robotRun = FIRST_ROBOT;
    allowRun = true;
    break;
  case S_RIGHT_SECOND:
    side = RIGHT;
    robotRun = SECOND_ROBOT;
    allowRun = true;
    break;
  case S_LEFT_FIRST:
    side = LEFT;
    robotRun = FIRST_ROBOT;
    allowRun = true;
    break;
  case S_LEFT_SECOND:
    side = LEFT;
    robotRun = SECOND_ROBOT;
    allowRun = true;
    break;
  }
}

boolean didTheThing = false;

void loop()
{

  updateValues();

  // motor.moveTo(0);

  // stop if button hit
  // while (allowRun == true)
  // {
  //   if (keyPress = STOP_BUTTON) // TODO
  //   {
  //     allowRun = false;
  //     drive.setEffort(0);
  //     motor.setEffort(0);
  //   }

  //   if (run())
  //   {
  //     allowRun = false;
  //   }
  // }

  // if (allowRun == false)
  // {
  //   startRobot();
  // }
}