#include "../../LIBs/STD_TYPES.h"
#include "../../LIBs/BIT_MATH.h"
#include "../../MCALL/DIO/DIO_Interface.h"
#include "MOTOR_Interface.h"
#include "MOTOR_Private.h"
#include "MOTOR_Config.h"


ErrorCode MOTOR_enumInit (const MOTOR_t *MotorConfig)
{
    ErrorCode Local_enumValidationStatus = VALID;

    if(MotorConfig != NULL)
    {
        DIO_SetPinMode(MotorConfig->PortID , MotorConfig->Pin1ID , DIO_PIN_OUTPUT);
        DIO_SetPinMode(MotorConfig->PortID , MotorConfig->Pin2ID , DIO_PIN_OUTPUT);

        DIO_SetPinValue(MotorConfig->PortID , MotorConfig->Pin1ID , DIO_PIN_LOW);
        DIO_SetPinValue(MotorConfig->PortID , MotorConfig->Pin2ID , DIO_PIN_LOW);
    }

    else
    {
        Local_enumValidationStatus = INVALID;
    }

    return Local_enumValidationStatus;
}





ErrorCode MOTOR_enumRotateRight (const MOTOR_t *MotorConfig)
{
    ErrorCode Local_enumValidationStatus = VALID;

    if(MotorConfig != NULL)
    {
        DIO_SetPinValue(MotorConfig->PortID , MotorConfig->Pin1ID , DIO_PIN_HIGH);
        DIO_SetPinValue(MotorConfig->PortID , MotorConfig->Pin2ID , DIO_PIN_LOW);
    }

    else
    {
        Local_enumValidationStatus = INVALID;
    }

    return Local_enumValidationStatus;
}





ErrorCode MOTOR_enumRotateLeft (const MOTOR_t *MotorConfig)
{
    ErrorCode Local_enumValidationStatus = VALID;

    if(MotorConfig != NULL)
    {
        DIO_SetPinValue(MotorConfig->PortID , MotorConfig->Pin1ID , DIO_PIN_LOW);
        DIO_SetPinValue(MotorConfig->PortID , MotorConfig->Pin2ID , DIO_PIN_HIGH);
    }

    else
    {
        Local_enumValidationStatus = INVALID;
    }

    return Local_enumValidationStatus;
}





ErrorCode MOTOR_enumStop (const MOTOR_t *MotorConfig)
{
    ErrorCode Local_enumValidationStatus = VALID;

    if(MotorConfig != NULL)
    {
        DIO_SetPinValue(MotorConfig->PortID , MotorConfig->Pin1ID , DIO_PIN_LOW);
        DIO_SetPinValue(MotorConfig->PortID , MotorConfig->Pin2ID , DIO_PIN_LOW);
    }

    else
    {
        Local_enumValidationStatus = INVALID;
    }

    return Local_enumValidationStatus;
}
