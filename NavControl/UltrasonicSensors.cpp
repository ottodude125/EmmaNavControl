/*
*    Created 1/25/11
*
*      The Ultrasonic Sensor source code obtains the latest distance readings from
*      given sensors and writes it into the most recent array location
*/


#include "ultraSonic.h"


// Constructor
ultraSonicSensorPair::ultraSonicSensorPair(int pin1, int pin2)
{
  //pass in analog pins the sensor pair will be using and set them to variables internal to the class
  analogPin1 = pin1;
  analogPin2 = pin2;
}

// initialize up the sensor loops (each pair of sensors is in a loop so they don't interfere with each others readings)
void ultraSonicSensorPair::setupUsSensor(int Rx, int Bw)
{
  // Set BW HIGH
  digitalWrite(Bw, HIGH);

  // Start sensors
  digitalWrite(Rx, HIGH);  
  // Then hold RX high for 20-50 microseconds
  delayMicroseconds(20);
  // Finally return to "HIGH IMPEDANCE STATE"
  pinMode(Rx, INPUT); 
 
}

// method shifts distance values in array based on age of value and then obtains the current distance
float ultraSonicSensorPair::getUltrasonicDistance()
{
   // loop through array to move distances to older location (0-new, numUssDistances-old)
   for(int i = 0; i < numUssDistances-1; i++)
   {
     for (int j = 0; j < 2; j++)
     {
       distance[j][i+1] = distance[j][i];       
     }
   }
   
   // add pin i's current distance into first array location
   distance[0][0] = analogRead(analogPin1)*1.27; // factor of 1.27 to convert analog read value into cm
   distance[1][0] = analogRead(analogPin2)*1.27; // factor of 1.27 to convert analog read value into cm
 
}


