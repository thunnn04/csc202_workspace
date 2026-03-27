#include <stdbool.h>
#include <stdint.h>
#include "LaunchPad.h"
#include "clock.h"
#include "lcd1602.h"
#include "adc.h"
#include <ti/devices/msp/msp.h>

#define OPA_GAIN_FACTOR 3
#define THRESHOLD 3500

int main(void)
{
    uint32_t adc_val;
    bool done = false;

    clock_init_40mhz();
    launchpad_gpio_init();

    I2C_mstr_init();
    lcd1602_init();

    leds_init();
    leds_disable();
    seg7_init();
    seg7_off();

    lcd_clear();
    lcd_set_ddram_addr(0x00);
    lcd_write_string("Before ADC");
    msec_delay(1000);

    OPA0_init(OPA_GAIN_FACTOR);
    OPA0_enable();
    ADC0_init(ADC12_MEMCTL_VRSEL_VDDA_VSSA);

    lcd_clear();
    lcd_set_ddram_addr(0x00);
    lcd_write_string("ADC init done");
    msec_delay(1000);

    lcd_clear();

    while (!done)
    {
        adc_val = ADC0_in(7);

        lcd_set_ddram_addr(0x00);
        if (adc_val < THRESHOLD)
            lcd_write_string("Status: Dark  ");
        else
            lcd_write_string("Status: Light ");

        lcd_set_ddram_addr(0x40);
        lcd_write_string("ADC = ");
        lcd_write_doublebyte(adc_val);
        lcd_write_string("   ");

        msec_delay(300);

        if (is_pb_down(PB1_IDX))
            done = true;
    }

    lcd_clear();
    lcd_set_ddram_addr(0x00);
    lcd_write_string("Program Stopped");

    while (1)
        ;
}