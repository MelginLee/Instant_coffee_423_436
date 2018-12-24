/******************************************************************************/
/*
* File				: User.c
* Description		: Description
* Author			; Melgin
* Created on		: Date
*/
/******************************************************************************/


/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

//#if defined(__XC)
//    #include <xc.h>         /* XC8 General Include File */
//#elif defined(HI_TECH_C)
//    #include <htc.h>        /* HiTech General Include File */
//#endif
#include <xc.h>         /* XC8 General Include File */

#include "User.h"
#include "M_Config.h"
#include "Driver.h"
#include "Input_Scan.h"
#include "EEPROM_Pro.h"
#include "Serial_Communications.h"


/******************************************************************************/
/* Global Variable Declaration                                           */
/******************************************************************************/
	uFlg uF1,uF2;	//定义联合体变量 =rly1 -rly14 Flag
	uFlg uF3;		//sys
	uFlg uF4;		//Err_Num
	uFlg uF5;		//UFlag1
	uFlg uF6;		//UFlag2
	uFlg uF7;		//UFlag3

/******************************************************************************/

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in User.h and insert code for User algorithms.> */

void InitApp(void)
{
    /* TODO Initialize User Ports/Peripherals/Project here */
    /* Setup analog functionality and port direction */
    /* Initialize peripherals */
    /* Enable Interrupts */
	Init_IO();
	Init_Rly();

    write(0x00,Coins);

	Frist_data_write_chk();
	data_read();
	reg_init();
}

void Init_IO(void)
{
	//TRISx 1=输入，0=输出

	PORTA	=0;
//	ADCON1	=0b10000000;	// AN or IO 配置选择
	TRISA	=0b10111111;			//
	//Pin  RA7: 投币器成功投币信号检测，0=成功投币，接受投币后产生20ms/40ms/60ms/ 低脉冲信号
	//Pin  RA6: 禁止投币器投币控制，1=禁止
	//Pin7 RA5:	矩阵按键行3
	//Pin6 RA4:	矩阵按键行2
	//Pin5 RA3:	有无杯检测口，1=有杯
	//Pin4 RA2:	矩阵按键行1
	//Pin3 RA1:	加热罐温度检测
	//Pin2 RA0:	水位检测

	PORTB	=0;
	nRBPU	=0;
	TRISB	=0b10100000;
	PORTB	=0b00000011;
	WPUB	=0b10100000;

	//Pin40 RB7: PGD(原图为 设置模式 检测）
	//Pin39 RB6: PGC(原图为 373OE 使能）
	//Pin38 RB5: 下杯完成信号，1=完成
	//Pin37 RB4: P_Voice_373_G 语音 74LS373 enable G =0时 Q=D, G =1时锁存
	//Pin36 RB3: 换杯电机供电控制，1=供电
	//Pin35 RB2: 加热罐供电控制，1=供电
	//Pin34 RB1: LS373_G1 74LS373 enable G =1时 Q=D, G =0时锁存
	//Pin33 RB0: LS373_G2 74LS373 enable G =1时 Q=D, G =0时锁存




	TRISC	=0b10000000;
	PORTC	=0b10000000;

	//Pin26 RC7:
	//Pin25 RC6:
	//Pin24 RC5: LS164_DSA/B
	//Pin23 RC4: LS164_CLK
	//Pin18 RC3: LED位选 DIG3
	//Pin17 RC2: LED位选 DIG2
	//Pin16 RC1: LED位选 DIG1
	//Pin15 RC0: LED位选 指示灯


	PORTD	=0;
	TRISD	=0;
	PORTD	=0b11111111;
	//Pin30 RD7: 蜂鸣器 0=使能
	//Pin29 RD6: DB7
	//Pin28 RD5: DB6
	//Pin27 RD4: DB5
	//Pin22 RD3: DB4
	//Pin21 RD2: DB3
	//Pin20 RD1: DB2
	//Pin19 RD0: DB1



	PORTE	=0;
	TRISE	=0b00001000;	//PORTE 输出
	PORTE	=0b00000111;
	//Pin10 RE2: 矩阵按键列3
	//Pin9	RE1: 矩阵按键列2
	//Pin8	RE0: 矩阵按键列1

	ANSEL  =0b00001011;
	ANSELH  =0;



}




/******************************************************************************/
//	Init_Drinks_set
/******************************************************************************/
	uchar	Coins=0;
	uchar	Seted_temperature =85;
	drinks_set_type	drinks_set[]=
	{
		 {1, 25, 45, 0},		//key1，key2
		 {1, 25, 45, 0},		//key3，key4
		 {1, 25, 45, 0},		//key5，key6
		 {1, 25, 45, 0}			//key7，key8
	};




void reg_init (void)
{
	uF1.Flg	=0;
	uF2.Flg	=0;
	uF3.Flg	=0;
	uF4.Flg	=0;
	uF5.Flg	=0;
	uF6.Flg	=0;
	uF7.Flg	=0;

	F_First_into_Setmodel =1;

	F_P_Set_init_bak =P_Set;
	F_wmq_on_line =1;

	Inout_Setmodel_init();

}

