
/*
 * SOFTWARE - FIRMWARE - PROGRAM PRO OVLÁDÁNÍ ARDUINA UNO A ŠKOLNÍHO ROBOTA  
 * GYMKT - Kroužek robotiky
 */

//#define DEBUG
//#define DEBUG_POT

//ZAHRNUTI KNIHOVNY
#include "QTRSensors.h"
#include <Servo.h>
#include <EEPROM.h> //lets go commit suicide

#define RYCHJENUL       // a = 0;
#define ANG     65      //max. úhel predniho kolečka (stupně) 65
#define SPK     71      //poměr SPD_MIN/SPD_MAX v % ---- nedávejte přes 100
#define TRE     575     //hranice černé a  420
#define NOP     2       //počet přejezdů kolmice

//DEFINOVANI PINU
#define redbutpin     3
#define greenbutpin   2
#define ledpin        4
#define potpin        A0
#define Ebtn          A2

//PWM piny
#define servopin      5
#define motorpin      6

//senzor povrchu - piny
   
#define iron          13  
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   

#define NUM_SENSORS             6     // number of sensors used
#define NUM_SAMPLES_PER_SENSOR  2500  // average 4 analog samples per sensor reading
#define EMITTER_PIN             13    // emitter is controlled by digital pin 13

// sensors 0 through 5 are connected to analog inputs 0 through 5, respectively
QTRSensorsRC qtrrc((unsigned char[]) {7, 8, 9, 10, 11, 12}, NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);// D12->nejvic vpravo
unsigned int sensorValues[NUM_SENSORS];

unsigned int PSV[NUM_SENSORS];

byte s = 255;
int a=0;

const float R_ANG = map(ANG, 0, 180, 0, PI);

Servo servo;

void led_blink(int x=800);
byte SPD_MAX (unsigned short x=0);
byte memcal(unsigned short x=0);

void setup() {
  pinMode(potpin, INPUT);
  pinMode(redbutpin, INPUT_PULLUP);
  pinMode(greenbutpin, INPUT_PULLUP);
  pinMode(ledpin, OUTPUT);
  pinMode(Ebtn, INPUT_PULLUP);
  
  
  pinMode(servopin, OUTPUT);
  pinMode(motorpin, OUTPUT);



  bool Ecal=0;
    
  servo.attach(servopin);
  delay(20);
  swrite(0);
  delay(20);

  #ifdef DEBUG
  Serial.begin(9600);
  #endif  
  
  
  
  while(digitalRead(greenbutpin)&&(!Ecal)){
    led_blink();
    if((!digitalRead(Ebtn))&&(!Ecal)) {
      Ecal=1;
      Eget();
    }
  }
  //po stisknuti zelene
  
  //digitalWrite(ledpin, 0);
  while(digitalRead(redbutpin)&&(!Ecal)){
    led_blink(150);
    qtrrc.calibrate();
    delay(20);
  }

  
  while(digitalRead(greenbutpin)){
    digitalWrite(ledpin, 1);
    if ((!digitalRead(Ebtn))&&(!Ecal)){
      Ethrow();
      Ecal=1;
    }
  }
  //po stisknuti zelene

  delay(1);  
  SPD_MAX(analogRead(potpin));
  delay(1);
  /*Serial.println(qtrrc.calibratedMaximumOn[0]);
  Serial.println(qtrrc.calibratedMaximumOn[1]);
  Serial.println(qtrrc.calibratedMaximumOn[2]);
  Serial.println(qtrrc.calibratedMaximumOn[3]);
  Serial.println(qtrrc.calibratedMaximumOn[4]);
  Serial.println(qtrrc.calibratedMaximumOn[5]);*/
}



void loop(){
  
  #ifdef DEBUG_POT
    Serial.print(analogRead(potpin));
    Serial.print(' ');
    Serial.print(SPD_MAX());
    Serial.print(' ');
    Serial.println(SPD_MIN());
  #endif
  #ifdef DEBUG_SNS
    Serial.print(sensorValues[0]);
    Serial.print("   ");
    Serial.print(sensorValues[1]);
    Serial.print("   ");
    Serial.print(sensorValues[2]);
    Serial.print("   ");
    Serial.print(sensorValues[3]);
    Serial.print("   ");
    Serial.print(sensorValues[4]);
    Serial.print("   ");
    Serial.println(sensorValues[5]);
  #endif
  
  
  led_blink(300);
  unsigned int position = qtrrc.readLine(sensorValues);

  #ifdef RYCHJENUL
  a=0;
  #endif
  
  if ((sensorValues[0]>TRE) || (sensorValues[1]>TRE) || (sensorValues[2]>TRE) || (sensorValues[3]>TRE) || (sensorValues[4]>TRE) || (sensorValues[5]>TRE)) {
    a = map(position, 0, 5000, -ANG, ANG);
    PSV[0] = sensorValues[0];
    PSV[5] = sensorValues[5];
  }

  if (!((sensorValues[0]>TRE) || (sensorValues[1]>TRE) || (sensorValues[2]>TRE) || (sensorValues[3]>TRE) || (sensorValues[4]>TRE) || (sensorValues[5]>TRE))) {
    if (PSV[0]>TRE) {
      a = -ANG;
    }

    if (PSV[5]>TRE) {
      a = ANG;
    }
  }

  float r_a = map(a, 0, 180, 0, PI);
  
  s = map (abs(sin(r_a)), 0, sin(R_ANG), SPD_MIN(), SPD_MAX());


  
  swrite(a);
  if (lineAnalyze()){
    delay(SPD_MAX()*4);
    s=0;
  }
  analogWrite(motorpin, s);
}





void swrite(int a){
  servo.write(95+a);
}

bool lineAnalyze(){
  static long t = 0;
  static int i=0;
  //digitalWrite(13, i);
  if ((sensorValues[0]>TRE) && (sensorValues[1]>TRE) && (sensorValues[2]>TRE) && (sensorValues[3]>TRE) && (sensorValues[4]>TRE) && (sensorValues[5]>TRE)&&((millis()-t)>5000)){
    if (i<NOP) {i++; t = millis();} 
    else {return 1;}
  }
  if (i>=NOP) {
    return 1;
  }
  return 0;
}

void led_blink(int x){
  static long t = millis();
  if ((millis()-t)<x){
    digitalWrite(ledpin, 1);
  } else {
    digitalWrite(ledpin, 0);
    if((millis()-t)>=(2*x)){
      t = millis();
    }
  }
}

byte SPD_MAX(unsigned short x){
  static byte spd = map(x, 0, 1023, 0, 255);
  return spd;
}

byte SPD_MIN(){
  static byte spd = SPD_MAX()*SPK/100;
  return spd;
}

byte memcal(unsigned short x){
  static unsigned short y;
  if (x){
    y=x;
  } else {
    y-=(y>=255?255:y%255);
  }
  return (y>=255?255:y%255);
}

void Ewrite(unsigned short a, unsigned short x){
  EEPROM.write((a*10), memcal(x));
  //Serial.println(memcal(x));
  for (int i=1; i<10; i++){
    EEPROM.write((a*10)+i, memcal());
    //Serial.println(memcal());
  }
}

unsigned short Eread(unsigned short a){
  unsigned short x=0;
  x=EEPROM.read((a*10));
  for (int i=1; i<10; i++){
    x+=EEPROM.read((a*10)+i);
  }
  return x;
}

void Ethrow(){
  for (int i=0; i<6; i++){
    Ewrite(i, qtrrc.calibratedMaximumOn[i]);
    Ewrite(i+6, qtrrc.calibratedMinimumOn[i]);
  }
}

void Eget(){
  qtrrc.calibrate();
  for (int i=0; i<6; i++){
    qtrrc.calibratedMaximumOn[i] = Eread(i);
    qtrrc.calibratedMinimumOn[i] = Eread(i+6);
  } 
}
