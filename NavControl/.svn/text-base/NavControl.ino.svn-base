
/*
*   Created 1/26/11
*
*    NavControl is the control center for managing the navigation system of Emma. 
*    It will be in charge of:
*      * Initialize all navigation systems
*      * Obtain current distance values from:
*        * IR Sensors
*            * A precise measuring device to detect the distance of an obstacle
*            * 4 sensors - Two facing forward and two facing rear  
*            * Placed horizontally on the lower corners of robot
*        * Ultrasonic Wave Sensors
*            * A secondary measuring device to measure distances obstacles
*            * 20 sensors total
*              * 3 on each side facing at a downward angle to detect changes in terrain ( stairs (up or down), curb, cliff, etc)
*              * 2 on each side placed horizontally to detect large obstacles ( wall, desk, chair, Professor Soules, etc)
*        * Accelerometer
*      * Process obstacle data and determine if drive system needs to be stopped to avoid accident
*      * Notify drive control of what action to take
*/

//#include "Arduino.h"
//#include "WProgram.h"
#include "defines.h"
#include "ultraSonic.h"
#include "DriveControl.h"

/**
* Array to hold IR sensor pin #'s
*/ 
const int irPins[numIRSensors] = {irSensorPinFL,
                                    irSensorPinFR,
                                    irSensorPinRL,
                                    irSensorPinRR};
/**
* Array of Two Most Recent IR Distance Values (FL,FR,RL,RR) <--- order of sensors in array
*/
float irDistances[numIRSensors][numIRDistances];

/**
* Array to hold Ultra Sonic sensor pin #'s
*/ 
const int ussPins[numUssSensors] = {ussPin0,
                                    ussPin1,
                                    ussPin2,
                                    ussPin3,
                                    ussPin4,
                                    ussPin5,
                                    ussPin6,
                                    ussPin7};

//make 4 instances of ultrasonic class
//analog pins are set up as follows
//front: right = 0  left = 1
//left: front = 2  rear = 3
//back: left = 4  right = 5
//right: rear = 6  front = 7
ultraSonicSensorPair frontPair(0,1);
ultraSonicSensorPair leftPair(2,3);
ultraSonicSensorPair backPair(4,5);
ultraSonicSensorPair rightPair(6,7);


/**
* Array to hold pin #'s which output to drive system
*/ 
int drivePins[6] = {interuptL,
                            interuptR,
                            dataL0,
                            dataL1,
                            dataR0,
                            dataR1};

// create an instance of DriveControl
DriveControl drive(drivePins);

void setup()
{  
  // set these pins as digital outputs
  pinMode(bwPin, OUTPUT);
  pinMode(interuptL, OUTPUT);
  pinMode(interuptR, OUTPUT);
  pinMode(dataL0, OUTPUT);
  pinMode(dataL1, OUTPUT);
  pinMode(dataR0, OUTPUT);
  pinMode(dataR1, OUTPUT);
  
  // Set Ultrasonic Sensors into a Chain
  frontPair.setupUsSensor(rxPinFront, bwPin);
  backPair.setupUsSensor(rxPinBack, bwPin);
  leftPair.setupUsSensor(rxPinLeft, bwPin);
  rightPair.setupUsSensor(rxPinRight, bwPin);
  
  // Initialize Drive Control Hardware Lines
  drive.initializePins();
    
  // this was previously at 9600 baud rate but the bus is running at 19200 so this has been changed    
  Serial.begin(19200);   
}

void loop()
{
  // get an updated distance from IR Sensors
  //getIRDistance(irPins, irDistances);  

  // get current distances from Ultrasonic sensors
  frontPair.getUltrasonicDistance_NEW();
  backPair.getUltrasonicDistance_NEW();
  leftPair.getUltrasonicDistance_NEW();
  rightPair.getUltrasonicDistance_NEW();
  
  drive.processSensors(leftPair.distance, rightPair.distance, frontPair.distance, backPair.distance);
  
  // call driveControl to update instructions to drive system
  drive.setInteruptPinValues();
  
  // print out last distances read
  print();
  
  delay(20);  
}
// Reset navigation system
void reset()
{
  // set these pins as digital outputs
  pinMode(bwPin, OUTPUT);
  pinMode(interuptL, OUTPUT);
  pinMode(interuptR, OUTPUT);
  pinMode(dataL0, OUTPUT);
  pinMode(dataL1, OUTPUT);
  pinMode(dataR0, OUTPUT);
  pinMode(dataR1, OUTPUT);
  
  // Set Ultrasonic Sensors into a Chain
  frontPair.setupUsSensor(rxPinFront, bwPin);
  backPair.setupUsSensor(rxPinBack, bwPin);
  leftPair.setupUsSensor(rxPinLeft, bwPin);
  rightPair.setupUsSensor(rxPinRight, bwPin);
  
  // set all drive control pins to low
  drive.resetDriveControl(drivePins);  
}

// Print out distances from all sensors
void print()
{  
  // cycle through the array of distances
  /*for(int i = 0; i < 1; i++)  //make sure to change these for loops to output the sensors you want... does not reflect polling code
  {
    Serial.print("IR Sensor #");
    Serial.print(i);
    Serial.print(" ");
    Serial.println(irDistances[i][0]);
  }*/
  
  //cycle through the array of ultrasonic distances
  Serial.print("FRONT(RIGHT: ");
  Serial.print(frontPair.distance[0][0]);//ussDistancesFront[0][0]);
  Serial.print("   LEFT: ");
  Serial.print(frontPair.distance[1][0]);//ussDistancesFront[1][0]);
  Serial.println(") \n");
  
  Serial.print("LEFT(FRONT: ");
  Serial.print(leftPair.distance[0][0]);//ussDistancesLeft[0][0]);
  Serial.print("   REAR: ");
  Serial.print(leftPair.distance[1][0]);//ussDistancesLeft[1][0]);
  Serial.println(") \n");
  
  Serial.print("REAR(LEFT: ");
  Serial.print(backPair.distance[0][0]);//ussDistancesBack[0][0]);
  Serial.print("   RIGHT: ");
  Serial.print(backPair.distance[1][0]);//ussDistancesBack[1][0]);
  Serial.println(") \n");
  
  Serial.print("RIGHT(REAR: ");
  Serial.print(rightPair.distance[0][0]);//ussDistancesRight[0][0]);
  Serial.print("   FRONT: ");
  Serial.print(rightPair.distance[1][0]);//ussDistancesRight[1][0]);
  Serial.println(") \n");
  Serial.println("************************************************** \n");
  
  delay(3000);
  
}


