//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  TBD
//
//       LAB NAME:  TBD
//
//      FILE NAME:  main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This project runs on the LP_MSPM0G3507 LaunchPad board interfacing to
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
#include <ti/devices/msp/msp.h>

//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------

void run_lab5_part1(void);
void run_lab5_part2(void);
void run_lab5_part3(void);
void run_lab5_part4(void);

//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------

// Define a structure to hold different data types

void run_lab5_part1(void)
{
  bool    is_display_on = false;
  bool    done          = false;
  uint8_t loop_cntr     = 0;
  

  while (!done)
  {
    if (is_pb_down(PB1_IDX))
    {
      if (is_display_on)
      {
        seg7_off();
        is_display_on = false;
        loop_cntr++;
      }
      else
      {
        seg7_hex(3, SEG7_DIG0_ENABLE_IDX);
        is_display_on = true;
      }
    }

    while (is_pb_down(PB1_IDX))
      ;

    msec_delay(debounce);

    if (loop_cntr == 3)
    {
      done = true;
    }
  }

  seg7_off();
}
typedef enum
{
  GET_LOW,
  GET_HIGH,
  DISPLAY

} fsm_state_t;

void run_lab5_part2(void)
{

  fsm_state_t current_state = GET_LOW;

  uint8_t low_nibble;
  uint8_t high_nibble;
  uint8_t loop_cntr = 0;
  uint8_t seg7_data;
  uint8_t debounce = 10;

  while (loop_cntr < 3)
  {
    switch (current_state)
    {

      case (GET_LOW):
        low_nibble = ReadSw() & 0x0F;
        if (is_lpsw_down(LP_SW2_IDX))
        {
          current_state = GET_HIGH;
          msec_delay(debounce);

          while (is_lpsw_down(LP_SW2_IDX))
            ;
          msec_delay(debounce);
        }

        break;

      case (GET_HIGH):
        high_nibble = ReadSw() & 0xF;
        if (is_lpsw_down(LP_SW2_IDX))
        {
          current_state = DISPLAY;
          msec_delay(debounce);

          while (is_lpsw_down(LP_SW2_IDX))
            ;
          msec_delay(debounce);
          seg7_data = (high_nibble) << 4 | (low_nibble);
        }

        break;

      case (DISPLAY):
        if (is_pb_down(PB1_IDX))
        {
          seg7_on(seg7_data, SEG7_DIG2_ENABLE_IDX);
          msec_delay(debounce);
          while (is_pb_down(PB1_IDX))
            ;
          msec_delay(debounce);
        }
        else
        {
          seg7_on(seg7_data, SEG7_DIG0_ENABLE_IDX);
        }

        if (is_lpsw_down(LP_SW2_IDX))
        {
          seg7_off();
          loop_cntr++;
          current_state = GET_LOW;
          msec_delay(debounce);

          while (is_lpsw_down(LP_SW2_IDX))
            ;
          msec_delay(debounce);
        }
        break;
    }
  }
}

void run_lab5_part3(void)
{
  uint8_t key;
  uint8_t loop_cntr = 0;

  leds_off();
  seg7_off();

  while (loop_cntr < 8)
  {
    key = getkey_pressed();
    leds_on(key);
    msec_delay(10);
    wait_no_key_pressed();
    msec_delay(10);
    loop_cntr++;
  }
  leds_off();
  seg7_off();
}

void run_lab5_part4(void)
{
  uint8_t key;
  uint8_t loop_cntr = 0;
  uint8_t i;

  while (loop_cntr < 4)
  {
    key = keypad_scan();
    if (key != NO_KEY_PRESSED)
    {
      for (i = 0; i < key; i++)
      {
        leds_on(0xFF);
        msec_delay(500);

        leds_off();
        msec_delay(500);
      }

      msec_delay(10);
      wait_no_key_pressed();
      msec_delay(10);
    }
  }
  leds_off();
  seg7_off();
}

int main(void)
{
  // Configure the LaunchPad board
  clock_init_40mhz();
  launchpad_gpio_init();

  leds_init();
  leds_disable();
  leds_off();

  seg7_init();
  seg7_off();

  dipsw_init();

  keypad_init();

  // enter your code here
  run_lab5_part1();

  lpsw_init();
  run_lab5_part2();

  leds_enable();
  run_lab5_part3();

  run_lab5_part4();

  // Endless loop to prevent program from ending
  while (1)
    ;

} /* main */
