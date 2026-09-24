#include "../../LIBs/STD_TYPES.h"
#include "../../LIBs/BIT_MATH.h"
#include "../../MCALL/DIO/DIO_Interface.h"
#include <util/delay.h>

#include "LCD_Config.h"
#include "LCD_Interface.h"
#include "LCD_Private.h"

/*****************************************************************************************************************************
* Brief: Function Used to Initalize an LCD                                                                                   *
* Parameters: Void                                                                                                           *
* Return: Void                                                                                                               *
*****************************************************************************************************************************/
void LCD_voidInit (void)
{
    DIO_SetPinMode(LCD_RS_PortID, LCD_RS_PinID, DIO_PIN_OUTPUT);
    DIO_SetPinMode(LCD_RW_PortID, LCD_RW_PinID, DIO_PIN_OUTPUT);
    DIO_SetPinMode(LCD_ENABLE_PortID, LCD_ENABLE_PinID, DIO_PIN_OUTPUT);

    #if(LCD_OPERATION_MODE == LCD_8BIT_MODE)
    {
        _delay_ms(50);

        DIO_SetPortMode(LCD_DATA_PortID, DIO_PORT_OUTPUT);

        LCD_voidSendCommand(LCD_Home);
        _delay_ms(1);

        LCD_voidSendCommand(LCD_FUNCTION_SET);
        _delay_ms(1);

        LCD_voidSendCommand(LCD_DisplayON_CursorOFF);
        _delay_ms(1);

        LCD_voidDisplayClear();

        LCD_voidSendCommand(LCD_EntryMode);
        _delay_ms(1);
    }

    #elif (LCD_OPERATION_MODE == LCD_4BIT_MODE)
    {
        DIO_SetPinMode(LCD_DATA_PortID, LCD_4BIT_PIN0, DIO_PIN_OUTPUT);
        DIO_SetPinMode(LCD_DATA_PortID, LCD_4BIT_PIN1, DIO_PIN_OUTPUT);
        DIO_SetPinMode(LCD_DATA_PortID, LCD_4BIT_PIN2, DIO_PIN_OUTPUT);
        DIO_SetPinMode(LCD_DATA_PortID, LCD_4BIT_PIN3, DIO_PIN_OUTPUT);

        _delay_ms(50);

        /* 1. إرسال أمر التحويل لـ 4-bit Mode أولاً */
        DIO_SetPinValue(LCD_RS_PortID, LCD_RS_PinID, DIO_PIN_LOW);
        DIO_SetPinValue(LCD_RW_PortID, LCD_RW_PinID, DIO_PIN_LOW);

        DIO_SetPinValue(LCD_DATA_PortID, LCD_4BIT_PIN0, 0);
        DIO_SetPinValue(LCD_DATA_PortID, LCD_4BIT_PIN1, 1);
        DIO_SetPinValue(LCD_DATA_PortID, LCD_4BIT_PIN2, 0);
        DIO_SetPinValue(LCD_DATA_PortID, LCD_4BIT_PIN3, 0);
        LCD_SendFallingEdge();
        _delay_ms(2);

        /* 2. إرسال باقي أوامر التهيئة */
        LCD_voidSendCommand(LCD_FUNCTION_SET);
        _delay_ms(1);

        LCD_voidSendCommand(LCD_DisplayON_CursorOFF);
        _delay_ms(1);

        LCD_voidDisplayClear();

        LCD_voidSendCommand(LCD_EntryMode);
        _delay_ms(1);
    }

    #else
    {
        #error INVALID LCD_OPERATION_MODE;
    }

    #endif /* LCD_OPERATION_MODE */
}

/*****************************************************************************************************************************
* Brief: Function Used to Send and Display a Char on LCD                                                                     *
* Parameters: Char want to Display                                                                                           *
* Return: Void                                                                                                               *
*****************************************************************************************************************************/
void LCD_voidSendData (u8 Copy_u8Data)
{
    #if(LCD_OPERATION_MODE == LCD_8BIT_MODE)
    {
        DIO_SetPinValue(LCD_RS_PortID, LCD_RS_PinID, DIO_PIN_HIGH);
        DIO_SetPinValue(LCD_RW_PortID, LCD_RW_PinID, DIO_PIN_LOW);

        DIO_SetPortValue(LCD_DATA_PortID, Copy_u8Data);
        LCD_SendFallingEdge();
    }

    #elif (LCD_OPERATION_MODE == LCD_4BIT_MODE)
    {
        DIO_SetPinValue(LCD_RS_PortID, LCD_RS_PinID, DIO_PIN_HIGH);
        DIO_SetPinValue(LCD_RW_PortID, LCD_RW_PinID, DIO_PIN_LOW);

        /* Send High Nibble */
        DIO_SetPinValue(LCD_DATA_PortID, LCD_4BIT_PIN0, GET_BIT(Copy_u8Data, 4));
        DIO_SetPinValue(LCD_DATA_PortID, LCD_4BIT_PIN1, GET_BIT(Copy_u8Data, 5));
        DIO_SetPinValue(LCD_DATA_PortID, LCD_4BIT_PIN2, GET_BIT(Copy_u8Data, 6));
        DIO_SetPinValue(LCD_DATA_PortID, LCD_4BIT_PIN3, GET_BIT(Copy_u8Data, 7));
        LCD_SendFallingEdge();

        /* Send Low Nibble */
        DIO_SetPinValue(LCD_DATA_PortID, LCD_4BIT_PIN0, GET_BIT(Copy_u8Data, 0));
        DIO_SetPinValue(LCD_DATA_PortID, LCD_4BIT_PIN1, GET_BIT(Copy_u8Data, 1));
        DIO_SetPinValue(LCD_DATA_PortID, LCD_4BIT_PIN2, GET_BIT(Copy_u8Data, 2));
        DIO_SetPinValue(LCD_DATA_PortID, LCD_4BIT_PIN3, GET_BIT(Copy_u8Data, 3));
        LCD_SendFallingEdge();
    }

    #else
    {
        #error INVALID LCD_OPERATION_MODE;
    }

    #endif /* LCD_OPERATION_MODE */

    _delay_ms(1);
}

/*****************************************************************************************************************************
* Brief: Function Used to Send Command to LCD                                                                                *
* Parameters: Command want to Perform                                                                                        *
* Return: Void                                                                                                               *
*****************************************************************************************************************************/
void LCD_voidSendCommand (u8 Copy_u8Command)
{
    #if(LCD_OPERATION_MODE == LCD_8BIT_MODE)
    {
        DIO_SetPinValue(LCD_RS_PortID, LCD_RS_PinID, DIO_PIN_LOW);
        DIO_SetPinValue(LCD_RW_PortID, LCD_RW_PinID, DIO_PIN_LOW);

        DIO_SetPortValue(LCD_DATA_PortID, Copy_u8Command);
        LCD_SendFallingEdge();
    }

    #elif (LCD_OPERATION_MODE == LCD_4BIT_MODE)
    {
        DIO_SetPinValue(LCD_RS_PortID, LCD_RS_PinID, DIO_PIN_LOW);
        DIO_SetPinValue(LCD_RW_PortID, LCD_RW_PinID, DIO_PIN_LOW);

        /* Send High Nibble */
        DIO_SetPinValue(LCD_DATA_PortID, LCD_4BIT_PIN0, GET_BIT(Copy_u8Command, 4));
        DIO_SetPinValue(LCD_DATA_PortID, LCD_4BIT_PIN1, GET_BIT(Copy_u8Command, 5));
        DIO_SetPinValue(LCD_DATA_PortID, LCD_4BIT_PIN2, GET_BIT(Copy_u8Command, 6));
        DIO_SetPinValue(LCD_DATA_PortID, LCD_4BIT_PIN3, GET_BIT(Copy_u8Command, 7));
        LCD_SendFallingEdge();

        /* Send Low Nibble */
        DIO_SetPinValue(LCD_DATA_PortID, LCD_4BIT_PIN0, GET_BIT(Copy_u8Command, 0));
        DIO_SetPinValue(LCD_DATA_PortID, LCD_4BIT_PIN1, GET_BIT(Copy_u8Command, 1));
        DIO_SetPinValue(LCD_DATA_PortID, LCD_4BIT_PIN2, GET_BIT(Copy_u8Command, 2));
        DIO_SetPinValue(LCD_DATA_PortID, LCD_4BIT_PIN3, GET_BIT(Copy_u8Command, 3));
        LCD_SendFallingEdge();
    }

    #else
    {
        #error INVALID LCD_OPERATION_MODE;
    }

    #endif /* LCD_OPERATION_MODE */

    _delay_ms(1);
}

/*****************************************************************************************************************************
* Brief: Function Used to Print and Display a String on LCD                                                                  *
* Parameters: String want to Display                                                                                         *
* Return: Void                                                                                                               *
*****************************************************************************************************************************/
void LCD_voidPrintString (const u8 Copy_u8Data[])
{
    u8 Local_u8Iterator = 0;
    while(Copy_u8Data[Local_u8Iterator] != '\0')
    {
        LCD_voidSendData(Copy_u8Data[Local_u8Iterator]);
        Local_u8Iterator++;
    }
}

/*****************************************************************************************************************************
* Brief: Function Used to Print and Display an Integer Number on LCD                                                         *
* Parameters: Integer Number want to Display                                                                                 *
* Return: Void                                                                                                               *
*****************************************************************************************************************************/
void LCD_voidPrintNumber (u64 Copy_u64Number)
{
    u64 Local_Reserved = 1;

    if(Copy_u64Number == 0)
    {
        LCD_voidSendData('0');
    }
    else
    {
        while(Copy_u64Number != 0)
        {
            Local_Reserved = ((Local_Reserved * 10) + (Copy_u64Number % 10));
            Copy_u64Number /= 10;
        }

        while(Local_Reserved != 1)
        {
            LCD_voidSendData(((Local_Reserved % 10) + 48));
            Local_Reserved /= 10;
        }
    }
}

/*****************************************************************************************************************************
* Brief: Function Used to Select a Position to start Display on LCD                                                          *
* Parameters:                                                                                                                *
*    [1]-Row        [ LCD_ROW1 -> LCD_ROW2  ]                                                                                *
*    [2]-Col        [ LCD_Col0 -> LCD_ROW15 ]                                                                                *
* Return: Void                                                                                                               *
*****************************************************************************************************************************/
void LCD_voidSetCursorPosition (u8 Copy_u8Row, u8 Copy_u8Col)
{
    u8 Local_u8CursorPosition = 0;
    if((Copy_u8Row > LCD_ROW2) || (Copy_u8Row < LCD_ROW1) || (Copy_u8Col > LCD_Col15) || (Copy_u8Col < LCD_Col0))
    {
        Local_u8CursorPosition = LCD_CursorPostion;
    }
    else if(Copy_u8Row == LCD_ROW1)
    {
        Local_u8CursorPosition = LCD_CursorPostion + Copy_u8Col;
    }
    else if(Copy_u8Row == LCD_ROW2)
    {
        Local_u8CursorPosition = LCD_CursorPostion + (64 + Copy_u8Col);
    }

    LCD_voidSendCommand(Local_u8CursorPosition);
    _delay_ms(1);
}

/*****************************************************************************************************************************
* Brief: Function Used to Clear LCD Display                                                                                  *
* Parameters: Void                                                                                                           *
* Return: Void                                                                                                               *
*****************************************************************************************************************************/
void LCD_voidDisplayClear (void)
{
    LCD_voidSendCommand(LCD_DisplayCLEAR);
    _delay_ms(10);
}

/*****************************************************************************************************************************
* Brief: Function Used to Enable LCD                                                                                         *
* Parameters: Void                                                                                                           *
* Return: Void                                                                                                               *
*****************************************************************************************************************************/
static void LCD_SendFallingEdge (void)
{
    DIO_SetPinValue(LCD_ENABLE_PortID, LCD_ENABLE_PinID, DIO_PIN_HIGH);
    _delay_ms(1);

    DIO_SetPinValue(LCD_ENABLE_PortID, LCD_ENABLE_PinID, DIO_PIN_LOW);
    _delay_ms(1);
}
