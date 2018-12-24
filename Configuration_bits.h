/******************************************************************************/
/* Files to Include                                                      */
/******************************************************************************/
    #include <xc.h>         /* XC8 General Include File */


// PIC16F887 Configuration Bit Settings

// 'C' source line config statements


#ifndef _Configuration_b_
	#define	_Configuration_b_

	#define _debug_Configuration_b_    0

	#if (_debug_Configuration_b_ ==1)
		#define _rebuild_flag_
		// CONFIG1
		#pragma config FOSC = INTRC_NOCLKOUT	// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
		#pragma config WDTE = ON				// Watchdog Timer Enable bit (WDT enabled)
		#pragma config PWRTE = ON				// Power-up Timer Enable bit (PWRT enabled)
		#pragma config MCLRE = ON				// RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR )
		#pragma config CP = ON					// Code Protection bit (Program memory code protection is enabled)
		#pragma config CPD = OFF				// Data Code Protection bit (Data memory code protection is disabled)
		#pragma config BOREN = ON				// Brown Out Reset Selection bits (BOR enabled)
		#pragma config IESO = OFF				// Internal External Switchover bit (Internal/External Switchover mode is disabled)
		#pragma config FCMEN = ON				// Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
		#pragma config LVP = OFF				// Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

		// CONFIG2
		#pragma config BOR4V = BOR21V			// Brown-out Reset Selection bit (Brown-out Reset set to 2.1V)
		#pragma config WRT = OFF				// Flash Program Memory Self Write Enable bits (Write protection off)
	#else
		// CONFIG1
		#pragma config FOSC = INTRC_NOCLKOUT	// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
		#pragma config WDTE = ON				// Watchdog Timer Enable bit (WDT enabled)
		#pragma config PWRTE = ON				// Power-up Timer Enable bit (PWRT enabled)
		#pragma config MCLRE = ON				// RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR )
		#pragma config CP = ON					// Code Protection bit (Program memory code protection is enabled)
		#pragma config CPD = OFF				// Data Code Protection bit (Data memory code protection is disabled)
		#pragma config BOREN = ON				// Brown Out Reset Selection bits (BOR enabled)
		#pragma config IESO = OFF				// Internal External Switchover bit (Internal/External Switchover mode is disabled)
		#pragma config FCMEN = ON				// Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
		#pragma config LVP = OFF				// Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

		// CONFIG2
		#pragma config BOR4V = BOR21V   // Brown-out Reset Selection bit (Brown-out Reset set to 2.1V)
		#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
	#endif
#endif

