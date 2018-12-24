/******************************************************************************/
/*
* FILE              : TASKSERVICE.C
* DESCRIPTION       : THE TASK MANAGER  AND THE Process Function
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
    #include "Serial_Communications.h"




/******************************************************************************/
/* Global Variable Declaration                                           */
/******************************************************************************/


/******************************************************************************/
/* Function Declaration                                                   */
/******************************************************************************/
    void Task_Input_Scan(void);
    void Task_Disp_Status(void);
    void Task_Drinks_Supply(void);
    void Task_Chk_Status(void);
    void Task_Freshe_Hardware(void);
    void alm_chk(void);
    void Water_chk(void);
    void Temperature_chk(void);


/******************************************************************************/
/*
* FunctionName   : TASK_COMPONENTS
* Description    : 任务结构体定义
* EntryParameter : None
* ReturnValue    : None
*/
/******************************************************************************/
typedef struct _TASK_COMPONENTS
{
    uchar Run;                                                                  // 程序运行标记：0-不运行，1运行
    uint Timer;                                                                 // 计时器
    uint ItvTime;                                                               // 任务运行间隔时间
    void (*TaskHook)(void);                                                     // 要运行的任务函数
} TASK_COMPONENTS;                                                              // 任务结构体定义

/******************************************************************************/
/*
* FunctionName   : TASK_COMPONENTS TaskComps[]
* Description    : TASK 结构初始化
* EntryParameter : None
* ReturnValue    : None
*/
/******************************************************************************/
static TASK_COMPONENTS TaskComps[] =
{
    {0, T_Task_Input_Scan,      T_Task_Input_Scan,      Task_Input_Scan},                                               // key scan
    {0, T_Task_Drinks_Supply,   T_Task_Drinks_Supply,   Task_Drinks_Supply},                                            // serve drink
    {0, T_Task_Freshe_Hardware, T_Task_Freshe_Hardware, Task_Freshe_Hardware},                                          // serve drink
    {0, T_Task_Chk_Status,      T_Task_Chk_Status,      Task_Chk_Status},                                               // check status
    {0, T_Task_Disp_Status,     T_Task_Disp_Status,     Task_Disp_Status}                                               // display status

    /* 这里添加你的任务 */
};








/******************************************************************************/
/*
* FunctionName   : TASK_LIST
* Description    : TASK 任务列表
* EntryParameter : None
* ReturnValue    : None
*/
/******************************************************************************/

typedef enum _TASK_LIST
{
    _Task_Input_Scan,
    _Task_Drinks_Supply,
    _Task_Freshe_Hardware,
    _Task_Chk_Status,
    _Task_Disp_Status,
    /* 这里添加你的任务 */
     TASKS_MAX                                                                  // 总的可供分配的定时任务数目
} TASK_LIST;




/******************************************************************************/
/*
* FunctionName   : TaskRemarks()
* Description    : 任务标志处理
* EntryParameter : None
* ReturnValue    : None
*/
/******************************************************************************/

void TaskRemarks(void)
{
    uint i;

    for (i=0; i<TASKS_MAX; i++)                                                 // 逐个任务时间处理
    {
         if (TaskComps[i].Timer)                                                // 时间不为0
        {
            TaskComps[i].Timer--;                                               // 减去一个节拍
            if (TaskComps[i].Timer == 0)                                        // 时间减完了
            {
                 TaskComps[i].Timer = TaskComps[i].ItvTime;                     // 恢复计时器值，从新下一次
                 TaskComps[i].Run = 1;                                          // 任务可以运行
            }
        }
   }
}

/******************************************************************************/
/*
* FunctionName   : TaskProcess()
* Description    : 任务处理
* EntryParameter : None
* ReturnValue    : None
*/
/******************************************************************************/

void TaskProcess(void)
{
    uint i;

    for (i=0; i<TASKS_MAX; i++)           // 逐个任务时间处理
    {
         if (TaskComps[i].Run)            // 时间不为0
        {
             TaskComps[i].TaskHook();     // 运行任务
             TaskComps[i].Run = 0;        // 标志清0
        }
    }
}









/**************************************************************************************/
/*
* FunctionName   : Task_Input_Scan()
* Description    : Description
* EntryParameter : None
* ReturnValue    : None
*/
/**************************************************************************************/
void Task_Input_Scan(void)
{
    KeyportScan();
    Key_exe();
    Coins_chk();
    Wmq_chk();
}


/**************************************************************************************/
/*
* FunctionName   : Task_Freshe_Hardware()
* Description    : Description
* EntryParameter : None
* ReturnValue    : None
*/
/**************************************************************************************/

void Task_Freshe_Hardware(void)
{
//  XOR_P=!XOR_P;   /* @debug */

    Freshe_Buzzer();
    Cup_down();
    //Supply_water();

    nP_373_E =0;    /* 使能373 如果硬件上能够满足 上电几秒钟让其=1 系统工作后让其=0  这个脚是可以空出来的 */
    if(!nP_373_E)
    {
        Freshe_Rly();
    }
}

/******************************************************************************/
/*
* FunctionName   : void Task_Drinks_Supply(void)
* Description    : Description
* EntryParameter : None
* ReturnValue    : None
*/
/******************************************************************************/
    uchar Offer_Step =0;

void Task_Drinks_Supply(void)
{

//  XOR_P=!XOR_P;   /* @debug */
    cnop;

    if(!F_set_ing) //非设置状态下
    {
        switch (Offer_Step)
        {
            case 0: ;break;
            case 1: offer_payment_chk();break;
            case 2: cup_supply();break;
            case 3: cup_supply_over_chk();break;
            case 4: water_supply();break;
            case 5: powder_supply();break;
            case 6: supply_over_chk();break;
            case 7: supply_over();break;
            case 8: welcome_back();break;
            default:Offer_Step =0;break;
        }
    }
}





/**************************************************************************************/
/*
* FunctionName   : Task_Chk_Status()
* Description    : Description
* EntryParameter : None
* ReturnValue    : None
*/
/**************************************************************************************/


void Task_Chk_Status(void)
{

        Serial_Pro();
        ADC_converter();
        Temperature_chk();
        if (R_Set_Step !=17)
        {
            F_rly14 = (R_Water_AD < 0x280)?1:0;
        }

    if (!F_set_ing)
    {

        if (Offer_Step <1 )
        {
            Cup_chk();
            Water_chk();
            alm_chk();
        }

        if((( KeyValue_drk >0 ) || ( wmq_selected_value >0 )) && !Offer_Step && !F_sys_err && ( Buz_alm_timer_cycle ==0 ) && ( Change_Cup_Channel_Timer ==0 ))
            {
                Offer_Step=1;
            }

    }
}





/******************************************************************************/
void alm_chk(void)
{

    if (!F_Err_water && !F_Err_cup && !F_Err_cup_down && !F_Err_coin && !F_Err_hot)
    {
        F_sys_err =0;
    }

    else
    {
        F_sys_err =1;
    }


    if (F_sys_err)
    {
        nP_coin_en =1;
        UF_rly1 =0;
        UF_rly2 &=0b11100000;
        F_cup_down_ing =0;
        F_cup_down_ok  =0;
        KeyValue =0;
        KeyValue_drk =0;
        KeyValue_set =0;
        KeyValue_nmdsp =0;
        wmq_selected_value =0;

        Offer_Step =0;

        if (!F_sys_err_alm_exed)
        {
            if (Buz_alm_times | Buz_alm_timer_cycle ==0)
                {
                    alm_set(6,0,5,45);
                    F_sys_err_alm_exed =1;
                    F_Status_change =1;
                }

        }
    }
    else
    {
        if (F_sys_err_alm_exed)
        {
            alm_set(0,0,0,0);
            F_sys_err_alm_exed =0;
            F_Status_change =1;
        }

        if (Buz_alm_times | Buz_alm_timer_cycle ==0)
        {
            F_sys_err_alm_exed =0;
            if (R_Cup_Free >1 )   //  是收费模式
            {
                if (Coins <99)
                {
                    nP_coin_en =0;
                }
                else
                {
                    nP_coin_en =1;
                }
            }
        }
    }
}




/******************************************************************************/
void    Water_chk(void)
{


/*----- water status chk------------------*/
    F_Err_water =R_Water_AD < 0x180 ?1:0;
    if(F_Err_water ==1) P_hot =0;
}



uint R_hot_timer =0;

void    Temperature_chk(void)
{
/*----- hot_temperature chk---------------*/
    uint *pTmp ; //定义一个字长的指针

    pTmp = TabNtc_10K ; //指向要温度表的地址
    Current_temperature = FineTab(pTmp,TabNtc_10K_Size,R_NTC_AD); //查表得出温度 R_NTCAD为NTC分压得出的AD值

//  P_hot = ((Current_temperature < Seted_temperature) && !F_sys_err && (!F_set_ing) && (Offer_Step <2))?1:0;

    if (!F_set_ing && (R_NTC_AD < 929))
    {
        if (F_hot_ing)
        {
            if ((Current_temperature < Seted_temperature) && !F_Err_water)
            {
                F_hot_ing =1;
                P_hot =1;
            }
            else
            {
                F_hot_ing =0;
                P_hot =0;
            }
        }
        else
        {
            if ((Current_temperature < (Seted_temperature -5)) && !F_Err_water)
            {
                F_hot_ing =1;
                P_hot =1;
            }
            else
            {
                F_hot_ing =0;
                P_hot =0;
            }
        }
    }
    else {
        P_hot =0;
        F_hot_ing =0;
    }

    if (P_hot) {
        R_hot_timer++;
        if (R_hot_timer > 20*1200) //20分
        {
            P_hot =0;
            F_hot_ing =0;
            F_Err_hot =1;
        }
    }
    else R_hot_timer =0;

}





/**************************************************************************************/
/*
* FunctionName   : Task_Disp_Status()
* Description    : Description
* EntryParameter : None
* ReturnValue    : None
*/
/**************************************************************************************/
void Task_Disp_Status(void)
{
    Setmodel_and_Dsp_Led_pro();
}


