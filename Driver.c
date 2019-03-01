/******************************************************************************/
/*
 * File              : Driver.c
 * Description       :
 * Author            ; Melgin
 * Created on        : 2018年1月28日 21:40
 */
/******************************************************************************/


/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <xc.h>         /* XC8 General Include File */
#include "M_Config.h"
#include "User.h"          /* User funct/params, such as InitApp */
#include "Driver.h"
#include "Input_Scan.h"
#include "Drinks_Supply.h"
#include "ADC.h"
#include "EEPROM_Pro.h"
#include "Configuration_bits.h"
#include "Serial_Communications.h"
#include "TaskService.h"



uchar Supply_water(void)
{
    uchar b;
    b =((F_key9_down ==1) && (F_set_ing==0) && (Offer_Step <2) && (F_Err_water ==0) && (Current_temperature >=70))?1:0;
    return b;
}


    uchar R_voice_dly_cnt =0;
    void Voice (uchar a)
    {
        //  RD6:0  **单选
        //  RD6 ==0 Voice 机器故障           （无语音数据，暂定）
        //  RD5 ==0 Voice 余额不足，请投币    （无语音数据，暂定）
        //  RD4 ==0 Voice 欢迎下次光临
        //  RD3 ==0 Voice 请品尝饮料
        //  RD2 ==0 Voice 请您稍等
        //  RD1 ==0 Voice 欢迎光临，请选择饮料各类
        //  RD0 ==0 Voice 欢迎光临
        PORTD |=0b01111111;
        cnop;
        cnop;
        cnop;
        clrbit(PORTD,a);
        for(uchar c=5;c>0;){c--;}

        P_Voice_373_G = 1;
        for(uchar c=10;c>0;){c--;}

        P_Voice_373_G = 0;
        R_voice_dly_cnt =250;
    }





    void    Freshe_Rly(void)
    {
/*------rly1 - rly7------------------*/
//      P_rly1  =(F_rly1?1:0);
//      P_rly2  =(F_rly2?1:0);
//      P_rly3  =(F_rly3?1:0);
//      P_rly4  =(F_rly4?1:0);
//      P_rly5  =(F_rly5?1:0);
//      P_rly6  =(F_rly6?1:0);
//      P_rly7  =(F_rly7?1:0);
        P_rly1  =F_rly1;
        P_rly2  =F_rly2;
        P_rly3  =F_rly3;
        P_rly4  =F_rly4;
        P_rly5  =F_rly5;
        P_rly6  =F_rly6;
        P_rly7  =F_rly7;
        cnop;
        cnop;
        cnop;
        cnop;
//      cnop;
//      cnop;
        P_373_G1=0;
        cnop;
        cnop;
        cnop;
        cnop;
        P_373_G1=1;

/*------rly1 - rly7------------------*/
//      P_rly8  =(F_rly8?1:0);
//      P_rly9  =(F_rly9?1:0);
//      P_rly10 =(F_rly10?1:0);
//      P_rly11 =(F_rly11?1:0);
//      P_rly12 =(F_rly12?1:0);
//      P_rly13 =(F_rly13?1:0);
//      P_rly14 =(F_rly14?1:0);
        P_rly8  =F_rly8;
        P_rly9  =F_rly9;
        P_rly10 =F_rly10 || Supply_water();
        P_rly11 =F_rly11 || Supply_water();
        P_rly12 =F_rly12;
        P_rly13 =F_rly13;
        P_rly14 =F_rly14;
        cnop;
        cnop;
        cnop;
        cnop;
        P_373_G2=0;
        cnop;
        cnop;
        cnop;
        cnop;
        P_373_G2=1;
    }








/******************************************************************************/
    void    Init_Rly(void)
    {
        nP_373_E =1;
        PORTD   =0b10000000;
        P_373_G1 =1;
        P_373_G2 =1;
//  cnop;
//  cnop;
        cnop;
        cnop;
        P_373_G1 =0;
        P_373_G2 =0;
//  cnop;
//  cnop;
        cnop;
        cnop;
        P_373_G1 =1;
        P_373_G2 =1;
    }



/******************************************************************************/
    uchar   Buz_alm_times =0;
    uchar   Buz_alm_timer =0;
    uchar   Buz_alm_timer_duty =0;
    uchar   Buz_alm_timer_cycle =0;


    void alm_set(uchar times,uchar timer,uchar duty,uchar cycle)
    {
        Buz_alm_times = times;
        Buz_alm_timer = timer;
        Buz_alm_timer_duty = duty;
        Buz_alm_timer_cycle = cycle;
    }



    void    Freshe_Buzzer(void)
    {

        if (Buz_alm_timer_cycle)
        {
            if (Buz_alm_timer > Buz_alm_timer_cycle)
            {
                Buz_alm_timer =0;
                if(Buz_alm_times !=0) Buz_alm_times--;
                if(Buz_alm_times==0)
                {
                    Buz_alm_timer_duty=0;
                    Buz_alm_timer_cycle=0;
                }
            }

            nP_buzzer = (Buz_alm_timer < Buz_alm_timer_duty ?0:1);
            Buz_alm_timer++;
        }
        else nP_buzzer=1;
    }


/******************************************************************************/

uint  Channel_Clear_timer =0;

void Channel_Clear(void)
{
    if(F_channel_clear_ing)
    {
        Channel_Clear_timer++;
        switch (Channel_Clear_timer/(5000/T_Task_Freshe_Hardware))
        {
        case 0:
            F_rly11 =1;
            F_rly8 =1;
            F_rly5 =1;
            break;
        case 1:
            F_rly8 =0;
            F_rly5 =0;
            F_rly11 =1;
            F_rly9 =1;
            F_rly6 =1;
            break;
        case 2:
            F_rly9 =0;
            F_rly6 =0;
            F_rly11 =1;
            F_rly10 =1;
            F_rly7 =1;
            break;
        case 3:
            F_rly11 =0;
            F_rly10 =0;
            F_rly7 =0;
            Channel_Clear_timer =0;
            F_channel_clear_ing =0;
            F_sms_dsp =0;
            F_Status_change =1;
            sms_host_maintain =0;
            break;
        default:break;
        }
    }
}

/******************************************************************************/

    uint  Cup_motor_on_timer =0;
    uchar Cup_flag_away_cnt =0;
    uchar Cup_flag_back_cnt =0;
    uchar Cup_motor_off_dly =0;


    void Cup_down(void)
    {
        if (F_cup_down_ing)
        {
            P_cup_motor =1;
            Cup_motor_on_timer++;

            if (P_cup_down==0)
            {
                if (Cup_flag_away_cnt > 500/T_Task_Freshe_Hardware)
                {
                    F_cup_flag_away =1;
                }
                else Cup_flag_away_cnt++;
            }
            else
            {
                Cup_flag_away_cnt =0;
            }


            if (F_cup_flag_away)
            {
                if (P_cup_down==1)
                {
                    Cup_flag_back_cnt++;

                    if (Cup_flag_back_cnt >= 100/T_Task_Freshe_Hardware)     // 行程开关归位的时间长短必须合适
                    {
                        F_cup_flag_back =1;
                        Cup_motor_off_dly =0;
                    }
                }
                else Cup_flag_back_cnt =0;
            }

            if (F_cup_flag_back)
            {
                if (Cup_motor_off_dly >=300/T_Task_Freshe_Hardware)
                {
                    F_cup_down_ing =0;
                    F_cup_down_ok  =1;
                    F_sms_dsp =0;
                    F_Status_change =1;
                    sms_host_maintain =0;
                }
                else Cup_motor_off_dly++;
            }

            if (Cup_motor_on_timer >= 8000/T_Task_Freshe_Hardware)
            {
                F_cup_down_ing =0;
                F_cup_down_ok  =0;
                F_sms_dsp =0;
                F_Status_change =1;
                sms_host_maintain =0;
            }
        }
        else
        {
            P_cup_motor =0;
            Cup_motor_on_timer =0;
            Cup_motor_off_dly =0;
            F_cup_flag_away=0;
            F_cup_flag_back =0;
        }
    }
/******************************************************************************/

    const uchar leddata[]=
        {
            0x3F,  //"0"
            0x06,  //"1"
            0x5B,  //"2"
            0x4F,  //"3"
            0x66,  //"4"
            0x6D,  //"5"
            0x7D,  //"6"
            0x07,  //"7"
            0x7F,  //"8"
            0x6F   //"9"
        };





    union _uLedBuf R_LedBuf[]=
        {
            0,0,0,0
        };






    uchar R_Set_Step =0;
    uchar R_Set_or_Dsp_Timer =0;
    uint R_Set_or_Dsp_Timer2 =0;
    uchar R_Cup_Free =3;


    void get_set_key_value(void)
    {
//  uchar a_temp;

        switch (KeyValue_set)
        {
        case 1:
        {
            if (R_Set_Step ==40) {R_Set_Step =10;break;}

            if (R_Set_Step >16) break;

            if ((R_Set_Step >= 10) && (R_Set_Step <15))
            {
                R_Set_Step++;
                R_Set_or_Dsp_Timer =0;
            }

            else R_Set_Step =10;

        }break;

        case 2:
        {
            if (R_Set_Step ==40) {R_Set_Step =20;break;}
            if (R_Set_Step >15) break;
            else {
                R_Set_Step =20;
                F_channel_clear_ing =1;
                R_Set_or_Dsp_Timer =0;
            }
        }break;

        case 3:
        {
            if (( R_Set_Step <=15 ) || ( R_Set_Step ==40 ))
            {
                R_Set_Step =30;
                F_cup_down_ing =1;
                F_cup_down_ok =0;
                F_Err_cup_down =0;
                R_Set_or_Dsp_Timer =0;
            }
        }break;
        case 4:
        {
            if (R_Set_Step !=0) break;

            R_Set_or_Dsp_Timer2++;

            if (R_Set_or_Dsp_Timer2 >= 5000/T_Task_Disp_Status)
            {
                R_Set_Step =40;
            }

        }break;
        case 5:
        {
            if (R_Set_Step==17|R_Set_Step==20|R_Set_Step==30) break;

            F_set_key5_puls =0;     //显示 Au.1 ,An.1 等

            if (( R_Set_Step <50 ) || ( R_Set_Step >=63 ))
            {
                R_Set_Step =50;
                break;
            }
            else R_Set_Step++;
            R_Set_or_Dsp_Timer =0;
        }break;

        case 6:
        {
            if (R_Set_Step >=50)
            {

                switch (R_Set_Step)
                {
                case 50:
                    if(Seted_temperature <93) Seted_temperature++;
                    else Seted_temperature =93;
                    break;
                case 51:
                case 52:
                case 53:
                case 54:
                    if(drinks_set[R_Set_Step-51].drink_price <99) drinks_set[R_Set_Step-51].drink_price++;
                    else drinks_set[R_Set_Step-51].drink_price =99;
                    break;
                case 55:
                case 57:
                case 59:
                case 61:
                    if (F_set_key5_puls ==1)
                    {
                        uchar a;
                        a = R_Set_Step-55;
                        if (a !=0)
                        {
                            a >>=1;
                        }
                        if(drinks_set[a].water_supply <99) drinks_set[a].water_supply++;
                        else drinks_set[a].water_supply =99;
                    }
                    F_set_key5_puls =1;
                    break;
                case 56:
                case 58:
                case 60:
                case 62:
                    if (F_set_key5_puls ==1)
                    {
                        uchar a;
                        a = R_Set_Step-56;
                        if (a !=0)
                        {
                            a >>=1;
                        }
                        if(drinks_set[a].powder_supply <99) drinks_set[a].powder_supply++;
                        else drinks_set[a].powder_supply =99;
                    }
                    F_set_key5_puls =1;
                    break;
                case 63:
                    if (F_set_key5_puls ==1)
                    {
                        if(R_Cup_Free <3) R_Cup_Free++;
                        else R_Cup_Free =3;
                    }
                    F_set_key5_puls =1;
                    break;
                default:    break;
                }

            }
        }break;


        case 7:
            if (R_Set_Step ==10) {R_Set_Step =17;break;}
            if ((R_Set_Step >=11)&& (R_Set_Step <=15)) {R_Set_Step =0;break;}
            if (R_Set_Step ==40)
            {
                RFS_data_write();
                data_read();
                R_Set_Step =0;
                break;
            }
            if (R_Set_Step >=50)
            {
                data_write();
                R_Set_Step=0;
            }break;

        case 8:
            if (R_Set_Step >=50)
            {
                switch (R_Set_Step)
                {
                case 50:
                    if(Seted_temperature >65) Seted_temperature--;
                    else Seted_temperature =65;
                    break;
                case 51:
                case 52:
                case 53:
                case 54:
                    if(drinks_set[R_Set_Step-51].drink_price >0) drinks_set[R_Set_Step-51].drink_price--;
                    else drinks_set[R_Set_Step-51].drink_price =0;
                    break;
                case 55:
                case 57:
                case 59:
                case 61:
                    if (F_set_key5_puls ==1)
                    {
                        uchar a;
                        a = R_Set_Step-55;
                        if (a !=0)
                        {
                            a >>=1;
                        }
                        if(drinks_set[a].water_supply >1) drinks_set[a].water_supply--;
                        else drinks_set[a].water_supply =1;
                    }
                    F_set_key5_puls =1;
                    break;
                case 56:
                case 58:
                case 60:
                case 62:
                    if (F_set_key5_puls ==1)
                    {
                        uchar a;
                        a = R_Set_Step-56;
                        if (a !=0)
                        {
                            a >>=1;
                        }
                        if(drinks_set[a].powder_supply >1) drinks_set[a].powder_supply--;
                        else drinks_set[a].powder_supply =1;
                    }
                    F_set_key5_puls =1;
                    break;
                case 63:
                    if (F_set_key5_puls ==1)
                    {
                        if(R_Cup_Free >0) R_Cup_Free--;
                    }
                    F_set_key5_puls =1;
                    break;
                default:    break;
                }
            }break;

        default: break;
        }

        if (KeyValue_set !=4)
        {
            R_Set_or_Dsp_Timer2 =0;
        }
        KeyValue_set =0;
//      R_LedBuf[1].uDig=code_T;
//      R_LedBuf[1].tDig.df=1;
//      R_LedBuf[2].uDig=leddata[R_Set_Step /10];
//      R_LedBuf[3].uDig=leddata[R_Set_Step%10];

    }


    void run_set(void)
    {

        uchar m=0,a_temp=0;
        ulong a_temp32 =0;

        UF_rly1 =0;
        UF_rly2 &=0b11100000;

        switch (R_Set_Step)
        {
        case 0:{
            R_LedBuf[1].uDig=code_S;
            R_LedBuf[2].uDig=code_E;
            R_LedBuf[3].uDig=code_T;
            R_Set_or_Dsp_Timer =0;
        }break;
        case 10:{
            R_LedBuf[1].uDig=code_T;
            R_LedBuf[2].uDig=code_E;
            R_LedBuf[3].uDig=code_S;
            R_Set_or_Dsp_Timer =0;
        }break;

        case 11:
        case 12:
        case 13:
        case 14:
        {
            a_temp32 = drinks_set[R_Set_Step-11].drink_offers;

            if (( R_Set_or_Dsp_Timer >= 1000/T_Task_Disp_Status ) && ( R_Set_or_Dsp_Timer <2000/T_Task_Disp_Status ))
            {
                a_temp = (uchar) (a_temp32 /100%10);
                R_LedBuf[1].uDig=leddata[a_temp];
                a_temp = (uchar) (a_temp32 /10%10);
                R_LedBuf[2].uDig=leddata[a_temp];
                a_temp = (uchar) (a_temp32 %10);
                R_LedBuf[3].uDig=leddata[a_temp];

                if(R_Set_or_Dsp_Timer ==2000/T_Task_Disp_Status)
                {
                    alm_set(1,0,5,10);
                }

            }
            else
            {
                R_LedBuf[1].uDig=leddata[R_Set_Step-10];
                R_LedBuf[1].tDig.df =1;

                a_temp = (uchar) (a_temp32 /10000%10);
                R_LedBuf[2].uDig=leddata[a_temp];
                a_temp = (uchar) (a_temp32 /1000%10);
                R_LedBuf[3].uDig=leddata[a_temp];
            }

            if (R_Set_or_Dsp_Timer <= 2500/T_Task_Disp_Status)
            {
                R_Set_or_Dsp_Timer++;
            }
        }break;
        case 15:
        {
            if (R_NTC_AD > 929) {
                R_LedBuf[1].uDig=code_mid_line;
                R_LedBuf[2].uDig=code_mid_line;
            }
            else {
                R_LedBuf[1].uDig=leddata[Current_temperature /10];
                R_LedBuf[2].uDig=leddata[Current_temperature %10];
            }
            R_LedBuf[2].tDig.df=1;
            R_LedBuf[3].uDig=code_C;
        }break;
        case 17:
        {
            R_Set_or_Dsp_Timer++;
            F_set_key_2_3_17 =1;
            switch (R_Set_or_Dsp_Timer/(1500/T_Task_Disp_Status))
            {
            case 0:
            {
                F_rly8 =1;
                R_LedBuf[1].uDig=code_U;
                R_LedBuf[2].uDig=code_l;
                R_LedBuf[2].tDig.df=1;
                R_LedBuf[3].uDig=code_1;
            }break;
            case 1:
            {
                //F_rly8 =0;
                F_rly9 =1;
                R_LedBuf[3].uDig=code_2;
            }break;
            case 2:
            {
                //F_rly9 =0;
                F_rly10 =1;
                R_LedBuf[3].uDig=code_3;
            }break;
            case 3:
            {
                //F_rly10 =0;
                F_rly11 =1;
                R_LedBuf[3].uDig=code_4;
            }break;
            case 4:
            {
                //F_rly11 =0;
                F_rly12 =1;
                R_LedBuf[3].uDig=code_5;
            }break;
            case 5:
            {
                //F_rly12 =0;
                if (R_Cup_AD > 205)       //
                {
                    F_rly13 =1;
                }

                R_LedBuf[3].uDig=code_6;
            }break;
            case 6:
            {
                F_rly13 =0;
                F_rly14 =1;
                R_LedBuf[3].uDig=code_7;
            }break;
            case 7:
            {
                F_rly14 =0;
                F_rly7 =1;
                R_LedBuf[1].uDig=code_N;
                R_LedBuf[2].uDig=code_o;
                R_LedBuf[2].tDig.df=1;
                R_LedBuf[3].uDig=code_7;
            }break;
            case 8:
            {
                //F_rly7 =0;
                F_rly6 =1;
                R_LedBuf[3].uDig=code_6;
            }break;
            case 9:
            {
                //F_rly6 =0;
                F_rly5 =1;
                R_LedBuf[3].uDig=code_5;
            }break;
            case 10:
            {
                //F_rly5 =0;
                F_rly4 =1;
                R_LedBuf[3].uDig=code_4;
            }break;
            case 11:
            {
                //F_rly4 =0;
                F_rly3 =1;
                R_LedBuf[3].uDig=code_3;
            }break;
            case 12:
            {
                //F_rly3 =0;
                F_rly2 =1;
                R_LedBuf[3].uDig=code_2;
            }break;
            case 13:
            {
                //F_rly2 =0;
                F_rly1 =1;
                R_LedBuf[3].uDig=code_1;
            }break;
            case 14:
            {
                //F_rly1 =0;
                if (R_NTC_AD > 929) {
                    R_LedBuf[1].uDig=code_mid_line;
                    R_LedBuf[2].uDig=code_mid_line;
                }
                else {
                    R_LedBuf[1].uDig=leddata[Current_temperature /10];
                    R_LedBuf[2].uDig=leddata[Current_temperature %10];
                }
                R_LedBuf[2].tDig.df=1;
                R_LedBuf[3].uDig=code_C;
            }break;
            default:
            {
                F_set_key_2_3_17 =0;
                R_Set_Step=0;
                alm_set(2,0,5,10);
            }break;

            }

        }break;

        case 20:        // ->Set -> Key2  自动清洗
        {
            F_set_key_2_3_17 =1;

            R_LedBuf[1].uDig= code_3h;
            R_LedBuf[2].uDig= code_3h;
            R_LedBuf[3].uDig= code_3h;

            if (!F_channel_clear_ing)
            {
                R_Set_Step =0;
                F_set_key_2_3_17 =0;
                alm_set(2,0,5,10);
            }
        }break;

        case 30:        // ->Set -> Key3  落杯测试
        {
            F_set_key_2_3_17 =1;

            R_LedBuf[1].uDig = code_3h;
            R_LedBuf[2].uDig = code_U;
            R_LedBuf[3].uDig = code_3h;

            if (!F_cup_down_ing)
            {
                R_Set_Step =0;
                F_set_key_2_3_17 =0;
                if (F_cup_down_ok)
                {
                    F_Err_cup_down =0;
                    alm_set(2,0,5,10);
                }
                else
                {
                    F_Err_cup_down =1;
                    alm_set(3,0,3,6);
                }
            }
        }break;
        case 40:        //  Key4  长按
        {
            R_LedBuf[1].uDig = code_r;
            R_LedBuf[2].uDig = code_F;
            R_LedBuf[3].uDig = code_S;
        }break;




        case 50:
        {
            R_LedBuf[1].uDig=code_T;
            R_LedBuf[1].tDig.df=1;
            R_LedBuf[2].uDig=leddata[Seted_temperature / 10];
            R_LedBuf[3].uDig=leddata[Seted_temperature % 10];
        }break;

        case 51:
        case 52:
        case 53:
        case 54:                                //显示粉盒号，及其价格
            switch (R_Set_Step-51)
            {
            case 0: R_LedBuf[1].uDig=code_1h;break;
            case 1: R_LedBuf[1].uDig=code_2h;break;
            case 2: R_LedBuf[1].uDig=code_3h;break;
            case 3: R_LedBuf[1].uDig=code_4h;break;
            default:    break;
            }

            R_LedBuf[1].tDig.df=1;
            a_temp = drinks_set[R_Set_Step-51].drink_price;

            R_LedBuf[2].uDig=leddata[a_temp/10];
            R_LedBuf[3].uDig=leddata[a_temp%10];
            break;

        case 55:
        case 57:
        case 59:
        case 61:
            m = R_Set_Step-55;
            if (m !=0)
            {
                m >>=1;
            }

            if (!F_set_key5_puls)
            {
                switch (m)
                {
                case 0:
                    R_LedBuf[1].uDig  = code_A;
                    break;
                case 1:
                    R_LedBuf[1].uDig  = code_b;
                    break;
                case 2:
                    R_LedBuf[1].uDig  = code_C;
                    break;
                case 3:
                    R_LedBuf[1].uDig  = code_d;
                    break;
                default: break;
                }

                R_LedBuf[2].uDig = code_mid_line;
                R_LedBuf[3].uDig = code_S;
            }
            else
            {
                a_temp =drinks_set[m].water_supply;

                R_LedBuf[2].uDig = leddata[a_temp /10];
                R_LedBuf[2].tDig.df =1;
                R_LedBuf[3].uDig = leddata[a_temp %10];
            }
            break;

        case 56:
        case 58:
        case 60:
        case 62:
            m = R_Set_Step-56;
            if (m !=0)
            {
                m >>=1;
            }

            if(!F_set_key5_puls)
            {
                switch (m)
                {
                case 0:
                    R_LedBuf[1].uDig  = code_A;
                    break;
                case 1:
                    R_LedBuf[1].uDig  = code_b;
                    break;
                case 2:
                    R_LedBuf[1].uDig  = code_C;
                    break;
                case 3:
                    R_LedBuf[1].uDig  = code_d;
                    break;
                default: break;
                }

                R_LedBuf[2].uDig=code_mid_line;
                R_LedBuf[3].uDig=code_F;
            }
            else
            {
                a_temp =drinks_set[m].powder_supply;
                R_LedBuf[2].uDig=leddata[a_temp /10];
                R_LedBuf[2].tDig.df=1;
                R_LedBuf[3].uDig=leddata[a_temp %10];
            }
            break;

        case 63:
            if(!F_set_key5_puls)
            {
                R_LedBuf[1].uDig=code_C;
                R_LedBuf[2].uDig=code_U;
                R_LedBuf[3].uDig=code_P;
            }
            else
            {
                R_LedBuf[1].uDig=code_n;
                R_LedBuf[1].tDig.df =1;
                R_LedBuf[2].uDig=code_0;
                R_LedBuf[3].uDig=leddata[R_Cup_Free];
            }
            break;

        default:
            break;

        }
    }



    uchar R_blink_timer =0;



    void Normal_dsp(void)
    {
        R_blink_timer++ ;

        if (R_blink_timer > 500/T_Task_Disp_Status)
        {
            F_blink_led = ~F_blink_led;
            R_blink_timer =0;
        }

        /*-----LED1 单价------------------*/
        if (( KeyValue_nmdsp >0 ) && ( KeyValue_nmdsp <9 ) && !F_sys_err )
        {
            uchar x,z;
            x =drinks_rly_list[KeyValue_nmdsp-1][3];        //取料盒号
            z =drinks_set[x-1].drink_price;

            R_Set_or_Dsp_Timer2++;
            if (R_Set_or_Dsp_Timer2 < (1000/T_Task_Disp_Status))
            {
                R_LedBuf[0].tDig.a=1;

                R_LedBuf[2].uDig=leddata[z /10];
                R_LedBuf[3].uDig=leddata[z %10];
                switch (x)
                {
                case 1: R_LedBuf[1].uDig=code_1h;
                    break;
                case 2: R_LedBuf[1].uDig=code_2h;
                    break;
                case 3: R_LedBuf[1].uDig=code_3h;
                    break;
                case 4: R_LedBuf[1].uDig=code_4h;
                    break;
                default:
                    break;
                }
                R_LedBuf[1].tDig.df=1;

            }
            else
            {
                KeyValue_nmdsp =0;
                R_LedBuf[1].uDig=code_P;
                R_LedBuf[2].uDig=leddata[Coins /10];
                R_LedBuf[3].uDig=leddata[Coins %10];
                R_LedBuf[0].tDig.a=0;
                R_Set_or_Dsp_Timer2=0;
            }
        }

        /*-----LED2 使用状态------------------*/
        if (!F_sys_err)
        {

            R_LedBuf[0].tDig.b =1;
            if(Current_temperature <70 )
            {
                R_LedBuf[0].tDig.b =F_blink_led;
            }
        }
        else
        {
            R_LedBuf[0].tDig.b =0;
        }


        /*-----LED3 加热状态------------------*/
        if(( Current_temperature <70 ) && F_hot_ing)
        {
            R_LedBuf[0].tDig.c=1;
        }
        else
        {
            R_LedBuf[0].tDig.c=0;
        }

        if (!F_sys_err && (KeyValue_nmdsp==0))
        {
            if((Current_temperature < 70) && (Offer_Step < 2) && (Coins ==0))
            {
                if (R_NTC_AD > 929) {
                    R_LedBuf[1].uDig=code_mid_line;
                    R_LedBuf[2].uDig=code_mid_line;
                }
                else {
                    R_LedBuf[1].uDig=leddata[Current_temperature /10];
                    R_LedBuf[2].uDig=leddata[Current_temperature %10];
                }
                R_LedBuf[2].tDig.df=1;
                R_LedBuf[3].uDig=code_C;
            }
            else
            {
                R_LedBuf[1].uDig=code_P;
                R_LedBuf[2].uDig=leddata[Coins /10];
                R_LedBuf[3].uDig=leddata[Coins %10];
            }
        }

        /*-----LED4，LED8 出货状态------------------*/
        if (Offer_Step >1)
        {
            R_LedBuf[0].tDig.d=1;
            R_LedBuf[0].tDig.e=1;
        }
        else
        {
            R_LedBuf[0].tDig.d=0;
            R_LedBuf[0].tDig.e=0;
        }

        /*-----LED5 投币金额------------------*/
        if (Coins >0) R_LedBuf[0].tDig.f=1;
        else R_LedBuf[0].tDig.f=0;
        /*-----LED6 缺水------------------*/
        if (F_Err_water)
        {
//      R_LedBuf[0].tDig.g=1;
            R_LedBuf[0].tDig.g=F_blink_led;

            R_LedBuf[1].uDig=code_blank;
            R_LedBuf[2].uDig=code_E;
            R_LedBuf[3].uDig=code_1;

        }
        else
        {
            R_LedBuf[0].tDig.g=0;
        }

        /*-----LED7 缺杯------------------*/
        if (F_Err_cup)
        {
//      R_LedBuf[0].tDig.df=1;

            R_LedBuf[0].tDig.df=F_blink_led;
            R_LedBuf[1].uDig=code_blank;
            R_LedBuf[2].uDig=code_E;
            R_LedBuf[3].uDig=code_2;
        }
        else
        {
            R_LedBuf[0].tDig.df=0;
        }

        if (F_Err_cup_down)
        {
            R_LedBuf[1].uDig=code_blank;
            R_LedBuf[2].uDig=code_E;
            R_LedBuf[3].uDig=code_3;
        }
        if (F_Err_coin)
        {
            R_LedBuf[1].uDig=code_blank;
            R_LedBuf[2].uDig=code_E;
            R_LedBuf[3].uDig=code_4;
        }
        if (F_Err_hot)
        {
            R_LedBuf[1].uDig=code_blank;
            R_LedBuf[2].uDig=code_E;
            R_LedBuf[3].uDig=code_5;
        }
    }


    void Inout_Setmodel_init(void)
    {
        data_read();

        F_Status_change =1;
        nP_coin_en =1;
        UF_rly1 =0;
        UF_rly2 &=0b11100000;

        F_channel_clear_ing =0;
        F_cup_down_ing =0;
        F_cup_down_ok =0;
        Change_Cup_Channel_Timer =0;

        P_hot =0;

        Offer_Step =0;
        KeyValue_drk =0;
        KeyValue_nmdsp =0;
        wmq_selected_value =0;
        Err_Num =0;
        Coins_low_timer =0;

        R_Set_Step =0;
        KeyValue_set =0;
        R_Set_or_Dsp_Timer =0;
        R_Set_or_Dsp_Timer2 =0;
        F_set_key5_puls =0;
        F_set_key_2_3_17 =0;
        R_blink_timer =0;

        Buz_alm_times = 0;
        Buz_alm_timer = 0;
        Buz_alm_timer_duty = 0;
        Buz_alm_timer_cycle = 0;

        R_LedBuf[0].uDig=code_blank;
    }



    void Setmodel_and_Dsp_Led_pro (void)
    {

        if((P_Set!=F_P_Set_init_bak) && Offer_Step <2)
        {
            if (F_First_into_Setmodel)
            {
                Inout_Setmodel_init();
                F_First_into_Setmodel =0;
                F_First_out_Setmodel =1;
                F_set_ing =1;
            }

            if (( KeyValue_set !=0 ) && ( KeyValue_set !=4 ))
            {
                alm_set(1,0,5,10);
            }

            get_set_key_value();
            run_set();
        }
        else if(F_sms_dsp)
        {
            if(F_channel_clear_ing){
                R_LedBuf[1].uDig= code_3h;
                R_LedBuf[2].uDig= code_3h;
                R_LedBuf[3].uDig= code_3h;
            }
            if(F_cup_down_ing){
                R_LedBuf[1].uDig = code_3h;
                R_LedBuf[2].uDig = code_U;
                R_LedBuf[3].uDig = code_3h;
            }
        }
        else
        {
            if (F_First_out_Setmodel)
            {
                Inout_Setmodel_init();
                F_First_into_Setmodel =1;
                F_First_out_Setmodel =0;
                F_set_ing =0;
            }
            Normal_dsp();
        }

    }
