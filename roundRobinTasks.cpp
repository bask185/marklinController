
#include "roundRobinTasks.h"
#include "ACcontrol.h"
#include "src/basics/io.h"
#include "src/basics/timers.h"

#define maximumCurrent 160


#define ON	1
#define OFF	0

void shortCircuit() {
	static uint8_t pulseCounter;

	if( (!rebootT) && regelaar.isOff() ) {
		regelaar.turn( ON );
	}


	if( !shortCircuitT ) {							// if timer = 0
		if( digitalRead( throttlePin ) ) { 			// only when the track power pin is enabled (if a weistrs short circuits, we cannot measure current during the off cycle.
			shortCircuitT = 5; 						// 5ms brake after ADC sample.

			int sample = analogRead( currentPin );
			if( sample > maximumCurrent ) pulseCounter ++;

			if( pulseCounter > 10 ) { 				// if 10 x 5ms = 50ms short circuit is detected, deactivate power to track
				pulseCounter = 0;

				regelaar.turn( OFF );				// disable weistra control
				rebootT = 100; 						// 10 SECONDS RETRY
			}
		}
	}
}


extern void processRoundRobinTasks(void) {
	static unsigned char taskCounter = 0;

// HIGH PRIORITY ROUND ROBIN TASKS
	//readSerialBus();
	//updateIO();
	regelaar.update();

// LOW PRIORITY ROUND ROBIN TASKS
	taskCounter ++;
	switch(taskCounter) {
		default: taskCounter = 0;

		case 0:
		shortCircuit();
		break;

		case 1:
		/* fill in a task */
		break;
	}
}