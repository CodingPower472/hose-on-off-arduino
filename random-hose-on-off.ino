
// Luke Bousfield - May 28, 2019

#include <Gaussian.h>

const double OFF_TIME_MEAN = 15000; // 15 seconds
const double OFF_TIME_STD_DEV = 3000; // 3 seconds
const double ON_TIME_MEAN = 10000; // 10 seconds
const double ON_TIME_STD_DEV = 1000; // 1 seconds
const bool START_ON = true;
const bool KS_INVERT = false; // if this is true, HIGH to kill switch pin means keep going, LOW to kill switch means stop
const int OUTPUT_PIN = 13;
const int KS_PIN = 7;

const Gaussian offTime(OFF_TIME_MEAN, OFF_TIME_STD_DEV);
const Gaussian onTime(ON_TIME_MEAN, ON_TIME_STD_DEV);

bool state; // false for off, true for on
long nextStateChange;

// turn on and schedule the next time to turn off
void setOn() {
  state = true;
  nextStateChange = millis() + offTime.random();
}

// turn off and schedule the next time to turn on
void setOff() {
  state = false;
  nextStateChange = millis() + onTime.random();
}

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  pinMode(OUTPUT_PIN, OUTPUT);
  pinMode(KS_PIN, INPUT);
  
  if (START_ON) {
    setOn();
  } else {
    setOff();
  }
}

void loop() {
  long currTime = millis();
  if (currTime > nextStateChange) {
    if (state) {
      setOff();
    } else {
      setOn();
    }
  }
  if (state && (digitalRead(KS_PIN) == HIGH) == KS_INVERT) {
    // set hardware on
    Serial.println(1);
    digitalWrite(OUTPUT_PIN, HIGH);
  } else {
    // set hardware off
    Serial.println(0);
    digitalWrite(OUTPUT_PIN, LOW);
  }
}
