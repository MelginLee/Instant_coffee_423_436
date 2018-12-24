/******************************************************************************/
/*
* File              : EEPROM_Pro.c
* Description       : 将0X55和0XAA写入EEPRIM的0X01地址，再读出来在LED上显示
* Author            : Melgin
* Created on        : 2018年3月5日
*/
/******************************************************************************/



/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/



/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/


#include <xc.h>
#include "User.h"
#include "M_Config.h"
 #include "Driver.h"




////////////EEPROM写函数/////////////////
void write(uchar addr,uchar date)
{
    do
    {
        GIE =0;
        while(WR==1);       //等待上一次写操作完成;
        EEADRH=0;       //写地址高位;
        EEADR=addr;     //写地址低位;
        EEDATA=date;        //写数据;
        EEPGD=0;            //选择EEPROM为访问对象
        WREN=1;         //开放写操作使能控制
        EECON2=0X55;        //送55H到EECON2(固定的)
        EECON2=0XAA;        //送AAH到EECON2(固定的)
        WR=1;               //启动写操作
        while(WR){};
        WREN=0;         //禁止写操作发生

        RD=1;           //读操作开始;
        cnop;
        cnop;
        while(RD){};
        GIE =1;
    }
    while (EEDATA != date);
}

///////////EEPROM读函数/////////////////
uchar read(uchar addr)
{
    GIE =0;
    EEADRH=0;       //读地址;
    EEADR=addr;     //读地址;
    EEPGD=0;        //选择EEPROM为访问对象
    RD=1;           //读操作开始;
    cnop;
    cnop;
    while(RD){};
    GIE =1;
    return(EEDATA); //返回数据;
}







///////////数据写入函数//////////////////////
void data_write(void)
{
        write(0x00,Coins);

        write(0x01,Seted_temperature);

        write(0x02,R_Cup_Free);


        for (uchar a=0,b=0x10;a<=3;a++)
        {

            ulong a_temp32=0;

            write(b+0x00,drinks_set[a].drink_price);

            write(b+0x01,drinks_set[a].water_supply);

            write(b+0x02,drinks_set[a].powder_supply);


            a_temp32 =drinks_set[a].drink_offers;

            write(b+0x03,(uchar)(a_temp32 >>24));    //drinks_set[a].drink_offers 高位

            write(b+0x04,(uchar)((a_temp32 & 0xffffff)>>16));  //drinks_set[a].drink_offers 低位

            write(b+0x05,(uchar)((a_temp32 & 0xffff)>>8));    //drinks_set[a].drink_offers 高位

            write(b+0x06,(uchar)(a_temp32 & 0xff));  //drinks_set[a].drink_offers 低位

            b=b+0x10;
        }

}




void data_read(void)

{

    Coins=read(0x00);


    Seted_temperature=read(0x01);


    R_Cup_Free=read(0x02);


    for (uchar a=0,b=0x10;a<=3;a++)
    {
        ulong a_temp32=0;

        drinks_set[a].drink_price=read(b+0x00);

        drinks_set[a].water_supply=read(b+0x01);

        drinks_set[a].powder_supply=read(b+0x02);


        a_temp32 =read(b+0x03);
        drinks_set[a].drink_offers = a_temp32<<24;              //drinks_set[a].drink_offers

        a_temp32 =read(b+0x04);
        drinks_set[a].drink_offers |= (a_temp32<<16);           //drinks_set[a].drink_offers

        a_temp32 =read(b+0x05);
        drinks_set[a].drink_offers |= (a_temp32<<8);            //drinks_set[a].drink_offers

        a_temp32 =read(b+0x06);
        drinks_set[a].drink_offers |= (a_temp32);               //drinks_set[a].drink_offers

        b=b+0x10;
    }
}



void RFS_data_write(void)
{

    write(0x00,0);                  //RFS Coins

    write(0x01,85);                 //RFS Seted_temperature

    write(0x02,3);                  //RFS R_Cup_Free


    for (uchar a=0,b=0x10;a<=3;a++)
    {
        write(b+0x00,2);            // RFS  drinks_set[a].drink_price

        write(b+0x01,50);           // RFS  drinks_set[a].water_supply);

        write(b+0x02,30);       // RFS  drinks_set[a].powder_supply);

        if (read(0xFF)!=0x88)
        {
            write(b+0x03,0);        // RFS  drinks_set[a].drink_offers 高位

            write(b+0x04,0);        // RFS  drinks_set[a].drink_offers 低位

            write(b+0x05,0);        // RFS  drinks_set[a].drink_offers 低位

            write(b+0x06,0);        // RFS  drinks_set[a].drink_offers 低位

        }
        b=b+0x10;
    }


    write(0xFF,0x88);

    for (uchar a=0,b=0;a<8;a++)
    {
        b =Ver_info[a];
        write(0xF0+a,b);    //RFS_Ver_info
    }

}


void Frist_data_write_chk(void)
{
    if (read(0xFF)!=0x88)
    {
        RFS_data_write();
    }
}

void Clear_eeprom(void)
{
    for (uint a=0;a<=0xff ;a++ )
    {
        write(a,0xff);
    }
}

