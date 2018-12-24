/******************************************************************************/
/*
* File              : User.h
* Description       : Description
* Author            : Melgin
* Created on        : Date
*/
/******************************************************************************/
#ifndef _User_h_
#define _User_h_

#include "M_Config.h"

#include <xc.h>         /* XC8 General Include File */

#define T_Task_Input_Scan 8
#define T_Task_Drinks_Supply 150
#define T_Task_Freshe_Hardware 21
#define T_Task_Chk_Status 49
#define T_Task_Disp_Status 153



const uchar Ver_info[8]=
{
    //软件版本  423 V1.5 [3432332056312e34]
    0x34,0x32,0x33,0x20,0x56,0x31,0x2e,0x35
};


/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

#define setbit(x,y) x|=(1<<y) //将X的第Y位置1

#define clrbit(x,y) x&=~(1<<y) //将X的第Y位清0

/*----- Sys_status_flag------------------*/
    #define Sys_status          uF3.Flg
    #define F_Sys_1MS           uF3.tFlg.Flg1
    #define F_ADC_OK            uF3.tFlg.Flg2
    #define F_sys_err           uF3.tFlg.Flg3
    #define F_set_key5_puls     uF3.tFlg.Flg4
    #define F_set_key_2_3_17    uF3.tFlg.Flg5
    #define F_Status_change     uF3.tFlg.Flg6
    #define F_key9_down         uF3.tFlg.Flg7


/*----- Err_Num------------------*/
    #define Err_Num             uF4.Flg           //=0  OK
    #define F_Err_water         uF4.tFlg.Flg1     //=1 故障 无水
    #define F_Err_cup           uF4.tFlg.Flg2     //=1 故障 无杯
    #define F_Err_coin          uF4.tFlg.Flg3     //=1 故障 投币器
    #define F_Err_cup_down      uF4.tFlg.Flg4     //=1 故障 下杯器行程开关
    #define F_Err_hot           uF4.tFlg.Flg5
//  #define F_Err_cup           uF4.tFlg.Flg6
//  #define F_Err_cup           uF4.tFlg.Flg7

/*----- UFlag1------------------*/
    #define UFlag1                  uF5.Flg
    #define F_coin_temp_dsp         uF5.tFlg.Flg1
    #define F_P_Set_init_bak        uF5.tFlg.Flg2
    #define F_blink_led             uF5.tFlg.Flg3
    #define F_sys_err_alm_exed      uF5.tFlg.Flg4
    #define F_wmq_on_line           uF5.tFlg.Flg5   //微码器在线
    #define F_hot_ing               uF5.tFlg.Flg6   //
    #define F_set_ing               uF5.tFlg.Flg7   //


/*----- UFlag2------------------*/
    #define UFlag2                          uF6.Flg
    #define F_RC_SMS_ok                     uF6.tFlg.Flg1
    #define F_sms_rc_packet                 uF6.tFlg.Flg2
    #define F_RC_SMS_ing                    uF6.tFlg.Flg3
    #define F_TX_SMS_ing                    uF6.tFlg.Flg4
    #define F_TX_SMS_start                  uF6.tFlg.Flg5
    #define F_wmq_selected_value_rechk      uF6.tFlg.Flg6
    #define F_wmq_selected_value_rechk_ok   uF6.tFlg.Flg7

/*----- UFlag3------------------*/
    #define UFlag3                  uF7.Flg
    #define F_cup_flag_away         uF7.tFlg.Flg1
    #define F_cup_flag_back         uF7.tFlg.Flg2
    #define F_cup_down_ing          uF7.tFlg.Flg3
    #define F_cup_down_ok           uF7.tFlg.Flg4
    #define F_First_into_Setmodel   uF7.tFlg.Flg5
    #define F_First_out_Setmodel    uF7.tFlg.Flg6




/*----- Rename IO------------------*/
//  #define XOR_P               RB4

    #define P_cup_down          RB5
    #define P_Voice_373_G       RB4
    #define P_cup_motor         RB3
    #define P_hot               RB2

    #define P_cup_ready         RA3

    #define nP_coin_en          RA6
    #define P_coin              RA7

    #define P_Set               RB7         /* 设置模式检测 */
//  uchar   P_Set   =1 ;        /* @debug */


/*------373_IO-----------------*/
    #define P_373_G2    RB0         /*74LS373 enable G =0时 Q=D, G =1时锁存*/
    #define P_373_G1    RB1         /*74LS373 enable G =0时 Q=D, G =1时锁存*/

    #define nP_373_E    RB6         /*74LS373 output control =0 输出使能*/
//  uchar   nP_373_E =0 ;            /* @debug */


/*----- Global Const def------------------*/
    #define C_water_supply_max  180 //  = 15000/T_Task_Drinks_Supply

/******************************************************************************/
/* Function Declaration                                                        */
/******************************************************************************/
    void InitApp(void);         /* I/O and Peripheral Initialization */
    void Init_IO(void);
    void reg_init(void);
    void delayUS(uchar);


/******************************************************************************/
/* Global Typedef Prototypes                                                  */
/******************************************************************************/
typedef union _uFLG
{
    uchar Flg ;     //定义整形数据
    struct FLAG
        {        //位域定义
        uchar   Flg1   : 1;
        uchar   Flg2   : 1;
        uchar   Flg3   : 1;
        uchar   Flg4   : 1;
        uchar   Flg5   : 1;
        uchar   Flg6   : 1;
        uchar   Flg7   : 1;
        uchar   Flg8   : 1;
        }tFlg;
}uFlg;


typedef struct _drinks_set_type
{
    uchar drink_price;          //price of drink
    uchar water_supply;         //percent of water supply max(100*150ms=15s)
    uchar powder_supply;        //percent of powder supply max(water supply max-0.5s)
    ulong  drink_offers;        //numbers of drink offer
}drinks_set_type;






/******************************************************************************/

extern uFlg uF1;    // UF_rly1
extern uFlg uF2;    // UF_rly2
extern uFlg uF3;    // Sys_sataus_flag
extern uFlg uF4;    // Err_Num
extern uFlg uF5;    // UFlag1
extern uFlg uF6;    // UFlag2
extern uFlg uF7;    // UFlag3



extern drinks_set_type  drinks_set[];
extern uchar Coins;
extern uchar Seted_temperature;


#endif
