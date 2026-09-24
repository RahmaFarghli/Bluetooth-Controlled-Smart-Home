#include "../../LIBs/STD_TYPES.h"
#include "../../LIBs/BIT_MATH.h"
#include "../../MCALL/ADC/ADC_Interface.h"
#include "TEMP_SENSOR_Interface.h"
#include "TEMP_SENSOR_Private.h"
#include "TEMP_SENSOR_Config.h"


/*****************************************************************************************************************************
* Brief: Function Used to Read the Temperature from an LM35 Sensor Connected to the ADC and Convert it to Celsius            *
* Parameters: Pointer to a Variable that will Hold the Temperature Value in Celsius                                          *
* Return: ErrorCode                                                                                                          *
*****************************************************************************************************************************/
ErrorCode TEMP_SENSOR_enumReadTemp (u8 *Ptr_u8TempValue)
{
    ErrorCode Local_enumValidationStatus = VALID;
    u16 ADC_u16Result = 0;

    if(Ptr_u8TempValue != NULL)
    {
        /* BUG FIX: ADC_u16ReadAsynchronus()'s 2nd parameter is a callback
         * function pointer, not a result-storage pointer - this was a raw
         * type mismatch. Use the blocking synchronous read instead. */
        ADC_u16Result = ADC_u16ReadSynchronus(TEMP_SENSOR_ADC_CHANNEL);

        /* Vout(mV) = ADC * Vref / 1023   ,   Temp(C) = Vout(mV) / 10  ( LM35 => 10mV/°C ) */
        *Ptr_u8TempValue = (u8)(((u32)ADC_u16Result * TEMP_SENSOR_VREF_mV) / (TEMP_SENSOR_ADC_MAX * 10));
    }

    else
    {
        Local_enumValidationStatus = INVALID;
    }

    return Local_enumValidationStatus;
}
