//------------------------------------------------------------------------------------------------
// Accelerometer
// Project date - 31 January 2011
// Navigation Team
// Team Members: Ryan Kelly, Mathew Comeau, Jonathan Katon, Mike B., Mike B., Nate Damon
//
// Purpose: This sketch will implement the accelerometer data.  It will calculate the position
// and orientation of Emma 5 and feed it back to Houston.
// 
// The following Code has been written primarily by Ryan Kelly as of 5/2011
//-------------------------------------------------------------------------------------------------

#include <math.h>
#include <float.h>
#include <limits.h>


#define gs1 22
#define gs2 23
#define xPin 13
#define yPin 14
#define zPin 15

int xVal = 0;
int yVal = 0;
int zVal = 0;

float xa = 0;
float ya = 0;
float za = 0;

int xMin = INT_MAX;
int xMax = 0;
int yMin = INT_MAX;
int yMax = 0;
int zMin = INT_MAX;
int zMax = 0;

int g0x = 0;
int g0y = 0;
int g0z = 0;

float rho = 0;
float phi = 0;
float theta = 0;

/*
void accelSetup()
{
  Serial.begin(9600);
  
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(zPin, INPUT);
}
*/

//This function was taken straight from a source code I discovered online
void autoZeroCalibration(int pfx, int pfy, int pfz)
{
  if ((pfx < xMin)||(pfy < yMin)||(pfz < zMin)||(pfx > xMax)||(pfy > yMax)||(pfz > zMax))
  {
    if (pfx < xMin)
      xMin = pfx;
    if (pfy < yMin)
      yMin = pfy;
    if (pfz < zMin)
      zMin = pfz;
    
    if (pfx > xMax)
      xMax = pfx;
    if (pfy > yMax)
      yMax = pfy;
    if (pfz > zMax)
      zMax = pfz;
      
    g0x = ((xMax - xMin)/2) + xMin;
    g0y = ((yMax - yMin)/2) + yMin;
    g0z = ((zMax - zMin)/2) + zMin;
    
    //printValues();
  }
}
  
void accelerometer()
{
  xVal = analogRead(xPin);        //read value for x-axis
  yVal = analogRead(yPin);        //read value for y-axis
  zVal = analogRead(zPin);        //read value for z-axis
  
  //begin calculating the angles for the orientation of the frame
  int pfx = xVal;
  int pfy = yVal;
  int pfz = zVal;
  
  //calibrate the accelerometer
  autoZeroCalibration(pfx, pfy, pfz);
  
  //set the values being used to the new calibrated values
  int fx = pfx - g0x;
  int fy = pfy - g0y;
  int fz = pfz - g0z;
  
  //covert values from int to float. The 3.3 is the source voltage and the 0.800 is used to normalize the value since the output
  //of the accelerometer is 800 mV/g
  float ax = fx*(3.3/(1024.0*0.800));
  float ay = fy*(3.3/(1024.0*0.800));
  float az = fz*(3.3/(1024.0*0.800));
  
  //Print the values in volts. When at rest, the voltage should be half of the supply voltage.  With foward acceleration, voltage rises,
  //and vice versa with reverse voltage 
  Serial.print("voltage on x-axis  ");
  Serial.print(ax);
  Serial.println(" volts");
  Serial.print("voltage on y-axis ");
  Serial.println(ay);
  Serial.println(" volts");
  Serial.print("voltage on z-axis ");
  Serial.println(ax);
  Serial.println(" volts");
  Serial.println("\n");
  
  //Use pythagoreans thereom to calclate the 3 angles with respect to the Earth
  rho = atan(ax/sqrt(pow(ay,2)+pow(az,2)))*(360/(2*3.141592));
  phi = atan(ay/sqrt(pow(ax,2)+pow(az,2)))*(360/(2*3.141592));
  theta = atan(sqrt(pow(ax,2)+pow(ay,2)))*(360/(2*3.141592));
  
  //Print the values in radians
  Serial.print("rho= ");
  Serial.print(rho);
  Serial.println(" radians");
  Serial.print("phi = ");
  Serial.println(phi);
  Serial.println(" radians");
  Serial.print("theta = ");
  Serial.println(theta);
  Serial.println(" radians");
  Serial.println("\n");
  
} 
