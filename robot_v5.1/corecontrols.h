byte getFrontDist()
{
    int dist = sonarfront.ping_cm();
    delay(2);
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

byte getSideDist()
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
    return dist;
}

void steer(int a)
{
    if ((a < MAXANG) && (a > -MAXANG))
        servo.write(STRAIGHT+a);
		float r_a = map(a, 0, 180, 0, PI);  // uhel v radianech
		byte speed = map (abs(sin(r_a)), 0, sin(R_ANG), MINSPEED, MAXSPEED); // vetsi rychlost do zatacky
		analogWrite(MOTORPIN, speed);
}

byte getMaxSpeed()
{
    static byte s = map(analogRead(POT), 0, 1023, MINSPEED, 255);
    return s;
}

bool lapAnalyze()
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

void perpendicularRight()
{
    steer(25);
    delay(190);
    steer(32);
    delay(380);
    steer(0);
}

void perpendicularLeft()
{
    delay(190);
    steer(-32);
    delay(380);
    steer(0);
}
