/*
*   Created 1/25/11
*   Developed by and tested by
*      * Jonathan Katon
*      * Mike B.
*
*      The Ultrasonic Sensor source code obtains the latest distance readings from
*      given sensors and writes it into the most recent array location
*/

#ifndef ultraSonic_h
#define ultraSonic_h

#include "Arduino.h"
#include "defines.h"


class ultraSonicSensorPair
{
   public:
     // Constructor
     ultraSonicSensorPair(int pin1, int pin2);
     
     // method it initialize the Ultrasonic Sensors and set them cycling
     void setupUsSensor(int Rx, int Bw);  

     // get current distance
     float getUltrasonicDistance();
     
     // array holding the measured distances from sensors
     float distance[2][numUssDistances];

   private:
     int analogPin1;
     int analogPin2;
};

#endif
