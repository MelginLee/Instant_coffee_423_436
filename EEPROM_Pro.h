/******************************************************************************/
/*
* File				: EEPROM_Pro.h
* Description		: Description
* Author			: Melgin
* Created on		: Date
*/
/******************************************************************************/

#ifndef _EEPROM_Pro_h_
#define	_EEPROM_Pro_h_
#include <xc.h>         /* XC8 General Include File */

/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/






/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/






/******************************************************************************/
/* Function Prototypes														  */
/******************************************************************************/
 void Frist_data_write_chk(void);
 void data_write(void);
 void data_read(void);
 void RFS_data_write(void);
 void Clear_eeprom(void);
 uchar read(uchar addr);
 void write(uchar addr,uchar date);
#endif

