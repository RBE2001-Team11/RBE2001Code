#include <Arduino.h>
#include <BlueMotor.h>
#include <Romi32U4.h>

int oldValue = 0;
int newValue;
int count = 0;
int errorCount = 0;
unsigned time = 0;

const char error = 5;

char encoderArray[4][4] = {
    {0, -1, 1, error},
    {1, 0, error, -1},
    {-1, error, 0, 1},
    {error, 1, -1, 0}};

BlueMotor::BlueMotor()
{
}

void BlueMotor::setup()
{
    pinMode(PWMOutPin, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(ENCA, INPUT);
    pinMode(ENCB, INPUT);
    TCCR1A = 0xA8; // 0b10101000; //gcl: added OCR1C for adding a third PWM on pin 11
    TCCR1B = 0x11; // 0b00010001;
    ICR1 = 400;
    OCR1C = 0;

    attachInterrupt(digitalPinToInterrupt(ENCA), isr, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCB), isr, CHANGE);
    reset();
}

void BlueMotor::resetEncoder()
{
    count = 0;
}

long BlueMotor::getPosition()
{
    return count;
}

void BlueMotor::reset()
{
    count = 0;
}

void BlueMotor::isr()
{

    newValue = (digitalRead(1) << 1) | digitalRead(0);
    char value = encoderArray[oldValue][newValue];
    if (value == error)
    {
        errorCount++;
    }
    else
    {
        count -= value;
    }
    oldValue = newValue;
}

void BlueMotor::setEffort(int effort)
{
    if (effort < 0)
    {
        setEffort(-effort, true);
    }
    else
    {
        setEffort(effort, false);
    }
}

void BlueMotor::setEffort(int effort, bool clockwise)
{
    if (clockwise)
    {
        digitalWrite(AIN1, HIGH);
        digitalWrite(AIN2, LOW);
    }
    else
    {
        digitalWrite(AIN1, LOW);
        digitalWrite(AIN2, HIGH);
    }
    OCR1C = constrain(effort, 0, 400);
}

float getRPM()
{
    return ((newValue - oldValue) / 270) / (time / 1000 / 60);
}

//
boolean BlueMotor::moveTo(long target) // Move to this encoder position within the specified
{                                      // tolerance in the header file using proportional control
                                       // then stop
    // Serial.println((target - getPosition()) * 2);

    setEffort((getPosition() - target) * kP); // TODO equation
    if ((getPosition() - target) > -DEADBAND && (getPosition() - target) < DEADBAND)
    {
        return true;
    }
    return false;
}

void BlueMotor::holdTo(long target)           // Move to this encoder position within the specified
{                                             // tolerance in the header file using proportional control
                                              // then stop
    setEffort((getPosition() - target) * kP); // TODO equation
}
