#include "../../../LIBs/STD_TYPES.h"
#include "../../../LIBs/BIT_MATH.h"
#include "../../../MCALL/DIO/DIO_Interface.h"
#include "BUZZER_Interface.h"
#include "BUZZER_Private.h"
#include "BUZZER_Config.h"


/*************************************************************************************************************************************** 
* Brief: Function Used to Configure a BUZZER [ PORT , PIN , ACTIVATION ]                                                               *
* Parameters:                                                                                                                          *
*   A Reference to an Object From [ BUZZER_t ] Defined Type:                                                                        *
*       [1]-PORT ID   [ BUZZER_PORTA , BUZZER_PORTB , BUZZER_PORTC , GPIO_PORTD ]                                                    *
*       [2]-PIN ID    [ BUZZER_PIN0 , BUZZER_PIN1 , BUZZER_PIN2 , BUZZER_PIN3 , BUZZER_PIN4 , BUZZER_PIN5 , BUZZER_PIN6 , GPIO_PIN7 ]*
*       [3]-BUZZER Activation [ ACTIVE_LOW or ACTIVE_HIGH ]                                                                            *
* Return: VALIDATION  [ VALID , INVALID ]                                                                                              *
***************************************************************************************************************************************/
ErrorCode BUZZER_enumInit (const BUZZER_t *BUZZER_CONFIG)
{
	ErrorCode Local_ValidationStatus = VALID;
    if(((BUZZER_CONFIG->PortID) <= DIO_PORTD) && ((BUZZER_CONFIG->PinID) <= DIO_PIN7) && ((BUZZER_CONFIG->Activation) <= ACTIVE_HIGH))
    {
        DIO_SetPinMode(BUZZER_CONFIG->PortID,BUZZER_CONFIG->PinID,DIO_PIN_OUTPUT);
        DIO_SetPinValue(BUZZER_CONFIG->PortID,BUZZER_CONFIG->PinID,~BUZZER_CONFIG->Activation);
    }

    else
    {
        Local_ValidationStatus = INVALID;
    }

    return Local_ValidationStatus;
}





/**************************************************************************************************************************************** 
* Brief: Function Used to Power ON a BUZZER Connected to Specific [ PORT , PIN , ACTIVATION ]                                           *
* Parameters:                                                                                                                           *
*   A Reference to an Object From [ BUZZER_t ] Defined Type:                                                                            *
*       [1]-PORT ID    [ BUZZER_PORTA , BUZZER_PORTB , BUZZER_PORTC , GPIO_PORTD ]                                                      *
*       [2]-PIN ID     [ BUZZER_PIN0 , BUZZER_PIN1 , BUZZER_PIN2 , BUZZER_PIN3 , BUZZER_PIN4 , BUZZER_PIN5 , BUZZER_PIN6 , GPIO_PIN7 ]  *
*       [3]-BUZZER Activation     [ ACTIVE_LOW or ACTIVE_HIGH ]                                                                         *
* Return: VALIDATION   [ VALID or INVALID ]                                                                                             *
****************************************************************************************************************************************/
ErrorCode BUZZER_enumPowerON (const BUZZER_t *BUZZER_CONFIG)
{
	ErrorCode Local_ValidationStatus = VALID;
    if(((BUZZER_CONFIG->PortID) <= DIO_PORTD) && ((BUZZER_CONFIG->PinID) <= DIO_PIN7) && ((BUZZER_CONFIG->Activation) <= ACTIVE_HIGH))
    {
        DIO_SetPinValue(BUZZER_CONFIG->PortID,BUZZER_CONFIG->PinID,BUZZER_CONFIG->Activation);
    }

    else
    {
        Local_ValidationStatus=INVALID;
    }

    return Local_ValidationStatus;
}





/**************************************************************************************************************************************** 
* Brief: Function Used to Power OFF a BUZZER Connected to Specific [ PORT , PIN , ACTIVATION ]                                          *
* Parameters:                                                                                                                           *
*   A Reference to an Object From [ BUZZER_t ] Defined Type:                                                                         *
*       [1]-PORT ID    [ BUZZER_PORTA , BUZZER_PORTB , BUZZER_PORTC , GPIO_PORTD ]                                                    *
*       [2]-PIN ID     [ BUZZER_PIN0 , BUZZER_PIN1 , BUZZER_PIN2 , BUZZER_PIN3 , BUZZER_PIN4 , BUZZER_PIN5 , BUZZER_PIN6 , GPIO_PIN7 ]*
*       [3]-BUZZER Activation     [ ACTIVE_LOW or ACTIVE_HIGH ]                                                                         *
* Return: VALIDATION   [ VALID or INVALID ]                                                                                             *
****************************************************************************************************************************************/
ErrorCode BUZZER_enumPowerOFF (const BUZZER_t *BUZZER_CONFIG)
{
	ErrorCode Local_ValidationStatus=VALID;
    if(((BUZZER_CONFIG->PortID) <= DIO_PORTD) && ((BUZZER_CONFIG->PinID) <= DIO_PIN7) && ((BUZZER_CONFIG->Activation) <= ACTIVE_HIGH))
    {
        DIO_SetPinValue(BUZZER_CONFIG->PortID,BUZZER_CONFIG->PinID,~BUZZER_CONFIG->Activation);
    }

    else
    {
        Local_ValidationStatus=INVALID;
    }

    return Local_ValidationStatus;
}





/*************************************************************************************************************************************** 
* Brief: Function Used to Toggle a BUZZER Connected to Specific [ PORT , PIN , ACTIVATION ]                                            *
* Parameters:                                                                                                                          *
*   A Reference to an Object From [ BUZZER_t ] Defined Type:                                                                        *
*       [1]-PORT ID   [ BUZZER_PORTA , BUZZER_PORTB , BUZZER_PORTC , GPIO_PORTD ]                                                    *
*       [2]-PIN ID    [ BUZZER_PIN0 , BUZZER_PIN1 , BUZZER_PIN2 , BUZZER_PIN3 , BUZZER_PIN4 , BUZZER_PIN5 , BUZZER_PIN6 , GPIO_PIN7 ]*
*       [3]-BUZZER Activation     [ ACTIVE_LOW or ACTIVE_HIGH ]                                                                        *
* Return: VALIDATION  [ VALID or INVALID ]                                                                                             *
***************************************************************************************************************************************/
ErrorCode BUZZER_enumToggle (const BUZZER_t *BUZZER_CONFIG)
{
	ErrorCode Local_ValidationStatus=VALID;
    if(((BUZZER_CONFIG->PortID) <= DIO_PORTD) && ((BUZZER_CONFIG->PinID) <= DIO_PIN7) && ((BUZZER_CONFIG->Activation) <= ACTIVE_HIGH))
    {
        DIO_TogglePinValue((BUZZER_CONFIG->PortID),(BUZZER_CONFIG->PinID));
    }

    else
    {
        Local_ValidationStatus=INVALID;
    }

    return Local_ValidationStatus;
}
