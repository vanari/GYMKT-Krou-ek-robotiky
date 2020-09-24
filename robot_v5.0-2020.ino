// FIRMWARE PRO ROBOTA GYMNAZIALNIHO KROUZKU V KLATOVECH

// DEFINICE DEBUG
//#define DEBUG_BAUD      // spust baudrate
//#define DEBUG_POT       // spust debug potenciometru
//#define DEBUG_SENSOR    // spust debug senzoru

// KNIHOVNY
#include "QTRSensors.h"
#include <Servo.h>
#include <EEPROM.h> // lets go commit suicide

// MAKRA PARAMETRU
#define RYCHNULL          // a = 0;
#define MAXANG     65     // max. uhel predniho kolecka 
#define SPDRTO     71     // pomer minimalni a maximalni rychlosti v procentech 
#define TRE        575    // hranice cerne a 420
#define LAPS       2      // pocet kol aneb přejezdu kolmice

// MAKRA PINU
#define REDBTN     3
#define GRNBTN     2
#define LEDPIN     4
#define POTPIN     A0
#define EPRBTN     A2

// MAKRA PWM PINU
#define SERVOPIN   5
#define MOTORPIN   6

// MAKRA SENZORU
#define NUMSENS    6
#define NUMSAMPS   2500
#define EMITTERPIN 13

// INICIALIZACE
QTRSensorsRC qtrrc((unsigned char[]) {7,8,9,10,11,12}, NUMSENS, NUMSAMPS, EMITTERPIN);
unsigned int sensorvalues[NUMSENS];
unsigned int psv[NUMSENS];
Servo servo;
byte minspeed;
byte maxspeed;

// SETUP
void setup(){
  pinMode(POTPIN, INPUT);
  pinMode(REDBTN, INPUT_PULLUP):
  pinMode(GRNBTN, INPUT_PULLUP);
  pinMode(LEDPIN, OUTPUT);
  pinMode(EPRBTN, INPUT_PULLUP);

  pinMode(SERVOPIN, OUTPUT);
  pinMode(MOTORPIN, OUTPUT);
  servo.attach(SERVOPIN);
  delay(20);
  swrite(0);
  delay(20);

  #ifdef DEBUG_BAUD
    Serial.begin(9600);
  #endif

  maxspeed = getmaxspeed(analogRead(POTPIN));
  minspeed = getminspeed * SPDRTO / 100;
}

// CYKLUS
void loop(){
  #ifdef DEBUG_POT
    Serial.print("Potenciometr: ");
    Serial.print(analogRead(POTPIN));
    Serial.print(",  Max rychlost: ");
    Serial.print(maxspeed);
    Serial.print(",  Min rychlost: ");
    Serial.print(minspeed); 
  #endif
  #ifdef DEBUG_SNS
  #endif



}



byte getmaxspeed(unsigned short pot){
  static byte speed = map(pot, 0, 1023, 0, 255);
  return speed;
}

