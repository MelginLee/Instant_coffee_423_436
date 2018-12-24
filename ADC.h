/******************************************************************************/
/*
* File				: ADC.h
* Description		: Description
* Author			: Melgin
* Created on		: Date
*/
/******************************************************************************/

#ifndef _ADC_h_
#define	_ADC_h_

/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/






/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
extern	uchar	ADC_channel;
extern	uchar	ADC_Conversion_Times;
extern	uint	ADC_buf;
extern	uint	R_Water_AD;
extern	uint	R_Cup_AD;
extern	uint	R_NTC_AD;
extern	uchar	Current_temperature;
extern	const uint TabNtc_10K[];






/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/
void Start_ADC (void);
void ADC_converter(void);
uchar FineTab(uint *a,uchar TabLong,uint data);

 #define TabNtc_10K_Size  95 //定义表长度


#endif
