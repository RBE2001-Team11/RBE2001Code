
#pragma once
#include <wpi-32u4-lib.h>
#include <RemoteConstants.h>
//#define PI 3.14159265358979323846

#ifndef MYDRIVE_H
#define MYDRIVE_H

class MyDrive
{
private:
    // constantssss ======================================
    const float WHEEL_DIAMETER = 2.75f;

    // centi to inch conversion
    const float CENTI_CONV = 2.54f;

    // Kp for the ultra moving
    const float ULTRA_PROP = .07f;

    // deadband for moving to a distance with ultra
    const float ULTRA_DEAD = .35f;

    // speed while using ultra
    const float ULTRA_DRIVE = 90.0f;

    // deadband for finding an object(bag) with thge ultra
    const float FIND_BAG_DEAD = 5.0f;

    // base follow line speed
    const float LINE_BASE_SPEED = 75;

    // Kp for following the line
    const float LINE_PROP = .09f;

    // voltage value for determining if a sensor is over the line
    const float LINE_SENSE_BLACK = 700;

    // max distance that the ultra will care about while scanning for a bag
    const float MAX_DIST = 25.0f;

        const float DIST_FROM_ROOF = 0.0; // TODO
    const float FINAL_ROOF_DRIVE = 0.0;

    enum MovePanelState
    {
        INIT_TURN,
        GO_ROOF,
        MOVE_PANEL
    };

    MovePanelState movePanelState = INIT_TURN;

    // end constantsssss+++++++++++++++++++++++++++++++++++++++++++

public:
    MyDrive();

    boolean turn(float degrees, float speed);
    void turnContinuous(int direct, float speed);
    boolean driveInches(float inches, float speed);
    boolean driveCentimeters(float centi, float speed);
    void setEffort(float effort);
    void setSpeed(float speed);
    boolean driveTo(float targetDist, float curDist);
    void followLine(float error);
    boolean driveTillLine(float speed, float leftSense, float rightSense);
    boolean lineFollowTillLine(float leftSense, float rightSense, float error);
    boolean lineFollowToTargetDistance(float leftSense, float rightSense, float error, float curDist, float targetDist);
    boolean alignToLine(int direct, float leftSense, float rightSense);

    boolean movePanelPickUp(boolean side, float curDist);
    boolean crossSide(boolean side);

    // medium drive speed
    const float DRIVE_SPEED_MED = 180.0f;

    // fast drive speed
    const float DRIVE_SPEED_FAST = 270.0f;

    // slow drive speed
    const float DRIVE_SPEED_SLOW = 70.0f;

    // angle for preparing to align to the line with the light sensors
    const float PREP_ALIGN_ANGLE = 60.0f;

    const float CENTER_ROBOT_DIST = 2.0f;

    // turn speed in degrees per second medium
    const float TURN_SPEED_MED = 180.0f;

    // turn speed in degrees per second
    const float TURN_SPEED_FAST = 270.0f;

    // turn speed in degrees per second
    const float TURN_SPEED_SLOW = 70.0f;

    // turn left
    const int DIR_LEFT = -1;

    // turn right
    const int DIR_RIGHT = 1;
};
#endif