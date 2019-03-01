/******************************************************************************/
/*
* File              : Drinks_Supply.c
* Description       : Description
* Author            ; Melgin
* Created on        : 2018年1月28日 13:37
*/
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
    #include <xc.h>             /* XC8 General Include File */
    #include "Drinks_Supply.h"
    #include "Driver.h"
    #include "User.h"
    #include "ADC.h"
    #include "EEPROM_Pro.h"
    #include "Input_Scan.h"
    #include "TaskService.h"
    #include "Serial_Communications.h"



/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
    uchar   Water_supplyed_time =0;


/******************************************************************************/



/******************************************************************************/
void offer_payment_chk(void)
{

    if (wmq_selected_value >0)                          // 收费模式 且 使用微码器 付款成功
    {
        KeyValue_drk = wmq_selected_value;

    }

    if (KeyValue_drk >0)
    {
        uchar   a;
        a=drinks_rly_list[KeyValue_drk-1][0];
        if( a != 11) Offer_Step++;
        else if(Current_temperature >=70) Offer_Step++;
        else
        {
            Offer_Step =0;
            KeyValue_drk =0;
            wmq_selected_value =0;
            alm_set(3,0,3,6);
        }
    }
}



/******************************************************************************/
void cup_supply(void)
{
    Voice(2); //请您稍等

    alm_set(1,0,5,45);
    F_Status_change =1;

    Offer_Step++;
//  F_rly14 =0;         //提供饮料前，关闭抽水

//  if (( R_Cup_Free==1 ) || ( R_Cup_Free==3 ))
    if (R_Cup_Free & 0x01)
    {
        F_cup_down_ing =1;
        F_cup_down_ok =0;
    }
}


/******************************************************************************/
void cup_supply_over_chk(void)
{
//  if (( R_Cup_Free==0 ) || ( R_Cup_Free==2 ))
    if (!(R_Cup_Free & 0x01))
    {
            Offer_Step++;
            alm_set(1,0,5,45);
    }
    else
    {
        if (!F_cup_down_ing)
        {

            if (F_cup_down_ok)
            {
                F_cup_down_ok =0;
                F_Err_cup_down =0;
                Offer_Step++;
                alm_set(1,0,5,45);

            }
            else
            {
                F_Err_cup_down =1;

                Offer_Step =0;
                KeyValue_drk =0;
                KeyValue_set =0;
                KeyValue_nmdsp =0;
                wmq_selected_value =0;
            }

        }
    }

}

/******************************************************************************/
void water_supply(void)
{
        uchar   a;
        a=drinks_rly_list[KeyValue_drk-1][0];
        UF_rly2 |=1<<(a-8);
        a=drinks_rly_list[KeyValue_drk-1][1];
        UF_rly2 |=1<<(a-8);
        Offer_Step++;
        Water_supplyed_time=0;
}
/******************************************************************************/
void powder_supply(void)
{
        Water_supplyed_time++;


    if ( Water_supplyed_time >= 4)
    {
        uchar   a;
        a =drinks_rly_list[KeyValue_drk-1][2];
        UF_rly1 |=1<<(a-1);
        a =drinks_rly_list[KeyValue_drk-1][3];
        UF_rly1 |=1<<(a-1);
        Offer_Step++;
    }
}
/******************************************************************************/
void supply_over_chk(void)
{
    Water_supplyed_time++;

    float f_temp1,f_temp2;

    uchar   x,a,b;
    x =drinks_rly_list[KeyValue_drk-1][3];      //取料盒号
    a = drinks_set[x-1].water_supply;
    b = drinks_set[x-1].powder_supply;

    f_temp1 = a * C_water_supply_max / 100 ;
    f_temp2 = b * f_temp1 /100 ;

    if ( Water_supplyed_time > f_temp1)
    {
        UF_rly1 =0;
        UF_rly2 &=0b11100000;
        Offer_Step++;
    }
    else if ( Water_supplyed_time > f_temp2 )
    {
        UF_rly1 &=~(1<<(x-1));
    }
}

/******************************************************************************/
void supply_over(void)
{
    alm_set(2,0,5,10);
    Offer_Step++;

    uchar   x;
    x =drinks_rly_list[KeyValue_drk-1][3];      //取料盒号
    ulong   z;
    z = drinks_set[x-1].drink_offers;

    if (z < 99999)
    {
        drinks_set[x-1].drink_offers ++ ;
    }
    else
    {
        drinks_set[x-1].drink_offers =1;
    }

//  if ( ( R_Cup_Free ==2 ) || ( R_Cup_Free ==3 ))
    if ( R_Cup_Free >1 )
    {
        if (wmq_selected_value ==0)
        {
            Coins -=drinks_set[x-1].drink_price;
        }
    }
    data_write();

    // 如有需要，发送信息：请您取饮料
    Voice(3);               //请品尝饮料
}

/******************************************************************************/
void welcome_back(void)
{
    if(R_voice_dly_cnt ==0)
    {
        KeyValue_drk =0;
        wmq_selected_value =0;
        Offer_Step =0;
        F_Status_change =1;

//      if ( ( R_Cup_Free ==2 ) || ( R_Cup_Free ==3 ) )
        if ( R_Cup_Free >1 )
        {
            if(Coins ==0)
            {
                Voice(4);           //欢迎下次光临
                //投币已消费完，发送信息 谢谢光临等
            }
            else
            {
                //投币未消费完，发送信息 请继续选择饮料
            }
        }
    }
}
/******************************************************************************/





