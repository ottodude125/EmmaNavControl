 /*
 *  BusDriver
 *
 *  1/11/2012
 *  Author: Emma5
 */

#include "BusDriver.h"
#include <stdint.h>


static void soundOffReply(uint8_t* data){
	if(data[2]==MY_SOUNDOFF_CUE){
		EmmaBus.writeDangerous(EmmaBus.soundOff);
		EmmaBus.soundOff = dontWantCtrl;
	}
}

static void takeCtrl(uint8_t data[]){
	int i;
	for(i=0; i<EmmaBus.queuePtr; i++){
		EmmaBus.writeDangerous(EmmaBus.queue[i]);
	}
	EmmaBus.queuePtr=0;
	EmmaBus.writeDangerous(releaseCtrl);
}

static int RXCallBack(){
	EmmaBus.TakeByte();
        return 0;
}



//********************************************************* 
//** BusDriver Class
//********************************************************* 
 BusDriver::BusDriver(){
}

void BusDriver::init(int baudRate){
	pinMode(BUS_CHIP, OUTPUT); //HIGH to transmit, LOW to receive
	digitalWrite(BUS_CHIP, LOW); //listening

	lastIn = 0;
	
	queuePtr=0;
	soundOff = dontWantCtrl;

	addFunctionToBrainLUT(soundOffReply, I_WANT);
	addFunctionToBrainLUT(soundOffReply, I_DONT_WANT);
	addFunctionToMyLUT(takeCtrl, YOU_TAKE);

	Serial.beginWithInterrupt(baudRate, RXCallBack);
}


void BusDriver::TakeByte(){

 byteIn = millis();

 static int byteCount;
 static uint8_t dataIn[DATA_SIZE]; //stores data while the interrupt handles it
 
	 /*
	 * times out if not all 5 bytes arrive in a timely fashion
	 * sometimes the millis() coutner will overflow and go back to 0
	 * so any timeout is excused if millis() is very low (meaning it rolled over recently)
	 */
	 if( (  (byteIn-lastIn)>READ_TIME_OUT )&&( byteIn>ROLLOVER_THRESHOLD  ) ){
		 byteCount=0;
	 }
 
 
 dataIn[byteCount] = Serial.read();
 byteCount++;

	 if(byteCount==DATA_SIZE){ //if all the data has arrived...
	   byteCount=0;
	   if(addressCheck(dataIn[0])){ //...and is addressed to you
		   handleBus(dataIn);
	   }
	 }
 lastIn=byteIn;
}

void BusDriver::handleBus(uint8_t data[]){ //this function is called when there are 5 bytes addressed to you in dataIn
  if(data[0]==MY_ADDRESS || data[0]==MY_OTHER_ADDRESS){
    //reference your LUT
    myLUT[data[1]](data);
  }

  else if(data[0]==BROADCAST_ADDRESS){
    //reference broadcast LUT
    broadcastLUT[data[1]](data);
  }

  else if(data[0]==BRAIN_ADDRESS){
    brainLUT[data[1]](data);
  }

  else{
   //and so on... 
  }
}

bool BusDriver::addressCheck(uint8_t x){
//fill in this switch statement with whatever addresses you want to pay attention to
  switch (x){
   case MY_ADDRESS:    	//messages to you
        return true;
   case MY_OTHER_ADDRESS:
		return true;  
   case BROADCAST_ADDRESS:         //broadcasts from brain to everyone
        return true;
   case BRAIN_ADDRESS:        //messages to the brain (to see if it's your turn to sound off)
        return true;
   default:
        return false;
  }
}

void BusDriver::addFunctionToMyLUT(void (*FunPointer)(uint8_t*), uint8_t cmd){

	myLUT[cmd]=FunPointer;
}

void BusDriver::addFunctionToBrainLUT(void (*FunPointer)(uint8_t*), uint8_t cmd){

	brainLUT[cmd]=FunPointer;
}

void BusDriver::addFunctionToBdcstLUT(void (*FunPointer)(uint8_t*), uint8_t cmd){

	broadcastLUT[cmd]=FunPointer;
}

void BusDriver::out(uint8_t data[]){
	soundOff=wantCtrl;
	queue[queuePtr] = data;
	queuePtr++;
}

void BusDriver::writeDangerous(uint8_t data[]){

  digitalWrite(BUS_CHIP, HIGH);
  delay(5);

  Serial.write(data, DATA_SIZE);

  delay(5);
  digitalWrite(BUS_CHIP, LOW);
}

//********************************************************* 
//********************************************************* 







