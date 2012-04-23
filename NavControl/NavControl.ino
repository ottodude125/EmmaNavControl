/*
*   Created 1/26/11
*
*   Developed by and tested by
*      * Jonathan Katon
*      * Mike B.
*   With additional assistance on network comminication by
*      * Mathew Comeau
*      * Thomas Pavlu
*
*    NavControl is the control center for managing the navigation system of Emma. 
*    It will be in charge of:
*      * Initialize all navigation systems
*      * Obtain current distance values from:
*        * Ultrasonic Wave Sensors
*            * A device to measure distances between robot and an obstacle
*            * 20 sensors total
*              * 3 on each side facing at a downward angle to detect changes in terrain ( stairs (up or down), curb, cliff, etc)
*              * 2 on each side placed horizontally to detect large obstacles ( wall, desk, chair, Professor Soules, etc)
*      * Process obstacle data and determine if drive system needs to be stopped to avoid accident
*      * Notify drive control of what action to take
*      * Provide data requested by bus
*/


#include "Arduino.h"
#include "defines.h"
#include "ultraSonic.h"
#include "DriveControl.h"
#include "BusDriver.h"

extern BusDriver EmmaBus;


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
* Array to hold pin #'s which control drive system interupts
*/ 
int drivePins[6] = {interuptL,
                            interuptR,
                            dataL0,
                            dataL1,
                            dataR0,
                            dataR1};

// create an instance of DriveControl
DriveControl drive(drivePins);

// set true when a request from the bus has been received
bool busDataRequest = false;

// holds data to be transmitted over the bus
uint8_t dangerousData[5];


// initialize pins, set up ultrasonic sensor loop pairs, set baud rate
void setup()
{  
  pinMode(13, OUTPUT);
  
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
  
  // Initialize Drive Interupt Lines
  drive.initializePins();
   
   
  /************************************************************************************************************/ 
  /**** EmmaBus.init() replaces the Serial.begin required by Arduino. Uncomment Serial.begin() and comment ****/
  /**** out EmmaBus.init() and all EmmaBus.addFunctionToMyLut() when you want to run the code normally     ****/
  /************************************************************************************************************/ 
  // Setup the EmmaBus
  EmmaBus.init(19200);
  //Serial.begin(19200);
  
  
  
  // Initialize the functions which will receive calls from the bus
  EmmaBus.addFunctionToMyLUT(sendFrontRightUssDistance, 0);
  EmmaBus.addFunctionToMyLUT(sendFrontLeftUssDistance, 1);
  EmmaBus.addFunctionToMyLUT(sendLeftFrontUssDistance, 2);
  EmmaBus.addFunctionToMyLUT(sendLeftRearUssDistance, 3);  
  EmmaBus.addFunctionToMyLUT(sendRearLeftUssDistance, 4);  
  EmmaBus.addFunctionToMyLUT(sendRearRightUssDistance, 5);  
  EmmaBus.addFunctionToMyLUT(sendRightRearUssDistance, 6);
  EmmaBus.addFunctionToMyLUT(sendRightFrontUssDistance, 7);

}

/****************************/
/*** Lovely Lollipop Loop ***/
/****************************/
void loop()
{

  // get current distances from Ultrasonic sensors
  frontPair.getUltrasonicDistance();
  backPair.getUltrasonicDistance();
  leftPair.getUltrasonicDistance();
  rightPair.getUltrasonicDistance();
  
  // process the distances and determine the drive interupt line values
  //drive.processNewDistances(leftPair.distance, rightPair.distance, frontPair.distance, backPair.distance);
  
  // call driveControl to update interupt lines to drive system
  //drive.setInteruptPinValues();
   
  // print out last distances read from sensors
  // In order to run print() the bus EmmaBus.init() must be commented out and Serial.begin() uncommented per instructions provided in setup() above
  //print();
  
  if(busDataRequest == true)
  {
    EmmaBus.writeDangerous(dangerousData);
    busDataRequest == false;
  }
  

}

/*******************************/
/*** Reset navigation system ***/
/*******************************/
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

/****************************************************************************************************/
/*** Methods for answering data requests from EmmaBrain. Each request returns five numbers.       ***/
/*** The first number is the address of where the data is going, the second number is the command ***/
/*** which was issued, and the third number is the address of the navigation system.              ***/
/*** Following this are two bytes which are data you want to send. The first eight methods below  ***/
/*** return the distance for a single sensor. The distance read from a sensor has been processed  ***/
/*** in UltrasonicSensors.cpp and broken down into two unsigned bytes. These two bytes are then   ***/
/*** The distance is in the form of two unsigned bytes which are sent in the same packet          ***/
/****************************************************************************************************/
void sendFrontLeftUssDistance(uint8_t* bytePtr)
{
  busDataRequest = true;
  dangerousData[0] = bytePtr[2];
  dangerousData[1] = bytePtr[1];
  dangerousData[2] = MY_ADDRESS;
  dangerousData[3] = frontPair.ussDistances4Brain[0];
  dangerousData[4] = frontPair.ussDistances4Brain[1];
}

void sendFrontRightUssDistance(uint8_t* bytePtr)
{
  busDataRequest = true;
  dangerousData[0] = bytePtr[2];
  dangerousData[1] = bytePtr[1];
  dangerousData[2] = MY_ADDRESS;
  dangerousData[3] = frontPair.ussDistances4Brain[2];
  dangerousData[4] = frontPair.ussDistances4Brain[3];		
}

void sendLeftRearUssDistance(uint8_t* bytePtr)
{
  busDataRequest = true;
  dangerousData[0] = bytePtr[2];
  dangerousData[1] = bytePtr[1];
  dangerousData[2] = MY_ADDRESS;
  dangerousData[3] = leftPair.ussDistances4Brain[0];
  dangerousData[4] = leftPair.ussDistances4Brain[1];				
}

void sendLeftFrontUssDistance(uint8_t* bytePtr)
{
  busDataRequest = true;
  dangerousData[0] = bytePtr[2];
  dangerousData[1] = bytePtr[1];
  dangerousData[2] = MY_ADDRESS;
  dangerousData[3] = leftPair.ussDistances4Brain[2];
  dangerousData[4] = leftPair.ussDistances4Brain[3];				
}

void sendRearLeftUssDistance(uint8_t* bytePtr)
{
  busDataRequest = true;
  dangerousData[0] = bytePtr[2];
  dangerousData[1] = bytePtr[1];
  dangerousData[2] = MY_ADDRESS;
  dangerousData[3] = backPair.ussDistances4Brain[0];
  dangerousData[4] = backPair.ussDistances4Brain[1];				
}

void sendRearRightUssDistance(uint8_t* bytePtr)
{
  busDataRequest = true;
  dangerousData[0] = bytePtr[2];
  dangerousData[1] = bytePtr[1];
  dangerousData[2] = MY_ADDRESS;
  dangerousData[3] = backPair.ussDistances4Brain[2];
  dangerousData[4] = backPair.ussDistances4Brain[3];				
}

void sendRightRearUssDistance(uint8_t* bytePtr)
{
  busDataRequest = true;
  dangerousData[0] = bytePtr[2];
  dangerousData[1] = bytePtr[1];
  dangerousData[2] = MY_ADDRESS;
  dangerousData[3] = rightPair.ussDistances4Brain[0];
  dangerousData[4] = rightPair.ussDistances4Brain[1];				
}

void sendRightFrontUssDistance(uint8_t* bytePtr)
{
  busDataRequest = true;
  dangerousData[0] = bytePtr[2];
  dangerousData[1] = bytePtr[1];
  dangerousData[2] = MY_ADDRESS;
  dangerousData[3] = rightPair.ussDistances4Brain[2];
  dangerousData[4] = rightPair.ussDistances4Brain[3];				
}


/*************************************/
/*** METHODS FOR TESTING BUSDRIVER ***/
/*************************************/

// cplit up a float into two bytes
void splitFloat()
{
  float test[2];
  test[0] = 786.123;
  
  uint8_t byte0 = (int)(test[0]) & 0x000000ff;
  uint8_t byte1 = (int)(test[0]) & 0x0000ff00;
  
  Serial.print("first: ");
  Serial.print(byte0);
  Serial.print("  second: ");
  Serial.print(byte1);
}

// flash some led's
void LED()
{
  static int ptrLED=3;
  static bool polarity=true;
  if(polarity) 
    digitalWrite(ptrLED, HIGH);
  else 
    digitalWrite(ptrLED, LOW);
	
  ptrLED++;
  
  if(ptrLED==11) 
  {
    ptrLED=3;
    polarity = !polarity;
  }
}

// Print out distances from all sensors
// Cannot be used when EmmaBus.init() is used
void print()
{  
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


