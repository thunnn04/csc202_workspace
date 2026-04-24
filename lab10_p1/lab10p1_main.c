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
void run_lab10_part1(void);
void config_pb1_interrupt(void);
void config_pb2_interrupt(void);

//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define UART_BAUD_RATE 115200
//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------

// Define a structure to hold different data types

volatile bool pb1_pressed = false;
volatile bool pb2_pressed = false;

void config_pb1_interrupt(void)
{
  GPIOB->POLARITY31_16 = GPIO_POLARITY31_16_DIO18_RISE;
  GPIOB->CPU_INT.ICLR  = GPIO_CPU_INT_ICLR_DIO18_CLR;
  GPIOB->CPU_INT.IMASK = GPIO_CPU_INT_IMASK_DIO18_SET;

  NVIC_SetPriority(GPIOB_INT_IRQn, 2);
  NVIC_EnableIRQ(GPIOB_INT_IRQn);
}

void config_pb2_interrupt(void)
{
  GPIOA->POLARITY15_0  = GPIO_POLARITY15_0_DIO15_RISE;
  GPIOA->CPU_INT.ICLR  = GPIO_CPU_INT_ICLR_DIO15_CLR;
  GPIOA->CPU_INT.IMASK = GPIO_CPU_INT_IMASK_DIO15_SET;

  NVIC_SetPriority(GPIOA_INT_IRQn, 2);
  NVIC_EnableIRQ(GPIOA_INT_IRQn);
}
void GROUP1_IRQHandler(void)
{
  uint32_t group_gpio_iidx;
  uint32_t gpio_mis;

  do
  {
    group_gpio_iidx = CPUSS->INT_GROUP[1].IIDX;

    switch (group_gpio_iidx)
    {
      case CPUSS_INT_GROUP_IIDX_STAT_INT0:
        gpio_mis = GPIOA->CPU_INT.MIS;

        if ((gpio_mis & GPIO_CPU_INT_MIS_DIO15_MASK) ==
            GPIO_CPU_INT_MIS_DIO15_SET)
        {
          pb2_pressed         = true;
          GPIOB->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO15_CLR;
        }
        break;

      case CPUSS_INT_GROUP_IIDX_STAT_INT1:
        gpio_mis = GPIOB->CPU_INT.MIS;

        if ((gpio_mis & GPIO_CPU_INT_MIS_DIO18_MASK) ==
            GPIO_CPU_INT_MIS_DIO18_SET)
        {
          pb1_pressed         = true;
          GPIOB->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO18_CLR;
        }
        break;
    }

  } while (group_gpio_iidx != 0);
}

void run_lab10_part1(void)
{

  bool    done = false;
  char    buffer[10];
  uint8_t idx = 0;
  char    ch;

  lcd_clear();

  while (!done)
  {
    ch = UART_in_char();
    UART_out_char(ch);

    if (ch == '\r')
    {
      buffer[idx] = '\0';
      done        = true;
    }

    else if (ch == '\b')
    {
      if (idx != 0)
      {
        idx--;
      }
    }
    else
    {
      if (idx < 9)
      {
        buffer[idx] = ch;
        idx++;
      }
    }
  } /* while */

  lcd_clear();

  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string("Name : ");
  lcd_write_string(buffer);

  lcd_set_ddram_addr(LCD_LINE2_ADDR);
  lcd_write_string("Program Stopped");
}

int main(void)
{
  // Configure the LaunchPad board
  clock_init_40mhz();
  UART_init(UART_BAUD_RATE);
  I2C_mstr_init();
  lcd1602_init();

  run_lab10_part1();

  // Endless loop to prevent program from ending
  while (1)
    ;

} /* main */