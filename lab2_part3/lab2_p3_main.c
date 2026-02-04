//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Thun Nadi
//
//       LAB NAME:  Lab 2 Part 3
//
//      FILE NAME:  main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This project runs on the LP_MSPM0G3507 LaunchPad board interfacing to
//    the CSC202 Expansion board.
//
//    This code prints MSPM0 data type sizes and union byte layout over UART ***
//
//*****************************************************************************
//*****************************************************************************

//-----------------------------------------------------------------------------
// Loads standard C include files
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

//-----------------------------------------------------------------------------
// Loads MSP launchpad board support macros and definitions
//-----------------------------------------------------------------------------
#include <ti/devices/msp/msp.h>
#include "clock.h"
#include "LaunchPad.h"
#include "uart.h"

void UART_init(uint32_t baud);
void UART_out_char(char c);

//-----------------------------------------------------------------------------
// Define a structure to hold different data types
//-----------------------------------------------------------------------------
typedef struct
{
    unsigned long int  LA;
    signed   long int  LB;
    unsigned      int  IA[2];
    signed        int  IB[2];
    unsigned short int SA[2];
    signed   short int SB[2];
    unsigned      char CA[4];
    signed        char CB[4];

} test_struct;

//-----------------------------------------------------------------------------
// Define a union to hold values in different formats
//-----------------------------------------------------------------------------
typedef union
{
    unsigned long int  LA;
    signed   long int  LB;
    unsigned      int  IA;
    signed        int  IB;
    unsigned short int SA[2];
    signed   short int SB[2];
    unsigned      char CA[4];
    signed        char CB[4];

} union_32;

//-----------------------------------------------------------------------------
// Function prototype (msp_printf is defined after main)
//-----------------------------------------------------------------------------
void msp_printf(const char* buffer, unsigned int value);

//-----------------------------------------------------------------------------
int main(void)
{
    test_struct struct_var;
    union_32    union_var;

    // Initialize long integer
    union_var.LA = 0x1234ABCD;   // Run #1 (later change to 0x1234ABCD for Run #2)

    clock_init_40mhz();
    launchpad_gpio_init();
    lp_leds_init();

    UART_init(115200);

    msp_printf(" *** PROGRAM RUNNING ***\r\n\r\n\r\n", 0);
    msp_printf("CSC-202 MSPM0G3507 (ARM M0+) Data Type Tester\r\n", 0);
    msp_printf("--------------------------------------------------------------\r\n", 0);

    msp_printf(" unsigned long int = 0x%x bytes\r\n", (unsigned int)sizeof(struct_var.LA));
    msp_printf("   signed long int = 0x%x bytes\r\n", (unsigned int)sizeof(struct_var.LB));
    msp_printf("      unsigned int = 0x%x bytes\r\n", (unsigned int)sizeof(struct_var.IA[0]));
    msp_printf("        signed int = 0x%x bytes\r\n", (unsigned int)sizeof(struct_var.IB[0]));
    msp_printf("unsigned short int = 0x%x bytes\r\n", (unsigned int)sizeof(struct_var.SA[0]));
    msp_printf("  signed short int = 0x%x bytes\r\n", (unsigned int)sizeof(struct_var.SB[0]));
    msp_printf("     unsigned char = 0x%x bytes\r\n", (unsigned int)sizeof(struct_var.CA[0]));
    msp_printf("       signed char = 0x%x bytes\r\n", (unsigned int)sizeof(struct_var.CB[0]));

    msp_printf("\r\n", 0);
    msp_printf(" The structure = 0x%x bytes\r\n", (unsigned int)sizeof(struct_var));
    msp_printf("     The union = 0x%x bytes\r\n", (unsigned int)sizeof(union_var));
    msp_printf("\r\n", 0);

    msp_printf("Dumping data values from Union\r\n", 0);
    msp_printf("--------------------------------------------------------------\r\n", 0);

    msp_printf(" union unsigned long int LA = 0x%X\r\n", (unsigned int)union_var.LA);
    msp_printf("   union signed long int LB = 0x%X\r\n", (unsigned int)union_var.LB);
    msp_printf("\r\n", 0);

    msp_printf("       union unsigned int IA = 0x%X\r\n", (unsigned int)union_var.IA);
    msp_printf("         union signed int IB = 0x%X\r\n", (unsigned int)union_var.IB);
    msp_printf("\r\n", 0);

    msp_printf(" union unsigned short int SA[0] = 0x%X\r\n", (unsigned int)union_var.SA[0]);
    msp_printf("   union signed short int SB[0] = 0x%X\r\n", (unsigned int)union_var.SB[0]);
    msp_printf(" union unsigned short int SA[1] = 0x%X\r\n", (unsigned int)union_var.SA[1]);
    msp_printf("   union signed short int SB[1] = 0x%X\r\n", (unsigned int)union_var.SB[1]);
    msp_printf("\r\n", 0);

    msp_printf("         union unsigned char CA[0] = 0x%X\r\n", (unsigned int)union_var.CA[0]);
    msp_printf("         union unsigned char CA[1] = 0x%X\r\n", (unsigned int)union_var.CA[1]);
    msp_printf("         union unsigned char CA[2] = 0x%X\r\n", (unsigned int)union_var.CA[2]);
    msp_printf("         union unsigned char CA[3] = 0x%X\r\n", (unsigned int)union_var.CA[3]);
    msp_printf("\r\n", 0);

    // ✅ cast to unsigned char so negative signed char doesn't print weird hex
    msp_printf("           union signed char CB[0] = 0x%X\r\n", (unsigned int)(unsigned char)union_var.CB[0]);
    msp_printf("           union signed char CB[1] = 0x%X\r\n", (unsigned int)(unsigned char)union_var.CB[1]);
    msp_printf("           union signed char CB[2] = 0x%X\r\n", (unsigned int)(unsigned char)union_var.CB[2]);
    msp_printf("           union signed char CB[3] = 0x%X\r\n", (unsigned int)(unsigned char)union_var.CB[3]);

    msp_printf("\r\n\r\n", 0);
    msp_printf(" *** PROGRAM TERMINATED ***\r\n", 0);

    for (;;)
    {
        // loop forever
    }
} /* main */

//-----------------------------------------------------------------------------
// msp_printf
//-----------------------------------------------------------------------------
void msp_printf(const char* buffer, unsigned int value)
{
    unsigned int i = 0;
    int len = 0;
    char string[80];

    // ✅ safer than sprintf
    len = snprintf(string, sizeof(string), buffer, value);
    if (len < 0)
    {
        return;
    }

    for (i = 0; i < (unsigned int)len; i++)
    {
        UART_out_char(string[i]);
    }
}/* msp_printf */
