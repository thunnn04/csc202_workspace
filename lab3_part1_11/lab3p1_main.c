
//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//
//  DESIGNER NAME:  Thun Nadi
//
//       LAB NAME:  Lab 3, part 1
//
//      FILE NAME:  lab3p1_main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This program serves as a practice exercise for students to implement 
//    bitwise operations in C. It includes a series of problems, each 
//    requiring the manipulation of bits in a test register using bitwise 
//    operations. The program initializes the UART communication, sets up 
//    the system clock, and provides a function 'msp_printf' to print 
//    messages over UART. 
//
//    Each problem is outlined with a description, and students are instructed 
//    to write code to solve the problem within the designated sections. 
//    Upon execution, the program demonstrates the effect of various 
//    bitwise operations on the test register, along with the corresponding 
//    output messages indicating the results of each operation. After 
//    completing all the problems, the program terminates.
//
//*****************************************************************************
//*****************************************************************************

//-----------------------------------------------------------------------------
// Loads standard C include files
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Loads MSP launchpad board support macros and definitions
//-----------------------------------------------------------------------------
#include <ti/devices/msp/msp.h>
#include "clock.h"
#include "LaunchPad.h"
#include "uart.h"


//-----------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------
void msp_printf(char* buffer, unsigned int value);

uint32_t set_bit(uint32_t reg_value, uint32_t bit_mask);
uint32_t clear_bit(uint32_t reg_value, uint32_t bit_mask);
bool check_bit(uint32_t reg_value, uint32_t bit_mask);


//-----------------------------------------------------------------------------
// Define symbolic constants used by program
//-----------------------------------------------------------------------------
#define BAUD_RATE 115200

// Symbolic constants to the bit fields with the register
// To do: enter your code here to create symbolic constants

#define PIE_MASK       (1u << 0)
#define EME_MASK        (1u << 1)
#define RD_MASK         (1u << 2)
#define MD_MASK         (1u << 3)

#define A0_MASK         (1u << 12)
#define A1_MASK         (1u << 13)
#define A2_MASK         (1u << 14)  
#define A3_MASK         (1u << 15)

#define CRS_MASK        (0x7u << 4)  
#define MODE_MASK       (0x3u << 7)  
#define PRS_MASK        (0x7u << 9)   
#define A_MASK          (0xFu << 12)  

#define MODE_10         (0x2u << 7)   
#define MODE_11         (0x3u << 7)


//-----------------------------------------------------------------------------
// Define global variable and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------

int main(void)
{
  // Create a pointer to memory location of the register
  volatile uint32_t* test_reg32 = (uint32_t *)(&SysTick->LOAD);

  // create local variable to hold register value
  uint32_t reg_value;

  // Zero out the register  
  *test_reg32  = 0x0;
  
  clock_init_40mhz();
  launchpad_gpio_init();
  lp_leds_init();

  UART_init(BAUD_RATE);

  msp_printf("******* PROGRAM RUNNING *******\r\n\r\n",0);
  msp_printf("      CSC202 SPRING 2026\r\n",0);
  msp_printf("ARM M0+ C Bitwise Test Program\r\n",0);
  msp_printf("--------------------------------------------------------\r\n",0);

  // Display the size of the test register
  msp_printf("The size of the test reg is 0x%X bytes\r\n", sizeof(*test_reg32));

  // Display the value of the test register
  msp_printf("The starting value of test reg is 0x%04X\r\n", *test_reg32);
  msp_printf("\r\n",0);


  // ***************************************************************************
  // PROBLEM 1: Set the PIE bit in test register (test_reg32)
  // ***************************************************************************
  msp_printf("PROBLEM 1: Setting PIE bit\r\n", 0);

  // enter your code here for problem 1

  reg_value = *test_reg32;
  reg_value = set_bit(reg_value, PIE_MASK);
  *test_reg32 = reg_value;

  msp_printf("    --> Test reg = 0x%04X\r\n", *test_reg32);
  msp_printf("\r\n",0);



  // ***************************************************************************
  // PROBLEM 2: Set the RD bit in test register
  // ***************************************************************************
  msp_printf("PROBLEM 2: Setting RD bit\r\n", 0);

  // enter your code here for problem 2
  reg_value = *test_reg32;
  reg_value = set_bit(reg_value, RD_MASK);
  *test_reg32 = reg_value;

  msp_printf("    --> Test reg = 0x%04X\r\n", *test_reg32);
  msp_printf("\r\n",0);


  // ***************************************************************************
  // PROBLEM 3: Set the CRS bits in test register
  // ***************************************************************************
  msp_printf("PROBLEM 3: Setting CRS bits\r\n", 0);

  // enter your code here for problem 3
  reg_value = *test_reg32;
  reg_value = set_bit(reg_value, CRS_MASK);
  *test_reg32 = reg_value;

  msp_printf("    --> Test reg = 0x%04X\r\n", *test_reg32);
  msp_printf("\r\n",0);


  // ***************************************************************************
  // PROBLEM 4: Set the A[3:0] bits in test register
  // ***************************************************************************
  msp_printf("PROBLEM 4: Setting A[3:0] bits\r\n", 0);

  // enter your code here for problem 4
  reg_value = *test_reg32;
  reg_value = set_bit(reg_value, A_MASK);
  *test_reg32 = reg_value;

  msp_printf("    --> Test reg = 0x%04X\r\n", *test_reg32);
  msp_printf("\r\n",0);


  // ***************************************************************************
  // PROBLEM 5: Use an IF statement to test it A2 bit is set
  //            if A2 = 1 then print "Bit A2 is 1"
  //            else print "The bit A2 is 0"
  // ***************************************************************************
  msp_printf("PROBLEM 5: Testing bit A2\r\n", 0);

  // enter your code here for problem 5
  if (check_bit(*test_reg32, A2_MASK))
      msp_printf("Bit A2 is 1\r\n", 0);
  else
      msp_printf("The bit A2 is 0\r\n", 0);

  msp_printf("\r\n",0);


  // ***************************************************************************
  // PROBLEM 6: Clear A2 bit in test register
  // ***************************************************************************
  msp_printf("PROBLEM 6: Clearing A[2] bit\r\n", 0);

  // enter your code here for problem 6
  reg_value = *test_reg32;
  reg_value = clear_bit(reg_value, A2_MASK);
  *test_reg32 = reg_value;

  msp_printf("    --> Test reg = 0x%04X\r\n", *test_reg32);
  msp_printf("\r\n",0);


  // ***************************************************************************
  // PROBLEM 7: Clear CRS bits and set PRS bits in test register
  // ***************************************************************************
  msp_printf("PROBLEM 7: Clear CRS bits and set PRS bits\r\n", 0);

  // enter your code here for problem 7
  reg_value = *test_reg32;
  reg_value = clear_bit(reg_value, CRS_MASK);
  reg_value = set_bit(reg_value, PRS_MASK);
  *test_reg32 = reg_value;

  msp_printf("    --> Test reg = 0x%04X\r\n", *test_reg32);
  msp_printf("\r\n",0);

  // ***************************************************************************
  // PROBLEM 8: Use an IF statement to test if A2 is set
  //            if A2 = 1 then
  //                print "Bit A2=1 so clearing it"
  //                modify the reg to clear the bit
  //            else
  //                print "Bit A2=0 so setting it"
  //                modify the reg to set the bit
  // ***************************************************************************
  msp_printf("PROBLEM 8: Testing bit A2\r\n", 0);

  // enter your code here for problem 8
  reg_value = *test_reg32;

  if (check_bit(reg_value, A2_MASK))
  {
      msp_printf("Bit A2=1 so clearing it\r\n", 0);
      reg_value = clear_bit(reg_value, A2_MASK);
  }
  else
  {
      msp_printf("Bit A2=0 so setting it\r\n", 0);
      reg_value = set_bit(reg_value, A2_MASK);
  }

  *test_reg32 = reg_value;

  msp_printf("    --> Test reg = 0x%04X\r\n", *test_reg32);
  msp_printf("\r\n",0);


  // ***************************************************************************
  // PROBLEM 9: Use an IF statement to test it MD is 0
  //            if MD = 0 then
  //                print "Bit MD=0, setting mode=10"
  //                set MODE to 10
  //            else
  //                print "Bit MD=1, setting mode=11"
  //                set MODE to 11
  // ***************************************************************************
  msp_printf("PROBLEM 9: Testing bit MD & setting mode bits\r\n", 0);

  // enter your code here for problem 9
  reg_value = *test_reg32;

  if (!check_bit(reg_value, MD_MASK))
  {
      msp_printf("Bit MD=0, setting mode=10\r\n", 0);
      reg_value = clear_bit(reg_value, MODE_MASK);
      reg_value = set_bit(reg_value, MODE_10);
  }
  else
  {
      msp_printf("Bit MD=1, setting mode=11\r\n", 0);
      reg_value = clear_bit(reg_value, MODE_MASK);
      reg_value = set_bit(reg_value, MODE_11);
  }

  *test_reg32 = reg_value;

  msp_printf("    --> Test reg = 0x%04X\r\n", *test_reg32);
  msp_printf("\r\n",0);



// ***************************************************************************
// PROBLEM 10: Clear all bits in test register
// ***************************************************************************
  msp_printf("PROBLEM 10: Clearing all bits\r\n", 0);

// enter your code here for problem 10
  *test_reg32 = 0x0;

  msp_printf("    --> Test reg = 0x%04X\r\n", *test_reg32);
  msp_printf("\r\n",0);

  msp_printf(" *** PROGRAM TERMINATED ***\r\n",0);

  for(;;);

} /* main */

uint32_t set_bit(uint32_t reg_value, uint32_t bit_mask)
{
    return reg_value | bit_mask;
}

uint32_t clear_bit(uint32_t reg_value, uint32_t bit_mask)
{
    return reg_value & ~bit_mask;
}

bool check_bit(uint32_t reg_value, uint32_t bit_mask)
{
    return (reg_value & bit_mask) == bit_mask;
}


//-----------------------------------------------------------------------------
// DESCRIPTION:
//  This function sets the specified bit(s) in a 32-bit register value using
//  the provided bit mask. It performs a bitwise OR operation to set the bit(s).
//
// INPUT PARAMETERS:
//  reg_value - The original 32-bit register value.
//  bit_mask  - The 32-bit mask indicating which bit(s) to clear.
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  uint32_t - The modified register value with the specified bit(s) set.
// -----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// DESCRIPTION:
//  This function clears the specified bit(s) in a 32-bit register value using
//  the provided bit mask. It performs a bitwise AND operation with the
//  complement of the bit mask to clear the bit(s).
//
// INPUT PARAMETERS:
//  reg_value - The original 32-bit register value.
//  bit_mask  - The 32-bit mask indicating which bit(s) to clear.
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  uint32_t - The modified register value with the specified bit(s) cleared.
// -----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// DESCRIPTION:
//  This function checks if the specified bit(s) in a 32-bit register value are
//  set using the provided bit mask. It performs a bitwise AND operation to 
//  verify if the bit(s) are set.
//
// INPUT PARAMETERS:
//  reg_value - a 32-bit register value to check.
//  bit_mask  - a 32-bit mask indicating which bit(s) to check.
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  bool - true if the specified bit(s) are set, false otherwise.
// -----------------------------------------------------------------------------