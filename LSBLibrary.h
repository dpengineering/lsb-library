#ifndef CPP_LSBLIBRARY_H
#define CPP_LSBLIBRARY_H

#include <Arduino.h>

//Board class
class Board {
public:
  uint8_t sensorPin;
  int coveredVal;
  int darkVal;
  Board(int covered, int dark, uint8_t pin);
  void checkOnOff(void (*code)());
  void DPEADelay(float duration);
  void DPEADelayMicroseconds(float duration);

  //Setup functions
  void setupAll();

  //Useful led blink/fade functions

  void ledBlink(uint8_t pin, int duration);
  void ledBlinkAll(int duration);

  void fadeUp(uint8_t pin, int duration);
  void fadeDown(uint8_t pin, int duration);
  void fadeUpAll(int duration);
  void fadeDownAll(int duration);

  //Array-based blink/fade functions
  void arrayLedBlink(uint8_t pins[], int duration, size_t arrsize);
  void fadeUpArray(uint8_t pins[], int duration, size_t arrsisze);
  void fadeDownArray(uint8_t pins[], int duration, size_t arrsize);

  //QOL functions
  void turnOnAll();
  void turnOffAll();
  void arrayOn(uint8_t arr[], size_t arrsize);
  void arrayOff(uint8_t arr[], size_t arrsize);
  int sensorValue();

private:

  int sensorVal;
  float timeStep = 1;
  const int msStep = 100;
  float fadingStepSize;
  int second = 1000;
  bool isOn = true;
  float gamma = 3.0;

  //Other functions
  void pinsOut();
  void pinsIn();

  void blinkThing();
  
  void pwm(uint8_t pin, float time);
  void allPWM(int time);
  void arrPWM(uint8_t pins[], int time, size_t arrsize);

  float gammaCorrect(float ratio) {
    float product = pow(ratio, gamma);
    return product;
  }

  float fractionOn = gammaCorrect(1);

  void updateFadingStepSize(int duration);
};

#endif //CPP_LSBLIBRARY_H