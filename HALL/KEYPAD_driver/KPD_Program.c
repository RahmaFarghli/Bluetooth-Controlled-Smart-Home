#include <util/delay.h>
#include "../../LIBs/STD_TYPES.h"
#include "../../LIBs/BIT_MATH.h"
#include "../../MCALL/DIO/DIO_Interface.h"

/* BUG FIX: removed local "#define F_CPU 8000000UL" - it shadowed the
 * project's real clock for the _delay_ms(20) debounce below, which matters
 * even more now that the keypad has to keep working correctly while the
 * main loop also polls Bluetooth every iteration. */

#include "KPD_Interface.h"
#include "KPD_Config.h"
#include "KPD_Private.h"

/* 1. Definitions MUST be before function implementations */
const u8 KPD_Au8KEYS[4][4] = KPD_KEYS;

static const u8 KPD_Au8RowsPins[4] = {
    KPD_u8_R1_PIN,
    KPD_u8_R2_PIN,
    KPD_u8_R3_PIN,
    KPD_u8_R4_PIN
};

static const u8 KPD_Au8ColsPins[4] = {
    KPD_u8_C1_PIN,
    KPD_u8_C2_PIN,
    KPD_u8_C3_PIN,
    KPD_u8_C4_PIN
};

/* 2. Keypad State Getting Function */
u8 KPD_u8GetKeyState(u8 *Copy_pu8RetuernKey)
{
    u8 Local_u8ErrorState = VALID;
    u8 Local_u8RowsCounter, Local_u8ColsCounter, Local_u8PinValue;

    if(Copy_pu8RetuernKey != NULL)
    {
        *Copy_pu8RetuernKey = KPD_u8_KEY_NOT_PRESSED;

        for (Local_u8RowsCounter = 0; Local_u8RowsCounter < 4; Local_u8RowsCounter++)
        {
            /* Activate current Row */
            DIO_SetPinValue(KPD_u8_PORT, KPD_Au8RowsPins[Local_u8RowsCounter], DIO_PIN_LOW);

            for (Local_u8ColsCounter = 0; Local_u8ColsCounter < 4; Local_u8ColsCounter++)
            {
                DIO_GetPinValue(KPD_u8_PORT, KPD_Au8ColsPins[Local_u8ColsCounter], &Local_u8PinValue);

                if(Local_u8PinValue == DIO_PIN_LOW)
                {
                    _delay_ms(20); /* Debouncing */
                    DIO_GetPinValue(KPD_u8_PORT, KPD_Au8ColsPins[Local_u8ColsCounter], &Local_u8PinValue);

                    if(Local_u8PinValue == DIO_PIN_LOW)
                    {
                        *Copy_pu8RetuernKey = KPD_Au8KEYS[Local_u8RowsCounter][Local_u8ColsCounter];

                        /* Deactivate current Row before leaving */
                        DIO_SetPinValue(KPD_u8_PORT, KPD_Au8RowsPins[Local_u8RowsCounter], DIO_PIN_HIGH);
                        return Local_u8ErrorState;
                    }
                }
            }

            /* Deactivate current Row */
            DIO_SetPinValue(KPD_u8_PORT, KPD_Au8RowsPins[Local_u8RowsCounter], DIO_PIN_HIGH);
        }
    }
    else
    {
        Local_u8ErrorState = INVALID;
    }
    return Local_u8ErrorState;
}

/* 3. Keypad Initialization Function */
void KEYPAD_Init(void)
{
    /* Configure Rows as OUTPUT and set HIGH */
    DIO_SetPinMode(KPD_u8_PORT, KPD_u8_R1_PIN, DIO_PIN_OUTPUT);
    DIO_SetPinValue(KPD_u8_PORT, KPD_u8_R1_PIN, DIO_PIN_HIGH);

    DIO_SetPinMode(KPD_u8_PORT, KPD_u8_R2_PIN, DIO_PIN_OUTPUT);
    DIO_SetPinValue(KPD_u8_PORT, KPD_u8_R2_PIN, DIO_PIN_HIGH);

    DIO_SetPinMode(KPD_u8_PORT, KPD_u8_R3_PIN, DIO_PIN_OUTPUT);
    DIO_SetPinValue(KPD_u8_PORT, KPD_u8_R3_PIN, DIO_PIN_HIGH);

    DIO_SetPinMode(KPD_u8_PORT, KPD_u8_R4_PIN, DIO_PIN_OUTPUT);
    DIO_SetPinValue(KPD_u8_PORT, KPD_u8_R4_PIN, DIO_PIN_HIGH);

    /* Configure Columns as INPUT and Enable Pull-up */
    DIO_SetPinMode(KPD_u8_PORT, KPD_u8_C1_PIN, DIO_PIN_INPUT);
    DIO_SetPinValue(KPD_u8_PORT, KPD_u8_C1_PIN, DIO_PIN_HIGH);

    DIO_SetPinMode(KPD_u8_PORT, KPD_u8_C2_PIN, DIO_PIN_INPUT);
    DIO_SetPinValue(KPD_u8_PORT, KPD_u8_C2_PIN, DIO_PIN_HIGH);

    DIO_SetPinMode(KPD_u8_PORT, KPD_u8_C3_PIN, DIO_PIN_INPUT);
    DIO_SetPinValue(KPD_u8_PORT, KPD_u8_C3_PIN, DIO_PIN_HIGH);

    DIO_SetPinMode(KPD_u8_PORT, KPD_u8_C4_PIN, DIO_PIN_INPUT);
    DIO_SetPinValue(KPD_u8_PORT, KPD_u8_C4_PIN, DIO_PIN_HIGH);
}
