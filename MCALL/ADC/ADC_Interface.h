#ifndef ADC_INTERFACE_H
#define ADC_INTERFACE_H

#include "../../LIBs/STD_TYPES.h"

#define ADC_PRESCALAR_BY_2                  1
#define ADC_PRESCALAR_BY_4                  2
#define ADC_PRESCALAR_BY_8                  3
#define ADC_PRESCALAR_BY_16                 4
#define ADC_PRESCALAR_BY_32                 5
#define ADC_PRESCALAR_BY_64                 6
#define ADC_PRESCALAR_BY_128                7


#define ADC_CHANNEL_0                       0
#define ADC_CHANNEL_1                       1
#define ADC_CHANNEL_2                       2
#define ADC_CHANNEL_3                       3
#define ADC_CHANNEL_4                       4
#define ADC_CHANNEL_5                       5
#define ADC_CHANNEL_6                       6
#define ADC_CHANNEL_7                       7



void ADC_voidInit(void);

/*This FUNC READS A SPECIFIC ADC CHANNEL*/
u16 ADC_u16ReadSynchronus(u8 Copy_u8ChannelNum);



/*This FUNC READS A SPECIFIC ADC CHANNEL.
Copy_u8ChannelNum Options : ADC_CHANNEL_0 ---->  ADC_CHANNEL_7
 *copy_pvCallBackFunctionPtr : Address of the Function to be executed*/
u16 ADC_u16ReadAsynchronus(u8 Copy_u8ChannelNum , void (*Copy_pvCallBackFunctionPtr)(void));












#endif
