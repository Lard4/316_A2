#include <assert.h>
#include "msp.h"

#define MAX_FREQ_MHZ _48_MHZ
#define MIN_FREQ_MHZ _1_5_MHZ

#define _48_MHZ 48000000
#define _1_5_MHZ 1500000
#define _20_MHZ 20000000

#define PORT_TEST_LED_OUT (P1)
#define BIT_TEST_LED_OUT (BIT0)

#define PORT_MCLK_OUT (P4)
#define BITM_MCLK_OUT (BIT3)


void set_MCLK_DCO(uint32_t);
void output_MCLK();
uint32_t myfunc(uint32_t);


void main(void) {
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	set_MCLK_DCO(_20_MHZ);
	output_MCLK();

	// light up the test LED
    PORT_TEST_LED_OUT->SEL0 &= ~BIT_TEST_LED_OUT;  // simp io
    PORT_TEST_LED_OUT->SEL1 &= ~BIT_TEST_LED_OUT;  // simp io
    PORT_TEST_LED_OUT->DIR |= BIT_TEST_LED_OUT;    // output
    PORT_TEST_LED_OUT->OUT &= ~BIT_TEST_LED_OUT;   // turn off

    // wait

    //time
    PORT_TEST_LED_OUT->OUT |= BIT_TEST_LED_OUT;  // turn on
    myfunc(10000000);
    PORT_TEST_LED_OUT->OUT &= ~BIT_TEST_LED_OUT;  // turn off
}

// a function to use the DCO as the source for MCLK and set the
// DCO frequency in MHz
void set_MCLK_DCO(uint32_t frequency) {
    assert(frequency <= MAX_FREQ_MHZ);
    assert(frequency >= MIN_FREQ_MHZ);

    CS->KEY = CS_KEY_VAL;  // unlock CS
    CS->CTL0 = frequency;
    CS->CTL1 |= CS_CTL1_SELM__DCOCLK; // sets MCLK
}

// output the MCLK on P4.3
void output_MCLK() {
    PORT_MCLK_OUT->SEL0 |= BITM_MCLK_OUT;
    PORT_MCLK_OUT->SEL1 &= ~BITM_MCLK_OUT;
    PORT_MCLK_OUT->DIR |= BITM_MCLK_OUT;
}

// iterate through a loop n times
// 100000 loops = 762.4ms
// 1ms = 131.1647 loops
uint32_t myfunc(uint32_t loops) {
    uint32_t i;
    uint32_t x;
    for (i = 0; i < loops; i++) {
        x += 1;
    }
    return x;
}
