/*
*    Created 2/1/11
*
*      The DriveControl is responsible for:
*        1) Analyze the previous data obtained from the USS Sensors
*        2) Determine if the interup lines should be set to High if normal operation or Low for take action
*        3) Set data lines high or low based on needs
*/


#ifndef DriveControl_h
#define DriveControl_h

#include "Arduino.h"
#include "defines.h"

class DriveControl
{
   public:
     DriveControl( int driveCtrlPins[6]); // constructor
     
     int pinStatus[6]; // value to be place on pin 0 = LOW, 1 = HIGH 
     
     void initializePins(); // initialize the values set on interupt/data pins
     
     void setInteruptPinValues(); // update values set on the interupt/data pins
    
     void resetDriveControl( int driveOutPins[6]); // reset the interupt/data pin values
     
     // process most recent values obtaine from ultrasonic sensors and decide what the interupt/data line values should be updated to
     void processSensors(float left[2][numUssDistances], float right[2][numUssDistances], float front[2][numUssDistances], float back[2][numUssDistances]);
     
     // pin numbers mapping to the arduino board for all the interupt pins used
     private:
       int digitalPin0;
       int digitalPin1;
       int digitalPin2;
       int digitalPin3;
       int digitalPin4;
       int digitalPin5;
    
};

#endif

