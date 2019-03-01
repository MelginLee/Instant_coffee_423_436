﻿/******************************************************************************/
/*
* File              : Ver_Info.h
* Description       : Description
* Author            : Melgin
* Created on        : Date
*/
/******************************************************************************/

/******************************************************************************/



//----------------------------
// OP_423_436_V1.6.X
//
// 1、Key9 输出通道更改为 outside [及第四路水]
// 2、热水：温度 >70 度
// 3、取消“加热时，最长加热时间为 20 分钟，之后显示 E5 加热超时报警”
// 4、增加 清洗、落杯、恢复出厂设置，重置并擦除EEPROM 协议



//----------------------------
// OP_423_436_V1.5.X
//
// 修复 V1.4 ISSUE

//----------------------------
// ISSUE V1.4
//1、上电默认无杯
//2、检测微码器丢失时，加一个故障提示，推送给后台。


// OP_423_436_V1.4.X

//
//更新
//修改：
//  1、Driver.c  void Normal_dsp(void)
//  2、修改出厂默认水量，粉量
//  3、Usart 接收中断 buff 清零


//----------------------------
// OP_423_436_V1.3.X

//
//更新
//增加：
//  1、增加 Key9 免费出热水，即按即停


//----------------------------
// OP_423_436_V1.2.X

//
//更新
//增加：
//  1、增加近距离 10NK 避障传感器 检测有无杯，原杯桶内微动开关，由常闭 改为 常开




//----------------------------
// OP_423_436_V1.1.X

//
//更新
//增加：
//  1、加热罐 NTC 掉线时，显示 “- - C”，并停止加热
//  2、加热时，最长加热时间为 20 分钟，之后显示 E5 加热超时报警



//----------------------------
// OP_423_436_V1.0.X

//
//更新
//
//原理图更新，P25,P26 分别与 P34，P33 对调 P25,P26,用于串口通信，及投币器
//更换主控IC为 887
//增加投币器与微码器同时使用
//
//

