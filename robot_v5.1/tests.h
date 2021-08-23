#include "defines.h"

// makra testu
#define TESTSPEED   140  // max 255
#define TESTANGLE   88   // rovnovazna poloha = STRAIGHT

void motortest()
{
    delay(30);
    digitalWrite(MOTORPIN, HIGH);
    delay(100);
    digitalWrite(MOTORPIN, LOW);
}

void servotest()
{
    int i;
    for (i = STRAIGHT; i < 170; i++)
    {
        servo.write(i);
        delay(10);
    }
    delay(30);
    for (i = 170; i > 10; i--)
    {
        servo.write(i);
        delay(10);
    }
    delay(30);
    for (i = 10; i < STRAIGHT; i++)
    {
        servo.write(i);
        delay(10);
    }
}

void turntest()
{
		delay(2000);
		analogWrite(MOTORPIN, 120);
		delay(1000);
		analogWrite(MOTORPIN, 150);
		perpendicularRight();
		delay(300);
		analogWrite(MOTORPIN, 0);
}

void wholetest()
{
    servo.write(TESTANGLE);
    analogWrite(MOTORPIN, TESTSPEED);
    delay(1000);
    analogWrite(MOTORPIN, 0);
    delay(2000);    
}

void straightservo()
{
    byte str;
    if (!digitalRead(GREENBTN))
    {
        str += 1;
        if (str > 120)
            str = 120;
    }
    if (!digitalRead(REDBTN))
    {
        str -= 1;
        if (str < 70)
            str = 70;
    }
    servo.write(str);
    delay(150);
    if ((!digitalRead(GREENBTN)) && (!digitalRead(REDBTN)))
    {
        analogWrite(MOTORPIN, 100);
        delay(3000);
        analogWrite(MOTORPIN, 0);
    }
    Serial.print("Servo: ");
    Serial.println(str);
}
