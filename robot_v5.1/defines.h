#pragma once // nemazat jinak kompilator bude picovat

#include "QTRSensors.h"
#include <NewPing.h>
#include <Servo.h>

// definovani pinu 
#define REDBTN      A4
#define GREENBTN    A5
#define POT         A3
#define MOTORPIN    6
#define SERVOPIN    7
#define TRIG_1      2   // predni sonar
#define ECHO_1      3   // predni sonar       
#define TRIG_2      4   // bocni sonar
#define ECHO_2      5   // bocni sonar
#define MAXDISTANCE 150

// definovani QTRSensoru
#define NUMSENS     6
#define NUMSAMPLES  2500
#define EMITTERPIN  A0 

// definovani serva
#define STRAIGHT    85  // pohled seshora - vetsi uhel odpovida vic doprava, mensi uhel vic doleva
#define MAXANG      55  // maximalni odchyleni

// definovani dalsiho
#define THRESHOLD   575
#define LAPS        2
#define MINSPEED    80 // max 255
#define MAXSPEED    140

// definovani sonaru
#define DISTCUBE    20  // vzdalenost robota od objizdene kostky v cm

NewPing sonarfront(TRIG_1, ECHO_1, MAXDISTANCE);
NewPing sonarside(TRIG_2, ECHO_2, MAXDISTANCE);
QTRSensorsRC qtrrc((unsigned char[]) {8, 9, 10, 11, 12, 13}, NUMSENS, NUMSAMPLES, EMITTERPIN);
unsigned int sensorValues[NUMSENS];
unsigned int PSV[NUMSENS];
Servo servo;

const float R_ANG = map(MAXANG, 0, 180, 0, PI);
