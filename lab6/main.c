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

uint8_t debounce = 10;

void wait_for_pb_pressed(uint8_t pb_idx)
{
  while (!is_pb_down(pb_idx))
    ;
  msec_delay(debounce);
}

void wait_for_pb_released(uint8_t pb_idx)
{
  while (is_pb_down(pb_idx))
    ;
  msec_delay(debounce);
}

void run_lab6_part1(void);
void run_lab6_part2(void);
void run_lab6_part3(void);
void run_lab6_part4(void);

void run_lab6_part1(void)
{
  char letter;
  lcd_clear();

  for (letter = 'A'; letter <= 'P'; letter++)
    lcd_write_char(letter);

  lcd_set_ddram_addr(LCD_LINE2_ADDR);

  for (letter = 'Q'; letter <= 'Z'; letter++)
    lcd_write_char(letter);
}

void run_lab6_part2(void)
{
  uint32_t num32 = 1234567890;
  uint16_t num16 = 1234;
  uint8_t  num8  = 123;

  lcd_clear();

  lcd_set_ddram_addr(LCD_CHAR_POSITION_3);
  lcd_write_quadbyte(num32);

  wait_for_pb_pressed(PB1_IDX);

  lcd_set_ddram_addr(0x46);
  lcd_write_doublebyte(num16);

  wait_for_pb_released(PB1_IDX);


  wait_for_pb_pressed(PB1_IDX);

  lcd_clear();

  wait_for_pb_released(PB1_IDX);

  lcd_set_ddram_addr(0x06);
  lcd_write_byte(num8);

  wait_for_pb_pressed(PB1_IDX);
  lcd_clear();
  wait_for_pb_released(PB1_IDX);

  lcd_set_ddram_addr(0x03);
  lcd_write_string("Part 2 Done");
}

void run_lab6_part3(void)
{
  uint16_t timer = 100;
  bool     done  = false;

  lcd_clear();

  while (!done)
  {
    lcd_set_ddram_addr(0x06);
    lcd_write_byte(timer);

    msec_delay(200);
    if (is_pb_down(PB1_IDX))
    {
      wait_for_pb_pressed(PB1_IDX);
      wait_for_pb_released(PB1_IDX);

      timer = 100;
    }

    if (is_pb_down(PB2_IDX))
    {
      wait_for_pb_pressed(PB2_IDX);
      wait_for_pb_released(PB2_IDX);

      lcd_clear();
      lcd_set_ddram_addr(0x03);
      lcd_write_string("Part 3 Done");

      done = true;
    }

    if (timer == 0)
    {
      timer = 100;
    }
    else
    {
      timer--;
    }
  }
}

void run_lab6_part4(void)
{
  uint8_t key;
  uint8_t key_cntr = 0;
  bool    done     = false;

  lcd_clear();

  while (!done)
  {
    key = keypad_scan();
    if (key != NO_KEY_PRESSED)
    {
      if (key_cntr == 16)
      {
        lcd_set_ddram_addr(LCD_LINE2_ADDR);
      }
      else if (key_cntr == 32)
      {
        lcd_clear();
        key_cntr = 0;
      }

      key_cntr++;

      hex_to_lcd(key);

      msec_delay(10);
      wait_no_key_pressed();
      msec_delay(10);
    }
    if (is_pb_down(PB1_IDX))
    {
      lcd_clear();
      lcd_set_ddram_addr(0x00);
      key_cntr = 0;
    }

    if (is_pb_down(PB2_IDX))
    {
      lcd_clear();
      lcd_set_ddram_addr(LCD_CHAR_POSITION_3);
      lcd_write_string("Part 4 Done");

      done = true;
    }
  }
  leds_off();
  seg7_off();
}

int main(void)
{
  clock_init_40mhz();
  launchpad_gpio_init();
  I2C_mstr_init();
  lcd1602_init();
  seg7_init();
  seg7_off();
  leds_init();
  dipsw_init();

  run_lab6_part1();

  wait_for_pb_pressed(PB2_IDX);
  lcd_clear();
  wait_for_pb_released(PB2_IDX);

  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string("Running Part 2");
  msec_delay(1000);

  run_lab6_part2();

  lcd_set_ddram_addr(LCD_LINE2_ADDR);
  lcd_write_string("Press PB2");

  wait_for_pb_pressed(PB2_IDX);
  lcd_clear();
  wait_for_pb_released(PB2_IDX);

  lcd_set_ddram_addr(0x00);
  lcd_write_string("Running Part 3");

  msec_delay(1000);

  run_lab6_part3();

  lcd_set_ddram_addr(LCD_LINE2_ADDR);
  lcd_write_string("Press PB2");

  wait_for_pb_pressed(PB2_IDX);
  lcd_clear();
  wait_for_pb_released(PB2_IDX);

  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string("Running Part 4");

  msec_delay(1000);

  keypad_init();
  run_lab6_part4();

  // Endless loop to prevent program from ending
  while (1)
    ;

} /* main */
