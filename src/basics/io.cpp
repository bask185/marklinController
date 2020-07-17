#include <Arduino.h>
#include "io.h"
extern void initIO(void) {
	pinMode(throttlePin, OUTPUT);
	pinMode(directionRelay, OUTPUT);
	pinMode(reverserPin, INPUT);
	pinMode(currentPin, INPUT);
}