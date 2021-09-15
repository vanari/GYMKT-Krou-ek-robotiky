#pragma once

#include "defines.h"


/*byte getMaxSpeed()
{
    static byte s = map(analogRead(POT), 0, 1023, MINSPEED, 150);
    return s;
}*/



byte getFrontDist()
{
    byte dist = sonarfront.ping_cm();
    return dist;
}

byte getSideDist()
{
    byte dist = sonarside.ping_cm();
    return dist;
}

void steer(int a)
{
    if ((a <= MAXANG) && (a >= -MAXANG))
        servo.write(STRAIGHT+a);
     //float r_a = map(abs(a), 0, 90, 0, PI/2);
     //byte speed = map (sin(r_a), 0, sin(R_ANG), MINSPEED, MAXSPEED);
     byte speed = map (abs(a), 0, MAXANG, MINSPEED, MAXSPEED);
     analogWrite(MOTORPIN, speed);
}

bool lapAnalyze()
{
    static long time = 0;
    static byte numlaps = 0;
    if ((sensorValues[0] > THRESHOLD) && (sensorValues[1] > THRESHOLD) && (sensorValues[2] > THRESHOLD) && (sensorValues[3] > THRESHOLD) && (sensorValues[4] > THRESHOLD) && (sensorValues[5] > THRESHOLD))
    {
        if (numlaps < LAPS)
        {
            numlaps++;
            time = millis();
        }
        else
            return 1;
    }
    if (numlaps >= LAPS)
        return 1;
    return 0;
}

void perpendicularRight()
{
    steer(25);
    delay(190);
    steer(32);
    delay(380);
    steer(0);
}

void perpendicularLeft()
{
    delay(190);
    steer(-32);
    delay(380);
    steer(0);
}

void fast_driveAroundRight()
{
    servo.write(50+STRAIGHT);
    analogWrite(MOTORPIN, 120);   
    delay(500); 
    servo.write(-50+STRAIGHT);
    delay(550);
    analogWrite(MOTORPIN, MINSPEED_OBSTACLE);
    servo.write(STRAIGHT);
}

void slow_driveAroundRight()
{
    servo.write(52+STRAIGHT);
    analogWrite(MOTORPIN, 120);   
    delay(700); 
    servo.write(STRAIGHT);
    analogWrite(MOTORPIN, MINSPEED);
    delay(450);
    analogWrite(MOTORPIN, 120);
    servo.write(-52+STRAIGHT);
    delay(300);
    analogWrite(MOTORPIN, MINSPEED_OBSTACLE);
    servo.write(STRAIGHT);
}

void driveAroundLeft()
{
   servo.write(-42+STRAIGHT);
   analogWrite(MOTORPIN, 110);  
   qtrrc.read(sensorValues);
   while ( !((sensorValues[0] > THRESHOLD) || (sensorValues[1] > THRESHOLD) || (sensorValues[2] > THRESHOLD) || (sensorValues[3] > THRESHOLD) || (sensorValues[4] > THRESHOLD) || (sensorValues[5] > THRESHOLD)) )
     qtrrc.read(sensorValues);
}
