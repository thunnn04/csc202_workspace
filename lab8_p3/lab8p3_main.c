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
#include <stdint.h>
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
#include <ti/devices/msp/msp.h>

#define THRESHOLD 2047
#define OPA_GAIN_FACTOR 0

void run_lab8_part3(void);

volatile bool pb1_pressed = false;
volatile bool pb2_pressed = false;
uint16_t      g_adc_temp;

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
          g_adc_temp          = ADC0_in(5);
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

int main(void)
{
  clock_init_40mhz();
  launchpad_gpio_init();
  I2C_mstr_init();
  ADC0_init(ADC12_MEMCTL_VRSEL_VDDA_VSSA);
  lcd1602_init();
  leds_init();
  leds_enable();

  dipsw_init();

  config_pb1_interrupt();
  config_pb2_interrupt();

  run_lab8_part3();

  while (1)
    ;
}

void run_lab8_part3(void)
{
  uint32_t adc_val;
  uint8_t  num_led;
  bool     done = false;
  uint16_t temp_c;
  uint16_t temp_f;

  ADC0_init(ADC12_MEMCTL_VRSEL_VDDA_VSSA);

  lcd_clear();

  while (!done)
  {
    adc_val = ADC0_in(7);

    num_led = adc_val / 455;

    leds_off();

    for (int idx = 0; idx < num_led; idx++)
    {
      led_on(idx);
    }

    lcd_set_ddram_addr(LCD_LINE1_ADDR);
    lcd_write_string("ADC =  ");
    lcd_write_doublebyte(adc_val);
    lcd_write_string("  ");

    msec_delay(300);
    if (pb1_pressed)
    {
      done = true;
    }
    if (pb2_pressed)
    {
      temp_c = thermistor_calc_temperature(g_adc_temp);
      temp_f = (temp_c *9) / 5 +32;

      lcd_set_ddram_addr(LCD_LINE2_ADDR);
      lcd_write_string("TEMP =  ");
      lcd_write_byte(temp_f);
      lcd_write_char(0xDF);
      lcd_write_string(" F");
    }
  }

leds_off();
  lcd_clear();
  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string("Part 3 Done");
}
