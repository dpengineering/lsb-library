#include <Arduino.h>
#include "LSBLibrary.h"


const int coveredVal = 40;      //Change this to be the value of your sensor when it is covered
const int darkVal = 150;        //Change this value to be the brightness of a dark room



/////////////////////////////////////////////////////////////////////////////////////////
//                                                                                     //
//  DO NOT EDIT ANY CODE OUTSIDE OF THE CODE() FUNCTION UNLESS STRICTLY TOLD TO DO SO  //
//                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////

Board LSB(coveredVal, darkVal, A2);

void setup() { //DO NOT TOUCH
   LSB.setupAll();
}

void loop() { //DO NOT TOUCH
   LSB.checkOnOff(code);
}


// ONLY EDIT CODE PAST THIS POINT


void code() {
    //This is where you will write the code for your program :)
   LSB.fadeUp(8, 1000);
   LSB.fadeDown(8, 1000);
}
