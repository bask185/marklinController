#include <Arduino.h>

class Weistra {
public:
    Weistra(unsigned char);
    void begin();
    void update();
    void setSpeed(unsigned char);
    void turn( uint8_t );
    uint8_t isOn();
    uint8_t isOff();

private:
    unsigned int  intervalTime;
    unsigned char dutyCycle;
    unsigned char trackPin;
    unsigned char state;

    volatile uint8_t *portx_p;
    const int ON = 1;
    const int OFF = 0;
};

