/******************************************************************************/
/*
* File				: Main.c
* Description		: OP423 / Body=PIC16F887
* Author			; Melgin
* Created on		: Date
*/
/******************************************************************************/


/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <xc.h>
#include "M_Config.h"

#include "System.h"        /* System funct/params, like osc/peripheral config */
#include "User.h"          /* User funct/params, such as InitApp */
#include "Serial_Communications.h"
#include "TaskService.h"
#include "Driver.h"
#include "ADC.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/


/******************************************************************************/
/* Exterb Function Prototypes                                                 */
/******************************************************************************/
void	interrupt isr(void);


/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/



/**************************************************************************************/
/*
* FunctionName   : main()
* Description    : 主函数
* EntryParameter : None
* ReturnValue    : None
*/
/**************************************************************************************/



void Power_on(void)
{
	InitApp();				               // 初始化
	ConfigureOscillator();                // 初始化
	Start_ADC();

	SerialInit();

	Voice(0);
	alm_set(2,0,5,10);
	nBOR =1;
	nPOR =1;
}




void main(void)
{
	Power_on();
	while (1)
    {
		if (!nBOR)
		{
			Power_on();
		}
		if (F_Sys_1MS)

		{
			TaskRemarks();
			F_Sys_1MS = 0;
		}

		TaskProcess();             // 任务处理

		asm("CLRWDT");
	}
}





