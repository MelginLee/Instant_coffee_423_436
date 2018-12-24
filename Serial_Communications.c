/******************************************************************************/
/*
* File              : Serial_Communications.c
* Description       : Description
* Author            ; Melgin
* Created on        : Date
*/
/******************************************************************************/



/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <xc.h>
//#include<pic.h>              //包含单片机内部资源预定义
#include "M_Config.h"
#include "User.h"
#include "TaskService.h"
#include "Input_Scan.h"
#include "Driver.h"
#include "ADC.h"
#include "EEPROM_Pro.h"



/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/



/********************************************************************************************
* RS232串行通迅实验
* CPU型号： PIC16F877A
* 晶振：    8MHz （如果更换其他频率的晶振需要自已相应修改程序）

* 说明:     串口调试软件波特率设为9600
            通过串口调试助手向单片机发送一个字节的数据
            单片机会返回一个字节的数据给计算机
* 作    者：MCU.2000
* 日    期：2010年8月8号
该实验程序在PIC最小系统V2.0上调试通过
*********************************************************************************************/


    uchar R_sms_buf[25];
    uchar T_sms_buf[25];
    uchar rc_byte_cnt=0;
    uchar tx_byte_long=0;
    uchar tx_byte_cnt=0;
    uint  sms_chk_sum=0;
    uchar wmq_selected_value =0;
    uchar wmq_selected_temp =0;
    uchar Coins_count =0;                   //设备自带投币器 投币数量 , 应答查询 数据 第四字节： 支付金额： 0x00 （设备上传服务器的投币数， 每次应答后清 0）
    uint R_rc_blank_cnt =0;
//  uchar wmq_status=0;

    uchar rtemp =0;
void SerialInit(void)
    {
//      BRGH =1;
        SPBRG=0x33;  //8M晶振，波特率9600，则SPBRG初值为51

//      TXSTA=0x24; //开启发射使能位、高波特率，TRMT初始值可0可1
//        RCSTA=0x90; //开启串口、连续接收、

        TXSTA=0x04; //开启发射使能位、高波特率，TRMT初始值可0可1
        RCSTA=0x80; //开启串口、连续接收、


//      SPEN =1;
//      SYNC =0;

//      TX9 =0;
//      TXIF =0;
        TXIE =1;    //允许接收
//      TXEN =0;    //禁止发送

//      RX9  =0;
//      RCIF =0;
        RCIE =1;    //开启串口接收完成中断
        CREN =1;

        GIE=1;      //开全局中断
        PEIE=1;     //开第一外设中断
    }



void RC_SMS_chk_sum_pro(void)
{
//  if (F_sms_rc_packet)
//  {
        sms_chk_sum =0;
        if (( R_sms_buf[0] ==0xaa ) && ( R_sms_buf[rc_byte_cnt] ==0xab ) && ( rc_byte_cnt ==(R_sms_buf[2]+5) ))
        {

            for (uchar i=1;i<=rc_byte_cnt-3;i++ )
            {
                sms_chk_sum +=R_sms_buf[i];
            }

            uint a=0;
            a =R_sms_buf[rc_byte_cnt-1];
            a <<=8;
            a +=R_sms_buf[rc_byte_cnt-2];
            if (sms_chk_sum == a)
            {
                F_RC_SMS_ok =1;
                F_sms_rc_packet =0;

            }
            else
            {                       //校验失败
                F_sms_rc_packet =0;
                F_RC_SMS_ok =0;
                F_RC_SMS_ing =0;
                CREN=1;             //重新 使能接收
//              RCIE=1;             //重新 使能接收
            }
        }
        else
        {       //帧头帧尾错误
            F_sms_rc_packet =0;
            F_RC_SMS_ok =0;
            F_RC_SMS_ing =0;
            CREN=1;                 //重新使能接收
//          RCIE=1;                 //重新使能接收
        }
//  }

}



void heat_beat_packet_back(void)
{
    T_sms_buf[1] =0x80;                 //应答查询命令
    T_sms_buf[2] =0x09;                 //应答查询命令字节长度
    T_sms_buf[3] =0x01;                 //应答查询 数据 第一字节： 设备投币类型， 0： 脉冲模式 1： 串口模式（默认不接串口为脉冲模式）
    T_sms_buf[4] =0x05;                 //应答查询 数据 第二字节： 设备类型， 00 娃娃机 01 充电桩（不可用） 02 兑币机 03 按摩椅 04 洗衣机 05 咖啡机

    if (F_set_ing)
    {
        T_sms_buf[5] =0x01;             //设置模式
    }
    else if (F_Err_water)
    {
        T_sms_buf[5] =0x02;             //缺水
    }
    else if (F_Err_cup)
    {
        T_sms_buf[5] =0x03;             //缺杯
    }
    else if (F_Err_cup_down)
    {
        T_sms_buf[5] =0x04;             //落杯器故障
    }
    else if (F_Err_coin)
    {
        T_sms_buf[5] =0x05;             //投币器故障
    }
    else if (F_Err_hot)
    {
        T_sms_buf[5] =0x06;             //加热超时故障
    }
    else if (Current_temperature < 60)
    {
        T_sms_buf[5] =0x07;             //温度小于60度
    }
    else if (Offer_Step >1)
    {
        T_sms_buf[5] =0x08;             //忙
    }
    else if (!F_wmq_on_line)
    {
        T_sms_buf[5] =0x09;             //微码器离线
    }



//  else if ( ( R_Cup_Free==0 ) || ( R_Cup_Free==1 ) )  // 免费模式，不接收付款
//  else if ( R_Cup_Free < 2 )  // 免费模式，不接收付款
//  {
//      T_sms_buf[5] =0x06;             //
//  }
    else
    {
        T_sms_buf[5] =0x00;             //应答查询 数据 第三字节： 消费允许状态字： 允许 0x00,， （1~15） ： 当作设备故障上传。 （如果该位非 0， 扫码支付页面出现不可支付字样）

    }

    if (F_set_ing)
    {
        T_sms_buf[8] =0x01;                 //应答查询 数据 第六字节： 设备工作状态: 正常 0x00 异常 0x01 其他状态待定（待定）
        T_sms_buf[9] =0x01;                 //应答查询 数据 第七字节： 异常的路数：0x00
    }
    else
    {
        T_sms_buf[8] =0x00;                 //应答查询 数据 第六字节： 设备工作状态: 正常 0x00 异常 0x01 其他状态待定（待定）
        T_sms_buf[9] =0x00;                 //应答查询 数据 第七字节： 异常的路数：0x00
    }
    T_sms_buf[6] =Coins_count;          //应答查询 数据 第四字节： 支付金额： 0x00 （设备上传服务器的投币数， 每次应答后清 0， 设备自带投币器）
    Coins_count =0;

    T_sms_buf[7] =0x01;                 //应答查询 数据 第五字节： 支付方式： 投币 0x01. 刷卡 0x02（现在只支持投币）


    T_sms_buf[10] =0x00;                //应答查询 数据 第八字节： 退奖数量 （设备上传服务器的退奖数， 每次应答后清 0）
    T_sms_buf[11] =0x00;                //应答查询 数据 第九字节： 现金数量 （设备上传服务器的投入的纸钞的数量， 每次应答后清 0， 设备自带纸钞机）

    tx_byte_long =15;                       //发送字节数量
    F_TX_SMS_start =1;
}








void RC_SMS_data_exed_pro(void)
{
//  if (F_RC_SMS_ok ==1)
//  {
        if (R_sms_buf[1]==0x00)
        {                                       //WMQ 查询命令
//              wmq_status =R_sms_buf[3];
                heat_beat_packet_back();
        }
        else if (R_sms_buf[1]==0x05)                //扫描支付命令
        {
                T_sms_buf[1] =0x85;                 //应答扫描支付命令
                T_sms_buf[2] =0x02;                 //应答扫描支付命令 数据长度
                uchar sms_3,sms_4;
                sms_3= R_sms_buf[3];
                sms_4= R_sms_buf[4];

                if ((sms_4==0) && (sms_3>0) && (sms_3<9) && !F_sys_err && (Offer_Step <2))
                {
                    wmq_selected_temp =sms_3;
                    T_sms_buf[3] =sms_3;
                    T_sms_buf[4] =sms_4;
                }
                else
                {                                   //收到 !(0 <币数 <9)
                    wmq_selected_temp =0;
                    T_sms_buf[3] =0;
                    T_sms_buf[4] =0;
                }
                tx_byte_long =8;                    //发送字节数量
                F_TX_SMS_start =1;
        }
        else if (( R_sms_buf[1]==0x85 ) && ( R_sms_buf[2] ==0x02 ) && ( R_sms_buf[3]==0 ) && ( R_sms_buf[4]==0 ))           //扫描支付 出货指令
        {
                wmq_selected_value =wmq_selected_temp;
        }

        else if (( R_sms_buf[1]==0x04 ) && ( R_sms_buf[2] ==0x02 ))
        {
            if (( R_sms_buf[3]==0 ) && ( R_sms_buf[4]==0 ))
            {
                Clear_eeprom();
                RFS_data_write();
                data_read();
                alm_set(1,0,5,10);
            }
            if (( R_sms_buf[3]==0 ) && ( R_sms_buf[4]==1 ))
            {
                RFS_data_write();
                data_read();
                alm_set(1,0,5,10);
            }

            T_sms_buf[1]=0x84;
            T_sms_buf[2]=0x02;
            T_sms_buf[3]=R_sms_buf[3];
            T_sms_buf[4]=R_sms_buf[4];

            tx_byte_long =8;                    //发送字节数量
            F_TX_SMS_start =1;
        }
//  }
}


void TX_SMS_back_pro(void)
{
    if (F_TX_SMS_start ==1)
    {
//      RCIE=0;                 //禁止接收
        CREN=0;                 //禁止接收
        F_TX_SMS_start =0;
        sms_chk_sum=0;

        for (uchar i=1;i<(tx_byte_long -3 );i++)
        {
            sms_chk_sum +=T_sms_buf[i];
        }

        T_sms_buf[0] =0xaa;                 //帧头
        T_sms_buf[tx_byte_long -3] =sms_chk_sum & 0x00FF;   //应答查询 校验低位
        T_sms_buf[tx_byte_long -2] =sms_chk_sum >>8;        //应答查询 校验低位
        T_sms_buf[tx_byte_long -1] =0xab;                   //帧尾
        tx_byte_cnt =0;                     //发送字节计数清零

        F_TX_SMS_ing =1;
        TXEN =1;                //使能发送
//      TXIE=1;                  //使能发送

    }
}





void Serial_Pro(void)
{
    if (F_sms_rc_packet ==1)
    {
        RC_SMS_chk_sum_pro();
        F_sms_rc_packet =0;
    }

    if (F_Status_change)
    {
        F_Status_change =0;
        heat_beat_packet_back();
        TX_SMS_back_pro();
        F_RC_SMS_ok =0;
    }
    else
    {
        if (F_RC_SMS_ok ==1)
        {
            RC_SMS_data_exed_pro();
            TX_SMS_back_pro();
            F_RC_SMS_ok =0;
        }
    }
}

