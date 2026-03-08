//ITH Klausur 4 - Aufgabe 1

#if 1
#include "mbed.h"

InterruptIn button(PC_13);
PortOut sevenSegmentDisplay(PortC, 0xFF);
BusOut sevenSegmentSwitch(PC_11, PC_12);
volatile int counter = 0;
volatile int takt = 0;

constexpr char sevenSegmentBA1[6] {0b0010000, 0b0100000, 0b0000001, 0b0000010, 0b0000100, 0b0001000};
constexpr char sevenSegmentBA2[6] {0b0110000, 0b0100001, 0b0000011, 0b0000110, 0b0001100, 0b0011000};
constexpr char sevenSegmentBA3[7] {0b0010000, 0b0110000, 0b0110001, 0b0110011, 0b0110111, 0b0111111, 0b0000000};
constexpr char sevenSegmentBA4[12] {0b0010000, 0b0110000, 0b0110001, 0b0110011, 0b0110111, 0b0111111, 0b0101111, 0b0001111, 0b0001110, 0b0001100, 0b0001000, 0b0000000};

void isr_TM() {
    counter = counter + 1;
    if (counter >= 4) {
        counter = 0;
    }
}

void isr_takt() {
    TIM7->SR = 0;
    TIM7->CNT = 0;
    takt = takt + 1;
}

void initExit(){
    button.mode(PullDown);
    button.rise(isr_TM);
    button.enable_irq();
}

void initTimer() {
    RCC->APB1ENR |= 0b100000;
    TIM7->CR1 = 0;
    TIM7->PSC = 32000 - 1;
    TIM7->ARR = 3509 - 1;
    TIM7->CNT = 0;
    TIM7->SR = 0;
    NVIC_SetVector(TIM7_IRQn, (uint32_t)&isr_takt);
    HAL_NVIC_EnableIRQ(TIM7_IRQn);
    TIM7->DIER = 1;
    TIM7->CR1 |= 1;
}

void up_BA1(){
    sevenSegmentSwitch = 0b01;
    sevenSegmentDisplay = sevenSegmentBA1[(takt%6)+1];
    ThisThread::sleep_for(10ms);
    sevenSegmentSwitch = 0b10;
    sevenSegmentDisplay = sevenSegmentBA1[takt%6];
}

void up_BA2(){
    sevenSegmentSwitch = 0b01;
    sevenSegmentDisplay = sevenSegmentBA2[(takt%6)+1];
    ThisThread::sleep_for(10ms);
    sevenSegmentSwitch = 0b10;
    sevenSegmentDisplay = sevenSegmentBA2[takt%6];
}

void up_BA3(){
    sevenSegmentSwitch = 0b01;
    sevenSegmentDisplay = sevenSegmentBA3[(takt%7)+1];
    ThisThread::sleep_for(10ms);
    sevenSegmentSwitch = 0b10;
    sevenSegmentDisplay = sevenSegmentBA3[takt%7];
}

void up_BA4(){
    sevenSegmentSwitch = 0b01;
    sevenSegmentDisplay = sevenSegmentBA4[(takt%12)+1];
    ThisThread::sleep_for(10ms);
    sevenSegmentSwitch = 0b10;
    sevenSegmentDisplay = sevenSegmentBA4[takt%12];
}

int main(){
    initExit();
    initTimer();

    while(true){
        switch (counter) {
            case 0:
                printf("BA1\n");
                up_BA1();
                break;
            case 1:
                printf("BA2\n");
                up_BA2();
                break;
            case 2:
                printf("BA3\n");
                up_BA3();
                break;
            case 3:
                printf("BA4\n");
                up_BA4();
                break;
            default:
                break;
        }
    }

}

#endif