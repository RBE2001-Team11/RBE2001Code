#pragma once

#ifndef BLUEMOTOR_H
#define BLUEMOTOR_H
class BlueMotor
{
public:
    BlueMotor();
    void setEffort(int effort);
    void moveTo(long position);
    long getPosition();
    void reset();
    void setup();

    const int Side25Place = 0;
    const int Side25Prep = 0;
    const int Side25Travel = 0;

    const int Side45Place = 0;
    const int Side45Prep = 0;
    const int Side45Travel = 0;

private:
    void setEffort(int effort, bool clockwise);
    static void isr();
    const int tolerance = 3;
    const int PWMOutPin = 11;
    const int AIN2 = 4;
    const int AIN1 = 13;
    const int ENCA = 0;
    const int ENCB = 1;
    const float kP = .5f;
};
#endif