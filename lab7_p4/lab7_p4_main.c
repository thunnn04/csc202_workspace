// //*****************************************************************************
// //*****************************    C Source Code    ***************************
// //*****************************************************************************
// //  DESIGNER NAME:  TBD
// //
// //       LAB NAME:  TBD
// //
// //      FILE NAME:  main.c
// //
// //-----------------------------------------------------------------------------
// //
// // DESCRIPTION:
// //    This project runs on the LP_MSPM0G3507 LaunchPad board interfacing to
// //    the CSC202 Expansion board.
// //
// //    This code ... *** COMPLETE THIS BASED ON LAB REQUIREMENTS ***
// //
// //*****************************************************************************
// //*****************************************************************************

// //-----------------------------------------------------------------------------
// // Loads standard C include files
// //-----------------------------------------------------------------------------
// #include <stdbool.h>
// #include <stdio.h>
// #include <stdlib.h>

// //-----------------------------------------------------------------------------
// // Loads MSP launchpad board support macros and definitions
// //-----------------------------------------------------------------------------
// #include "LaunchPad.h"
// #include "clock.h"
// #include "lcd1602.h"
// #include <ti/devices/msp/msp.h>

// // Define a structure to hold different data types

// #define MSPM0_CLOCK_FREQUENCY (40E6)
// #define SYST_TICK_PERIOD (10.25E-3)
// #define SYST_TICK_PERIOD_COUNT (SYST_TICK_PERIOD * MSPM0_CLOCK_FREQUENCY)

// volatile bool pb1_pressed = false;
// volatile bool pb2_toggle  = false;

// void config_pb_interrupts(void)
// {
//   GPIOA->ICLR = (1 << PB1_PIN) | (1 << PB2_PIN);

//   GPIOA->POLARITY |= (1 << PB1_PIN) | (1 << PB2_PIN);

//   GPIOA->IMASK |= (1 << PB1_PIN) | (1 << PB2_PIN);
//   NVIC_EnableIRQ(GPIOA_INT_IRQn);
// }

// void GROUP1_IRQHandler(void)
// {
//   uint32_t status = GPIOA->MIS;

//   if (status & (1 << PB1_PIN))
//   {
//     pb1_pressed = true;
//     GPIOA->ICLR = (1 << PB1_PIN);
//   }

//   if (status & (1 << PB2_PIN))
//   {
//     pb2_toggle  = !pb2_toggle;
//     GPIOA->ICLR = (1 << PB2_PIN);
//   }
// }

// void run_lab7_part4(void)
// {
//   uint8_t value = 0;
//   bool    done  = false;

//   lcd_clear();
//   leds_off();
//   seg7_off();

//   config_pb_interrupts();

//   while (!done)
// {
//     lcd_clear();
//     lcd_set_ddram_addr(0x06);
//     lcd_write_byte(value);

//     leds_output(value);

//     msec_delay(200);

//     value++;
//     if (value == 100)
//         value = 0;

//     lcd_set_ddram_addr(0x40);

//     if (pb2_toggle)
//         lcd_write_string("PB2 PRESSED     ");
//     else
//         lcd_write_string("                ");

//     if (pb1_pressed)
//         done = true;
// }

//   lcd_clear();
//   lcd_set_ddram_addr(0x00);
//   lcd_write_string("Program Stopped");
// }

// int main(void)
// {

//   clock_init_40mhz();
//   launchpad_gpio_init();

//   I2C_mstr_init();
//   lcd1602_init();

//   leds_init();
//   leds_disable();
//   seg7_init();
//   dipsw_init();

//   --enable_irq();

//   run_lab7_part4();

//   while (1)
//     ;
// }
