/******************************************************************************/
/*
* File              : Filename.c
* Description       : Description
* Author            ; Melgin
* Created on        : Date
*/
/******************************************************************************/



/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/



/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/



/******************************************************************************/
/*
* File              : ADC.c
* Description       : Description
* Author            ; Melgin
* Created on        : Date
*/
/******************************************************************************/



/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <xc.h>         /* XC8 General Include File */
#include "User.h"
#include "M_Config.h"
#include "ADC.h"



/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
    uchar   ADC_Conversion_Times =0;
    uint    ADC_buf =0;
    uint    R_Water_AD =0xffff;
    uint    R_NTC_AD =0;
    uint    R_Cup_AD =0;
    uchar   ADC_channel =0;
    uchar   Current_temperature =25;


void Start_ADC(void)
{
    ADCON1 = 0b10000000;    // AN or IO 配置选择
    ADCON0 = 0b01000001;
    ADC_Conversion_Times =0;
    ADIF =0;
    ADIE =1;
    GO_nDONE =1;

}





void ADC_converter(void)
{
   if(F_ADC_OK)
    {

        uint    c,b;
        c =ADRESH;
        c =c<<8;
        b =ADRESL;
        ADC_buf = ADC_buf + c + b;

        if (ADC_Conversion_Times >= 15)
        {
            ADC_buf = ADC_buf>>4;
            switch (ADC_channel)
            {
                case 0:
                    R_Water_AD = ADC_buf;
                    break;
                case 1:
                    R_NTC_AD = ADC_buf;
                    break;
                case 2:
                    R_Cup_AD =ADC_buf;
                    break;
                default:
                    break;
            }

            ADC_channel = (ADC_channel >=2?0:ADC_channel+1);
            switch (ADC_channel)
            {
                case 0:
                    ADCON0 = 0b01000001;                //for AN0
                    break;
                case 1:
                    ADCON0 = 0b01000101;                //for AN1
                    break;
                case 2:
                    ADCON0 = 0b01001101;                //for AN3
                    break;
            }

            ADC_buf =0;
            ADC_Conversion_Times =0;
        }
        else
        {
            ADC_Conversion_Times++;
        }

        F_ADC_OK =0;
        ADIF =0;
        ADIE =1;
        GO_nDONE =1;
    }


}









 //*************************************
// 函数名称：FineTab  二分查找算法 ->查温度表
// 函数功能：查找数据在表中对应的位置 表中数据从大到小
// 入口参数：表地址、表长度、要查找的数据
// 出口参数：无
// 返 回 值：数据在表中的位置
//*************************************
uchar FineTab(uint *a,uchar TabLong,uint data)//表中数据从大到小
{
    uchar st,ed,m ;
    uchar i ;

    st = 0 ;
    ed = TabLong-1 ;
    i = 0  ;

    if(data >= a[st]) return st ;
    else if(data <= a[ed]) return ed ;

    while(st < ed)
    {
        m = (st+ed)/2 ;

        if(data == a[m] ) break ;
        if((data < a[m]) && (data > a[m+1])) break ;


        if(data > a[m])  ed = m ;
        else st = m ;

        if(i++ > TabLong) break ;
    }

    if(st > ed ) return 0 ;

    return m+5 ;
}




const uint TabNtc_10K[TabNtc_10K_Size]=
{
    702,        //5 度
    693,        //6
    684,        //7
    675,        //8
    666,        //9
    656,        //10
    647,        //11
    637,        //12
    628,        //13
    618,        //14
    609,        //15
    599,        //16
    589,        //17
    580,        //18
    570,        //19
    560,        //20
    550,        //21
    541,        //22
    531,        //23
    522,        //24
    512,        //25
    502,        //26
    493,        //27
    484,        //28
    474,        //29
    465,        //30
    456,        //31
    447,        //32
    438,        //33
    429,        //34
    420,        //35
    411,        //36
    402,        //37
    394,        //38
    386,        //39
    377,        //40
    369,        //41
    361,        //42
    353,        //43
    345,        //44
    338,        //45
    330,        //46
    323,        //47
    315,        //48
    308,        //49
    301,        //50
    294,        //51
    287,        //52
    281,        //53
    274,        //54
    268,        //55
    262,        //56
    256,        //57
    250,        //58
    244,        //59
    238,        //60
    233,        //61
    227,        //62
    222,        //63
    216,        //64
    211,        //65
    206,        //66
    201,        //67
    197,        //68
    192,        //69
    187,        //70
    183,        //71
    178,        //72
    174,        //73
    170,        //74
    166,        //75
    162,        //76
    158,        //77
    154,        //78
    150,        //79
    146,        //80
    143,        //81
    139,        //82
    136,        //83
    133,        //84
    129,        //85
    126,        //86
    123,        //87
    120,        //88
    117,        //89
    115,        //90
    112,        //91
    109,        //92
    107,        //93
    104,        //94
    102,        //95
    99,         //96
    97,         //97
    95,         //98
    92          //99
};


