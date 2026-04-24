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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
// Loads MSP launchpad board support macros and definitions
//-----------------------------------------------------------------------------
#include "LaunchPad.h"
#include "adc.h"
#include "clock.h"
#include "lcd1602.h"
#include "uart.h"
#include <ti/devices/msp/msp.h>

//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void run_lab11_part1(void);
void display_menu(void);
void uart_write_string(char *str);
uint8_t get_spi_data(void);
uint8_t send_spi_data(uint8_t data);
void update_leds(void);
void lcd_show_spi(uint8_t sent, uint8_t recv);

//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define UART_BAUD_RATE 115200
//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------

// Define a structure to hold different data types

uint16_t g_adc_temp;

void display_menu(void)
{
  uart_write_string("\r\nMENU OPTIONS\r\n");
  uart_write_string("1. Increment count on 7-segment display\r\n");
  uart_write_string("2. Show current temperature\r\n");
  uart_write_string("3. Flash LEDs 3 times\r\n");
  uart_write_string("4. End Program\r\n");
  uart_write_string("Enter your selection: ");
}

void uart_write_string(char *str)
{
  while (*str)
  {
    UART_out_char(*str);
    str++;
  }
}

void update_seg7(bool increment_enabled)
{
  static uint8_t count = 0;

  if (increment_enabled)
  {
    count++;

    if (count > 9)
      count = 0;
  }

  seg7_hex(count, 0);
}

void display_temp(void)
{

  uint16_t temp_c;
  uint16_t temp_f;
  g_adc_temp = ADC0_in(5);

  temp_c = thermistor_calc_temperature(g_adc_temp);
  temp_f = (temp_c * 9) / 5 + 32;

  lcd_clear();
  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string("Temp = ");
  lcd_write_doublebyte(temp_f);

  lcd_write_char(0xDF);
  lcd_write_string("F");
}

void flash_leds(void)
{
  uint8_t loop_cntr = 0;

  leds_off();
  seg7_off();

  while (loop_cntr < 3)
  {
    leds_on(0xFF);
    msec_delay(300);

    // turn display off
    leds_off();
    msec_delay(200);

    loop_cntr++;
  }

  seg7_off();
  leds_off();
}

void run_lab10_part2(void)
{
  bool done = false;
  char choice;

  lcd_clear();
  update_seg7(false);

  while (!done)
  {
    display_menu();

    choice = UART_in_char();
    UART_out_char(choice);

    uart_write_string("\r\n");

    if (choice == '1')
    {
      update_seg7(true);
      uart_write_string("\r\nOption 1 Detected.\r\n");
    }

    else if (choice == '2')
    {
      display_temp();
      uart_write_string("\r\nOption 2 Detected.\r\n");
    }

    else if (choice == '3')
    {
      leds_enable();
      flash_leds();
      uart_write_string("\r\nOption 3 Detected.\r\n");

      leds_disable();
      update_seg7(false);
    }

    else if (choice == '4')
    {
      uart_write_string("\n\rThank you for using the program\r\n");

      lcd_clear();
      lcd_set_ddram_addr(LCD_LINE1_ADDR);
      lcd_write_string("Program Stopped");

      seg7_off();

      done = true;
    }

    else
    {
      uart_write_string("Invalid option\r\n");
    }
  } /* while */
}

int main(void)
{
  // Configure the LaunchPad board
  clock_init_40mhz();
  launchpad_gpio_init();
  UART_init(UART_BAUD_RATE);
  I2C_mstr_init();

  ADC0_init(ADC12_MEMCTL_VRSEL_VDDA_VSSA);
  lcd1602_init();
  leds_init();
  seg7_init();
  leds_disable();
  seg7_off();

  run_lab10_part2();

  // Endless loop to prevent program from ending
  while (1)
    ;

} /* main */
