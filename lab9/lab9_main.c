//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Thun Nadi
//
//       LAB NAME:  Lab 9
//
//      FILE NAME:  lab9_main.c
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
#include "demo_melodies.h"
#include "lcd1602.h"
#include "pitches.h"
#include <ti/devices/msp/msp.h>

//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void run_lab9(void);
void motor0_set_pwm_freq(uint16_t pwm_frequency);
void config_pb1_interrupt(void);
void config_pb2_interrupt(void);
void play_note(uint16_t frequency, uint16_t duration_ms);

//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define DEFAULT_DUTY_CYCLE (50) // %
#define DEFAULT_FREQUENCY (50)  // Hz

#define disable_active_buzzer led_on(LED_BAR_LD1_IDX)
#define enable_active_buzzer led_off(LED_BAR_LD1_IDX)
#define disable_passive_buzzer led_on(LED_BAR_LD2_IDX)
#define enable_passive_buzzer led_off(LED_BAR_LD2_IDX)
//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------

// Define a structure to hold different data types

volatile bool pb1_pressed = false;
volatile bool pb2_pressed = false;

void play_note(uint16_t frequency, uint16_t duration_ms)
{
  if (frequency != 0)
  {
    motor0_set_pwm_freq(frequency);
    motor0_set_pwm_dc(50);
    motor0_pwm_enable();
    msec_delay(duration_ms);
    motor0_pwm_disable();
  }
}

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

void motor0_set_pwm_freq(uint16_t pwm_frequency)
{
  // Determine what the timer clock divider and prescaler are
  uint32_t tim_clk_divider = TIMA0->CLKDIV + 1;
  uint32_t tim_clk_scaler = (TIMA0->COMMONREGS.CPS & GPTIMER_CPS_PCNT_MASK) + 1;
  // Now determine what the timer clock frequency is
  uint32_t tim_clk = get_bus_clock_freq() / (tim_clk_divider * tim_clk_scaler);
  // Update the load value based on new PWM frequency
  uint32_t load_value = (tim_clk / pwm_frequency);
  // Disable timer and update the load register
  motor0_pwm_disable();
  TIMA0->COUNTERREGS.LOAD = (load_value - 1) & GPTIMER_LOAD_LD_MASK;
} /* */

void run_lab9(void)
{
  typedef enum
  {
    BUZZER_IDLE = 0,
    ACTIVE_BUZZER,
    PASSIVE_BUZZER,
    BOTH_BUZZER,
    PASSIVE_MELODY
  } fsm_state_t;

  fsm_state_t current_state = BUZZER_IDLE;
  fsm_state_t next_state    = BUZZER_IDLE;

  bool     done = false;
  uint8_t  key_value;
  uint16_t duty_cycle = 0;
  uint16_t freq       = DEFAULT_FREQUENCY;

  lcd_clear();

  while (!done)
  {
    if (pb1_pressed)
    {
      done        = true;
      pb1_pressed = false;
    }

    if (pb2_pressed)
    {
      current_state = next_state;
      pb2_pressed   = false;
      msec_delay(500);
    }

    if (current_state != PASSIVE_MELODY)
    {
      key_value = keypad_scan();

      if (key_value != NO_KEY_PRESSED)
      {
        duty_cycle = (key_value * 100) / 15;

        msec_delay(10);
        wait_no_key_pressed();
        msec_delay(10);
      }
    }

    switch (current_state)
    {
      case BUZZER_IDLE:
        next_state = ACTIVE_BUZZER;

        disable_active_buzzer;
        disable_passive_buzzer;

        freq = DEFAULT_FREQUENCY;

        motor0_set_pwm_freq(freq);
        motor0_set_pwm_dc(duty_cycle);
        motor0_pwm_enable();

        lcd_set_ddram_addr(LCD_LINE1_ADDR);
        lcd_write_string("Buzzers Disabled");

        lcd_set_ddram_addr(LCD_LINE2_ADDR);
        lcd_write_string("F=");
        lcd_write_doublebyte(freq);
        lcd_write_string(" DC=");
        lcd_write_byte(duty_cycle);
        lcd_write_string("% ");
        break;

      case ACTIVE_BUZZER:
        next_state = PASSIVE_BUZZER;

        enable_active_buzzer;
        disable_passive_buzzer;

        motor0_set_pwm_freq(freq);
        motor0_set_pwm_dc(duty_cycle);
        lcd_set_ddram_addr(LCD_LINE1_ADDR);
        lcd_write_string("Active Buzzer   ");

        lcd_set_ddram_addr(LCD_LINE2_ADDR);
        lcd_write_string("F=");
        lcd_write_doublebyte(freq);
        lcd_write_string(" DC=");
        lcd_write_byte(duty_cycle);
        lcd_write_string("% ");
        break;

      case PASSIVE_BUZZER:
        next_state = BOTH_BUZZER;

        disable_active_buzzer;
        enable_passive_buzzer;

        motor0_set_pwm_freq(freq);
        motor0_set_pwm_dc(duty_cycle);

        lcd_set_ddram_addr(LCD_LINE1_ADDR);
        lcd_write_string("Passive Buzzer   ");

        lcd_set_ddram_addr(LCD_LINE2_ADDR);
        lcd_write_string("F=");
        lcd_write_doublebyte(freq);
        lcd_write_string(" DC=");
        lcd_write_byte(duty_cycle);
        lcd_write_string("% ");
        break;

      case BOTH_BUZZER:
        next_state = PASSIVE_MELODY;
        enable_active_buzzer;
        enable_passive_buzzer;
        motor0_set_pwm_freq(freq);
        motor0_set_pwm_dc(duty_cycle);
        motor0_pwm_enable();

        lcd_set_ddram_addr(LCD_LINE1_ADDR);
        lcd_write_string("Both Buzzers     ");

        lcd_set_ddram_addr(LCD_LINE2_ADDR);
        lcd_write_string("F=");
        lcd_write_doublebyte(freq);
        lcd_write_string(" DC=");
        lcd_write_byte(duty_cycle);
        lcd_write_string("% ");
        break;

      case PASSIVE_MELODY:
        next_state = BUZZER_IDLE;

        disable_active_buzzer;
        enable_passive_buzzer;
        motor0_pwm_enable();

        lcd_set_ddram_addr(LCD_LINE1_ADDR);
        lcd_write_string("Play Melody     ");

        // Play the melody
        for (int i = 0; i < MARIO_LENGTH; i++)
        {
          lcd_set_ddram_addr(LCD_LINE2_ADDR);
          lcd_write_string("F=");
          lcd_write_doublebyte(mario[i].freq);
          lcd_write_string(" DC=");
          lcd_write_byte(mario[i].duration);
          lcd_write_string("ms");

          play_note(mario[i].freq, mario[i].duration);
          msec_delay(30); // short pause between notes
        }                 /* for */


        freq = DEFAULT_FREQUENCY;
        duty_cycle = DEFAULT_DUTY_CYCLE;

        break;
    }

    msec_delay(10);
  }

  motor0_pwm_disable();

  lcd_clear();
  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string("Program Stopped");
}

int main(void)
{
  // Configure the LaunchPad board
  clock_init_40mhz();
  launchpad_gpio_init();
  dipsw_init();
  leds_init();
  leds_enable();
  I2C_mstr_init();
  lcd1602_init();
  keypad_init();

  // Config TIMER pin
  motor0_init();

  // Configure PWM for 50Hz at 100% Duty
  motor0_pwm_init(4000, 0);
  motor0_pwm_enable();

  // Configure PB GPIO interrupts
  config_pb1_interrupt();
  config_pb2_interrupt();

  lcd_clear();

  run_lab9();

  // Endless loop to prevent program from ending
  while (1)
    ;

} /* main */
