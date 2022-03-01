#include <Arduino.h>
#include <wpi-32u4-lib.h>
#include <RemoteConstants.h>
#include <IRdecoder.h>
#include <MyDrive.h>
#include <LineSensor.h>
#include <BlueMotor.h>
#include <MyUltraSonic.h>
#include <JawServo.h>
#include <Rangefinder.h>

// start Object declarations ++++++++++++++++++++++++++++++++++++++++++++++++
Chassis chassis;

MyDrive drive;

LineSensor lSense;

BlueMotor motor;

IRDecoder decoder(14);

MyUltraSonic ultra;

JawServo servo;

Romi32U4ButtonB buttonB;

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
boolean allowRun =false;

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
  LEFT,
  RIGHT,
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

enum PlacePlatState
{
  CENTER,
  TURN_1,
  MOVE_PLAT,
  MOVE_GRIP,
  OPEN_JAW
};

PlacePlatState placePlatState = CENTER;

enum PickPlatState
{
  WAIT_AND_CLOSE_PP,
  CLOSE_GRIP_PP,
  BACK_UP_PP,
  TURN_AROUND_PP,
  MOVE_TO_SECT_PP,
  CENTER_ROBOT_PP,
  TURN_ROOF_PP,
  MOVE_START_PP,
  FINISH_GRIP_MOVE_PP
};
PickPlatState pickPlatState = WAIT_AND_CLOSE_PP;

enum PlaceRoofState
{
  PREP_PANEL_PR,
  PLACE_PANEL_PR,
  RELEASE_PR,
  BACKUP_PR,
  TURN_PR
};

PlaceRoofState placeRoofState = PREP_PANEL_PR;

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
  ultra.initI();
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
    pickRoofState = CLOSE_GRIP;    // TODO
    if (keyPress == PICKUP_BUTTON) // TODO
    {
      pickRoofState = CLOSE_GRIP;
    }
    break;
  case CLOSE_GRIP:
    if (servo.closeJaw())
    {
      pickRoofState = REMOVE_PANEL;
      if(s == RIGHT){
        motor.setCount(motor.Side25Place);
      } else {
        motor.setCount(motor.Side45Place);
      }
      
    }
    break;
  case REMOVE_PANEL:
    if (s == RIGHT) {
      // motor.setCount(motor.Side25Place);
      if (motor.moveTo(motor.Side25Prep)) {
        pickRoofState = BACKUP;
      }
    } else {
      if (motor.moveTo(motor.Side45Prep)) {
        pickRoofState = BACKUP;
      }
    }
    break;
  case BACKUP:
    if (drive.driveInches(-3.5, 5))
    {
      pickRoofState = TURN_AROUND;
    }
    break;
  case TURN_AROUND:
    if (drive.turn(180, drive.TURN_SPEED_SLOW))
    {
      pickRoofState = DRIVE_INTER;
    }
    break;
  case DRIVE_INTER:
    if (drive.lineFollowTillLine(leftSense, rightSense, error))
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
  switch (placePlatState)
  {
  case CENTER:
    if (drive.driveInches(drive.CENTER_ROBOT_DIST, drive.DRIVE_SPEED_MED))
    {
      placePlatState = TURN_1;
    }
    break;
  case TURN_1:
    if (s == RIGHT)
    {
      if (drive.alignToLine(-1, leftSense,rightSense))
      {
        placePlatState = MOVE_PLAT;
      }
    }
    else
    {
      if(drive.alignToLine(1, leftSense,rightSense)){
        placePlatState = MOVE_PLAT;
      }
      // if (drive.turn(-90, drive.TURN_SPEED_MED))
      // {
      //   placePlatState = MOVE_PLAT;
      // }
    }
    break;
  case MOVE_PLAT:
  //Serial.println("moving plat");
    if (drive.lineFollowToTargetDistance(leftSense, rightSense, error, curDistIN, 9)) // TODO make constant once tested
    {
      placePlatState = MOVE_GRIP;
    }
    break;
  case MOVE_GRIP:
    if (motor.moveTo(motor.platPlace))
    {
      placePlatState = OPEN_JAW;
    }
    break;
  case OPEN_JAW:
    if (servo.openJaw())
    {
      placePlatState = CENTER;
      return true;
    }
    break;
  }
  return false;
}

boolean pickUpPlat(RobotSide s)
{
  switch (pickPlatState)
  {
  case WAIT_AND_CLOSE_PP:
    // TODO LED
    if (keyPress == PICKUP_BUTTON) // TODO
    {
      pickPlatState = CLOSE_GRIP_PP;
    }
    break;
  case CLOSE_GRIP_PP:
    if (servo.closeJaw())
    {
      pickPlatState = BACK_UP_PP;
    }
    break;
  case BACK_UP_PP:
    if (drive.driveInches(-4, drive.DRIVE_SPEED_MED))
    {
      pickPlatState = TURN_AROUND_PP;
    }
    break;
  case TURN_AROUND_PP:
    if (drive.turn(180, drive.TURN_SPEED_MED))
    {
      pickPlatState = MOVE_TO_SECT_PP;
    }
    break;

  case MOVE_TO_SECT_PP:
    if (drive.lineFollowTillLine(leftSense, rightSense, error))
    {
      pickPlatState = CENTER_ROBOT_PP;
    }
    break;
  case CENTER_ROBOT_PP:
    if (drive.driveInches(drive.CENTER_ROBOT_DIST, drive.DRIVE_SPEED_MED))
    {
      pickPlatState = TURN_ROOF_PP;
    }

    break;
  case TURN_ROOF_PP:
    if (s == RIGHT)
    {
      if (drive.turn(90, drive.TURN_SPEED_MED))
      {
        placePlatState = MOVE_PLAT;
      }
    }
    else
    {
      if (drive.turn(-90, drive.TURN_SPEED_MED))
      {
        placePlatState = MOVE_PLAT;
      }
    }
    break;
  case MOVE_START_PP:
    if (s == RIGHT)
    {
      motor.holdTo(motor.Side45Prep);
    }
    else
    {
      motor.holdTo(motor.Side25Prep);
    }
    if (drive.lineFollowToTargetDistance(leftSense, rightSense, error, curDistIN, 3))
    // TODO distance
    {
      pickPlatState = FINISH_GRIP_MOVE_PP;
    }
    break;
  case FINISH_GRIP_MOVE_PP:

    if (s == RIGHT)
    {
      if (motor.moveTo(motor.Side45Prep))
      {
        pickPlatState = WAIT_AND_CLOSE_PP;
        return true;
      }
    }
    else
    {
      if (motor.moveTo(motor.Side25Prep))
      {
        pickPlatState = WAIT_AND_CLOSE_PP;
        return true;
      }
    }
    break;
  }
  return false;
}

boolean placeRoof(RobotSide s)
{

  switch (placeRoofState)
  {
  case PREP_PANEL_PR:
    if (s == RIGHT)
    {
      if (motor.moveTo(motor.Side25Prep))
      {
        placeRoofState = PLACE_PANEL_PR;
      }
    }
    else
    {
      if (motor.moveTo(motor.Side45Prep))
      {
        placeRoofState = PLACE_PANEL_PR;
      }
    }
    break;
  case PLACE_PANEL_PR:
    if (s == RIGHT)
    {
      if (motor.moveTo(motor.Side25Place))
      {
        placeRoofState = RELEASE_PR;
      }
    }
    else
    {
      if (motor.moveTo(motor.Side45Place))
      {
        placeRoofState = RELEASE_PR;
      }
    }
    break;
  case RELEASE_PR:
    if (servo.openJaw())
    {
      placeRoofState = RELEASE_PR;
    }
    break;
  case BACKUP_PR:

    if (drive.driveInches(-4, drive.DRIVE_SPEED_MED))

    {
      placeRoofState = TURN_PR;
    }
    break;
  case TURN_PR:
    if (s == RIGHT)
    {
      if (drive.turn(90, drive.TURN_SPEED_MED))
      {
        placeRoofState = PREP_PANEL_PR;
      }
    }
    else
    {
      if (drive.turn(-90, drive.TURN_SPEED_MED))
      {
        placeRoofState = PREP_PANEL_PR;
      }
    }
    return true;
    break;
  }
  return false;
}

boolean run()
{

  // starting near the intersection facing the roof
  switch (runState)
  {
  case HOW_PICKUP:
    // return to intersection facing away from roof
    runState = PICKUP_ROOF;
    // if (robotRun == FIRST_ROBOT)
    // {
    //   runState = PICKUP_ROOF;
    // }
    // else
    // {
    //   runState = MOVE_ROOF;
    // }
    break;
  case MOVE_ROOF:
    if (drive.movePanelPickUp(robotSide, ultra.getDistanceIN(), leftSense, rightSense, error))
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
    if (drive.crossSide(robotSide, leftSense, rightSense, error))
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

void testLoop()
{
  motor.moveTo(-500);
}

boolean didTheThing = false;

boolean startYes = false;

void loop()
{
  updateValues();
 // Serial.println(curDistCM);
  // drive.movePanelPickUp(1, curDistIN, leftSense, rightSense, error);
  // Serial.println(decoder.getCode());
  // delay(100);
  // 4261527296 play/pause
  // 4194680576 button below
  // 4127833856

  // if (buttonA.isPressed())
  // {
  //   Serial.println("down");
  //   motor.setEffort(-200);
  // }
  // if (buttonB.isPressed())
  // {
  //   Serial.println("up");
  //   motor.setEffort(200);
  // }

  // if (buttonC.isPressed())
  // {
  //   Serial.println("stop");
  //   motor.setEffort(0);
  // }
  // motor.moveTo(-9500);
  // Serial.println(motor.getPosition());
  //  pickUpPanelRoof(RIGHT);


  robotRun = FIRST_ROBOT;
  side = LEFT;

  if(buttonB.isPressed()){
    startYes = true;
  }

  if(startYes){
    if (didTheThing == false && pickUpPanelRoof(LEFT)){
      didTheThing = true;
    }

    if (didTheThing == true) {
      placePlat(RIGHT);
      allowRun = false;
    }
  }

  // run();
  

  while (allowRun == true)
  {
    if (keyPress == STOP_BUTTON) // TODO
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

  // if (allowRun == false)
  // {
  //   startRobot();
  // }
  
}