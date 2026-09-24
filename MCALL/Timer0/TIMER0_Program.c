#include "../../LIBs/STD_TYPES.h"
#include "../../LIBs/BIT_MATH.h"

#include "TIMER0_Config.h"
#include "TIMER0_Interface.h"
#include "TIMER0_Private.h"


static void (*TIMER_pvTimer0NotificationFunction)(void) = NULL;
static void (*TIMER_pvICUNotificationFunction)(void) = NULL;

void TIMER_voidTimer0Init(void)
{
    #if WAVEFORM_GEN_MODE == NORMAL_MODE

				CLEAR_BIT(TIM0_u8_TCCR0_REG,TIM0_u8_TCCR0_WGM00);
				CLEAR_BIT(TIM0_u8_TCCR0_REG,TIM0_u8_TCCR0_WGM01);

    #elif WAVEFORM_GEN_MODE == PWM_OR_PHASE_CORRECT_MODE
        SET_BIT(TIM0_u8_TCCR0_REG , TIM0_u8_TCCR0_WGM00);
        CLEAR_BIT(TIM0_u8_TCCR0_REG , TIM0_u8_TCCR0_WGM01);

    #elif WAVEFORM_GEN_MODE == CTC_MODE    
        CLEAR_BIT(TIM0_u8_TCCR0_REG , TIM0_u8_TCCR0_WGM00);
        SET_BIT(TIM0_u8_TCCR0_REG , TIM0_u8_TCCR0_WGM01);

        switch(OC0_ACTION)
        {
            case NON_PWM_NORML_PORT_OPERATION:                          CLEAR_BIT(TIM0_u8_TCCR0_REG , TIM0_u8_TCCR0_COM00);
                                                                        CLEAR_BIT(TIM0_u8_TCCR0_REG , TIM0_u8_TCCR0_COM01);
                                                                        break;

            case NON_PWM_TOGGLE_OC0_ON_COMPARE_MATCH:                   SET_BIT(TIM0_u8_TCCR0_REG , TIM0_u8_TCCR0_COM00);
                                                                        CLEAR_BIT(TIM0_u8_TCCR0_REG , TIM0_u8_TCCR0_COM01);
                                                                        break;

            case NON_PWM_CLEAR_OC0_ON_COMPARE_MATCH:                    CLEAR_BIT(TIM0_u8_TCCR0_REG , TIM0_u8_TCCR0_COM00);
                                                                        SET_BIT(TIM0_u8_TCCR0_REG , TIM0_u8_TCCR0_COM01);
                                                                        break;

            case NON_PWM_SET_OC0_ON_COMPARE_MATCH:                      SET_BIT(TIM0_u8_TCCR0_REG , TIM0_u8_TCCR0_COM00);
                                                                        SET_BIT(TIM0_u8_TCCR0_REG , TIM0_u8_TCCR0_COM01);
                                                                        break;

            default:                                                    break;
        }
    #elif WAVEFORM_GEN_MODE == FAST_PWM_MODE     
        SET_BIT(TIM0_u8_TCCR0_REG , TIM0_u8_TCCR0_WGM00);
        SET_BIT(TIM0_u8_TCCR0_REG , TIM0_u8_TCCR0_WGM01);

        switch(OC0_ACTION)
        {
            case FAST_PWM_NORMAL_PORT_OPERATION:                        CLEAR_BIT(TIM0_u8_TCCR0_REG , TIM0_u8_TCCR0_COM00);
                                                                        CLEAR_BIT(TIM0_u8_TCCR0_REG , TIM0_u8_TCCR0_COM01);
                                                                        break;

            case FAST_PWM_CLEAR_OC0_ON_COMPARE_MATCH_SET_OC0_AT_TOP:    CLEAR_BIT(TIM0_u8_TCCR0_REG , TIM0_u8_TCCR0_COM00);
                                                                        SET_BIT(TIM0_u8_TCCR0_REG , TIM0_u8_TCCR0_COM01);
                                                                        break;

            case FAST_PWM_SET_OC0_ON_COMPARE_MATCH_CLEAR_OC0_AT_TOP:    SET_BIT(TIM0_u8_TCCR0_REG , TIM0_u8_TCCR0_COM00);
                                                                        SET_BIT(TIM0_u8_TCCR0_REG , TIM0_u8_TCCR0_COM01);
                                                                        break;
            default:                                                    break;
        }
    #endif

    TIM0_u8_TCCR0_REG &= PRESCALER_MASK;
    TIM0_u8_TCCR0_REG |= PRESCALER_TYPES;
}



void TIMER_voidTimer1Init(void)
{
    /*Select The Normal Mode*/
	/* Fast PWM Mode 14 (TOP = ICR1) */
	CLEAR_BIT(TIM1_u8_TCCR1A_REG , TIM1_u8_TCCR1A_WGM10);
	SET_BIT  (TIM1_u8_TCCR1A_REG , TIM1_u8_TCCR1A_WGM11);
	SET_BIT  (TIM1_u8_TCCR1B_REG , TIM1_u8_TCCR1B_WGM12);
	SET_BIT  (TIM1_u8_TCCR1B_REG , TIM1_u8_TCCR1B_WGM13);

	/* Non-Inverting Mode on OC1A */
	CLEAR_BIT(TIM1_u8_TCCR1A_REG , TIM1_u8_TCCR1A_COM1A0);
	SET_BIT  (TIM1_u8_TCCR1A_REG , TIM1_u8_TCCR1A_COM1A1);

   /*Set Prescalar to be 8 */ 
   CLEAR_BIT(TIM1_u8_TCCR1B_REG , TIM1_u8_TCCR1B_CS10 );
   SET_BIT(TIM1_u8_TCCR1B_REG , TIM1_u8_TCCR1B_CS11 );
   CLEAR_BIT(TIM1_u8_TCCR1B_REG , TIM1_u8_TCCR1B_CS12 );
}



void TIMER_voidTimer0SetCompareValue(u8 Copy_u8CompareValue)
{
    TIME0_u8_OCR0_REG = Copy_u8CompareValue;
}




void TIMER_voidTimer0CallBack(void(*Copy_pvNotificationFunction)(void))
{
    TIMER_pvTimer0NotificationFunction = Copy_pvNotificationFunction;
}


void TIMER_voidTimer1ASetCompareValue(u16 Copy_u16CompareValue)
{
    TIME1_u16_OCR1A_REG = Copy_u16CompareValue ;
}



void TIMER_voidTimer1BSetCompareValue(u16 Copy_u16CompareValue)
{
    TIME1_u16_OCR1B_REG = Copy_u16CompareValue ;
}



void TIMER_voidTimer1SetInputCaptureValue(u16 Copy_u16CaptureValue)
{
    TIME1_u16_ICR1_REG = Copy_u16CaptureValue;
}




void ICU_voidInit(void)
{
    // SET trigger to rising edge
    SET_BIT(TIM1_u8_TCCR1B_REG , TIM1_u8_TCCR1B_ICES1);
    //Enable ICU Interrupt
    SET_BIT(TIM1_u8_TCCR1B_REG , TIM1_u8_TCCR1B_ICNC1);
}



void ICU_voidSetTrigger(u8 Copy_u8TriggerSource)
{
    switch(Copy_u8TriggerSource)
    {
        case RISING_EDGE:       SET_BIT(TIM1_u8_TCCR1B_REG , TIM1_u8_TCCR1B_ICES1);
                                break;

        case FALLING_EDGE:      CLEAR_BIT(TIM1_u8_TCCR1B_REG , TIM1_u8_TCCR1B_ICES1);
                                break;                        

        default:                break;
    }
}



u16 ICU_u16ReadInputCapture(void)
{
    return TIME1_u16_ICR1_REG;
}




void ICU_voidInterruptDisable(void)
{
    CLEAR_BIT(TIME0_u8_TIMSK_REG , TIME0_u8_TIMSK_TICIE1);
}




void ICU_voidSetCallBack(void(*Copy_pvNotificationFunction)(void))
{
    TIMER_pvICUNotificationFunction = Copy_pvNotificationFunction;
}






/****************************TIMER1 CAPT ISR************************************/
void __vector_6(void)    __attribute__((signal));
void __vector_6(void)
{
    if(TIMER_pvICUNotificationFunction != NULL)
    {
        TIMER_pvICUNotificationFunction();
    }
}



/****************************TIMER0 COMP ISR************************************/
void __vector_10(void)    __attribute__((signal));
void __vector_10(void)
{
    if(TIMER_pvTimer0NotificationFunction != NULL)
    {
        TIMER_pvTimer0NotificationFunction();
    }
}


/**************************** ISR************************************/
void __vector_11(void)    __attribute__((signal));
void __vector_11(void)
{
    if(TIMER_pvTimer0NotificationFunction != NULL)
    {
        TIMER_pvTimer0NotificationFunction();
    }
    else
    {

    }
}

