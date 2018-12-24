/******************************************************************************/
/*
* File				: Input_Scan.h
* Description		: Description
* Author			: Melgin
* Created on		: Date
*/
/******************************************************************************/
#ifndef _Input_Scan_h_
#define	_Input_Scan_h_


#include "M_Config.h"

extern uchar KeyValue;
extern uchar KeyValue_nmdsp;
extern uchar KeyValue_drk;
extern uchar KeyValue_set;
extern uchar Change_Cup_Channel_Timer;

extern	uint Coins_low_timer;
extern	uint R_wmq_lose_timer;




#define	KeyInPort	PORTA
#define	KeyOutPort	PORTE

void KeyportScan(void);
void Coins_chk(void);
void Cup_chk(void);
void Key_exe(void);
void Wmq_chk(void);

#endif
