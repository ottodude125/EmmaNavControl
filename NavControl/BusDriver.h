/*
 * BusDriver.h
 *
 *  Created on: Nov 7, 2011
 *      Author: Emma5
 */


#ifndef BusDriver_h
#define BusDriver_h
#include <stdint.h>



//definitions

		static const uint8_t BUS_CHIP = 2;   //transmit enable/disable
		static const int DATA_SIZE = 5;
		static const int QUEUE_SIZE = 16;
		static unsigned long READ_TIME_OUT = 99000; //define how long the readBus function has to run in microseconds
		static unsigned long ROLLOVER_THRESHOLD = 5;

		//addresses
		static const uint8_t MY_ADDRESS = 0xaa;  // 170 in decimal 
		static const uint8_t MY_OTHER_ADDRESS = MY_ADDRESS+3; //also references your look up table (currently used for kickoff of testing from tail)
		
		static const uint8_t BROADCAST_ADDRESS = 0xff;
		static const uint8_t BRAIN_ADDRESS = 0x00;

		static const uint8_t MY_SOUNDOFF_CUE = MY_ADDRESS-1;

		//commands for sound-off routine
		static const uint8_t I_WANT = 0xff;      //command you send if you want bus control
		static const uint8_t I_DONT_WANT = 0x0f; //command you send to decline bus control
		static const uint8_t YOU_TAKE = 0xee;    //you get one of these when you gain explicit bus control, and send one of these to brain to relinquish control
		
		//premade packets to reply to manage bus control
		static uint8_t wantCtrl[DATA_SIZE] = {BRAIN_ADDRESS,I_WANT,MY_ADDRESS,0x00,0xFF};
		static uint8_t dontWantCtrl[DATA_SIZE] = {BRAIN_ADDRESS,I_DONT_WANT,MY_ADDRESS,0x00,0xFF};
		static uint8_t releaseCtrl[DATA_SIZE] = {BRAIN_ADDRESS,YOU_TAKE,MY_ADDRESS,0x00,0xFF};




class BusDriver {

			
		unsigned long byteIn;
		unsigned long lastIn;

		void (*myLUT[256])(uint8_t[]);
		void (*broadcastLUT[256])(uint8_t[]);
		void (*brainLUT[256])(uint8_t[]);


		//functions
		void handleBus(uint8_t[]);
		bool addressCheck(uint8_t);
	

		public: 
		  int queuePtr;

		  uint8_t* queue[QUEUE_SIZE];
		  uint8_t* soundOff;
		  
                  //Constructor(s)
                  BusDriver();

		  //functions
		  void init(int);
		  void TakeByte();
		  void addFunctionToMyLUT(void (*)(uint8_t*), uint8_t);
                  void addFunctionToBdcstLUT(void (*)(uint8_t*), uint8_t);
                  void addFunctionToBrainLUT(void (*)(uint8_t*), uint8_t);
                
				
		  void writeDangerous(uint8_t[]);
		  void out(uint8_t[]);
};

BusDriver EmmaBus;

#endif /* BusDriver_H_ */
