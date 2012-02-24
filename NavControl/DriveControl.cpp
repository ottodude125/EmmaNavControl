/*
*    Created 2/1/11
*
*      The DriveControl is responsible for:
*        1) Analyze the previous data obtained from the USS Sensors
*        2) Determine if the interup lines should be set to High if normal operation or Low for take action
*        3) Set data lines high or low based on needs
*/



#include "DriveControl.h"
#include "ultraSonic.h"


// Constructor
DriveControl::DriveControl(int driveCtrlPins[])
{
  digitalPin0 = driveCtrlPins[0]; // InteruptL
  digitalPin1 = driveCtrlPins[1]; // InteruptR
  digitalPin2 = driveCtrlPins[2]; // dataL0 
  digitalPin3 = driveCtrlPins[3]; // dataL1
  digitalPin4 = driveCtrlPins[4]; // dataR0
  digitalPin5 = driveCtrlPins[5]; // dataR1  
}

// initialize the interupts to high and data bytes to low
void DriveControl::initializePins()
{
    // Set InteruptL HIGH
  digitalWrite(digitalPin0, HIGH);
  // Set InteruptR HIGH
  digitalWrite(digitalPin1, HIGH);
  
  // set all drive control data pins to LOW
  digitalWrite(digitalPin2, LOW);
  digitalWrite(digitalPin3, LOW);
  digitalWrite(digitalPin4, LOW);
  digitalWrite(digitalPin5, LOW);   

}

// Update the values on each interupt pin based on decision from processSensors
// If it is a 1 then set pin to high and if it is 0 set the pin to low
void DriveControl::setInteruptPinValues()
{
   // if interuptL is set to 1 keep the pin high
   if(pinStatus[0] == 1)
      digitalWrite(digitalPin0, HIGH);
   // otherwise set it to low indicating action should be taken
   else 
      digitalWrite(digitalPin0, LOW);
      
   // if interuptR is set to 1 keep the pin high
   if(pinStatus[1] == 1)
      digitalWrite(digitalPin1, HIGH);
   // otherwise set it to low indicating action should be taken
   else 
      digitalWrite(digitalPin1, LOW);

   // if dataL0 is set to 1 set the pin high
   if(pinStatus[2] == 1)
      digitalWrite(digitalPin2, HIGH);
   // otherwise keep it low
   else 
      digitalWrite(digitalPin2, LOW);
      
   // if dataL1 is set to 1 set the pin high
   if(pinStatus[3] == 1)
      digitalWrite(digitalPin3, HIGH);
   // otherwise keep it low
   else 
      digitalWrite(digitalPin3, LOW);
      
   // if dataR0 is set to 1 set the pin high
   if(pinStatus[4] == 1)
      digitalWrite(digitalPin4, HIGH);
   // otherwise keep it low
   else 
      digitalWrite(digitalPin4, LOW);
      
   // if dataR1 is set to 1 set the pin high
   if(pinStatus[5] == 1)
      digitalWrite(digitalPin5, HIGH);
   // otherwise keep it low
   else 
      digitalWrite(digitalPin5, LOW);   
      
  
}

// process the ultrasonic sensors and determine what the new interupt values should be set to
void DriveControl::processNewDistances(float left[2][numUssDistances], float right[2][numUssDistances], float front[2][numUssDistances], float back[2][numUssDistances])
{
    float distances[8][numUssDistances];
  //populate array with distanceinformation
   /*distance[0][0] = 
   distance[1][0] =
   distance[2][0] =
   distance[3][0] =
   distance[4][0] =
   distance[5][0] =    ill get to this...
   distance[6][0] =
   distance[7][0] =*/
  
}

// reset the drive control system
void DriveControl::resetDriveControl( int driveCtrlPins[])
{
  // Set InteruptL HIGH
  digitalWrite(digitalPin0, HIGH);
  // Set InteruptR HIGH
  digitalWrite(digitalPin1, HIGH);
  
  // set all drive control data pins to LOW
  digitalWrite(digitalPin2, LOW);
  digitalWrite(digitalPin3, LOW);
  digitalWrite(digitalPin4, LOW);
  digitalWrite(digitalPin5, LOW);   

}
