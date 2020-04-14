#include "util.h"
#include "delay.h"

/**
 * comp.overhead of 43us @1.5MHz
 * 10us per loop after overhead @1.5MHz
 * 1us = 0.1 loops @1.5MHz
 **/
void delay_us(uint32_t us, uint32_t frequency) {
    // NOTE: REQUIRES -o0 to work
    uint32_t i;
    uint32_t x = 0;
    uint32_t time_comp = 1 << (frequency / 0x10000);

    us = us - (41 / time_comp);

    uint32_t loops = us * time_comp / 10;
    //loops = loops +10;

    for (i = 0; i < loops; i++) {
        x+=1;
    }
}
