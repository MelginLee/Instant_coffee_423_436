/******************************************************************************/
/*
* File              : Driver.h
* Description       : Description
* Author            : Melgin
* Created on        : Date
*/
/******************************************************************************/

#ifndef _Driver_h_
#define _Driver_h_

#include "M_Config.h"
#include <xc.h>         /* XC8 General Include File */

/******************************************************************************/
/* Global Typedef Prototypes                                                  */
/******************************************************************************/

//Type_Led_buf Led_buf;



/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
extern  uchar Buz_alm_times;            //
extern  uchar Buz_alm_timer;            //
extern  uchar Buz_alm_timer_duty;       // Buz_alm_timer < Buz_alm_timer_duty ?( buzzer on ：buzzer off )
extern  uchar Buz_alm_timer_cycle;      // Buz_alm_timer < Buz_alm_timer_cycle ?( buzzer on ：(buzzer off, Buz_alm_timer clear）)
extern  uchar R_Cup_Free;               //
extern  uchar R_Set_Step;               //
extern  uint  Cup_motor_on_timer;
extern  uchar Cup_motor_off_dly;
extern  uchar R_voice_dly_cnt;



/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/
    #define P_164_DA    RC5
    #define P_164_CLK   RC4
    #define P_LED3_EN   RC3
    #define P_LED2_EN   RC2
    #define P_LED1_EN   RC1
    #define P_LED0_EN   RC0
/*------UF_rly1------------------*/
    #define UF_rly1         uF1.Flg

    #define F_rly1          uF1.tFlg.Flg1
    #define F_rly2          uF1.tFlg.Flg2
    #define F_rly3          uF1.tFlg.Flg3
    #define F_rly4          uF1.tFlg.Flg4
    #define F_rly5          uF1.tFlg.Flg5
    #define F_rly6          uF1.tFlg.Flg6
    #define F_rly7          uF1.tFlg.Flg7

/*------UF_rly2------------------*/
    #define UF_rly2         uF2.Flg

    #define F_rly8          uF2.tFlg.Flg1
    #define F_rly9          uF2.tFlg.Flg2
    #define F_rly10         uF2.tFlg.Flg3
    #define F_rly11         uF2.tFlg.Flg4
    #define F_rly12         uF2.tFlg.Flg5
    #define F_rly13         uF2.tFlg.Flg6
    #define F_rly14         uF2.tFlg.Flg7


/*------Rly_IO-----------------*/
    #define P_rly1  RD0
    #define P_rly2  RD1
    #define P_rly3  RD2
    #define P_rly4  RD3
    #define P_rly5  RD4
    #define P_rly6  RD5
    #define P_rly7  RD6

    #define P_rly8  RD0
    #define P_rly9  RD1
    #define P_rly10 RD2
    #define P_rly11 RD3
    #define P_rly12 RD4
    #define P_rly13 RD5
    #define P_rly14 RD6
/*----- P_buzzer------------------*/
    #define nP_buzzer RD7
/*------Cup_motor-----------------*/


//  #define Alm_1 Buz_alm_times=1;Buz_alm_timer =0;Buz_alm_timer_duty =5;Buz_alm_timer_cycle =45
//  #define Alm_S1 Buz_alm_times=1;Buz_alm_timer =0;Buz_alm_timer_duty =5;Buz_alm_timer_cycle =10
//  #define Alm_2 Buz_alm_times=2;Buz_alm_timer =0;Buz_alm_timer_duty =5;Buz_alm_timer_cycle =10
//  #define Alm_S3 Buz_alm_times=3;Buz_alm_timer =0;Buz_alm_timer_duty =3;Buz_alm_timer_cycle =6
//  #define Alm_6 Buz_alm_times=6;Buz_alm_timer =0;Buz_alm_timer_duty =5;Buz_alm_timer_cycle =45




#define code_0          0x3F  //"0"
#define code_1          0x06  //"1"
#define code_2          0x5B  //"2"
#define code_3          0x4F  //"3"
#define code_4          0x66  //"4"
#define code_5          0x6D  //"5"
#define code_6          0x7D  //"6"
#define code_7          0x07  //"7"
#define code_8          0x7F  //"8"
#define code_9          0x6F  //"9"




#define code_A          0x77
#define code_b          0x7C
#define code_C          0x39
#define code_d          0x5E
#define code_E          0x79
#define code_F          0x71
#define code_H          0x76
#define code_L          0x38
#define code_l          0x18
#define code_N          0x37
#define code_n          0x54
#define code_U          0x3E
#define code_P          0x73
#define code_o          0x5C
#define code_r          0x50
#define code_S          0x6D
#define code_T          0x31
#define code_1h         0x01
#define code_2h         0x41
#define code_3h         0x49
#define code_4h         0x08
#define code_blank      0x00
#define code_mid_line   0x40
#define code_und_line   0x08


/******************************************************************************/
/* Function Declaration                                                       */
/******************************************************************************/
void    Freshe_Rly(void);
void    Init_Rly(void);
void    Freshe_Buzzer(void);
void    Cup_down(void);
void    Channel_Clear(void);
void    Setmodel_and_Dsp_Led_pro(void);
void    Voice(uchar a);
void    Inout_Setmodel_init(void);

uchar Supply_water(void);
extern union _uLedBuf R_LedBuf[];
void alm_set(uchar times,uchar timer,uchar duty,uchar cycle);

typedef union _uLedBuf
{
    uchar uDig ;                        //定义整形数据
    struct
        {        //位域定义
        uchar   a   : 1;
        uchar   b   : 1;
        uchar   c   : 1;
        uchar   d   : 1;
        uchar   e   : 1;
        uchar   f   : 1;
        uchar   g   : 1;
        uchar   df  : 1;
        }tDig;
 };


#endif
