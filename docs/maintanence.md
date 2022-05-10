layout: page
title: "Maintanence"
permalink: /maintanence/

### Note to future maintainers

This is a portion of the website designed to detail the inner workings of each function to make the library more maintainable in the future. This is not designed to detail the usage of the library. For that, see the [home page](https://tarekelkurjie.github.io/lsb-library/) of the site.

## Board class

All the functionality of the library is encapsulated in the Board class. This class contains all data & logic required for all the functions to work. Those include the different light value sensors for when the room is dark and when the sensor is covered. 

```c
Board::Board(int darkVal, int coveredVal, uint8_t sensorPin)
```
*Class Constructor*

## DPEADelay

DPEADelay is one of the most important functions of the entire codebase. So, I'm going to detail its functionality line by line.

```c
void Board::DPEADelay(float duration) {
    duration = duration * second;
    sensorVal = analogRead(sensorPin);
    checkLight();
    duration *= second;
    for (int i = 0; i < duration; i++) {
        sensorVal = analogRead(sensorPin);
        checkLight();
        duration *= second;
        delay(1);
    }
}
```

```duration = duration * second;```

There is a global variable called 'second'. This value is either 1.0 or 0. The delay value gets multiplied by this value, causing the length of the delay to either be whatever the parameter calls or 0. When the delay is zero, the program zips through all the code in a split second, returning back to the ```checkOnOff()``` function to change the state of the LSB.

```c
for (int i = 0; i < duration; i++)
```

This line is fairly simple, it iterates over each millisecond in the delay duration. I just felt it was necessary to cover as it will be important later.

```c
sensorVal = analogRead(sensorPin);
checkLight();
```

These lines update the sensor value and check this condition: 
```
if (sensorVal <= coveredVal || sensorVal >= darkVal) {turnOffAll(); second = 0; pinsIn(); return;}
```

This is checking if the current sensor value is lower than the coveredVal or darkVal instance variables. If so, it turns off all the LEDs, changes the second variable to 0, makes all pins input pins, and returns out of the function. I think most of this is self-explainatory, the only exception being switching all the pins to be input pins. This makes the lights nearly invisible, so whatever previously visible remnants from the remaining program would become invisible to the naked eye. This is a helper function that is used throughout the codebase, so come back to this section if necessary.

```c
duration *= second;
```

If the ```checkLight()``` function updates any state, we will need to react to it. So, each time we check that condition, we also multiply the duration by the second variable. This means if the second variable gets updated to a value of 0, the duration will update, causing the for loop to end and the program to zip through to its end, where its state can be updated properly.

```c
delay(1);
```

This line actually preforms the raw Arduino delay. It simply runs a 1 millisecond delay for each iteration of the for loop. 

*Note: All this calculation, sensor reading, and the like does make the delay itself slightly longer, and this was noticeable when dealing with microsecond values, but we assumed the difference was so marginal it didn't make enough of a difference for the larger, millisecond delays*

### checkOnOff

This is another function necessary for the LSB to function properly. I will also detail the workings of this function line by line.

```c
void Board::checkOnOff(void (*code)()) {
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
```

```c
pinsOut();
```

The function starts by setting all the pins to output. If you recall from the DPEADelay function, by the time this function is reached there is a possibility all the pins are set to input, so we need to reset that condition as the student's code itself has already finished running.

```c
if (sensorVal > darkVal) {turnOffAll();}
```

After updating the value of the sensor, we check it against the darkVal variable. If the sensor is reading greater than the darkVal variable, we need to turn off all the pins, as the LEDs should remain off when the LSB is not in the dark.

```c
if (isOn == true)
```

Another board state variable used in the LSB is the 'isOn' variable. This is a boolean variable that stores whether the board is on or not. 

```c
if (sensorVal <= coveredVal) {turnOffAll(); isOn = false; blinkThing(); blinkThing(); delay(50);}
```

This line checks if the sensor value is less than the coveredVal constant. If so, it turns off all LEDs, sets isOn to false, and blinks twice using the aptly named 'blinkThing()' private function. It then does a short 50 millisecond delay.

```c
else {code();}
```

If the previous condition is false, it runs the code provided by the function parameter. After doing either of these it updates the sensor value.

```c
if (sensorVal <= coveredVal) {blinkThing(); blinkThing(); delay(50); code(); isOn = true;}
```

If isOn is false, and the value of the sensor does read less than the coveredVal constant, the program blinks twice, delays, runs the user provided code and sets the isOn state variable to true. Pretty simple.

```c
else {turnOfAll();}
```

Otherwise, it turns off all the pins once again and doesn't update any state.

All of these are followed by an updating of the sensor value.

## Fades

One of the most important functions used by the DPEA students in the LSB library is the fade functions. These functions need to fade up/down a given pin/set of pins for the given duration, while using exclusively digital (NOT analog) pins. We accomplish this by using a psuedo-PWM method that rapidly turns on and off a pin, so fast that it appears to be getting dimmer/brighter over time.

All the fades are architecturally the same, with just some numbers moved around and/or some iteration, so I will be only covering the function that fades up a single pin.

```c
void Board::fadeUp(uint8_t pin, int duration) {
    if (duration >= 1000) duration = 999;
    for (float fractionOn = 0; fractionOn < 1; fractionOn += fadingStepSize) {
        pwm(pin, gammaCorrect(fractionOn) * 10000);
        updateFadingStepSize(duration);
    }
    digitalWrite(pin, HIGH); // make sure pin is at full power before leaving fadeUp()
}
```

The first line is a gaurd clause preventing the duration from going over a thousand, which is the upper bound for making the fade nice and smooth. 

```c
for (float fractionOn = 0; fractionOn < 1; fractionOn += fadingStepSize)
```

This is the iterator controlling how long our fade is going to be. It isn't really counting anything; updateFadingStepSize() is really what's controlling the length of the fade. This simply adds the fading step size to the variable each time the loop passes. This value will be used as a ratio later on in the function. 

```c
pwm(pin, gammaCorrect(fractionOn) * 10000);
```

This is the line where the magic happens. It calls the pwm function - which we whill cover in a second - and passes in the desired pin followed by the duration of the pulse, calculated by the inline 'gammaCorrect()' function times ten thousand. 

gammaCorrect is a function designed to help the fade look smoother to the human eye. It simply raises the ratio to the third power and returns the result. We multiply it by 10,000 to get the duration needed for the PWM pulse. 

```c
void Board::pwm(uint8_t pin, float time) {
    sensorVal = analogRead(sensorPin);
    checkLight();

    digitalWrite(pin, HIGH);
    DPEADelayMicroseconds(time);
    digitalWrite(pin, LOW);
    DPEADelayMicroseconds(10000 - time);
}
```

This is what the PWM function loops like. It starts by updating the light sensor value as well as checking the light condition we covered previously with DPEADelay. 

The function then turns on and off the pin very quickly - in a matter of microseconds. The delay function we're using is DPEADelayMicroseconds, which is nearly identical under the hood compared to DPEADelay, except it is working with microseconds instead of milliseconds. With a lot of these pwm pulses chained together with different lengths, we can get an effect similar to an analog fade. Cool!

```c
updateFadingStepSize(duration);
```

This function is the final piece in the fading puzzle. It adjusts the fadingStepSize variable in relation to the duration provided as a function parameter. The math is a little weird, and uses a lot of magic constants, so I'm not going to get into it right now, but if you'd like here's what the function looks like.

```c
void Board::updateFadingStepSize(int duration) {
    float stepsOfDuration = duration * (1 - (750 / 768)) / 10;
    fadingStepSize = 1 / stepsOfDuration;
}
```

That's basically all the meat of the library. You can take a look at the other functions on the repo, but they're not worth getting into in this explaination due to their simplicity, and I much rathered going into depth about what all the core functions are doing. 


