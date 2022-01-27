//
#include <wpi-32u4-lib.h>
#include <RemoteConstants.h>
//#define PI 3.14159265358979323846

class MyDrive
{
private:
    Chassis chassis;

    LeftMotor left = chassis.leftMotor;
    RightMotor right = chassis.rightMotor;

    //constantssss ======================================
    const float WHEEL_DIAMETER = 2.75f;

    //centi to inch conversion
    const float CENTI_CONV = 2.54f;

    //Kp for the ultra moving
    const float ULTRA_PROP = .07f;

    //deadband for moving to a distance with ultra
    const float ULTRA_DEAD = .35f;

    //speed while using ultra
    const float ULTRA_DRIVE = 90.0f;

    //deadband for finding an object(bag) with thge ultra
    const float FIND_BAG_DEAD = 5.0f;

    //base follow line speed
    const float LINE_BASE_SPEED = .22 * 300;

    //Kp for following the line
    const float LINE_PROP = .2f;

    //voltage value for determining if a sensor is over the line
    const float LINE_SENSE_BLACK = 800;

    //max distance that the ultra will care about while scanning for a bag
    const float MAX_DIST = 25.0f;

    //speed to turn while scanning for object in degrees per second
    const float SCAN_SPEED = 270.0f;

    //degrees per second to move in teleop
    const float TELEOP_SPEED = 180.0f;

    //medium drive speed
    const float DRIVE_SPEED_MED = 180.0f;

    //fast drive speed
    const float DRIVE_SPEED_FAST = 270.0f;

    //slow drive speed
    const float DRIVE_SPEED_SLOW = 70.0f;

    //angle for preparing to align to the line with the light sensors
    const float PREP_ALIGN_ANGLE = 60.0f;

    const float CENTER_ROBOT_DIST = 2.0f;

    //turn speed in degrees per second medium
    const float TURN_SPEED_MED = 180.0f;

    //turn speed in degrees per second
    const float TURN_SPEED_FAST = 270.0f;

    //turn speed in degrees per second
    const float TURN_SPEED_SLOW = 70.0f;

    //turn left
    const int DIR_LEFT = -1;

    //turn right
    const int DIR_RIGHT = 1;

    //end constantsssss+++++++++++++++++++++++++++++++++++++++++++

public:
    void init()
    {
        chassis.setMotorPIDcoeffs(1.0, 0.5);
        chassis.init();
    }
    /**
    * turns a certain amount of degrees
    * @param degrees degrees to turn, negative to turn counter-clockwise
    * @param speed degrees per second to move
    * @return true when complete
    */
    boolean turn(float degrees, float speed)
    {

        chassis.turnFor(degrees, speed, true); //change block based on requirments
        //degrees for each wheel for move
        //   float moveDegrees = 2 * degrees;
        //move
        //   left.startMoveFor(moveDegrees, speed);
        //  right.moveFor(-moveDegrees, speed);

        return true;
    }

    /**
     * turn until not called
     * @param direct -1 for left, 1 for right
     * @param speed in cm/s
     */
    void turnContinuous(int direct, float speed)
    {
        if (direct <= 0)
        {
            chassis.setMotorEfforts(-speed, speed);
            //left.setMotorEffort(-effort);
            // right.setMotorEffort(effort);
        }
        else if (direct > 0)
        {
            chassis.setMotorEfforts(speed, -speed);
            //left.setMotorEffort(effort);
            //  right.setMotorEffort(-effort);
        }
    }

    /**
    * drive straight a certain amount of inches
    * @param inches inches to move, negative to go backwars
    * @param speed degrees per second to move
    * @return true when complete
    */
    boolean driveInches(float inches, float speed)
    {
        //TODO
        //degrees for each wheel to move
        float moveDegrees = (inches / (2 * PI * (WHEEL_DIAMETER / 2))) * 360;

        chassis.driveFor(inches, speed, true);
        //move
        // left.startMoveFor(moveDegrees, speed);
        // right.moveFor(moveDegrees, speed);

        return true;
    }

    /**
    * drive straight a certain amount of inches
    * @param centi centimeters to move, negative to go backwars
    * @param speed degrees per second to move
    * @return true when complete
    */
    boolean driveCentimeters(float centi, float speed)
    {
        //degrees for each wheel to move
        float moveDegrees = (centi / (2 * PI * ((WHEEL_DIAMETER / CENTI_CONV) / 2))) * 360;

        chassis.driveFor(centi / CENTI_CONV, speed, true);
        //move
        //left.startMoveFor(moveDegrees, speed);
        // right.moveFor(moveDegrees, speed);

        return true;
    }

    /**
    * drive based on effort
    * @param effort -1  to 1
    */
    void setEffort(float effort)
    {

        chassis.setMotorEfforts(effort, effort);
        //  left.setEffort(effort);
        //  right.setEffort(effort);
    }

    /**
    * drive based on effort
    * @param speed in degrees per second, negative goes backwards 
    */
    void setSpeed(float speed)
    {

        chassis.setMotorEfforts(speed, speed);
        //left.setSpeed(speed);
        // right.setSpeed(speed);
    }

    /**
    * drives to a set distance away from a target using the ultrasonic
    * @param targetDist distance to move to
    * @param curDist the ultrasonic same unit as targetDist
    * @return true when at proper distance
    */
    boolean driveTo(float targetDist, float curDist)
    {
        //if the robot is at the distance within a deadband
        if (curDist > targetDist - ULTRA_DEAD && curDist < targetDist + ULTRA_DEAD)
        {
            //stop
            setEffort(0);
            return true;
        }

        //move the robot to right distance
        setSpeed(ULTRA_DRIVE);

        return false;
    }

    /**
     * follows the black line using p control
     * @param error the currect difference between the two line sensors getDifference()
     * @param leftSense the current value of left Sensor 
     * @param rightSense current value of the right sensor
     */
    void followLine(float error, float leftSense, float rightSense)
    {
        float leftEffort = LINE_BASE_SPEED + (error * LINE_PROP);
        float rightEffort = LINE_BASE_SPEED - (error * LINE_PROP);

        chassis.setMotorEfforts(leftEffort, rightEffort);
        // left.setEffort(LINE_BASE_SPEED + (error * LINE_PROP));
        //right.setEffort(LINE_BASE_SPEED - (error * LINE_PROP));
    }

    /**
     * drive straight forward until find a line
     * @param speed the speed in degrees per second
     * @param leftSense the current value of left Sensor
     * @param rightSense current value of the right sensor
     * @return true when hits an line
     */
    boolean driveTillLine(float speed, float leftSense, float rightSense)
    {
        //if either light sensors og above the dark value
        if (leftSense > LINE_SENSE_BLACK || rightSense > LINE_SENSE_BLACK /*&& error < lineFollowTurnDead*/)
        {
            return true;
        }
        //keep driving
        setSpeed(speed);
        return false;
    }

    /**
     * line follow until find a t intersection
     * @param speed the speed in degrees per second
     * @param leftSense the current value of left Sensor
     * @param rightSense current value of the right sensor
     * @return true when hits an line
     */
    boolean lineFollowTillLine(float leftSense, float rightSense, float error)
    {
        //if either light sensors og above the dark value
        if (leftSense > LINE_SENSE_BLACK && rightSense > LINE_SENSE_BLACK /*&& error < lineFollowTurnDead*/)
        {
            return true;
        }
        //keep following line
        followLine(error, leftSense, rightSense);
        return false;
    }

    /**
     * line follow until ultra reaches target distance, only goes forward
     * @param leftSense the current value of left Sensor
     * @param rightSense current value of the right sensor
     * @param error difference between left and right sensors
     * @param curDist current ultrasonic distance 
     * @param targetDist target distance, same unit as curDist
     * @return true when at target distance
     */
    boolean lineFollowToTargetDistance(float leftSense, float rightSense, float error, float curDist, float targetDist)
    {
        //if not in target distance
        if (curDist <= targetDist)
        {
            return true;
        }
        followLine(error, leftSense, rightSense);
        return false;
    }

    /**
     * turn until find a line
     * 
    * @param direct -1 for left, 1 for right
    * @param leftSense the current value of left Sensor
    * @param rightSense current value of the right sensor
    * @return true found a line
    */
    boolean alignToLine(int direct, float leftSense, float rightSense)
    {
        //turn left
        if (direct < 0)
        {
            //right light sensor found line
            if (rightSense > LINE_SENSE_BLACK)
            {
                chassis.setMotorEfforts(0, 0);
                // left.setSpeed(0);
                // right.setSpeed(0);
                return true;
            }
        }
        //turn right
        else if (direct >= 0)
        {
            //left line sensor found line

            if (leftSense > LINE_SENSE_BLACK)
            {
                chassis.setMotorEfforts(0, 0);
                return true;
            }
        }
        //keep turning
        turnContinuous(direct, TURN_SPEED_MED);
        return false;
    }
};
