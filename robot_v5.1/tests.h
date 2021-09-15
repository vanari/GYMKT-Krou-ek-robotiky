#include "defines.h"

// makra testu
#define TESTSPEED   140  // max 255
#define TESTANGLE   83   // rovnovazna poloha = STRAIGHT
byte str = TESTANGLE;

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
    for (i = TESTANGLE; i < 170; i++)
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
    for (i = 10; i < TESTANGLE; i++)
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
    servo.write(str);
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
    delay(50);
    if ((!digitalRead(GREENBTN)) && (!digitalRead(REDBTN)))
    {
        analogWrite(MOTORPIN, 130);
        delay(3000);
        analogWrite(MOTORPIN, 0);
    }
    Serial.print("Servo: ");
    Serial.println(str);
}

void fast_driveAroundTest()
{
  analogWrite(MOTORPIN, MINSPEED);
  servo.write(STRAIGHT);
  byte dist = getFrontDist();
  if (dist == 0)
    dist = MAXDISTANCE;
  if (dist <= 35) {
    servo.write(50+STRAIGHT);
    analogWrite(MOTORPIN, 120);  
    delay(500); 
    servo.write(-50+STRAIGHT);
    delay(550);
    analogWrite(MOTORPIN, MINSPEED_OBSTACLE);
    servo.write(STRAIGHT);
    delay(2000);
  } 
}

void slow_driveAroundTest()
{
  analogWrite(MOTORPIN, MINSPEED);
  servo.write(STRAIGHT);
  byte dist = getFrontDist();
  if (dist == 0)
    dist = MAXDISTANCE;
  if (dist <= 35)
  {
    servo.write(50+STRAIGHT);
    analogWrite(MOTORPIN, 120);   
    delay(600); 
    servo.write(STRAIGHT);
    analogWrite(MOTORPIN, MINSPEED);
    delay(600);
    analogWrite(MOTORPIN, 100);
    servo.write(-50+STRAIGHT);
    delay(300);
    analogWrite(MOTORPIN, MINSPEED_OBSTACLE);
    servo.write(STRAIGHT);
    delay(2000);
  }
  
}
