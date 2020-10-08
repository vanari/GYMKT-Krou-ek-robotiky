/* FIRMWARE PRO ROBOTA SLEDUJICIHO CARU, ROCNIK 3.
 * KROUZEK ROBOTIKY GYMNAZIA J.V. V KLATOVECH
 * COPYRIGHT 2017-2020 */

// knihovny
#include "QTRSensors.h"
#include <NewPing.h>
#include <Servo.h>

// debugy
//#define DEBUG
//#define DEBUGBAUT
//#define DEBUGSONIC
//#define DEBUGBTN
//#define DEBUGPOT
//#define DEBUGQTR

// testy
//#define TURNTEST
//#define STRAIGHTSERVO
//#define SERVOTEST
//#define MOTORTEST
//#define WHOLETEST

#define RACE

// makra testu
#define TESTSPEED   140  // max 255
#define TESTANGLE   80   // rovnovazna poloha = STRAIGHT

// definovani pinu 
#define REDBTN      A5
#define GREENBTN    A3
#define POT         A4
#define MOTORPIN    6
#define SERVOPIN    7
#define TRIG_1      2   //predni sonar
#define ECHO_1      3          
#define TRIG_2      4   //bocni sonar
#define ECHO_2      5
#define MAXDISTANCE 150

// definovani QTRSensoru
#define NUMSENS     6
#define NUMSAMPLES  2500
#define EMITTERPIN  A0 

// definovani serva
#define STRAIGHT    100  //pohled seshora - vetsi uhel odpovida vic doprava, mensi uhel vic doleva
#define MAXANG      40   //maximalni odchyleni


// definovani dalsiho
#define THRESHOLD   575
#define LAPS        2
#define MINSPEED    140 //max 255

// definovani sonaru
#define DISTCUBE    9  // vzdalenost robota od objizdene kostky v cm


QTRSensorsRC qtrrc((unsigned char[]) {8, 9, 10, 11, 12, 13}, NUMSENS, NUMSAMPLES, EMITTERPIN);
unsigned int SensorValues[NUMSENS];
unsigned int PSV[NUMSENS];

NewPing sonarfront(TRIG_1, ECHO_1, MAXDISTANCE);
NewPing sonarside(TRIG_2, ECHO_2, MAXDISTANCE);
Servo servo;
const float R_ANG = map(MAXANG, 0, 180, 0, PI);

byte speed = 100;
int angle = 0;
byte MaxSpeed = 0;
bool GoAround = 0;
byte CubePhase = 1;
const float SineMaxAng = sin((PI / 180)*MAXANG); 

void setup()
{    
    #ifdef DEBUGBAUT
    Serial.begin(9600);
    #endif

    pinMode(REDBTN, INPUT_PULLUP);
    pinMode(GREENBTN, INPUT_PULLUP);
    pinMode(POT, INPUT);
    pinMode(MOTORPIN, OUTPUT);
    pinMode(SERVOPIN, OUTPUT);
    servo.attach(SERVOPIN);
    delay(20);
    steer(0);
    delay(20);
    analogWrite(MOTORPIN, 0);

    while(digitalRead(REDBTN));
    
        for (int i = 0; i < 300; i++)
        {
            qtrrc.calibrate();
            delay(20);
        }
    while(digitalRead(GREENBTN));
}


void loop()
{
    #ifdef DEBUG
        #ifdef DEBUGBTN
            debugbtn();
        #endif
       
        #ifdef DEBUGSONIC
            debugsonic();
        #endif

        #ifdef DEBUGPOT
            debugpot();
        #endif

        #ifdef DEBUGQTR
            debugqtr();
        #endif 
    
        #ifdef SERVOTEST
            servotest();
        #endif
    
        #ifdef MOTORTEST
            motortest();
        #endif 
        delay(500);
        Serial.println();
    #endif

    #ifdef STRAIGHTSERVO       //pohled seshora - vetsi uhel na servu odpovida vic doprava, mensi uhel vic doleva
        straightservo();
    #endif

    #ifdef WHOLETEST       //pohled seshora - vetsi uhel na servu odpovida vic doprava, mensi uhel vic doleva
        wholetest();
    #endif

    #ifdef TURNTEST
        delay(2000);
        analogWrite(MOTORPIN, 120);
        delay(1000);
        analogWrite(MOTORPIN, 150);
        PerpendicularRight();
        delay(300);
        analogWrite(MOTORPIN, 0);
    #endif 
   
    #ifdef RACE
        unsigned int position = qtrrc.readLine(SensorValues);
            
            if ((SensorValues[0] > THRESHOLD) || (SensorValues[1] > THRESHOLD) || (SensorValues[2] > THRESHOLD) || (SensorValues[3] > THRESHOLD) || (SensorValues[4] > THRESHOLD) || (SensorValues[5] > THRESHOLD))
            {
                angle = map(position, 0, 5000, -MAXANG-15, MAXANG+15);
                PSV[0] = SensorValues[0];
                PSV[5] = SensorValues[5];
            }

            if(!((SensorValues[0] > THRESHOLD) || (SensorValues[1] > THRESHOLD) || (SensorValues[2] > THRESHOLD) || (SensorValues[3] > THRESHOLD) || (SensorValues[4] > THRESHOLD) || (SensorValues[5] > THRESHOLD)))
            {
                if (PSV[0] > THRESHOLD)
                    angle = -MAXANG;
                if (PSV[5] > THRESHOLD)
                    angle = MAXANG;
                if ((SensorValues[0] < THRESHOLD) && (SensorValues[5] < THRESHOLD))
                    steer(0);
             }
            
        float r_a = map(angle, 0, 180, 0, PI);
  
        speed = map (abs(sin(r_a)), 0, sin(R_ANG), 90, 160);
        steer(angle);
        analogWrite(MOTORPIN, speed);
        
    #endif
} 

byte GetFrontDist()
{
    int dist = sonarfront.ping_cm();
    delay(5);
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


byte GetSideDist()
{
    int dist = sonarside.ping_cm();
    delay(5);
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

void steer(int a)
{
    if ((a < MAXANG) && (a > -MAXANG))
        servo.write(STRAIGHT+a);
}

byte GetMaxSpeed()
{
    static byte s = map(analogRead(POT), 0, 1023, 255, MINSPEED);
    return s;
}

bool LapAnalyze()
{
    static long time = 0;
    static byte numlaps = 0;
    if ((SensorValues[0] > THRESHOLD) && (SensorValues[1] > THRESHOLD) && (SensorValues[2] > THRESHOLD) && (SensorValues[3] > THRESHOLD) && (SensorValues[4] > THRESHOLD) && (SensorValues[5] > THRESHOLD))
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

void debugbtn()
{
    if (!digitalRead(GREENBTN))
        Serial.println("Zelene tlacitko zmacknuto.");
    if (!digitalRead(REDBTN))
        Serial.println("Cervene tlacitko zmacknuto.");
}

void debugsonic()
{

}
void debugpot()
{
    Serial.print("Potenciometr: ");
    Serial.println(analogRead(POT));
}

void debugqtr()
{
    for (byte i = 0; i < NUMSENS; i++)
    {
        Serial.print("Sensor c.");
        Serial.print(i+1);
        Serial.print(": ");
        Serial.println(SensorValues[i]);
    }
}

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

void PerpendicularRight()
{
    steer(25);
    delay(190);
    steer(32);
    delay(380);
    steer(0);
}

void PerpendicularLeft()
{
    steer(-25);
    delay(190);
    steer(-32);
    delay(380);
    steer(0);
}
