/******************************************************************************/
/*
* File				: Drinks_Supply.h
* Description		: Description
* Author			; Melgin
* Created on		: Date
*/
/******************************************************************************/

#ifndef _Drinks_Supply_H_
#define	_Drinks_Supply_H_
#include "M_Config.h"
#include <xc.h>         /* XC8 General Include File */

/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/
const uchar drinks_rly_list[8][4]=
{
	{11,8,5,1},			//K1键，继电器闭合11号（热水切换），8号（电磁阀），5号（搅拌），1号（粉料）(开启时5号，1号比8号延迟0.5-1秒钟）
	{12,8,5,1},			//K2键。。。
	{11,9,6,2},
	{12,9,6,2},

	{11,9,6,3},
	{12,9,6,3},
	{11,10,7,4},
	{12,10,7,4}

};





/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/






/******************************************************************************/
/* Function Declaration                                                   */
/******************************************************************************/
	void offer_payment_chk(void);
	void hot_cold_ready_chk(void);
	void cup_supply(void);
	void cup_supply_over_chk(void);
	void water_supply(void);
	void powder_supply(void);
	void supply_over_chk(void);
	void supply_over(void);
	void welcome_back(void);
//extern const uchar drinks_rly_list;
#endif


