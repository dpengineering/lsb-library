# LSB Library Documentation

**Getting Started**

First, navigate to the Class Examples folder. Then, navigate to the programming folder. There should be a folder in there called YourName_LuminismShadowBox. Copy that to your desktop.

Open up the copied folder and double click on the file called 'YourName_LuminismShadowBox.ino'. This should open up the Arduino IDE. Now, you're ready to get started. You can start writing your code in the ```code()``` function. ***DO NOT*** alter the contents of ANY other function in the program unless strictly instructed to do so.

## Quality of Life/Maintanence


```cpp
void Board::DPEADelay(int time) 
```

The DPEADelay function is used as a replacement from Arduino's ```delay(int time)``` function, with the added functionality of quickly checking the value of the light sensor each second, allowing the LSB to react to changes in light.


```c
void Board::checkOnOff(void (*code)())
```

This function is integral to the way the LSB works. It takes in a [function pointer](https://www.geeksforgeeks.org/function-pointer-in-c/) and runs the given function, while constantly checking the 'state' of the LSB. This 'state' is changed by helper functions such as DPEADelay. If the state is changed, the checkOnOff function will react to that change.

```c
void Board::setupAll() 
```

Sets up the LSB by setting pinModes and other things needed at startup.

## Blinks

```c 
void Board::ledBlink(uint8_t pin, int duration)
```
Blinks the given pin for the given duration (in milliseconds).

Example:

```c
Board LSB(coveredVal, darkVal, A2);

...

void code() {
  // Blinks pin 3 for 2 seconds.
  LSB.ledBlink(3, 2000);
}
```

```c
void Board::ledBlinkAll(int duration)
```
Blinks all LEDs for the given duration (in milliseconds).

Example:

```c
Board LSB(coveredVal, darkVal, A2);

...

void code() {
  // Blinks all pins for 1 second.
  LSB.ledBlinkAll(1000);
}
```

```c
void Board::arrayLedBlink(uint8_t pins[], int duration)
```
Blinks all LEDs in the given array for the given duration (in milliseconds).

Example: 

```c
Board LSB(coveredVal, darkVal, A2);

...

void code() {
  uint8_t pins[] = {1, 2, 3, 4};
  // Blinks pins 1, 2, 3, and 4 for 3 seconds.
  LSB.ledBlink(pins, 3000); // note the removal of the brackets
}
```

## Fades

```c
void Board::fadeUp(uint8_t pin, int duration)
```
Fades up the given pin for the given duration (in milliseconds).

Example:

```c
Board LSB(coveredVal, darkVal, A2);

...

void code() {
  // Fades up pin 3 for 2 seconds.
  LSB.fadeUp(3, 2000);
}
```

```c
void Board::fadeDown(uint8_t pin, int duration)
```
Fades down the given pin for the given duration (in milliseconds).

Example:

```c
Board LSB(coveredVal, darkVal, A2);

...

void code() {
  // Fades down pin 4 for 0.5 seconds.
  LSB.fadeDown(4, 500);
}
```

```c
void Board::fadeUpAll(int duration)
```
Fades up all pins for the given duration (in milliseconds).

Example: 

```c
Board LSB(coveredVal, darkVal, A2);

...

void code() {
  // Fades up all pins for 1 second.
  LSB.fadeUpAll(1000);
}
```

```c
void Board::fadeDownAll(int duration)
```
Fades down all pins for the given duration (in millinseconds).

Example:

```c
Board LSB(coveredVal, darkVal, A2);

...

void code() {
  // Fades down all pins for 1 second.
  LSB.fadeDownAll(1000);
}
```

```c
void Board::fadeUpArray(uint8_t pins[], int duration)
```
Fades up all pins in the given array for the given duration (in milliseconds).

Example:

```c
Board LSB(coveredVal, darkVal, A2);

...

void code() {
  uint8_t pins[] = {5, 4, 3, 2, 1};
  // Fades up pins 5, 4, 3, 2, and 1 for 0.2 seconds.
  LSB.fadeUpAll(pins, 200);
}
```

```c
void Board::fadeUpArray(uint8_t pins[], int duration)
```
Fades down all pins in the given array for the given duration (in milliseconds).

Example:

```c
Board LSB(coveredVal, darkVal, A2);

...

void code() {
  uint8_t pins[] = {5, 4, 3, 2, 1};
  // Fades down pins 5, 4, 3, 2, and 1 for 0.2 seconds.
  LSB.fadeDownArray(pins, 200);
}
```
