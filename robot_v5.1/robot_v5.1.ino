/* FIRMWARE PRO ROBOTA SLEDUJICIHO CARU, ROCNIK 3.
 * KROUZEK ROBOTIKY GYMNAZIA J.V. V KLATOVECH
 * 2017-2021 
 */

#include "corecontrols.h"
#include "tests.h"
#include "debugs.h"

// debugy
//#define DEBUG
//#define DEBUGSONIC
//#define DEBUGBTN
//#define DEBUGPOT
//#define DEBUGQTR

// testy
//#define TESTS
//#define TURNTEST
//#define STRAIGHTSERVO
//#define SERVOTEST
//#define MOTORTEST
//#define WHOLETEST

#define RACE               // odkomentuj pro zavodeni, zakomentuj pro debug
//#define CYCLESDEBUG      // pocitej pocet pruchodu loop cyklem za sekundu, musi byt definovan i RACE
//#define INCLUDEBAUT      // inicializuj bautrate

unsigned long starttime;

void setup()
{    
    #ifdef INCLUDEBAUT
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
		#ifdef RACE
			while(digitalRead(REDBTN));    // cekani, az se zmackne cervene tlacitko a zacne kalibrace QTRRC sensoru
			for (int i = 0; i < 300; i++)
			{
					qtrrc.calibrate();
					delay(20);
			}
			Serial.println(qtrrc.calibratedMaximumOn[0]);
      Serial.println(qtrrc.calibratedMaximumOn[1]);
      Serial.println(qtrrc.calibratedMaximumOn[2]);
      Serial.println(qtrrc.calibratedMaximumOn[3]);
      Serial.println(qtrrc.calibratedMaximumOn[4]);
      Serial.println(qtrrc.calibratedMaximumOn[5]);
			while(digitalRead(GREENBTN));  // cekani, az se zmackne zelene tlacitko a zacne nekonecny loop
      analogWrite(MOTORPIN, MINSPEED);
		#endif
    starttime = millis();
}

void loop()
{
    byte cycleObstacle = 0;
    int cycleDebug = 0;
    byte cycleRace = 0;
    byte angle = 0;
    bool obstacle = 0;
    byte obstaclePhase = 0;
    byte dist = 0;
    byte dist2;
 
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
			delay(200);
			Serial.println();
    #endif
		#ifdef TESTS
			#ifdef SERVOTEST
          servotest();
      #endif   
      #ifdef MOTORTEST
          motortest();
      #endif
			#ifdef STRAIGHTSERVO       // pohled seshora - vetsi uhel na servu odpovida vic doprava, mensi uhel vic doleva
					straightservo();
			#endif
			#ifdef WHOLETEST       // pohled seshora - vetsi uhel na servu odpovida vic doprava, mensi uhel vic doleva
					wholetest();
			#endif
			#ifdef TURNTEST
					turntest();
			#endif 
			delay(200);
			Serial.println();
		#endif
   
    #ifdef RACE
		/*
			cubePhase:
			    ^
					|  2
			---------
			|       |
			|       | 1
			|       |
			---------
			    ^  0
					|
		*/
    /*if (obstacle) // objizdeni prekazky
    {
      
    }
    else // nasledovani cary
    {*/
				#ifdef CYCLESDEBUG
				if ((millis()-starttime)<1000)
						cycleDebug++;
				else
				{
						starttime = millis();
						Serial.print("Cykly: ");
						Serial.println(cycleDebug);
						cycleDebug = 0;
				}
				#endif

        cycleRace++;
        dist2 = getFrontDist();
        if (dist2 != 0)
        {
          cycleObstacle++;
          dist += dist2;
        }
        unsigned int position = qtrrc.readLine(sensorValues);            
        if ((sensorValues[0] > THRESHOLD) || (sensorValues[1] > THRESHOLD) || (sensorValues[2] > THRESHOLD) || (sensorValues[3] > THRESHOLD) || (sensorValues[4] > THRESHOLD) || (sensorValues[5] > THRESHOLD))
				{
            angle += map(position, 0, 5000, -MAXANG, MAXANG);
            PSV[0] = sensorValues[0];
            PSV[5] = sensorValues[5];
        }
        if(!((sensorValues[0] > THRESHOLD) || (sensorValues[1] > THRESHOLD) || (sensorValues[2] > THRESHOLD) || (sensorValues[3] > THRESHOLD) || (sensorValues[4] > THRESHOLD) || (sensorValues[5] > THRESHOLD)))
        {
            if (PSV[0] > THRESHOLD)
                angle = -MAXANG;
            if (PSV[5] > THRESHOLD)
                angle = MAXANG;
        }
        
        /*if (lapAnalyze())
        {
          delay(500);
          analogWrite(MOTORPIN, 0);
          while (1);
        }*/
        
        if (cycleRace == 10)
        {
          angle = angle / 10;
          steer(angle);
					angle = 0;
          cycleRace = 0;
        }
        /*if (cycleObstacle == 5)
        {
          dist = dist / 5;
          cycleObstacle = 0;
          if (dist < 30)
          {
            obstacle = 1;
            obstaclePhase = 1;
            perpendicularRight();
            servo.write(STRAIGHT);
            delay(2000);       
          }
          dist = 0;
        }*/
    //}     
    #endif
} 
