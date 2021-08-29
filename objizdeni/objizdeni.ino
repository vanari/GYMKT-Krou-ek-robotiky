#include <Servo.h>
#include <NewPing.h>
#include "corecontrols.h"
#include "debugs.h"


int cycle = 0;
int dist = 0;

void setup() {
  Serial.begin(9600);
  pinMode(MOTORPIN, OUTPUT);
  pinMode(SERVOPIN, OUTPUT);
  analogWrite(MOTORPIN, 0);
  servo.attach(SERVOPIN);
  servo.write(STRAIGHT);
  delay(2000);
}

void loop() {
   analogWrite(MOTORPIN, 150);
   int dist2 = getFrontDist();
   if (dist2 != 0 && dist2 < 50) 
   {
    dist += dist2;
    cycle ++;
   }
   
   if (cycle = 10) 
   { 
     dist = dist / 10;
     cycle = 0;
   
     if (dist < 20)
     {
        perpendicularRight();
        servo.write(STRAIGHT);
        analogWrite(MOTORPIN, 0);
        delay(1000);
     }
   }

}
