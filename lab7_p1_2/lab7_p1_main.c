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

//---------------------------------------------------------------
// Define symbolic constants used by the program
//---------------------------------------------------------------
#define MSPM0_CLOCK_FREQUENCY (40E6)
#define SYST_TICK_PERIOD (10.25E-3)
#define SYST_TICK_PERIOD_COUNT (SYST_TICK_PERIOD * MSPM0_CLOCK_FREQUENCY)

#define NUM_STATES 18

const uint8_t seg7_letter_code[] = {0x6D, 0x00, 0x6D, 0x00, 0x6D, 0x00,
                                    0x3F, 0x00, 0x3F, 0x00, 0x3F, 0x00,
                                    0x6D, 0x00, 0x6D, 0x00, 0x6D, 0x00};

const uint8_t delay_count[] = {0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x18,
                               0x24, 0x24, 0x24, 0x24, 0x24, 0x24,
                               0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x30};

void SysTick_Handler(void)
{
  static uint16_t delay_time = 1;
  static uint16_t code_index = 0;

  delay_time--;

  if (delay_time == 0)
  {
    seg7_on(seg7_letter_code[code_index], SEG7_DIG0_ENABLE_IDX);

    delay_time = delay_count[code_index];

    code_index++;

    if (code_index == NUM_STATES)
    {
      code_index = 0;
    }
  }
}

void lcd_write_string_window(char *string, uint8_t start_lcd_addr, uint8_t max_lcd_addr)
{
  uint8_t addr = start_lcd_addr;
  uint8_t i    = 0;

  lcd_set_ddram_addr(start_lcd_addr);

  while (string[i] != '\0' && addr <= max_lcd_addr)
  {
    lcd_write_char(string[i]);
    i++;
    addr++;
  }
}

void run_lab7_part1(void)
{
  char message[] = "Microcontrollers are fun.";
  bool done      = false;

  lcd_clear();
  leds_off();

  while (!done)
  {
    for (int i = 0x4F; i >= 0x40 && !done; i--)
    {
      lcd_clear();
      lcd_set_ddram_addr(i);
      lcd_write_string(message);

      msec_delay(200);

      if (is_pb_down(PB1_IDX))
      {
        done = true;
      }
    }

    uint8_t index = 0;

    while (message[index] != '\0' && !done)
    {
      lcd_clear();
      lcd_set_ddram_addr(0x40);
      lcd_write_string(&message[index]);

      msec_delay(200);

      index++;

      if (is_pb_down(PB1_IDX))
      {
        done = true;
      }
    }
  }

  lcd_clear();
  lcd_set_ddram_addr(0x00);
  lcd_write_string("Part 1 Done");
}

void run_lab7_part2(void)
{
  char message[] =
      "Microcontrollers are fun. I love programming in MSPM0+ assembly code!!!";
  bool done = false;

  lcd_clear();
  leds_off();

  while (!done)
  {
    for (int i = 0x4F; i >= 0x40 && !done; i--)
    {
      lcd_clear();
      lcd_set_ddram_addr(i);

      lcd_write_string_window(message, i, 0x4F);

      msec_delay(200);

      if (is_pb_down(PB1_IDX))
      {
        done = true;
      }
    }

    uint8_t index = 0;

    while (message[index] != '\0' && !done)
    {
      lcd_clear();
      lcd_set_ddram_addr(0x40);

      lcd_write_string_window(&message[index], 0x40, 0x4F);

      msec_delay(200);

      index++;

      if (is_pb_down(PB1_IDX))
      {
        done = true;
      }
    }
  }

  lcd_clear();
  lcd_set_ddram_addr(0x00);
  lcd_write_string("Part 2 Done");
}

int main(void)
{
  clock_init_40mhz();
  launchpad_gpio_init();

  I2C_mstr_init();
  lcd1602_init();

  leds_init();
  leds_disable();
  seg7_init();
  dipsw_init();

  sys_tick_init(SYST_TICK_PERIOD_COUNT);

  run_lab7_part1();

  sys_tick_disable();
  seg7_off();

  lcd_clear();
  lcd_set_ddram_addr(0x40);
  lcd_write_string("Press PB2");

  while (!is_pb_down(PB2_IDX))
    ;

  lcd_clear();
  lcd_set_ddram_addr(0x00);
  lcd_write_string("Running Part 2");

  msec_delay(1000);

  run_lab7_part2();

  sys_tick_disable();
  seg7_off();

  while (1)
    ;
}
