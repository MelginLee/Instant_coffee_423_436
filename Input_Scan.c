/******************************************************************************/
/*
* File              : Input_Scan.c
* Description       : Description
* Author            ; Melgin
* Created on        : Date
*/
/******************************************************************************/


/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

    #include <xc.h>         /* XC8 General Include File */
//#elif defined(HI_TECH_C)
//    #include <htc.h>        /* HiTech General Include File */
//#endif

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "System.h"        /* System funct/params, like osc/peripheral config */
#include "User.h"          /* User funct/params, such as InitApp */
#include "M_Config.h"
#include "Input_Scan.h"
#include "Driver.h"
#include "ADC.h"
#include "Drinks_Supply.h"
#include "EEPROM_Pro.h"
#include "TaskService.h"
#include "Serial_Communications.h"





/******************************************************************************/
//KeyportScan
/******************************************************************************/

uchar KeyValue =0;

uchar Change_Cup_Channel_Timer;
/******************************************************************************/
void KeyportScan(void)
{

    if(!KeyValue && (Buz_alm_timer_cycle==0) && !F_set_key_2_3_17)
    {

        uchar i,j, temp,temp1,tempa,Buffer[3] = {0x06, 0x05, 0x03};

        for(j=0; j<3; j++)
        {
            KeyOutPort =Buffer[j];
            cnop;
            cnop;
            cnop;

            /*以下三个_nop_();作用为让 P1 口的状态稳定*/

            temp = 0x01;
            for(i=1; i<4; i++)
            {
                temp1 =KeyInPort;
                temp1 &=0b00110100;
                tempa =(temp1>>2)&1;
                tempa +=temp1>>3;
                if(!(tempa & temp))
                {
                    KeyValue=i+j*3;          //返回取得的按键值

                }
                temp <<= 1;
            }
        }
    }
}


uchar KeyValue_nmdsp;
uchar KeyValue_drk;
uchar KeyValue_set;

/******************************************************************************/
void Key_exe(void)
{
    F_key9_down =(KeyValue==9)?1:0;

    if (KeyValue >0 && KeyValue <9)
    {
        if (F_set_ing)
        {
            KeyValue_set =KeyValue;
        }
        else
        {
            if (( Offer_Step >2 ) || (Change_Cup_Channel_Timer !=0 ) || (F_sys_err ==1))
            {
                if (Buz_alm_times ==0)
                {
                    alm_set(3,0,3,6);
                }
            }
            else
            {
                //if ( ( R_Cup_Free==0 ) || ( R_Cup_Free==1 ) ) // 免费模式，Keyvalue 给 Task_Drinks_Supply
                if ( R_Cup_Free <2 )    // 免费模式，Keyvalue 给 Task_Drinks_Supply
                {
                    KeyValue_drk =KeyValue;
                }
                else        //收费模式
                {
                    uchar x,y;
                    x =drinks_rly_list[KeyValue-1][3];      //取料盒号
                    y =drinks_rly_list[KeyValue-1][0];      //取料盒号

                    if (Current_temperature <60 && y==11)
                    {
                        alm_set(3,0,3,6);
                    }
                    else
                    {
                        if (Coins >= drinks_set[x-1].drink_price)
                        {
                            KeyValue_drk =KeyValue;         //不差钱儿
                        }
                        else
                        {
                            KeyValue_nmdsp =KeyValue;       //差钱儿，去显示价格
                        }
                    }
                }
            }
        }
    }
    KeyValue=0;
}









/******************************************************************************/
void Cup_chk(void)
{
    if (R_Cup_Free & 0x01)
    {
		if (R_Cup_AD >=72)		// R_Cup_AD >= 0.7V ( 二极管压降 ) 取0.35V  1024*0.35/5=71.68
//        if (R_Cup_AD <=205)     // R_Cup_AD <= 1V  1024*1/5=204.8
        {
            Change_Cup_Channel_Timer =0;
            F_rly13 =0;
            F_Err_cup =0;              //有杯
        }
        else if(!F_Err_cup)
        {

            if (Change_Cup_Channel_Timer < 3500/T_Task_Chk_Status )
            {
                Change_Cup_Channel_Timer++;
                F_rly13 = ((Change_Cup_Channel_Timer < 200/T_Task_Chk_Status)||(( Change_Cup_Channel_Timer >1800/T_Task_Chk_Status )  && ( Change_Cup_Channel_Timer < 2000/T_Task_Chk_Status  )))?1:0;
                if(Change_Cup_Channel_Timer >= 3500/T_Task_Chk_Status)  F_Err_cup =1;

            }
        }
    }
    else
    {
        Change_Cup_Channel_Timer =0;
        F_Err_cup =0;
    }
}




/******************************************************************************/




/******************************************************************************/
    uint Coins_low_timer =0;

void Coins_chk(void)
{
    if(!P_coin)
    {
        if(Coins_low_timer >= 5000)
        {
            F_Err_coin = 1;
            Coins_low_timer =0;
        }
    }
    else
    {
        F_Err_coin = 0;

        if(( Coins_low_timer >= 10 ) && ( Coins_low_timer <= 100 ))
            {
                F_coin_temp_dsp =1;
                if (Coins ==0)
                {
                    Voice(1); //欢迎光临，请选择饮料种类
                }
                if (Coins <99)
                {
                    Coins++;
                }
                else Coins =99;
                Coins_count++;
                write(0x00,Coins);
            }
        Coins_low_timer =0;
    }
}



/******************************************************************************/
    uint R_wmq_lose_timer =0;


void Wmq_chk(void)
{
    if(R_wmq_lose_timer > 60000/T_Task_Input_Scan)
    {
        F_wmq_on_line =0;
    }
    else if (R_wmq_lose_timer == 59000/T_Task_Input_Scan)
    {
        alm_set(3,0,3,6);
        R_wmq_lose_timer++;
    }
    else
    {
        R_wmq_lose_timer++;
        F_wmq_on_line =1;
    }
}


