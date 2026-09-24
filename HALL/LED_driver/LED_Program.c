#include "../../LIBs/STD_TYPES.h"
#include "../../LIBs/BIT_MATH.h"
#include "../../MCALL/DIO/DIO_Interface.h"
#include"LED_Interface.h"
#include"LED_Private.h"
#include"LED_Config.h"


ErrorCode LED_enumInit(const LED_t *LEDConfig)
{
    ErrorCode Local_enumValidationStatus = VALID;

    if(LEDConfig != NULL)
    {
        if((LEDConfig->PortID <= DIO_PORTD) && (LEDConfig->PinID <= DIO_PIN7) && (LEDConfig->ActiveMode <= ACTIVE_HIGH))
        {
            DIO_SetPinMode(LEDConfig->PortID,LEDConfig->PinID,DIO_PIN_OUTPUT);
            //DIO_SetPinValue(LEDConfig->PortID,LEDConfig->PinID,~LEDConfig->ActiveMode);
        }

        else
        {
            Local_enumValidationStatus = INVALID;
        }
    }

    else
    {
        Local_enumValidationStatus = INVALID;
    }

    return Local_enumValidationStatus;
}



ErrorCode LED_enumPowerON(const LED_t *LEDConfig)
{
    ErrorCode Local_enumValidationStatus = VALID;

    if(LEDConfig != NULL)
    {
        if((LEDConfig->PortID <= DIO_PORTD) && (LEDConfig->PinID <= DIO_PIN7) && (LEDConfig->ActiveMode <= ACTIVE_HIGH))
        {
            DIO_SetPinValue(LEDConfig->PortID,LEDConfig->PinID,LEDConfig->ActiveMode);
        }

        else
        {
            Local_enumValidationStatus = INVALID;
        }
    }

    else
    {
        Local_enumValidationStatus = INVALID;
    }

    return Local_enumValidationStatus;
}



ErrorCode LED_enumPowerOFF(const LED_t *LEDConfig)
{
    ErrorCode Local_enumValidationStatus = VALID;

    if(LEDConfig != NULL)
    {
        if((LEDConfig->PortID <= DIO_PORTD) && (LEDConfig->PinID <= DIO_PIN7) && (LEDConfig->ActiveMode <= ACTIVE_HIGH))
        {
            DIO_SetPinValue(LEDConfig->PortID,LEDConfig->PinID,~LEDConfig->ActiveMode);
        }

        else
        {
            Local_enumValidationStatus = INVALID;
        }
    }

    else
    {
        Local_enumValidationStatus = INVALID;
    }

    return Local_enumValidationStatus;
}



ErrorCode LED_enumToggle(const LED_t *LEDConfig)
{
    ErrorCode Local_enumValidationStatus = VALID;

    if(LEDConfig != NULL)
    {
        if((LEDConfig->PortID <= DIO_PORTD) && (LEDConfig->PinID <= DIO_PIN7) && (LEDConfig->ActiveMode <= ACTIVE_HIGH))
        {
            DIO_TogglePinValue(LEDConfig->PortID,LEDConfig->PinID);
        }

        else
        {
            Local_enumValidationStatus = INVALID;
        }
    }

    else
    {
        Local_enumValidationStatus = INVALID;
    }

    return Local_enumValidationStatus;
}
