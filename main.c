#include "util.h"
#include "delay.h"
#include "msp.h"

#define MAX_FREQ_MHZ _24_MHZ
#define MIN_FREQ_MHZ _1_5_MHZ

#define PORT_TEST_LED_OUT (P1)
#define BIT_TEST_LED_OUT (BIT0)

#define PORT_MCLK_OUT (P4)
#define BITM_MCLK_OUT (BIT3)


void set_MCLK_DCO(uint32_t);
void output_MCLK();
void test_LED(uint32_t, uint32_t);


void main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    const uint32_t frequency = _3_MHZ;

    set_MCLK_DCO(frequency);
    output_MCLK();
    test_LED(200000, frequency);

    while(1);
}

// a function to use the DCO as the source for MCLK and set the
// DCO frequency in MHz
void set_MCLK_DCO(uint32_t frequency) {
    assert(frequency <= MAX_FREQ_MHZ);

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

void test_LED(uint32_t us, uint32_t frequency) {
    // configure the test LED
    PORT_TEST_LED_OUT->SEL0 &= ~BIT_TEST_LED_OUT;  // simp io
    PORT_TEST_LED_OUT->SEL1 &= ~BIT_TEST_LED_OUT;  // simp io
    PORT_TEST_LED_OUT->DIR |= BIT_TEST_LED_OUT;    // output
    PORT_TEST_LED_OUT->OUT &= ~BIT_TEST_LED_OUT;   // turn off

    // time the function with a LED output
    PORT_TEST_LED_OUT->OUT |= BIT_TEST_LED_OUT;  // turn on
    delay_us(us, frequency);
    PORT_TEST_LED_OUT->OUT &= ~BIT_TEST_LED_OUT;  // turn off
}
