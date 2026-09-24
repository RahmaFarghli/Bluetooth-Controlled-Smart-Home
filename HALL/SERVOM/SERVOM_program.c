//#include "../../LIBs/STD_TYPES.h"
//#include "../../LIBs/BIT_MATH.h"
//
//#include "../../MCALL/DIO/DIO_Interface.h"
//#include "../../MCALL/Timer0/TIMER0_Interface.h"
//
//#include "SERVOM_Config.h"
//#include "SERVOM_Private.h"
//#include "SERVOM_Interface.h"
//
///*****************************************************************************************************************************
//* Brief: Function Used to Initialize Servo Pin and Top Value for 50Hz PWM                                                    *
//* Parameters: Void                                                                                                           *
//* Return: ErrorCode                                                                                                          *
//*****************************************************************************************************************************/
//ErrorCode SERVOM_enumInit(void)
//{
//    ErrorCode Local_enumValidationStatus = VALID;
//
//    /* 1. Set Servo Pin (PD5) as Output */
//    DIO_SetPinMode(SERVOM_u8_PORT, SERVOM_u8_PIN, DIO_PIN_OUTPUT);
//
//    /* 2. Set ICR1 Top value to 20000 for 20ms period (50Hz PWM) */
//    TIMER_voidTimer1SetInputCaptureValue(SERVOM_u16_PERIOD_TOP);
//
//    /* 3. Set Initial Angle to 0 (1000us Pulse) */
//    TIMER_voidTimer1ASetCompareValue(SERVOM_u16_MIN_PULSE);
//
//    /* 4. Initialize Timer1 */
//    TIMER_voidTimer1Init();
//
//    return Local_enumValidationStatus;
//}
///*****************************************************************************************************************************
//* Brief: Function Used to Set Servo Angle (0 to 180 degrees)                                                                *
//* Parameters: Copy_u8Angle                                                                                                  *
//* Return: ErrorCode                                                                                                          *
//*****************************************************************************************************************************/
//ErrorCode SERVOM_enumSetAngle(u8 Copy_u8Angle)
//{
//    ErrorCode Local_enumValidationStatus = VALID;
//    u16 Local_u16CompareMatchValue = 0;
//
//    if (Copy_u8Angle <= 180)
//    {
//        /* Map angle (0 -> 180) to Duty Cycle Pulse (1000us -> 2000us) */
//        Local_u16CompareMatchValue = SERVOM_u16_MIN_PULSE + (((u32)Copy_u8Angle * 1000) / 180);
//
//        /* Set Compare Match Value for Channel A (OCR1A) */
//        TIMER_voidTimer1ASetCompareValue(Local_u16CompareMatchValue);
//    }
//    else
//    {
//        Local_enumValidationStatus = INVALID;
//    }
//
//    return Local_enumValidationStatus;
//}
//
///*****************************************************************************************************************************
//* Brief: Function Used to Reset Servo Motor to 0 Degrees                                                                    *
//* Parameters: Void                                                                                                           *
//* Return: ErrorCode                                                                                                          *
//*****************************************************************************************************************************/
//ErrorCode SERVOM_enumStop(void)
//{
//    ErrorCode Local_enumValidationStatus = VALID;
//
//    /* Set pulse to 1000us (0 degrees position) */
//    TIMER_voidTimer1ASetCompareValue(SERVOM_u16_MIN_PULSE);
//
//    return Local_enumValidationStatus;
//}
#include "../../LIBs/STD_TYPES.h"
#include "../../LIBs/BIT_MATH.h"

#include "../../MCALL/DIO/DIO_Interface.h"
#include "../../MCALL/Timer0/TIMER0_Interface.h"

#include "SERVOM_Config.h"
#include "SERVOM_Private.h"
#include "SERVOM_Interface.h"

#include "../../LIBs/STD_TYPES.h"
#include "../../LIBs/BIT_MATH.h"

#include "../../MCALL/DIO/DIO_Interface.h"
#include "../../MCALL/Timer0/TIMER0_Interface.h"

#include "SERVOM_Config.h"
#include "SERVOM_Private.h"
#include "SERVOM_Interface.h"

ErrorCode SERVOM_enumInit(void)
{
    ErrorCode Local_enumValidationStatus = VALID;

    /* 1. ضبط طرف التحكم (PD5 / OC1A) كـ Output */
    DIO_SetPinMode(SERVOM_u8_PORT, SERVOM_u8_PIN, DIO_PIN_OUTPUT);

    /* 2. ضبط قيمة الـ TOP (ICR1 = 20000) للحصول على زمن دوري 20ms */
    TIMER_voidTimer1SetInputCaptureValue(SERVOM_u16_PERIOD_TOP);

    /* 3. تهيئة إعدادات Timer1 (Fast PWM mode 14) */
    TIMER_voidTimer1Init();

    /* 4. ضبط النبضة الابتدائية على زاوية صفر (1000us) */
    TIMER_voidTimer1ASetCompareValue(SERVOM_u16_MIN_PULSE);

    return Local_enumValidationStatus;
}

/*******************************************************************************
 * Brief: Sets Servo Angle (0 to 180 degrees)
 *******************************************************************************/
ErrorCode SERVOM_enumSetAngle(u8 Copy_u8Angle)
{
    ErrorCode Local_enumValidationStatus = VALID;
    u16 Local_u16CompareMatchValue = 0;

    if (Copy_u8Angle <= 180)
    {
        /* تحويل الزاوية إلى عرض النبضة المقابل بالـ microseconds */
        Local_u16CompareMatchValue = SERVOM_u16_MIN_PULSE + (((u32)Copy_u8Angle * (SERVOM_u16_MAX_PULSE - SERVOM_u16_MIN_PULSE)) / 180);

        /* تحديث مسجل المقارنة للقناة OCR1A */
        TIMER_voidTimer1ASetCompareValue(Local_u16CompareMatchValue);
    }
    else
    {
        Local_enumValidationStatus = INVALID;
    }

    return Local_enumValidationStatus;
}

/*******************************************************************************
 * Brief: Reset Servo Motor to 0 Degrees (Fixes compilation error)
 *******************************************************************************/
ErrorCode SERVOM_enumStop(void)
{
    ErrorCode Local_enumValidationStatus = VALID;

    /* إرجاع النبضة لـ 1000us لقفل القفل أو البوابة */
    TIMER_voidTimer1ASetCompareValue(SERVOM_u16_MIN_PULSE);

    return Local_enumValidationStatus;
}
