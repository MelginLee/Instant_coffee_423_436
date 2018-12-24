/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#include <xc.h>         /* XC8 General Include File */

#include "System.h"        /* System funct/params, like osc/peripheral config */
#include "TaskService.h"
#include "M_Config.h"
#include "User.h"
#include "ADC.h"
#include "Driver.h"
#include "Serial_Communications.h"
#include "Input_Scan.h"



/******************************************************************************/
/* FunctionName  : interrupt isr()
* Description    : Description
* EntryParameter : None
* ReturnValue    : None
*/
/******************************************************************************/
    void interrupt isr(void)
{

    if(TMR0IE && TMR0IF)
    {

        T0IF =0;
        TMR0 =C_TMR0_CNT;
//      XOR_P=!XOR_P;   /* @debug */
    }

  /*----- TMR1IF------------------*/
    if(TMR1IE && TMR1IF)
    {
        TMR1IF =0;
        TMR1H =C_TMR1H_CNT;
        TMR1L =C_TMR1L_CNT;
//      XOR_P=!XOR_P;   /* @debug */

        if (R_voice_dly_cnt >0)
        {
            R_voice_dly_cnt--;
        }

        uchar b,c=0;
        while (c <=3)
        {
            PORTC &=0b11110000;
            if (R_LedBuf[3-c].uDig != 0)
            {
                b = ~R_LedBuf[3-c].uDig;

                for (uchar a =8;a>0 ;a--)
                {
                    P_164_DA = (b & 0x80)?1:0 ;
                    P_164_CLK =0;
                    b <<=1;
                    cnop;
                    cnop;
                    P_164_CLK =1;

                }
                cnop;
    //          P_164_CLK =1;
                PORTC |= (1<<c);
            }
            c++;
            for (uchar a =0;a<25 ;a++ ){};
        }
    }
  /*----- TMR2IF------------------*/
    if(TMR2IE && TMR2IF)
    {
        TMR2IF =0;
        PR2 =C_PR2_CNT;
//      XOR_P=!XOR_P;   /* @debug */
        F_Sys_1MS =1;

        if (!P_coin)
        {
            if (Coins_low_timer <5000)
            {
                Coins_low_timer++;
            }
        }

        if (R_rc_blank_cnt <168)
        {
            R_rc_blank_cnt++;
        }
        else
        {
            if (( F_RC_SMS_ok ==0 ) && ( TXEN==0 ))
            {
                F_sms_rc_packet =0;
                F_RC_SMS_ing =0;
//              RCIE=1;             //使能接收
                CREN=1;             //使能接收

            }
        }
    }
/*----- INTF------------------*/
    if(INTE && INTF)
    {
        INTF=0;
    }
/*----- ADIF------------------*/
    if(ADIE && ADIF)
    {
        ADIF =0;
        ADIE =0;
        F_ADC_OK =1;
    }

/*----- RCIF------------------*/
    if(RCIE && RCIF)
    {
        R_rc_blank_cnt =0;
        R_wmq_lose_timer =0;

        rtemp=RCREG;

        if (( rtemp ==0xaa ) && ( F_RC_SMS_ing ==0 ))
        {
            F_sms_rc_packet =0;
            F_RC_SMS_ing =1;
            rc_byte_cnt =0;
            R_sms_buf[0]=rtemp;
            rc_byte_cnt++;
            for (uchar i=1;i<25;i++)
            {
                R_sms_buf[i] =0;
            }

        }
        else if (( rtemp ==0xab ) && ( F_RC_SMS_ing ==1 ) && ( rc_byte_cnt ==(R_sms_buf[2]+5) ))
        {
            F_RC_SMS_ing=0;
            R_sms_buf[rc_byte_cnt]=rtemp;
            F_sms_rc_packet =1;
            CREN=0;
        //  RCIE=0;

        }
        else
        {
            R_sms_buf[rc_byte_cnt]=rtemp;
            rc_byte_cnt++;
            if (rc_byte_cnt >24)
            {
                F_RC_SMS_ing=0;
                F_sms_rc_packet =0;
                rc_byte_cnt =0;
                for (uchar i=0;i<25;i++)
                {
                    R_sms_buf[i] =0;
                }
                CREN=0;
                //  RCIE=0;
            }
        }
//      RCIF=0;
    }
/*----- TXIF------------------*/
    if(TXIE && TXIF)
    {
        F_TX_SMS_ing =1;

        if ((tx_byte_cnt <= tx_byte_long) && (tx_byte_long <24))
        {
            TXREG=T_sms_buf[tx_byte_cnt];
        }
        else
        {
            F_TX_SMS_ing =0;
//          TXIE=0;
            TXEN =0;
            CREN=1;
//          RCIE=1;
            F_RC_SMS_ing =0;
            tx_byte_long =0;
        }
//      TXIF=0;
        tx_byte_cnt++;
    }
/*-----interrupt isr end-------*/
    else;
 }
