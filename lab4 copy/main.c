//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Thun Nadi
//
//       LAB NAME:  Lab 4
//
//      FILE NAME:  main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This project runs on the LP_MSPM0G3507 LaunchPad board uint8_terfacing to
//    the CSC202 Expansion board.
//
//    This code ... *** COMPLETE THIS BASED ON LAB REQUIREMENTS ***
//
//*****************************************************************************
//*****************************************************************************

//-----------------------------------------------------------------------------
// Loads standard C include files
//-----------------------------------------------------------------------------
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
// Loads MSP launchpad board support macros and definitions
//-----------------------------------------------------------------------------
#include "LaunchPad.h"
#include "clock.h"
#include "ti/devices/msp/peripherals/hw_gpio.h"
#include <ti/devices/msp/msp.h>

void run_lab4_part1(void);
void run_lab4_part2(void);
void run_lab4_part3(void);
void run_lab4_part4(void);

void run_lab4_part1(void)
{
  // Turn ON
  led_on(LED_BAR_LD5_IDX);
  led_on(LED_BAR_LD4_IDX);
  led_on(LED_BAR_LD3_IDX);
  led_on(LED_BAR_LD2_IDX);

  msec_delay(1000);

  // Turn them OFF
  led_off(LED_BAR_LD5_IDX);
  led_off(LED_BAR_LD4_IDX);
  led_off(LED_BAR_LD3_IDX);
  led_off(LED_BAR_LD2_IDX);
}

void run_lab4_part2(void)
{
  uint8_t i;
  uint8_t count = 0;

  while (count < 5)
  {
    // 0 → 7
    for (i = LED_BAR_LD0_IDX; i <= LED_BAR_LD7_IDX; i++)
    {
      led_on(i);
      msec_delay(200);
      led_off(i);
    }

    // 6 → 1
    for (i = LED_BAR_LD6_IDX; i > LED_BAR_LD0_IDX; i--)
    {
      led_on(i);
      msec_delay(200);
      led_off(i);
    }

    count++;
  }

  leds_off();
  seg7_off();
}

void run_lab4_part3(void)
{
  uint16_t counter;
  uint8_t  loop_cntr = 0;
  uint16_t delay_time;

  while (loop_cntr < 2)
  {
    if (loop_cntr == 0)
      delay_time = 100; 
    else
      delay_time = 50; 

    for (counter = 0; counter <= 255; counter++)
    {
      leds_on(counter);
      msec_delay(delay_time);
    }

    loop_cntr++;
  }

  msec_delay(500);

  leds_off();
}

void run_lab4_part4(void)
{
  seg7_on(0x38, SEG7_DIG0_ENABLE_IDX);
  msec_delay(500);
}

void run_lab4_part5(void)
{
    uint8_t loop_cntr = 0;

    leds_off();     
    seg7_off();     

    while(loop_cntr < 4)
    {
        seg7_hex(4, SEG7_DIG2_ENABLE_IDX);
        msec_delay(3000);     

        // turn display off
        seg7_off();
        msec_delay(2000);     

        loop_cntr++;
    }

    seg7_off();
    leds_off();
}

void run_lab4_part6(void)
{
    uint16_t loop_cntr = 0;

    leds_off();     

    while(loop_cntr < 200)
    {
        seg7_hex(0xC, SEG7_DIG0_ENABLE_IDX);   
        msec_delay(2);

        seg7_hex(0xA, SEG7_DIG1_ENABLE_IDX);   
        msec_delay(2);

        seg7_hex(0xF, SEG7_DIG2_ENABLE_IDX);   
        msec_delay(2);

        seg7_hex(0xE, SEG7_DIG3_ENABLE_IDX);   
        msec_delay(2);

        loop_cntr++;
    }

    seg7_off();     
    leds_off();    
}

int main(void)
{
  clock_init_40mhz();
  launchpad_gpio_init();

  leds_init();
  leds_enable();
  leds_off();

  run_lab4_part1();
  msec_delay(1000);

  run_lab4_part2();
  msec_delay(1000);

  run_lab4_part3();
  msec_delay(1000);

  leds_disable();
  seg7_init();

  run_lab4_part4();
  msec_delay(1000);

  run_lab4_part5();
  msec_delay(1000);

  run_lab4_part6();
}