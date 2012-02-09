#ifndef ultraSonic_h
#define ultraSonic_h

#include "Arduino.h"
#include "defines.h"

class ultraSonicSensorPair
{
   public:
     ultraSonicSensorPair(int pin1, int pin2);//constructor
     float distance[2][numUssDistances];  

    //all functions related to this class will have the NEW modifier
    //remove once implemented.  Either delete or add OLD modifier to old code base
    float getUltrasonicDistance_NEW();
    

    // method it initialize the Ultrasonic Sensors and set them cycling
    void setupUsSensor(int Rx, int Bw);
   
    
   private:
     int analogPin1;
     int analogPin2;
};

#endif
