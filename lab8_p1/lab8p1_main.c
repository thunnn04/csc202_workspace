// <<<<<<< HEAD
// #include "LaunchPad.h"
// #include "adc.h"
// #include "clock.h"
// #include "lcd1602.h"
// #include "ti/devices/msp/m0p/mspm0g350x.h"
// #include <stdbool.h>
// #include <stdint.h>
// #include <ti/devices/msp/msp.h>

// #define THRESHOLD 2047
// #define OPA_GAIN_FACTOR 0

// void run_lab8_part1(void);

// volatile bool pb1_pressed = false;
// volatile bool pb2_pressed = false;

// void config_pb1_interrupt(void)
// {
//   GPIOB->POLARITY31_16 = GPIO_POLARITY31_16_DIO18_RISE;
//   GPIOB->CPU_INT.ICLR  = GPIO_CPU_INT_ICLR_DIO18_CLR;
//   GPIOB->CPU_INT.IMASK = GPIO_CPU_INT_IMASK_DIO18_SET;

//   NVIC_SetPriority(GPIOB_INT_IRQn, 2);
//   NVIC_EnableIRQ(GPIOB_INT_IRQn);
// }

// void config_pb2_interrupt(void)
// {
//   GPIOA->POLARITY15_0  = GPIO_POLARITY15_0_DIO15_RISE;
//   GPIOA->CPU_INT.ICLR  = GPIO_CPU_INT_ICLR_DIO15_CLR;
//   GPIOA->CPU_INT.IMASK = GPIO_CPU_INT_IMASK_DIO15_SET;

//   NVIC_SetPriority(GPIOA_INT_IRQn, 2);
//   NVIC_EnableIRQ(GPIOA_INT_IRQn);
// }
// void GROUP1_IRQHandler(void)
// {
//   uint32_t group_gpio_iidx;
//   uint32_t gpio_mis;

//   do
//   {
//     group_gpio_iidx = CPUSS->INT_GROUP[1].IIDX;

//     switch (group_gpio_iidx)
//     {
//       case CPUSS_INT_GROUP_IIDX_STAT_INT0:
//         gpio_mis = GPIOA->CPU_INT.MIS;

//         if ((gpio_mis & GPIO_CPU_INT_MIS_DIO15_MASK) ==
//             GPIO_CPU_INT_MIS_DIO15_SET)
//         {
//           pb2_pressed         = true;
//           GPIOB->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO15_CLR;
//         }
//         break;

//       case CPUSS_INT_GROUP_IIDX_STAT_INT1:
//         gpio_mis = GPIOB->CPU_INT.MIS;

//         if ((gpio_mis & GPIO_CPU_INT_MIS_DIO18_MASK) ==
//             GPIO_CPU_INT_MIS_DIO18_SET)
//         {
//           pb1_pressed         = true;
//           GPIOB->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO18_CLR;
//         }
//         break;
//     }

//   } while (group_gpio_iidx != 0);
// }

// int main(void)
// {
//   clock_init_40mhz();
//   launchpad_gpio_init();
//   I2C_mstr_init();
//   ADC0_init(ADC12_MEMCTL_VRSEL_VDDA_VSSA);
//   lcd1602_init();
//   leds_init();
//   leds_disable();
//   dipsw_init();

//   config_pb1_interrupt();
//   config_pb2_interrupt();

//   run_lab8_part1();

//   while (1)
//     ;
// }

// void run_lab8_part1(void)
// {
//   uint32_t adc_val;
//   bool     done = false;

//   OPA0_init(OPA_GAIN_FACTOR);
//   OPA0_enable();
//   ADC0_init(ADC12_MEMCTL_VRSEL_VDDA_VSSA);

//   lcd_clear();
  
//   while (!done)
//   {
//     adc_val = ADC0_in(7);

//     lcd_set_ddram_addr(LCD_LINE1_ADDR);

//     if (adc_val < THRESHOLD)
//       lcd_write_string("Status: Dark ");
//     else
//       lcd_write_string("Status: Light");

//     lcd_set_ddram_addr(LCD_LINE2_ADDR);
//     lcd_write_string("ADC = ");
//     lcd_write_doublebyte(adc_val);

//     msec_delay(300);

//     if (pb1_pressed)
//     {
//       done = true;
//     }
//   }

//   lcd_clear();
//   lcd_set_ddram_addr(LCD_LINE1_ADDR);
//   lcd_write_string("Part 1 Done");
// }
// =======
// #include <stdbool.h>
// #include <stdint.h>
// #include "LaunchPad.h"
// #include "clock.h"
// #include "lcd1602.h"
// #include "adc.h"
// #include <ti/devices/msp/msp.h>

// #define OPA_GAIN_FACTOR 3
// #define THRESHOLD 3500

// int main(void)
// {
//     uint32_t adc_val;
//     bool done = false;

//     clock_init_40mhz();
//     launchpad_gpio_init();

//     I2C_mstr_init();
//     lcd1602_init();

//     leds_init();
//     leds_disable();
//     seg7_init();
//     seg7_off();

//     lcd_clear();
//     lcd_set_ddram_addr(0x00);
//     lcd_write_string("Before ADC");
//     msec_delay(1000);

//     OPA0_init(OPA_GAIN_FACTOR);
//     OPA0_enable();
//     ADC0_init(ADC12_MEMCTL_VRSEL_VDDA_VSSA);

//     lcd_clear();
//     lcd_set_ddram_addr(0x00);
//     lcd_write_string("ADC init done");
//     msec_delay(1000);

//     lcd_clear();

//     while (!done)
//     {
//         adc_val = ADC0_in(7);

//         lcd_set_ddram_addr(0x00);
//         if (adc_val < THRESHOLD)
//             lcd_write_string("Status: Dark  ");
//         else
//             lcd_write_string("Status: Light ");

//         lcd_set_ddram_addr(0x40);
//         lcd_write_string("ADC = ");
//         lcd_write_doublebyte(adc_val);
//         lcd_write_string("   ");

//         msec_delay(300);

//         if (is_pb_down(PB1_IDX))
//             done = true;
//     }

//     lcd_clear();
//     lcd_set_ddram_addr(0x00);
//     lcd_write_string("Program Stopped");

//     while (1)
//         ;
// }
// >>>>>>> f75d180ebd3aed475433496b459261076edc211e
