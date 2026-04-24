//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Thun Nadi
//
//       LAB NAME:  LAB 11
//
//      FILE NAME:  lab11_p1_main.c
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
#include "spi.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"
#include "uart.h"
#include <ti/devices/msp/msp.h>

//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void     run_lab11_part1(void);
void     display_menu(void);
void     uart_write_string(char *str);
uint16_t string_to_uint16(const char string[]);
uint8_t  get_spi_data(void);
void     update_leds(void);

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
  uart_write_string("1. Set Data to Send\r\n");
  uart_write_string("2. Send Data\r\n");
  uart_write_string("3. Update LEDs\r\n");
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

uint16_t string_to_uint16(const char string[])
{
  uint16_t result = 0;
  uint8_t  index  = 0;

  while (string[index] != NULL)
  {
    result = result * 10 + (string[index] - '0');
    index++;
  }

  return result;
}

uint8_t get_spi_data(void)
{
  char     buffer[6];
  uint8_t  idx = 0;
  char     ch;
  uint16_t value;

  uart_write_string("Set SPI xmit data menu selected\r\n");
  uart_write_string("Enter a valid decimal number (0 to 255): ");

  while (1)
  {
    ch = UART_in_char();

    if (ch == '\r')
    {
      buffer[idx] = '\0';
      uart_write_string("\r\n");
      break;
    }
    else if (ch == '\b')
    {
      if (idx > 0)
      {
        idx--;
        UART_out_char('\b');
        UART_out_char(' ');
        UART_out_char('\b');
      }
    }
    else
    {

      if (idx < 5)
      {
        buffer[idx++] = ch;
        UART_out_char(ch);
      }
    }
  }

  for (int i = 0; buffer[i] != '\0'; i++)
  {
    if (buffer[i] < '0' || buffer[i] > '9')
    {
      {
        uart_write_string("ERROR: Invalid data entered.\r\n");
        uart_write_string("SPI data set to 0.\r\n");
        return 0;
      }
    }

    value = string_to_uint16(buffer);

    if (value > 255)
    {
      uart_write_string("ERROR: Invalid data entered.\r\n");
      uart_write_string("SPI data set to 0.\r\n");
      return 0;
    }

    uart_write_string("SPI data set to : ");
    uart_write_string(buffer);
    uart_write_string("\r\n");
    return (uint8_t)value;
  }
}

void update_leds(void)
{
  GPIOB->DOUT31_0 |= LP_SPI_CS0_MASK;
  msec_delay(5);
  GPIOB->DOUT31_0 &= ~LP_SPI_CS0_MASK;
}

void run_lab11_part1(void)
{
  bool done = false;
  char choice;

  uint8_t spi_data  = 0;
  uint8_t recv_data = 0;

  lcd_clear();
  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string("Program Running");
  lcd_set_ddram_addr(LCD_LINE2_ADDR);
  lcd_write_string("See serial port");

  while (!done)
  {
    display_menu();

    choice = UART_in_char();
    UART_out_char(choice);
    uart_write_string("\r\n");

    if (choice == '1')
    {
      spi_data = get_spi_data();
    }

    else if (choice == '2')
    {
      spi1_write_data(spi_data);
      recv_data = spi1_read_data();

      lcd_clear();

      lcd_set_ddram_addr(LCD_LINE1_ADDR);
      lcd_write_string("SENT = ");
      lcd_write_doublebyte(spi_data);

      lcd_set_ddram_addr(LCD_LINE2_ADDR);
      lcd_write_string("RCVD = ");
      lcd_write_doublebyte(recv_data);
    }

    else if (choice == '3')
    {
      update_leds();
    }

    else if (choice == '4')
    {
      uart_write_string("Thank you for using the program\r\n");

      lcd_clear();
      lcd_set_ddram_addr(LCD_LINE1_ADDR);
      lcd_write_string("Program Stopped");

      done = true;
    }

    else
    {
      uart_write_string("ERROR: Invalid option\r\n");
    }
  }
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

  spi1_init();

  // Remap CS0 pin back to GPIO
  IOMUX->SECCFG.PINCM[LP_SPI_CS0_IOMUX] =
      (IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM23_PF_GPIOB_DIO06);
  GPIOB->DOE31_0 |= LP_SPI_CS0_MASK;

  run_lab11_part1();

  // Endless loop to prevent program from ending
  while (1)
    ;
}

