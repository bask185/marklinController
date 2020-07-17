#include "src/modules/weistra.h"
#include <Arduino.h>

enum ACcontrolStates {
	ACcontrolIDLE,
	moving,
	stationairy };

extern bool ACcontrol(void); 
extern void ACcontrolInit();

extern Weistra regelaar;
