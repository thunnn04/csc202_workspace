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
#include "lcd1602.h"
#include <ti/devices/msp/msp.h>

// Define a structure to hold different data types

#define MSPM0_CLOCK_FREQUENCY (40E6)
#define SYST_TICK_PERIOD (10.25E-3)
#define SYST_TICK_PERIOD_COUNT (SYST_TICK_PERIOD * MSPM0_CLOCK_FREQUENCY)

void SysTick_Handler(void)
{
  uint8_t dip   = dipsw_read();
  uint8_t count = 0;
  uint8_t mask  = 1;

  for (uint8_t i = 0; i < 4; i++)
  {
    if ((dip & mask) == mask)
    {
      count++;
    }

    mask = mask << 1;
  }

  seg7_hex(count, SEG7_DIG0_ENABLE_IDX);
}

void run_lab7_part3(void)
{
  uint16_t value = 0;
  bool     done  = false;

  lcd_clear();
  leds_off();

  while (!done)
  {
    lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_CHAR_POSITION_5);
    lcd_write_doublebyte(value);

    msec_delay(200);
    value++;

    if (value == 100)
    {
      value = 0;
    }
  }
}

int main(void)
{
  clock_init_40mhz();
  launchpad_gpio_init();

  I2C_mstr_init();
  lcd1602_init();
  lcd_clear();

  leds_init();
  leds_disable();

  seg7_init();
  dipsw_init();

  sys_tick_init(SYST_TICK_PERIOD_COUNT);

  run_lab7_part3();

  sys_tick_disable();
  seg7_off();

  while (1)
    ;
}
