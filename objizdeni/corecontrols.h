#pragma once

#include "defines.h"
#define MAXSPEED 150
#define MINSPEED_OBSTACLE 70
#define MAXSPEED_OBSTACLE 90

byte getMaxSpeed()
{
    static byte s = map(analogRead(POT), 0, 1023, MINSPEED, 255);
    return s;
}

//byte MAXSPEED = getMaxSpeed;

byte getFrontDist()
{
    int dist = sonarfront.ping_cm();
    delay(2);
    if ((dist > 0) && (dist < 50)) {
        dist = 0;
        for (int i = 0; i < 5; i++) {
            dist += sonarfront.ping_cm();
            delay(1);
        }
        dist = dist / 5;
    }
    return dist;
}

byte getSideDist()
{
    int dist = sonarside.ping_cm();
    delay(2);
    if ((dist > 0) && (dist < 50)) {
        dist = 0;
        for (int i = 0; i < 5; i++) {
            dist += sonarside.ping_cm();
            delay(1);
        }
        dist = dist / 5;
    }
    return dist;
}

void steer(byte a)
{
    if ((a < MAXANG) && (a > -MAXANG))
        servo.write(STRAIGHT+a);
     byte speed = map (abs(a), 0, MAXANG, MINSPEED, MAXSPEED); // vetsi rychlost do zatacky
     analogWrite(MOTORPIN, speed);

void steerObstacle(byte a)
{
    if ((a < MAXANG) && (a > -MAXANG))
        servo.write(STRAIGHT+a);
     byte speed = map (abs(a), 0, MAXANG, MINSPEED_OBSTACLE, MAXSPEED_OBSTACLE); // vetsi rychlost do zatacky
     analogWrite(MOTORPIN, speed);
}

void perpendicularRight()
{
   servo.write(55+STRAIGHT);
   analogWrite(MOTORPIN, 110);
   delay(900);
   analogWrite(MOTORPIN, MINSPEED_OBSTACLE);
}

void perpendicularLeft()
{
   servo.write(-55+STRAIGHT);
   analogWrite(MOTORPIN, 110);
   delay(900);
   analogWrite(MOTORPIN, MINSPEED_OBSTACLE);
}
