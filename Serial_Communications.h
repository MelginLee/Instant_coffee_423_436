/******************************************************************************/
/*
* File				: Serial_Communications.h
* Description		: Description
* Author			: Melgin
* Created on		: Date
*/
/******************************************************************************/

#ifndef _Serial_Communications_h_
#define	_Serial_Communications_h_

/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/
#include <xc.h>         /* XC8 General Include File */
#include "M_Config.h"






/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
extern	uchar rc_byte_cnt;
extern	uchar tx_byte_long;
extern	uchar tx_byte_cnt;
extern	uint  sms_chk_sum;
extern	uchar wmq_selected_value;
extern	uchar Coins_count;					//设备自带投币器 投币数量 , 应答查询 数据 第四字节： 支付金额： 0x00 （设备上传服务器的投币数， 每次应答后清 0）
extern	uint R_rc_blank_cnt;
extern	uchar wmq_status;
extern	uchar rtemp;
extern	uchar R_sms_buf[25];
extern	uchar T_sms_buf[25];







/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/
void Serial_Pro(void);
void SerialInit(void);

#endif
