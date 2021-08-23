#include "defines.h"

void debugbtn()
{
    if (!digitalRead(GREENBTN))
        Serial.println("Zelene tlacitko zmacknuto.");
    if (!digitalRead(REDBTN))
        Serial.println("Cervene tlacitko zmacknuto.");
}

void debugsonic()
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
    Serial.print("Bocni sonar: ");
    Serial.print(dist);
    Serial.println(" cm.");
    
    dist = sonarfront.ping_cm();
    delay(2);
    if ((dist > 0) && (dist < 50)) {
        dist = 0;
        for (int i = 0; i < 5; i++) {
            dist += sonarfront.ping_cm();
            delay(1);
        }
        dist = dist / 5;
    }
    Serial.print("Predni sonar: ");
    Serial.print(dist);
    Serial.println(" cm.");   
}

void debugpot()
{
    Serial.print("Potenciometr: ");
    Serial.println(analogRead(POT));
}

void debugqtr()
{
    digitalWrite(EMITTERPIN, LOW);
    for (byte i = 0; i < NUMSENS; i++)
    {
        Serial.print("Sensor c.");
        Serial.print(i+1);
        Serial.print(": ");
        Serial.println(SensorValues[i]);
    }
}
