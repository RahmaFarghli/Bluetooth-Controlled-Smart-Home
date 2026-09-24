#include "../../LIBs/STD_TYPES.h"
#include "../../LIBs/BIT_MATH.h"
#include "../../MCALL/DIO/DIO_Interface.h"
#include"EXTI_Interface.h"
#include"EXTI_Private.h"
#include"EXTI_Config.h"

static void (*pf[3]) (void) = {NULL,NULL,NULL};




void EXTI_voidEXTIEnable(u8 Copy_u8EXTINo , u8 Copy_u8EXTISense)
{
    //Input Validation
    if((Copy_u8EXTINo > EXTI_u8_EXTI2) || (Copy_u8EXTISense > EXTI_u8_RISING_EDGE))
    {

    }
    else if((Copy_u8EXTINo == EXTI_u8_EXTI2) && (Copy_u8EXTISense < 2))
    {

    }
    else
    {
        switch(Copy_u8EXTINo)
        {
            case EXTI_u8_EXTI0: SET_BIT(EXTI_u8_GICR_REG , EXTI_u8_GICR_INT0);
                    switch(Copy_u8EXTISense)
                    {
                        case EXTI_u8_LOW_LEVEL:             CLEAR_BIT(EXTI_u8_MCUCR_REG , EXTI_u8_MCUCR_ISC00);
                                                            CLEAR_BIT(EXTI_u8_MCUCR_REG , EXTI_u8_MCUCR_ISC01);
                                                            break;
                        case EXTI_u8_ANY_LOGICAL_CHANGE:    SET_BIT(EXTI_u8_MCUCR_REG , EXTI_u8_MCUCR_ISC00);
                                                            CLEAR_BIT(EXTI_u8_MCUCR_REG , EXTI_u8_MCUCR_ISC01);
                                                            break;
                        case EXTI_u8_FALLING_EDGE:          CLEAR_BIT(EXTI_u8_MCUCR_REG , EXTI_u8_MCUCR_ISC00);
                                                            SET_BIT(EXTI_u8_MCUCR_REG , EXTI_u8_MCUCR_ISC01);
                                                            break;
                        case EXTI_u8_RISING_EDGE:           SET_BIT(EXTI_u8_MCUCR_REG , EXTI_u8_MCUCR_ISC00);
                                                            SET_BIT(EXTI_u8_MCUCR_REG , EXTI_u8_MCUCR_ISC01);
                                                            break;
                    }
                    break;
            case EXTI_u8_EXTI1: SET_BIT(EXTI_u8_GICR_REG , EXTI_u8_GICR_INT1);
                    switch(Copy_u8EXTISense)
                    {
                        case EXTI_u8_LOW_LEVEL:             CLEAR_BIT(EXTI_u8_MCUCR_REG , EXTI_u8_MCUCR_ISC10);
                                                            CLEAR_BIT(EXTI_u8_MCUCR_REG , EXTI_u8_MCUCR_ISC11);
                                                            break;
                        case EXTI_u8_ANY_LOGICAL_CHANGE:    SET_BIT(EXTI_u8_MCUCR_REG , EXTI_u8_MCUCR_ISC10);
                                                            CLEAR_BIT(EXTI_u8_MCUCR_REG , EXTI_u8_MCUCR_ISC11);
                                                            break;
                        case EXTI_u8_FALLING_EDGE:          CLEAR_BIT(EXTI_u8_MCUCR_REG , EXTI_u8_MCUCR_ISC10);
                                                            SET_BIT(EXTI_u8_MCUCR_REG , EXTI_u8_MCUCR_ISC11);
                                                            break;
                        case EXTI_u8_RISING_EDGE:           SET_BIT(EXTI_u8_MCUCR_REG , EXTI_u8_MCUCR_ISC10);
                                                            CLEAR_BIT(EXTI_u8_MCUCR_REG , EXTI_u8_MCUCR_ISC11);
                                                            break;
                    }
                    break;
            case EXTI_u8_EXTI2: SET_BIT(EXTI_u8_GICR_REG , EXTI_u8_GICR_INT2);
                    switch(Copy_u8EXTISense)
                    {

                        case EXTI_u8_FALLING_EDGE:          CLEAR_BIT(EXTI_u8_MCUCSR_REG , EXTI_u8_MCUCSR_ISC2);
                                                            break;
                        case EXTI_u8_RISING_EDGE:           SET_BIT(EXTI_u8_MCUCSR_REG , EXTI_u8_MCUCSR_ISC2);
                                                            break;
                    }        
 

        }
    }
}





void EXTI_voidEXTIDisable(u8 Copy_u8EXTINo)
{
    if(Copy_u8EXTINo > EXTI_u8_EXTI2)
    {

    }
    else
    {
        switch(Copy_u8EXTINo)
        {
            case EXTI_u8_EXTI0: CLEAR_BIT(EXTI_u8_GICR_REG , EXTI_u8_GICR_INT0);    break;
            case EXTI_u8_EXTI1: CLEAR_BIT(EXTI_u8_GICR_REG , EXTI_u8_GICR_INT1);    break;
            case EXTI_u8_EXTI2: CLEAR_BIT(EXTI_u8_GICR_REG , EXTI_u8_GICR_INT2);    break;
        }

    }
}





void EXTI_voidSetCallBack(void(*Copy_ptrToFunc)(void) , u8 Copy_u8EXTIIndex)
{
    pf[Copy_u8EXTIIndex] = Copy_ptrToFunc;
}

/****************************EXTI0 ISR************************************/

void __vector_1(void)    __attribute__((signal));
void __vector_1(void)
{
    if(pf[EXTI_u8_EXTI0] !=NULL)
    {
        pf[EXTI_u8_EXTI0]();
    }
}




/****************************EXTI1 ISR************************************/

void __vector_2(void)    __attribute__((signal));
void __vector_2(void)
{
    if(pf[EXTI_u8_EXTI1] !=NULL)
    {
        pf[EXTI_u8_EXTI1]();
    }
}





/****************************EXTI2 ISR************************************/

void __vector_3(void)    __attribute__((signal));
void __vector_3(void)
{
    if(pf[EXTI_u8_EXTI2] !=NULL)
    {
        pf[EXTI_u8_EXTI2]();
    }
}
