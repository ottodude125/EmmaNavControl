/*
*    Created 1/25/11
* 	Jonathan Katon
*
*      The IR Sensor source code functionality is:
*        * Obtain IR Sensor voltage readings from an analog input on Arduino board for given list of sensors
*        * Convert into a distance in cm and store it into given array
*/

#include <math.h>

// method to obtain current distance measurement in centimeters
void getIRDistance(const int pins[], float irData[][numIRDistances])
{ 
  // var to hold value obtained from sensors
  float irVal = 0;  
 
  // get update of IR distances for all sensors
  for(int i = 0; i < numIRSensors; i++)
  {
    for(int j = 0; j < numIRDistances -1; j++)
    {
      // move pin i's previous distance into next array location
      irData[i][j+1] = irData[i][j];
    }
    
    // read voltage currently on pin i which is returned as an integer in the following range 0 =< irVal =< 1023
    // and convert it to a voltage  
    irVal = analogRead(pins[i])*(.00477539);
     
    // convert voltage into a distance in cm and add pin i's current distance into first array location
    irData[i][0] = 27.871*pow(irVal,-1.41);
  }
}
  
