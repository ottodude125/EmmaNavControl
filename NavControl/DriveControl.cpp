/*
*   Created 2/1/11
*   Developed by and tested by
*      * Jonathan Katon
*      * Mike B.
*
*      The DriveControl is responsible for:
*        1) Analyze the previous data obtained from the USS Sensors
*        2) Determine if the interupt lines should be set to High if normal operation or Low for take action
*        3) Set data lines high or low based on needs
*/


#include "DriveControl.h"


// Constructor
DriveControl::DriveControl(int driveCtrlPins[])
{
  digitalPin0 = driveCtrlPins[0]; // InteruptL
  digitalPin1 = driveCtrlPins[1]; // InteruptR
  digitalPin2 = driveCtrlPins[2]; // dataL0 
  digitalPin3 = driveCtrlPins[3]; // dataL1
  digitalPin4 = driveCtrlPins[4]; // dataR0
  digitalPin5 = driveCtrlPins[5]; // dataR1 
 
  for(int i = 0; i < numUssSensors; i++)
  {
    weightedAvg[i] = 0;
  }

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
// This method is a work in progress. The idea was that a weighted average would be incorporated
// So if a bad reading was taken it would not cause a huge problem
void DriveControl::processNewDistances(float left[2][numUssDistances], float right[2][numUssDistances], float front[2][numUssDistances], float back[2][numUssDistances])
{
  
  // update weighted averages
  for(int i = 0; i < numUssDistances; i++)
  {   
    float alpha = 2/(i+1);
    
    if(i == 0)
    {
      weightedAvg[0] = left[0][0];
      weightedAvg[1] = left[1][0];
      weightedAvg[2] = right[0][0];
      weightedAvg[3] = right[1][0];
      weightedAvg[4] = front[0][0];
      weightedAvg[5] = front[1][0];
      weightedAvg[6] = back[0][0];      
      weightedAvg[7] = back[1][0];      
    }
    else
    {
      weightedAvg[0] = weightedAvg[0] + alpha * (left[0][i] - weightedAvg[0]);
      weightedAvg[1] = weightedAvg[1] + alpha * (left[1][i] - weightedAvg[1]);
      weightedAvg[2] = weightedAvg[2] + alpha * (right[0][i] - weightedAvg[2]);
      weightedAvg[3] = weightedAvg[3] + alpha * (right[1][i] - weightedAvg[3]);
      weightedAvg[4] = weightedAvg[4] + alpha * (front[0][i] - weightedAvg[4]);
      weightedAvg[5] = weightedAvg[5] + alpha * (front[1][i] - weightedAvg[5]);
      weightedAvg[6] = weightedAvg[6] + alpha * (back[0][i] - weightedAvg[6]);
      weightedAvg[7] = weightedAvg[7] + alpha * (back[1][i] - weightedAvg[7]); 
    }
    //newValue = oldValue + alpha * (value - oldValue);
    //total[0]  = total[i] + alpha * (total[i+1] - total[i]);
  }
  
    /*
     class ExponentialMovingAverage {
     private double alpha;
     private Double oldValue;
     public ExponentialMovingAverage(double alpha) {
     this.alpha = alpha;
     }
     
     public double average(double value) {
     if (oldValue == null) {
     oldValue = value;
     return value;
     }
     double newValue = oldValue + alpha * (value - oldValue);
     oldValue = newValue;
     return newValue;
     }
     }
*/
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
