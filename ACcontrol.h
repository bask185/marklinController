#include "src/modules/weistra.h"
#include <Arduino.h>

enum ACcontrolStates {
	ACcontrolIDLE,
	moving,
	stationairy };

extern bool ACcontrol(void); 
extern void ACcontrolInit();
extern void ACcontrolSetState(unsigned char);
extern unsigned char ACcontrolGetState(void);

extern Weistra regelaar;
