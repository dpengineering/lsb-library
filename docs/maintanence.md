---
layout: page
title: "Maintanence"
permalink: /maintanence/
---

### Note to future maintainers

This is a portion of the website designed to detail the inner workings of each function to make the library more maintainable in the future. This is not designed to detail the usage of the library. For that, see the [home page](https://tarekelkurjie.github.io/lsb-library/) of the site.

## Board class

All the functionality of the library is encapsulated in the Board class. This class contains all data & logic required for all the functions to work. Those include the different light value sensors for when the room is dark and when the sensor is covered. 

```c
Board::Board(int darkVal, int coveredVal, uint8_t sensorPin)
```
*Class Constructor*

## DPEADelay

DPEADelay is one of the most important functions of the entire codebase. I'm going to detail its functionality line by line.

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
        Serial.println(duration);
        delay(1);
    }
}
```

```duration = duration * second;```

There is a global variable called 'second'. This value is either 1.0 or 0. The delay value gets multiplied by this value, causing the length of the delay to either be whatever the parameter calls or 0. When the delay is zero, the program zips through all the code in a split second, returning back to the ```checkOnOff()``` function to change the state of the LSB.

```c
sensorVal = analogRead(sensorPin);
checkLight();
```

These lines update the sensor value and check this condition: 
```
if (sensorVal <= coveredVal || sensorVal >= darkVal) {turnOffAll(); second = 0; pinsIn(); return;}
```

This is checking if the current sensor value is lower than the coveredVal or darkVal instance variables. If so, it turns off all the LEDs, changes the second variable to 0, makes all pins input pins, and returns out of the function. I think most of this is self-explainatory, the only exception being switching all the pins to be input pins. This makes the lights nearly invisible, so whatever previously visible remnants from the remaining program would become invisible to the naked eye. This is a helper function that is used throughout the codebase, so come back to this section if necessary.





