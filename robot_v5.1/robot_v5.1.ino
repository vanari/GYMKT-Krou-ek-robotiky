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
#define SERVOTEST
//#define MOTORTEST
//#define WHOLETEST

//#define RACE             // odkomentuj pro zavodeni, zakomentuj pro debug
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
			Serial.println(qtrrc.calibratedMaximum[0]);
      Serial.println(qtrrc.calibratedMaximum[1]);
      Serial.println(qtrrc.calibratedMaximum[2]);
      Serial.println(qtrrc.calibratedMaximum[3]);
      Serial.println(qtrrc.calibratedMaximum[4]);
      Serial.println(qtrrc.calibratedMaximum[5]);
			while(digitalRead(GREENBTN));  // cekani, az se zmackne zelene tlacitko a zacne nekonecny loop
		#endif
    starttime = millis();
}

void loop()
{
    byte cycleObstacle = 0;
    int cycleDebug = 0;
    byte cycleRace = 0;
    int angle = 0;
    bool obstacle = 0;
    byte obstaclePhase = 1;
    byte dist;
    
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
					|  3
			---------
			|       |
			|       | 2
			|       |
			---------
			    ^  1
					|
		*/
    if (obstacle) // objizdeni prekazky
    {
        if ((obstaclePhase == 1) || (obstaclePhase == 2))
        {
            cycleObstacle++;
            dist = 0;
            for (int i = 0; i < 4; i++)
              dist += getSideDist;
            dist = dist / 4;
            if (dist < 2*DISTCUBE)
                angle += map(dist, 0, 2*DISTCUBE, MAXANG, -MAXANG); 
            else
            {
                if (obstaclePhase == 1)
                {
                  delay(100);
                  perpendicularLeft;
                }
                obstaclePhase++;
            }
            if (cycleObstacle = 4)
            {
              angle = angle / 4;
              steer(angle);
              cycleObstacle = 0;
            }
        }
        else
        {
          obstacle = 0;           
          delay(200);
					// otaceni se doleva trochu
					steer(-30);
					delay(200);
					steer(0);
          while (!((SensorValues[0] > THRESHOLD) || (SensorValues[1] > THRESHOLD) || (SensorValues[2] > THRESHOLD) || (SensorValues[3] > THRESHOLD) || (SensorValues[4] > THRESHOLD) || (SensorValues[5] > THRESHOLD)));
        }
    }
    else // nasledovani cary
    {
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
        unsigned int position = qtrrc.readLine(SensorValues);            
        if ((SensorValues[0] > THRESHOLD) || (SensorValues[1] > THRESHOLD) || (SensorValues[2] > THRESHOLD) || (SensorValues[3] > THRESHOLD) || (SensorValues[4] > THRESHOLD) || (SensorValues[5] > THRESHOLD))
				{
            angle += map(position, 0, 5000, -MAXANG+5, MAXANG-5);
            PSV[0] = SensorValues[0];
            PSV[5] = SensorValues[5];
        }
        if(!((SensorValues[0] > THRESHOLD) || (SensorValues[1] > THRESHOLD) || (SensorValues[2] > THRESHOLD) || (SensorValues[3] > THRESHOLD) || (SensorValues[4] > THRESHOLD) || (SensorValues[5] > THRESHOLD)))
        {
            if (PSV[0] > THRESHOLD)
                angle = -MAXANG-5;
            if (PSV[5] > THRESHOLD)
                angle = MAXANG+5;
        }            
				// zde probiha manipulace s robotem po zprumerovani hodnot pro lepsi stabilitu
        if (cycleRace = 20)
        {
          angle = angle / 20;
          steer(angle);
					angle = 0;
          cycleRace = 0;
        }
				if (getFrontDist < DISTCUBE+5)
				{
						cycleObstacle = 0;
						obstacle = 1;
						perpendicularRight;
				}         
    }      
    #endif
} 
