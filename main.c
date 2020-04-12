#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "msp.h"

#define MAX_FREQ_MHZ _24_MHZ
#define MIN_FREQ_MHZ _1_5_MHZ

#define _1_5_MHZ  CS_CTL0_DCORSEL_0
#define _3_MHZ    CS_CTL0_DCORSEL_1
#define _6_MHZ    CS_CTL0_DCORSEL_2
#define _12_MHZ   CS_CTL0_DCORSEL_3
#define _24_MHZ   CS_CTL0_DCORSEL_4

#define PORT_TEST_LED_OUT (P1)
#define BIT_TEST_LED_OUT (BIT0)

#define PORT_MCLK_OUT (P4)
#define BITM_MCLK_OUT (BIT3)


void set_MCLK_DCO(int);
void output_MCLK();
int us_to_loops(uint32_t, int);
void delay(uint32_t);
void test_LED(int);


void main(void) {
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	const int frequency = _3_MHZ;

	set_MCLK_DCO(frequency);
	output_MCLK();
	test_LED(frequency);

	while(1);
}

// a function to use the DCO as the source for MCLK and set the
// DCO frequency in MHz
void set_MCLK_DCO(int frequency) {
    assert(frequency <= MAX_FREQ_MHZ);
    assert(frequency >= MIN_FREQ_MHZ);

    CS->KEY = CS_KEY_VAL;  // unlock CS
    CS->CTL0 = frequency;
    CS->CTL1 |= CS_CTL1_SELM__DCOCLK | CS_CTL1_DIVM__1; // set MCLK=(div by 1)
}

// output the MCLK on P4.3
void output_MCLK() {
    PORT_MCLK_OUT->SEL0 |= BITM_MCLK_OUT;
    PORT_MCLK_OUT->SEL1 &= ~BITM_MCLK_OUT;
    PORT_MCLK_OUT->DIR |= BITM_MCLK_OUT;
}

// iterate through a loop n times
// 100000 loops = 291ms
// 1us = .344 loops
// 3us ~= 1 loop for 20MHz


// 10000 loops = 0.107s @1.5MHz
// 9.3458 loops = 100us @1.5MHz
void delay(uint32_t loops) {
    // NOTE: REQUIRES -o0 to work
    uint32_t i;
    uint32_t x = 0;
    for (i = 0; i < loops; i++) {
        x+=1;
    }
}

int us_to_loops(uint32_t us, int frequency) {
    float std_loops = (float)us * 0.093458;
    int time_comp = 1;
    if (frequency != _1_5_MHZ) {
        time_comp = 1 + (frequency / 0x10000);
    }

    printf("std_loops = %f\ntime_comp = %d\n", std_loops, time_comp);

    return roundf(std_loops * time_comp);
}

void test_LED(int frequency) {
    // configure the test LED
    PORT_TEST_LED_OUT->SEL0 &= ~BIT_TEST_LED_OUT;  // simp io
    PORT_TEST_LED_OUT->SEL1 &= ~BIT_TEST_LED_OUT;  // simp io
    PORT_TEST_LED_OUT->DIR |= BIT_TEST_LED_OUT;    // output
    PORT_TEST_LED_OUT->OUT &= ~BIT_TEST_LED_OUT;   // turn off

    // setup time
    __delay_cycles(100);

    // time the function with a LED output
    const int loops = us_to_loops(200000, frequency);
    printf("loops = %d\n", loops);
    fflush(stdout);

    PORT_TEST_LED_OUT->OUT |= BIT_TEST_LED_OUT;  // turn on
    delay(loops);
    PORT_TEST_LED_OUT->OUT &= ~BIT_TEST_LED_OUT;  // turn off
}
