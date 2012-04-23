//For now and now get rid of summing and averages, get it to work with just what is going on now, and then move into arrays


// accelerometer part #    A7260
//                        K911014
// Yellow and Purple wire need to both input 00 into the accelerometer. yellow wire is in arduino pin #22, purple is in #23
//x output is blue wire arduino pin 13, y output is orange wire arduino pin 14, z output is red wire arduino pin 15. 

#define maxCurb 0.1016      //max height of scalable curb

#include <math.h> 

//definitions
#define angle 0.972412853719443        //1.2065 meters is bottom of triangle 0.56515 is side
#define sensorElev 0.5842             //.186182 meters

//ultrasonic
#define bw 24
#define rx 25
#define aAN 0
#define bAN 1

//accelerometer
#define gs1 22
#define gs2 23
#define xPin 13
#define yPin 14
#define zPin 15
#define offset 1690

// mV/g for each axis
#define sz 830
#define sx 850
#define sy 680

#define g 9.81
#define numSampls 50  //Trying a lower number, original was 100   

#define slowVal 1.1144
#define stopVal 0.5064
#define slowValu 0.1524
#define stopValu 0.0508

#define inclineLimit 2  //max incline
#define declineLimit 2  //max decline

//pins for testing, will be replaced by writes to the bus
#define testSlow 51
#define testStop 48
#define testGo 50


//global variables
int SumX, SumY, SumZ, Xavg, Yavg, Zavg;
//float Xavg, Yavg, Zavg, aX, aY, aZ, vX, vY, vZ, pvX, pvY, pvZ, cpY, tElev, tDist;
float aX, aY, aZ, vX, vY, vZ, pvX, pvY, pvZ, cpY, tElev, tDist;
unsigned long dt, prevTime;
int yPs[10];
int tEs[10];
int aVal = 0;             //The value read from pin A0
//int bVal = 0;             //The value read for pin A2
int aSum = 0;             //Sum of the read values of a
//int bSum = 0;             //Sum of the read values of b 
//float bAvg = 0;             //Average of the sums of b
float aAvg = 0;
float sensorInput;
float slope;

//setup
void setup(){
  
  Serial.begin(9600);
  
  //testing
  pinMode(testSlow, OUTPUT);
  pinMode(testStop, OUTPUT);
  pinMode(testGo, OUTPUT);
  
  
  //pinMode(gs1, OUTPUT);
  //pinMode(gs2, OUTPUT);
  
  pinMode(bw, OUTPUT);
  pinMode(rx, OUTPUT);
  
  
   //analog inputs
  pinMode(aAN, INPUT);
  pinMode(bAN, INPUT);
  
  //hold BW HIGH
  digitalWrite(bw, HIGH);

    
  //start sensors
  digitalWrite(rx, HIGH);
  //hold RX high for 20-50 microseconds
  delayMicroseconds(20);
  pinMode(rx, INPUT);    //return to "HIGH IMPEDANCE STATE"
  
  
  //digitalWrite(gs1, LOW);
  //digitalWrite(gs2, LOW);
  Serial.begin(9600);
}


void loop(){
  
  
  //it would probably be better to alternate an accelerometer and ultrasonic reads
  
  
  //get brief average of accelerometer reads
  SumX = 0;
  SumY = 0;
  SumZ = 0;
  
  analogReference(EXTERNAL);
  //Serial.println(xPin);
  //Serial.println(yPin);
  //Serial.println(zPin);
  
  for(int i=0; i<numSampls; i++){
    SumX = SumX + analogRead(xPin);
    SumY = SumY + analogRead(yPin);
    SumZ = SumZ + analogRead(zPin);
    Serial.println(SumX);
    Serial.println(SumY);
    Serial.println(SumZ);
  }

    Xavg = SumX/numSampls;
    Yavg = SumY/numSampls;
    Zavg = SumZ/numSampls;
    //Serial.println("x value is" + Xavg);
    //Serial.println("y value is" + Yavg);
    //Serial.println("z value is" + Zavg);
  
  aX = (((Xavg * 3.301)-offset)/sx)*g;
  aY = (((Yavg * 3.301)-offset)/sy)*g;
  aZ = (((Zavg * 3.301)-offset)/sz)*g;
  
  dt = micros()-prevTime;
  
  vX = pvX + aX*dt;  //velocity
  
  cpY = yPs[0] + .5*aY*dt*dt;  //position
  
  //an array of previous positions. should be a for loop with a definition indicating the length of the array
  yPs[9] = yPs[8];
  yPs[8] = yPs[7];
  yPs[7] = yPs[6];
  yPs[6] = yPs[5];
  yPs[5] = yPs[4];
  yPs[4] = yPs[3];
  yPs[3] = yPs[2];
  yPs[2] = yPs[1];
  yPs[1] = yPs[0];
  yPs[0] = cpY;
  
  prevTime = prevTime + dt;
  
  
  
  
  //get distance sensor input
  
   aSum = 0;
   //bSum = 0;
   
   analogReference(DEFAULT);
   //The loop sums up 100 values read from the sensors
   for (int i=0; i<numSampls; i++) {
     aSum = aSum + analogRead(aAN); //Get sensor read and convert the values into meters
     //bSum = bSum + analogRead(bAN);
   }
   
   //Average the sums of the values.  The for loop and this step together provide a
   //more precise reading of the distance.
   aAvg = aSum / numSampls;
   //bAvg = bSum / numSampls;
   sensorInput = aAvg/*+bAvg)/2*/ * 0.01295;
  

  
  //testing
  Serial.print("sensor input ");
  Serial.println(sensorInput);
  
  tElev = sensorElev - cos(angle)*sensorInput;
  
  //testing
  Serial.print("Elevation is ");
  Serial.println(tElev);
  
  //array of target elevations
  /*tEs[9] = tEs[8];
  tEs[8] = tEs[7];
  tEs[7] = tEs[6];
  tEs[6] = tEs[5];
  tEs[5] = tEs[4];
  tEs[4] = tEs[3];
  tEs[3] = tEs[2];
  tEs[2] = tEs[1];
  tEs[1] = tEs[0];
  tEs[0] = tElev;*/
  
  
  tDist = sin(angle)*sensorInput;
  
  //testing
  Serial.print("Distance is ");
  Serial.println(tDist);
  Serial.println("");
   
  
  
  if (tDist <= slowVal && tDist > stopVal) {
    digitalWrite(testSlow, HIGH);
        //delay(500);
    //digitalWrite(testSlow, LOW);

  }
  if (tDist > slowVal && tDist > stopVal) {
    digitalWrite(testSlow, LOW);
  }
    
   
  if (tDist <= stopVal) {
    digitalWrite(testStop, HIGH);
       // delay(500);
    //digitalWrite(testStop, LOW);
    digitalWrite(testSlow, LOW);
  }
    if (tDist > stopVal) {
          digitalWrite(testStop, LOW);
    }
  
  //at the moment, it just uses the extremes (oldest and newest) of it's stored data to calculate the slope, there is a better way to do this
  slope = (tEs[9]-tEs[0])/(yPs[9]-yPs[0]);
  
  //if(slope>inclineLimit    ||    ((slope*-1)>declineLimit)  )}
       //^uphill too much                    //^downhill too much
    
    
    //check to see if the height is scalable
    if(tElev<maxCurb){   
        //scaleable
            //drive
            
        //testing for now
        digitalWrite(testGo, LOW);
        //delay(500);
        //igitalWrite(testGo, LOW);
    }
        
    //else{   
     if(tElev>=maxCurb){ 
        //not scaleable
            //stop the robot 
        digitalWrite(testGo, HIGH);
          
        //testing
        //digitalWrite(testStop, HIGH);
        //delay(500);
        //digitalWrite(testStop, LOW);
          
    }
  }
  
  //testing
  //delay(1000);
//}

