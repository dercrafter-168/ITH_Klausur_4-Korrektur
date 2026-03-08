//ITH Klausur 4 - Aufgabe 2

#if 1
#include "mbed.h"

PwmOut servo(PC_6);
PortIn maus(PortB, 0xFF);

void hw_init() {
    maus.mode(PullDown);
    servo.period_ms(20);
}

int main(){
    hw_init();

    while(true){
        servo.pulsewidth_ms(0.70 + (maus.read()/255.00)*1.60);
    }

}

#endif