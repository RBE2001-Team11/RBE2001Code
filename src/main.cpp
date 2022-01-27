#include <Arduino.h>
#include <wpi-32u4-lib.h>
#include <MyDrive.cpp>
//#include <MyDrive.cpp>
#include <LineSense.cpp>

MyDrive drive;
LineSense sensor;
Chassis chassis;

void setup()
{

  //chassis.init();
  drive.init();
  Serial.begin(9600);
  // put your setup code here, to run once:
}

void loop()
{
  //Serial.println(sensor.getLeft());
  //chassis.setMotorEfforts(100, 100);
  //Serial.println(sensor.getDifference());
  if (drive.lineFollowTillLine(300, sensor.getLeft(), sensor.getRight()))
  {
    //drive.driveInches(3, 300);
    // drive.turnContinuous();
    drive.alignToLine(1, sensor.getLeft(), sensor.getRight());
  }

  //drive.followLine(sensor.getDifference(), sensor.getLeft(), sensor.getRight());
}