#include <Servo.h>
#include <NewPing.h>
#include "corecontrols.h"
#include "debugs.h"

int cycleObstacle = 0;
int dist = 0;
int dist2;
bool obstacle = 0;
byte obstaclePhase;
byte numZeros = 0;
byte angle;

void setup() {
  Serial.begin(9600);
  pinMode(MOTORPIN, OUTPUT);
  pinMode(SERVOPIN, OUTPUT);
  analogWrite(MOTORPIN, 0);
  servo.attach(SERVOPIN);
  servo.write(STRAIGHT);
  delay(2000);
  analogWrite(MOTORPIN, MINSPEED);
}

void loop() 
{
  if (!obstacle)
  {
    analogWrite(MOTORPIN, MINSPEED);
    dist2 = getFrontDist();
    if (dist2 != 0)
    {
      cycleObstacle ++;
      dist += dist2;
    }
    //brzdeni nema smysl, robot dobiha na setrvacnost a zdrzuje to
    if (cycleObstacle == 3)
    {
      dist = dist / 3;
      cycleObstacle = 0;
      if ( dist < 30 )
      {
        perpendicularRight();
        servo.write(STRAIGHT);
        obstacle = 1;
        obstaclePhase = 0; 
      }
      dist = 0;
    }
  }
  else
  {
     if (obstaclePhase != 2)
     {
       for (int i = 0; i < 8; i++)
         dist += getSideDist();
       dist = dist / 8;
       if (dist != 0)
       {
         angle = map(dist, 0, 2*DISTCUBE, 15, -15);
         steerObstacle(angle);
       }
       else
       {
         obstaclePhase++;
         perpendicularLeft();
         servo.write(STRAIGHT);
         analogWrite(MOTORPIN, MINSPEED_OBSTACLE);
       }
       dist = 0;
     }
     else
     {
       qtrrc.read(sensorValues);
       while (!((sensorValues[0] > THRESHOLD) || (sensorValues[1] > THRESHOLD) || (sensorValues[2] > THRESHOLD) || (sensorValues[3] > THRESHOLD) || (sensorValues[4] > THRESHOLD) || (sensorValues[5] > THRESHOLD)))
       {
         for (int i = 0; i < 10; i++)
           dist += getSideDist();
         dist = dist / 10;
         angle = map(dist, 0, 2*DISTCUBE, 15, -15);
         steerObstacle(angle);
         dist = 0;
         qtrrc.read(sensorValues);
       }
       obstacle = 0;
       perpendicularRight();
       analogWrite(MOTORPIN, MINSPEED);
       
     }
  }
  
}
