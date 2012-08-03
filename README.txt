This project is a navigational system for the Emma5 Robot which is being developed through the collaboration of students M5 at the University of Massachusetts Amherst. 

The codebase is written using Arduino 1.0 with a focus around C++. 

The purpose of the system is to
	a) transmit data about current surroundings via a 485 bus
	b) detect obstacles in the robots path using:
		i) ultrasonic sensors
		ii) ir sensors
		iii) pressure sensor bar
	c) when an obstacle is detected communicate with the drive system via interrupts and data lines what action is to be taken

Currently the robot is fitted with an AtMega1280 micro-controller which interfaces with multiple ultrasonic sensors. Analog readings are taken from the sensors and then analyzed for obstacles detected in the vicinity of the robots path. 
