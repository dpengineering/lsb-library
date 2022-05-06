#include <Arduino.h>
#include "LSBLibrary.h"


//////////////////////////////////////////////////////////////////
//                                                              //
//                      PUBLIC, ACCESSIBLE                      //
//                                                              //
//////////////////////////////////////////////////////////////////

/**
 * The main function used to check for changes in light value. Will check if the light level is below two thresholds,
 * the threshold for tuning on and off the board just while the light level is too high, or the threshold for turning
 * off the board regardless of light level.
 * @param code function pointer
 */
void checkOnOff(void (*code)()) {
    pinsOut();
    delay(50);
    second = 1;
    sensorVal = analogRead(sensorPin);
    if (sensorVal > darkVal) {turnOffAll();}
    else {
      if (isOn == true) {
          if (sensorVal <= coveredVal) {turnOffAll(); isOn = false; blinkThing(); blinkThing(); delay(50);}
          else {code();}
          sensorVal = analogRead(sensorPin);
      } else if (isOn == false) {
          if (sensorVal <= coveredVal) {blinkThing(); blinkThing(); delay(50); code(); isOn = true;}
          else {turnOffAll();}
          sensorVal = analogRead(sensorPin);
          delay(150);
      }
    }
}

/**
 * Delay function that checks if the photoresistor (pin A2) is reading a value below the threshold for turning on/off
 * the board. If it is, it will turn on/off the board depending on the current state of the board.
 * 
 * @param duration int
 */
void DPEADelay(float duration) {
    duration = duration * second;
    sensorVal = analogRead(sensorPin);
    checkLight();
    duration *= second;
    for (int i = 0; i < duration; i++) {
        sensorVal = analogRead(sensorPin);
        checkLight();
        duration *= second;
        Serial.println(duration);
        delay(1);
    }
}

void DPEADelayMicroseconds(float duration) {
    duration *= second;
    sensorVal = analogRead(sensorPin);
    checkLight();
    duration *= second;
    for (int i = 0; i < duration / 1000; i++) {
//        long oldTime = millis();
        sensorVal = analogRead(sensorPin);
        checkLight();
        duration *= second;
//        Serial.println(millis() - oldTime);
        delayMicroseconds(1000);
    }
}

/////////////////////////Setup functions//////////////////////////


void setupAll(int darkVal, int coveredVal, uint8_t sensorPin) {
    sensorPin = sensorPin;
    ::darkVal = darkVal;
    ::coveredVal = coveredVal;
    pinsOut();
    Serial.begin(9600);
    Serial.println("Listening on 9600");
    pinMode(sensorPin, INPUT);
}


/////////////////Useful led blink/fade functions/////////////////


//Blinks

void blinkThing() {
  for (int x = 2; x <= 15; x++) {digitalWrite(x, HIGH);}
  delay(100);
  for (int x = 2; x <= 15; x++) {digitalWrite(x, LOW);}
  delay(100);
}

/**
 * Blinks the provided pin for the provided duration.
 * 
 * @param pin int
 * @param duration uint8_t
 */
void ledBlink(uint8_t pin, int duration) {
    digitalWrite(pin, HIGH);
    DPEADelay(duration);
    digitalWrite(pin, LOW);
}

/**
 * Blinks all leds for the provided duration.
 * 
 * @param duration int
 */
void ledBlinkAll(int duration) {
    turnOnAll();
    DPEADelay(duration);
    turnOffAll();
}

//Fades

/**
 * Fades up the given pin for the given duration
 * 
 * @param pin uint8_t
 * @param duration int
 */
void fadeUp(uint8_t pin, int duration) {
    if (duration >= 1000) duration = 999;
    for (float fractionOn = 0; fractionOn < 1; fractionOn += fadingStepSize) {
        pwm(pin, gammaCorrect(fractionOn) * 10000);
        updateFadingStepSize(duration);
    }
    // digitalWrite(pin, HIGH); // make sure pin is at full power before leaving fadeUp()
}

/**
 * Fades down the given pin for the given duration
 * 
 * @param pin uint8_t
 * @param duration int
 */
void fadeDown(uint8_t pin, int duration) {
    if (duration >= 1000) duration = 999;
    for (float fractionOn = 1; fractionOn > 0; fractionOn -= fadingStepSize) {
        pwm(pin, gammaCorrect(fractionOn) * 10000);
        updateFadingStepSize(duration);
    }
}

/**
 * Fades up all LEDs for the given duration
 * 
 * @param duration int
 */
void fadeUpAll(int duration) {
    if (duration >= 1000) duration = 999;
    for (float fractionOn = 0; fractionOn < 1; fractionOn += fadingStepSize) {
        allPWM(gammaCorrect(fractionOn) * 10000);
        updateFadingStepSize(duration);
    }
}

/**
 * Fades down all LEDs for the given duration
 * 
 * @param duration int
 */
void fadeDownAll(int duration) {
    if (duration >= 1000) duration = 999;
    for (float fractionOn = 1; fractionOn > 0; fractionOn -= fadingStepSize) {
        allPWM(gammaCorrect(fractionOn) * 10000);
        updateFadingStepSize(duration);
    }
}

////////////////Array-based blink/fade functions////////////////

/**
 * Blinks all LEDs in the given array for the given duration.
 * 
 * @param pins uint8_t[]
 * @param duration int
 * @param arrsize size_t
 */
void arrayLedBlink(uint8_t pins[], int duration, size_t arrsize) {
    arrayOn(pins, arrsize);
    DPEADelay(duration);
    arrayOff(pins, arrsize);
}

/**
 * Fades up all LEDs in the given array for the given duration.
 * 
 * @param pins uint8_t[]
 * @param duration int
 * @param arrsize size_t
 */
void fadeUpArray(uint8_t pins[], int duration, size_t arrsize) {
    for (float fractionOn = 0; fractionOn < 1; fractionOn += fadingStepSize) {
        arrPWM(pins, gammaCorrect(fractionOn) * 10000, arrsize);
        updateFadingStepSize(duration);
    }
}

/**
 * Fades down all LEDs in the given array for the given duration.
 * 
 * @param pins uint8_t[]
 * @param duration int
 * @param arrsize size_t
 */
void fadeDownArray(uint8_t pins[], int duration, size_t arrsize) {
    for (float fractionOn = 1; fractionOn > 0; fractionOn -= fadingStepSize) {
        arrPWM(pins, gammaCorrect(fractionOn) * 10000, arrsize);
        updateFadingStepSize(duration);
    }
}

//////////////////////////QOL functions//////////////////////////


void turnOnAll() {
    for (int x = 2; x <= 13; x++) {
        digitalWrite(x, HIGH);
    }
}

void turnOffAll() {
    for (int x = 2; x <= 13; x++) {
        digitalWrite(x, LOW);
    }
}

void arrayOn(uint8_t arr[], size_t arrsize) {
    for (int x = 0; x <= arrsize; x++) {
        digitalWrite(arr[x], HIGH);
    }
}

void arrayOff(uint8_t arr[], size_t size) {
    for (int x = 0; x <= size; x++) {
        digitalWrite(arr[x], LOW);
    }
}

int sensorValue() {
    int a = analogRead(A2);
    return a;
}


//////////////////////////////////////////////////////////////////
//                                                              //
//                    PRIVATE, INACCESSIBLE                     //
//                                                              //
//////////////////////////////////////////////////////////////////

void pwm(uint8_t pin, float time) {
    sensorVal = analogRead(sensorPin);
    checkLight();

    digitalWrite(pin, HIGH);
    DPEADelayMicroseconds(time);
    digitalWrite(pin, LOW);
    DPEADelayMicroseconds(10000 - time);
}

void allPWM(int time) {
    sensorVal = analogRead(sensorPin);
    checkLight();

    turnOnAll();
    DPEADelayMicroseconds(time);
    turnOffAll();
    DPEADelayMicroseconds(10000 - time);;
}

void arrPWM(uint8_t arr[], int time, size_t arrsize) {
    sensorVal = analogRead(sensorPin);
    checkLight();

    arrayOn(arr, arrsize);
    DPEADelayMicroseconds(time);
    arrayOff(arr, arrsize);
    DPEADelayMicroseconds(10000 - time);
}

void pinsOut() {
    for (int x = 2; x <= 15; x++) {
        pinMode(x, OUTPUT);
    }
}

void pinsIn() {
    for (int x = 2; x <= 15; x++) {
        pinMode(x, INPUT);
    }
}

void updateFadingStepSize(int duration) {
    float knobRatio = 750 / 768; 
    float stepsOfDuration = duration * (1 - knobRatio) / 10;
    fadingStepSize = 1 / stepsOfDuration;
}

void checkLight() {
    if (sensorVal <= coveredVal || sensorVal >= darkVal) {turnOffAll(); second = 0; pinsIn(); return;}
}