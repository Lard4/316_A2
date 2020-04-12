#include "util.h"
#include "delay.h"

/**
 * 10000 loops = 0.107s @1.5MHz
 * 9.3458 loops = 100us @1.5MHz
 * 1 loop = 10.67us @1.5MHz
 **/
void delay_us(uint32_t us, uint32_t frequency) {
    // NOTE: REQUIRES -o0 to work
    uint32_t i;
    uint32_t x = 0;
    uint32_t time_comp = 1;

    if (frequency != _1_5_MHZ) {
        time_comp = 1 + (frequency / 0x10000);
    }

    uint32_t loops = roundf((float)us * 0.093458 * time_comp);

    for (i = 0; i < loops; i++) {
        x+=1;
    }
}
