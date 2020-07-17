// HEADER FILES
#include <Arduino.h>
#include "ACcontrol.h"
#include "serial.h"
#include "src/basics/timers.h"
#include "src/basics/io.h"


// MACROS
#define stateFunction(x) static bool x##F(void)
#define entryState if(runOnce) 
#define onState runOnce = false; if(!runOnce)
#define exitState if(!exitFlag) return false; else
#define State(x) break; case x: if(runOnce) Serial.println(#x); if(x##F())
#define STATE_MACHINE_BEGIN if(!enabled) { \
	if(!ACcontrolT) enabled = true; } \
else switch(state){\
	default: Serial.println("unknown state executed, state is idle now"); state = ACcontrolIDLE; case ACcontrolIDLE: return true;
#define STATE_MACHINE_END break;}return false;


#define beginState stationairy
#ifndef beginState
#error beginState not yet defined
#endif

const int updateInterval = 10; // 100ms

Weistra regelaar( throttlePin );

enum directions {
	NEUTRAL,
	LEFT,
	RIGHT,
};

// VARIABLES
static unsigned char state = beginState;
static bool enabled = true, runOnce = true, exitFlag = false;

int8_t speed, regulator; // reverser is the inp


// FUNCTIONS
extern void ACcontrolInit(void) { 
	state = beginState;
	regelaar.begin();
}

static void nextState(unsigned char _state, unsigned char _interval) {
	runOnce = true;
	exitFlag = false;
	if(_interval) {
		enabled = false;
		ACcontrolT = _interval;
	} 
	state = _state;
}

void updateRegulator() {
	regulator = map( analogRead( reverserPin ), 0 , 1023, -20, 20); 
	regulator *= 5;
}

void updateSpeed() {
	static int8_t previousSpeed;

	updateRegulator();

	if( regulator < speed ) speed--; // throttle level follows reverser with a delay
	if( regulator > speed ) speed++;

	if( speed != previousSpeed ) regelaar.setSpeed( speed );

	previousSpeed = speed;
}
	

// STATE FUNCTIONS
stateFunction(moving) {
	entryState {
		throttleT = 0; // ensure this timer is 0, shoudln't be needed
	}
	onState {
		repeat( &throttleT, updateInterval, updateSpeed );	// this function is always atlease called once before the speed is examined
															// the speed will therefor never be 0 when this function is called the first time
		if( speed == 0 ) exitFlag = true; 
	}
	exitState {

		return true;
	}
}


stateFunction(stationairy) {
	static uint8_t oldDirection = LEFT;
	uint8_t = newDirection = NEUTRAL;
	
	entryState {
	}
	onState {
		repeat( &throttleT, 10, updateRegulator );

		if( regulator <= -10 ) newDirection = LEFT; // throttle must be alteast -10 or 10
		if( regulator >=  10 ) newDirection = RIGHT;
		
		if( newDirection != NEUTRAL ) {
			if( newDirection != oldDirection ) digitalWrite( directionRelay, HIGH ); // apply 24V when direction has changed
			if( !ACcontrolT ) exitFlag = true; // if time has expired -> exit
		}
		else {
			ACcontrolT = 30; // force this timer at 300ms when no direction is picked, this time will be used to keep the 24V applied.
		}
	}
	exitState {
		digitalWrite( directionRelay, LOW );
		oldDirection = newDirection;
		return true;
	}
}

// STATE MACHINE
extern bool ACcontrol(void) {
	STATE_MACHINE_BEGIN

	State(moving) {
		nextState(stationairy, 100); }

	State(stationairy) {
		nextState(moving, 100); }

	STATE_MACHINE_END
}
