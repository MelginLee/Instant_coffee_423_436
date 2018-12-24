/******************************************************************************/
/*
* File				: System.h
* Description		: Description
* Author			: Melgin
* Created on		: Date
*/
/******************************************************************************/

/******************************************************************************/
/* System Level #define Macros                                                */
/******************************************************************************/

#ifndef _SYSTEM_H_
#define	_SYSTEM_H_
#include <xc.h>         /* XC8 General Include File */



/* TODO Define System operating frequency */

/* Microcontroller MIPs (FCY) */
#define SYS_FREQ        500000L
#define FCY             SYS_FREQ/4


#define	C_TMR0_CNT	256-1000/4					/*  1000us 中断	4分频 */
#define	C_TMR1H_CNT	((65536-15000)/1)>>8		/* 	10000us 中断 1分频 */
#define	C_TMR1L_CNT	((65536-15000)/1)%256
#define	C_PR2_CNT	124


/******************************************************************************/
/* Function Prototypes                                                 */
/******************************************************************************/
/* Custom oscillator configuration funtions, reset source evaluation
functions, and other non-peripheral microcontroller initialization functions
go here. */

void ConfigureOscillator(void); /* Handles clock switching/osc initialization */
void init_timer0(void);
void init_timer1(void);
void init_timer2(void);
void init_INT0(void);
/******************************************************************************/
#endif
