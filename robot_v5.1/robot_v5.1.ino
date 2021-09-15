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
//#define FAST_DRIVEAROUNDTEST
//#define SLOW_DRIVEAROUNDTEST

#define RACE               // odkomentuj pro zavodeni, zakomentuj pro debug
//#define CYCLESDEBUG      // pocitej pocet pruchodu loop cyklem za sekundu, musi byt definovan i RACE
//#define INCLUDEBAUT      // inicializuj bautrate

unsigned long starttime;
int t1, t2;

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
			for (int i = 0; i < 200; i++)
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
    t1 = millis();
}

void loop()
{
    int cycleDebug = 0;
    signed char angle = 0;
    bool obstacle = 0;
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
      #ifdef FAST_DRIVEAROUNDTEST
          fast_driveAroundTest();
      #endif
       #ifdef SLOW_DRIVEAROUNDTEST
          slow_driveAroundTest();
      #endif
			delay(5);
			Serial.println();
		#endif
   
    #ifdef RACE

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

    if (obstacle)
    {
       for (int i = 0; i < 3; i++)
       dist += getSideDist();
       dist = dist / 3;
       if (dist == 0)
       {
         driveAroundLeft();
         obstacle = 0;      
       }
       dist = 0;
       t1 = millis();
    }   
    else
    {   
        dist = getFrontDist();     
        if (dist == 0)
          dist = MAXDISTANCE;
        unsigned int position = qtrrc.readLine(sensorValues);            
        if ((sensorValues[0] > THRESHOLD) || (sensorValues[1] > THRESHOLD) || (sensorValues[2] > THRESHOLD) || (sensorValues[3] > THRESHOLD) || (sensorValues[4] > THRESHOLD) || (sensorValues[5] > THRESHOLD))
				{
            angle = map(position, 0, 5000, -MAXANG, MAXANG);
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
        
        steer(angle);
        if (dist <= 20)
        {
          obstacle = 1;
          t2 = millis();
          t1 = t2 - t1;      
          if (t1 <= 600)
            slow_driveAroundRight();
          else if ((t1 > 600) && (t1 <= 2500))
            fast_driveAroundRight();
          else if (t1 > 2500)
          {
            delay(200);
            fast_driveAroundRight();
          }
          delay(200);
        }

        /*if (dist < 30)
        {
          obstacle = 1;
          analogWrite(MOTORPIN, MINSPEED_OBSTACLE);
          driveAroundRight();       
          while (obstacle)
          {
            dist = 0;
            for (int i = 0; i < 5; i++)
              dist += getSideDist();
            dist = dist / 5 ;
            if (dist == 0)
            {
              driveAroundLeft();
              obstacle = 0;      
            }
          }  
        }*/
        /*if (lapAnalyze())
        {
          delay(500);
          analogWrite(MOTORPIN, 0);
          while (1);
        }*/
    } 
    #endif
} 
