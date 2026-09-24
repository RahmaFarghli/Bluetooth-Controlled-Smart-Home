#include "../LIBs/STD_TYPES.h"

#include "../LIBs/BIT_MATH.h"

#include "../MCALL/DIO/DIO_Interface.h"

#include "../MCALL/TWI/TWI_Interface.h"

#include "../HALL/LCD_driver/LCD_Interface.h"

#include "../MCALL/UART/UART_Interface.h"

#include "../MCALL/ADC/ADC_Interface.h"

#include "../HALL/BUZZER/BUZZER/BUZZER_Interface.h"

#include "../HALL/SERVOM/SERVOM_Interface.h"

#include "../HALL/KEYPAD_driver/KPD_Interface.h"

#include "../HALL/EEPROM/EEPROM_Interface.h"

#include "HOME_Interface.h"

#include "HOME_Private.h"

#include "HOME_Config.h"

#include <util/delay.h>


static u8 Global_u8FanState = 0; /* 0: OFF, 1: ON */
static u8 Global_u8FanManualMode = 0; /* 0: AUTO, 1: MANUAL */

/* Room Status Variables */

static u8 Room1_State = 0;

static u8 Room2_State = 0;

/* Data Logging (Feature 2): RAM-cached copy of the EEPROM write index,
 * loaded once at boot by HOME_voidLogInit() so every HOME_voidLogEvent()
 * call only needs one EEPROM write instead of a read-then-write each time. */
static u8 Global_u8LogIndex = 0;

static BUZZER_t Home_Buzzer =

{

    .PortID = HOME_BUZZER_PORT,

    .PinID = HOME_BUZZER_PIN,

    .Activation = ACTIVE_HIGH

};

u8 Global_u8Password[PASSWORD_SIZE] = {'1','2','3','4'};

typedef enum {

    STATE_MAIN_MENU = 0,

    STATE_LEDS_MENU,

    STATE_ROOM1_CONTROL,

    STATE_ROOM2_CONTROL,

    STATE_FAN_CONTROL,

STATE_TEMP_DISPLAY

} MenuState_t;

static MenuState_t CurrentMenuState = STATE_MAIN_MENU;

/***************************************************************/
/* Data Logging (Feature 2): internal helpers                  */
/***************************************************************/

/* Description : Loads the log write-index from EEPROM into RAM. Call     */
/*               once at boot, before any HOME_voidLogEvent() call.       */
static void HOME_voidLogInit(void)
{
    Global_u8LogIndex = EEPROM_u8ReadDataByte(LOG_INDEX_ADDR);

    /* First boot / blank EEPROM (erased = 0xFF), or a corrupted index -
     * start the log over from the beginning instead of writing out of
     * the reserved LOG_BASE_ADDR..LOG_BASE_ADDR+LOG_CAPACITY-1 range. */
    if (Global_u8LogIndex >= LOG_CAPACITY)
    {
        Global_u8LogIndex = 0;
        EEPROM_voidSendDataByte(LOG_INDEX_ADDR, Global_u8LogIndex);
    }
}

/* Description : Appends one event byte to the EEPROM log (circular -    */
/*               wraps back to the start once LOG_CAPACITY is reached)   */
/*               and persists the new index so the log survives reset.  */
static void HOME_voidLogEvent(u8 Copy_u8EventCode)
{
    EEPROM_voidSendDataByte((u16)(LOG_BASE_ADDR + Global_u8LogIndex), Copy_u8EventCode);

    Global_u8LogIndex++;
    if (Global_u8LogIndex >= LOG_CAPACITY)
    {
        Global_u8LogIndex = 0; /* wrap - oldest entries get overwritten next */
    }

    EEPROM_voidSendDataByte(LOG_INDEX_ADDR, Global_u8LogIndex);
}

/* Description : Reads every stored event back out of EEPROM and prints  */
/*               it over UART, oldest first, so the log can be reviewed  */
/*               later from a serial terminal (see HOME_Interface.h).    */
void HOME_voidPrintEventLog(void)
{
    u8 Local_u8i;
    u8 Local_u8Event;

    UART_voidPrintf((u8*)"--- Event Log ---\r\n");

    for (Local_u8i = 0; Local_u8i < LOG_CAPACITY; Local_u8i++)
    {
        Local_u8Event = EEPROM_u8ReadDataByte((u16)(LOG_BASE_ADDR + Local_u8i));

        switch (Local_u8Event)
        {
            case LOG_EVT_ROOM1_OFF:  UART_voidPrintf((u8*)"Room 1 OFF\r\n");  break;
            case LOG_EVT_ROOM1_ON:   UART_voidPrintf((u8*)"Room 1 ON\r\n");   break;
            case LOG_EVT_ROOM2_OFF:  UART_voidPrintf((u8*)"Room 2 OFF\r\n");  break;
            case LOG_EVT_ROOM2_ON:   UART_voidPrintf((u8*)"Room 2 ON\r\n");   break;
            case LOG_EVT_FAN_OFF:    UART_voidPrintf((u8*)"Fan OFF\r\n");     break;
            case LOG_EVT_FAN_ON:     UART_voidPrintf((u8*)"Fan ON\r\n");      break;
            case LOG_EVT_DOOR_CLOSE: UART_voidPrintf((u8*)"Door CLOSE\r\n");  break;
            case LOG_EVT_DOOR_OPEN:  UART_voidPrintf((u8*)"Door OPEN\r\n");   break;
            case LOG_EVT_EMPTY:      /* fall through - blank slot, nothing logged here yet */
            default:                 break;
        }
    }

    UART_voidPrintf((u8*)"--- End of Log ---\r\n");
}


/* Displays the active menu depending on the current state */

void HOME_voidDisplayCurrentMenu(void)

{
    u8 Local_u8Temp = HOME_u8GetTemperature();

    LCD_voidDisplayClear();

    switch(CurrentMenuState)

    {

        case STATE_MAIN_MENU:

            LCD_voidSetCursorPosition(0, 0);

            LCD_voidPrintString((u8*)"1:LEDs 2:Fan");

            LCD_voidSetCursorPosition(1, 0);

            LCD_voidPrintString((u8*)"3:Temp 4:Log");

            break;

        case STATE_LEDS_MENU:

            LCD_voidSetCursorPosition(0, 0);

            LCD_voidPrintString((u8*)"1:Room1  2:Room2");

            LCD_voidSetCursorPosition(1, 0);

            LCD_voidPrintString((u8*)"0:Back");

            break;

        case STATE_ROOM1_CONTROL:

            LCD_voidSetCursorPosition(0, 0);

            LCD_voidPrintString((u8*)"Room1  1:ON 2:OFF");

            LCD_voidSetCursorPosition(1, 0);

            LCD_voidPrintString((u8*)"0:Back");

            break;

        case STATE_ROOM2_CONTROL:

            LCD_voidSetCursorPosition(0, 0);

            LCD_voidPrintString((u8*)"Room2  1:ON 2:OFF");

            LCD_voidSetCursorPosition(1, 0);

            LCD_voidPrintString((u8*)"0:Back");

            break;

        case STATE_FAN_CONTROL:

            LCD_voidSetCursorPosition(0, 0);

            LCD_voidPrintString((u8*)"Fan  1:ON 2:OFF");

            LCD_voidSetCursorPosition(1, 0);

            LCD_voidPrintString((u8*)"0:Back");

            break;

        case STATE_TEMP_DISPLAY:

            LCD_voidSetCursorPosition(0, 0);

            LCD_voidPrintString((u8*)"Temp: ");

            LCD_voidPrintNumber(Local_u8Temp);

            LCD_voidSendData('C');

            LCD_voidSetCursorPosition(1, 0);

            LCD_voidPrintString((u8*)"0:Back");

            break;

        default:

            break;

    }

}

/* Processes keypad choices and moves between sub-menus */

void HOME_voidMenuTask(void)

{

    u8 Local_u8Key = 255;



    KPD_u8GetKeyState(&Local_u8Key);



    if(Local_u8Key != 255 && Local_u8Key != 0)

    {

        /* Common action: Key '0' always goes back to main menu */

        if(Local_u8Key == '0')

        {

            CurrentMenuState = STATE_MAIN_MENU;

            HOME_voidDisplayCurrentMenu();

        }

        else

        {

            switch(CurrentMenuState)

            {

                case STATE_MAIN_MENU:

                    if(Local_u8Key == '1')       /* 1: LEDs Menu */

                    {

                        CurrentMenuState = STATE_LEDS_MENU;

                        HOME_voidDisplayCurrentMenu();

                    }

                    else if(Local_u8Key == '2')  /* 2: Fan Menu */

                    {

                        CurrentMenuState = STATE_FAN_CONTROL;

                        HOME_voidDisplayCurrentMenu();

                    }

                    else if(Local_u8Key == '3')  /* 3: Temp Display */

                    {

                        CurrentMenuState = STATE_TEMP_DISPLAY;

                        HOME_voidDisplayCurrentMenu();

                    }

                    else if(Local_u8Key == '4')  /* 4: Dump Event Log over UART */

                    {

                        HOME_voidPrintEventLog(); /* Data Logging (Feature 2) */

                        LCD_voidDisplayClear();

                        LCD_voidSetCursorPosition(0, 0);

                        LCD_voidPrintString((u8*)"Log sent via");

                        LCD_voidSetCursorPosition(1, 0);

                        LCD_voidPrintString((u8*)"Bluetooth/Serial");

                        _delay_ms(200);

                        HOME_voidDisplayCurrentMenu();

                    }

                    break;



                case STATE_LEDS_MENU:

                    if(Local_u8Key == '1')       /* 1: Room1 */

                    {

                        CurrentMenuState = STATE_ROOM1_CONTROL;

                        HOME_voidDisplayCurrentMenu();

                    }

                    else if(Local_u8Key == '2')  /* 2: Room2 */

                    {

                        CurrentMenuState = STATE_ROOM2_CONTROL;

                        HOME_voidDisplayCurrentMenu();

                    }

                    break;



                case STATE_ROOM1_CONTROL:

                    if(Local_u8Key == '1')       /* 1: ON */

                    {

                        HOME_voidRoomOn(1);

                    }

                    else if(Local_u8Key == '2')  /* 2: OFF */

                    {

                        HOME_voidRoomOff(1);

                    }

                    break;



                case STATE_ROOM2_CONTROL:

                    if(Local_u8Key == '1')       /* 1: ON */

                    {

                        HOME_voidRoomOn(2);

                    }

                    else if(Local_u8Key == '2')  /* 2: OFF */

                    {

                        HOME_voidRoomOff(2);

                    }

                    break;



                case STATE_FAN_CONTROL:          /* Fan control in sub-menu */
                    if(Local_u8Key == '1')       /* 1: ON */
                    {
                        Global_u8FanManualMode = 1; /* Turn ON Manual Mode so Auto control stops overriding */
                        HOME_voidFanOn();
                    }
                    else if(Local_u8Key == '2')  /* 2: OFF */
                    {
                        Global_u8FanManualMode = 1; /* Keep OFF Manually */
                        HOME_voidFanOff();
                    }
                    break;

                case STATE_TEMP_DISPLAY:

                    /* No choices inside Temp screen except '0' to go back */

                    break;
            }

        }
        /* Wait until key release */

        u8 Local_u8KeyCheck = Local_u8Key;

        while(Local_u8KeyCheck != 255 && Local_u8KeyCheck != 0)

        {

            KPD_u8GetKeyState(&Local_u8KeyCheck);

        }

        _delay_ms(100); /* Debouncing */

    }

}

/******************************************************************/
/* Description : Initialize Home Automation System                */
/******************************************************************/
void HOME_voidInit(void)

{

/* LEDs Direction Configuration */

    DIO_SetPinMode(GREEN_LED_PORT, GREEN_LED_PIN, DIO_PIN_OUTPUT);

    DIO_SetPinMode(RED_LED_PORT, RED_LED_PIN, DIO_PIN_OUTPUT);

    DIO_SetPinMode(HOME_BUZZER_PORT , HOME_BUZZER_PIN, DIO_PIN_OUTPUT);

    /* Initial State: OFF */

    DIO_SetPinValue(GREEN_LED_PORT, GREEN_LED_PIN, DIO_PIN_LOW);

    DIO_SetPinValue(RED_LED_PORT, RED_LED_PIN, DIO_PIN_LOW);

    DIO_SetPinValue(HOME_BUZZER_PORT , HOME_BUZZER_PIN, DIO_PIN_LOW);

    /* Initialize LCD */
    LCD_voidInit();

    /* Initialize SERVO */

  //  SERVOM_enumInit();
  //  SERVOM_enumSetAngle(0);

    /* Initialize Room LEDs */

    HOME_voidRoomsInit();

    /* Initialize Fan */
    HOME_voidFanInit();

    /* Initialize Buzzer */
    BUZZER_enumInit(&Home_Buzzer);

    /* Initialize KPD */

    KEYPAD_Init();

    /* Initialize feedback system */

    HOME_voidFeedbackInit();

    /* 2. Set LM35 Pin (PA0) Direction as INPUT */

      DIO_SetPinMode(DIO_PORTA, DIO_PIN0, DIO_PIN_INPUT);

    /* Initialize ADC for Temperature Sensor */

      HOME_voidTempSensorInit();

      /* Initialize Manual Push Buttons */

      HOME_voidButtonsInit();

      /* Data Logging (Feature 2): TWI/I2C was never initialized anywhere
       * in this project - without this, every EEPROM transaction fails. */

      TWI_voidInitMaster(0x00);

      /* Data Logging (Feature 2): load the EEPROM log write-index into RAM */

      HOME_voidLogInit();
}

/******************************************************************/

/* Description : Display Welcome Screen on LCD                    */

/******************************************************************/
void HOME_voidWelcomeScreen(void)

{

    LCD_voidDisplayClear();

    LCD_voidSetCursorPosition(0,0);

    LCD_voidPrintString((u8*)"Home");

    LCD_voidSetCursorPosition(1,0);

    LCD_voidPrintString((u8*)"Automation");

    _delay_ms(200);

    LCD_voidDisplayClear();

}

/******************************************************************/

/* Description : Initialize All Room LEDs                         */

/******************************************************************/

void HOME_voidRoomsInit(void)

{

    DIO_SetPinMode(ROOM1_PORT, ROOM1_PIN, DIO_PIN_OUTPUT);

    DIO_SetPinMode(ROOM2_PORT, ROOM2_PIN, DIO_PIN_OUTPUT);

    DIO_SetPinValue(ROOM1_PORT, ROOM1_PIN, DIO_PIN_LOW);

    DIO_SetPinValue(ROOM2_PORT, ROOM2_PIN, DIO_PIN_LOW);

    Room1_State = 0;
    Room2_State = 0;
}

/******************************************************************/

/* Description : Turn Selected Room LED ON                        */

/* Input       : Room ID (1 ~ 2)                                  */

/******************************************************************/

void HOME_voidRoomOn(u8 Copy_u8RoomID)
{
    switch(Copy_u8RoomID)
    {
        case 1:
            DIO_SetPinValue(ROOM1_PORT, ROOM1_PIN, DIO_PIN_HIGH);
            Room1_State = 1; /* تحديث حالة الغرفة */
            UART_voidPrintf((u8*)"LOG: Room 1 Light ON\r\n");
            HOME_voidLogEvent(LOG_EVT_ROOM1_ON);
            break;

        case 2:
            DIO_SetPinValue(ROOM2_PORT, ROOM2_PIN, DIO_PIN_HIGH);
            Room2_State = 1; /* تحديث حالة الغرفة */
            UART_voidPrintf((u8*)"LOG: Room 2 Light ON\r\n");
            HOME_voidLogEvent(LOG_EVT_ROOM2_ON);
            break;
    }
}

/******************************************************************/

/* Description : Turn Selected Room LED OFF                       */

/* Input       : Room ID (1 ~ 2)                                  */

/******************************************************************/

void HOME_voidRoomOff(u8 Copy_u8RoomID)
{
    switch(Copy_u8RoomID)
    {
        case 1:
            DIO_SetPinValue(ROOM1_PORT, ROOM1_PIN, DIO_PIN_LOW);
            Room1_State = 0; /* تحديث حالة الغرفة */
            HOME_voidLogEvent(LOG_EVT_ROOM1_OFF);
            break;

        case 2:
            DIO_SetPinValue(ROOM2_PORT, ROOM2_PIN, DIO_PIN_LOW);
            Room2_State = 0; /* تحديث حالة الغرفة */
            HOME_voidLogEvent(LOG_EVT_ROOM2_OFF);
            break;

        default: break;
    }
}

/***************************************************************/

/* Description : Receive Keypad Commands & Control Peripherals  */

/***************************************************************/

void HOME_voidControlTask(void)
{
    u8 Local_u8Data = 255;

    /* فحص البلوتوث فقط لمنع التداخل مع القوائم */
    if(UART_u8IsDataAvailable())
    {
        Local_u8Data = UART_ReciveData();
    }

    if(Local_u8Data != 255 && Local_u8Data != 0)
    {
        switch(Local_u8Data)
        {
            case '1': HOME_voidRoomOn(1);  break; /* Room 1 ON  */
            case '2': HOME_voidRoomOff(1); break; /* Room 1 OFF */
            case '3': HOME_voidRoomOn(2);  break; /* Room 2 ON  */
            case '4': HOME_voidRoomOff(2); break; /* Room 2 OFF */
            case '5': HOME_voidFanOn();     break; /* Fan ON     */
            case '6': HOME_voidFanOff();    break; /* Fan OFF    */
            case '7': HOME_voidCloseDoor(); break; /* Close Door */
            default: break;
        }
    }
}
/***************************************************************/

/* Description : Initialize Feedback LEDs                       */

/***************************************************************/

void HOME_voidFeedbackInit(void)

{

    /* Configure Green LED Pin as OUTPUT */

    DIO_SetPinMode(GREEN_LED_PORT, GREEN_LED_PIN, DIO_PIN_OUTPUT);

    /* Configure Red LED Pin as OUTPUT */

    DIO_SetPinMode(RED_LED_PORT, RED_LED_PIN, DIO_PIN_OUTPUT);

    /* Ensure initial state is LOW (OFF) */

    DIO_SetPinValue(GREEN_LED_PORT, GREEN_LED_PIN, DIO_PIN_LOW);

    DIO_SetPinValue(RED_LED_PORT, RED_LED_PIN, DIO_PIN_LOW);

}

/***************************************************************/

/* Description : Success Operation Feedback                    */

/***************************************************************/

void HOME_voidSuccessFeedback(void)

{

    /* Turn ON Green LED and Buzzer */

    DIO_SetPinValue(GREEN_LED_PORT, GREEN_LED_PIN, DIO_PIN_HIGH);

    _delay_ms(300);

    /* Turn OFF Green LED and Buzzer */



    DIO_SetPinValue(GREEN_LED_PORT, GREEN_LED_PIN, DIO_PIN_LOW);

}

/***************************************************************/

/* Description : Error Operation Feedback (Red LED)             */

/***************************************************************/

void HOME_voidErrorFeedback(void)

{

    /* 1. Turn ON Red LED */

    DIO_SetPinValue(RED_LED_PORT, RED_LED_PIN, DIO_PIN_HIGH);


    /* 3. Wait for 1 second so it is clearly visible */

    _delay_ms(200);

    DIO_SetPinValue(RED_LED_PORT, RED_LED_PIN, DIO_PIN_LOW);

}

/***************************************************************/

/* Description : Check User Password                            */

/* Return      : 1 Correct Password                             */

/*               0 Wrong Password                              */

/***************************************************************/

u8 HOME_u8CheckPassword(void)

{

    u8 Local_u8Key = 255;

    u8 Local_u8Password[PASSWORD_SIZE];

    u8 Local_u8Counter;



    for(Local_u8Counter = 0; Local_u8Counter < PASSWORD_SIZE; Local_u8Counter++)

    {

        /* Wait for Key Press */

        do

        {

            KPD_u8GetKeyState(&Local_u8Key);

        } while(Local_u8Key == 255 || Local_u8Key == 0);



        /* Save Key and display mask */

        Local_u8Password[Local_u8Counter] = Local_u8Key;

        LCD_voidSendData('*');



        /* Wait for Key Release */

        do

        {

            KPD_u8GetKeyState(&Local_u8Key);

        } while(Local_u8Key != 255 && Local_u8Key != 0);



        _delay_ms(100); /* Debouncing */

    }

    /* Check Password Match */

    for(Local_u8Counter = 0; Local_u8Counter < PASSWORD_SIZE; Local_u8Counter++)

    {

        if(Local_u8Password[Local_u8Counter] != Global_u8Password[Local_u8Counter])

        {

            return 0; /* Wrong Password */

        }

    }

    SERVOM_enumInit();
    SERVOM_enumSetAngle(0);

    HOME_voidOpenDoor();
    return 1; /* Correct Password */

}

/***************************************************************/

/* Description : Check Password and Control Door                */

/*               Open door if password is correct               */

/*               and activate feedback system                   */

/***************************************************************/

void HOME_voidDoorControl(void)

{

    u8 Local_u8PasswordStatus;

    static u8 Local_u8TrialsCount = 0; /* Counter for failed password attempts */

    /* Display password request prompt */

    LCD_voidDisplayClear();

    LCD_voidPrintString((u8*)"Password:");

    /* Check entered password */

    Local_u8PasswordStatus = HOME_u8CheckPassword();

    if(Local_u8PasswordStatus == 1)

    {

        /* Reset failed attempts counter on success */

        Local_u8TrialsCount = 0;

        /* Trigger success feedback (Green LED) */

        HOME_voidSuccessFeedback();

        /* Trigger short buzzer beep for door unlock success */

        HOME_voidBuzzerOn();

        _delay_ms(100);

        HOME_voidBuzzerOff();

        /* Open door using servo motor */

        HOME_voidOpenDoor();

        /* Display door opened message */

        LCD_voidDisplayClear();

        LCD_voidPrintString((u8*)"Door Open");

        _delay_ms(200);

    }

    else

    {

        /* Increment failed attempts counter */

        Local_u8TrialsCount++;

        /* Trigger error feedback (Red LED) */

        HOME_voidErrorFeedback();

        /* Trigger buzzer beep for wrong password */

        HOME_voidBuzzerOn();

        _delay_ms(300);

        HOME_voidBuzzerOff();

        /* Display wrong password message */

        LCD_voidDisplayClear();

        LCD_voidPrintString((u8*)"Wrong Pass");

        _delay_ms(100);

        /* Check if system should lock completely */

        if(Local_u8TrialsCount >= 3)

        {

            LCD_voidDisplayClear();

            LCD_voidPrintString((u8*)"System Locked!");


            /* Continuous alarm output */

            HOME_voidBuzzerOn();

            DIO_SetPinValue(RED_LED_PORT, RED_LED_PIN, DIO_PIN_HIGH);


            /* Freeze system completely until microcontroller reset */

            while(1);

        }

        /* Retry password prompt for attempts less than 3 */

        HOME_voidDoorControl();

        /* Clear any remaining key press from password entry */

        u8 Local_u8FlushKey = 255;

        while(Local_u8FlushKey != 255 && Local_u8FlushKey != 0)

        {

            KPD_u8GetKeyState(&Local_u8FlushKey);

        }

        _delay_ms(200); /* Delay to ensure user released key */

    }

}

/***************************************************************/

/* Description : Start Home Automation System                  */

/*               Check password before accessing the system    */

/***************************************************************/

void HOME_voidSystemStart(void)
{
    /* 1. Welcome Screen */
    HOME_voidWelcomeScreen();

    /* 2. Check Password */
    HOME_voidDoorControl();

    /* 3. تفريغ أي ضغطات زوائد من الكيباد بعد إدخال الباسورد */
    u8 Local_u8KeyCheck = 255;
    do {
        KPD_u8GetKeyState(&Local_u8KeyCheck);
    } while(Local_u8KeyCheck != 255 && Local_u8KeyCheck != 0);

    /* تأخير زمني لضمان رفع اليد عن الكيباد تماماً */
    _delay_ms(500);


    /* 4. Display Main Menu */
    CurrentMenuState = STATE_MAIN_MENU;
    HOME_voidDisplayCurrentMenu();

    while(UART_u8IsDataAvailable())
        {
            (void)UART_ReciveData();
        }

    /* 5. Main Loop */
    while(1)
    {
        /* Automatic Temperature Monitoring & Fan Control */
        HOME_voidAutoFanControlTask();

        /* Menu Handling via Keypad (Sub-menus) */
        HOME_voidMenuTask();

        /* Quick Control Shortcuts via Bluetooth/Serial only */
        HOME_voidControlTask();

        /* Manual Push Buttons for Rooms */
        HOME_voidButtonsTask();
    }
}

/******************************************************************/

/* Description : Initialize Fan Pin                               */
/******************************************************************/

void HOME_voidFanInit(void)

{

    DIO_SetPinMode(HOME_FAN_PORT, HOME_FAN_PIN, DIO_PIN_OUTPUT);

    DIO_SetPinValue(HOME_FAN_PORT, HOME_FAN_PIN, DIO_PIN_LOW);
}

/******************************************************************/
/* Description : Turn Fan ON                                      */
/******************************************************************/
void HOME_voidFanOn(void)
{
    if(Global_u8FanState == 0) /* only log a genuine OFF->ON transition */
    {
        HOME_voidLogEvent(LOG_EVT_FAN_ON); /* Data Logging (Feature 2) */
    }
    Global_u8FanState = 1;
    DIO_SetPinValue(HOME_FAN_PORT, HOME_FAN_PIN, DIO_PIN_HIGH);
}

/******************************************************************/
/* Description : Turn Fan OFF                                     */
/******************************************************************/
void HOME_voidFanOff(void)
{
    if(Global_u8FanState == 1) /* only log a genuine ON->OFF transition */
    {
        HOME_voidLogEvent(LOG_EVT_FAN_OFF); /* Data Logging (Feature 2) */
    }
    Global_u8FanState = 0;
    DIO_SetPinValue(HOME_FAN_PORT, HOME_FAN_PIN, DIO_PIN_LOW);
}

/******************************************************************/
/* Description : Initialize Buzzer Pin                            */

/******************************************************************/

void HOME_voidBuzzerInit(void)


{


    DIO_SetPinMode(HOME_BUZZER_PORT, HOME_BUZZER_PIN, DIO_PIN_OUTPUT);


    DIO_SetPinValue(HOME_BUZZER_PORT, HOME_BUZZER_PIN, DIO_PIN_LOW);


}

/***************************************************************/

/* Description : Turn Buzzer ON                                */

/***************************************************************/

void HOME_voidBuzzerOn(void)

{

    /* Turn ON buzzer pin */

    DIO_SetPinValue(HOME_BUZZER_PORT, HOME_BUZZER_PIN, DIO_PIN_HIGH);

}

/***************************************************************/

/* Description : Turn Buzzer OFF                               */

/***************************************************************/

void HOME_voidBuzzerOff(void)

{

    /* Turn OFF buzzer pin */

    DIO_SetPinValue(HOME_BUZZER_PORT, HOME_BUZZER_PIN, DIO_PIN_LOW);

}


/******************************************************************/

/* Description : Initialize Temperature Sensor (ADC)              */

/******************************************************************/

void HOME_voidTempSensorInit(void)

{
    /* Initialize ADC driver */

    ADC_voidInit();

}

/******************************************************************/

/* Description : Read Temperature Value in Celsius from LM35      */

/* Return      : Temperature value (0 - 150 C)                   */

/******************************************************************/

u8 HOME_u8GetTemperature(void)

{
    u16 Local_u16ADCRead = 0;

    u8  Local_u8Temp = 0;

    /* Read digital value from ADC channel connected to LM35 */

    Local_u16ADCRead = ADC_u16ReadSynchronus(HOME_LM35_ADC_CHANNEL);


    /* Convert ADC Digital Reading to Temperature in Celsius

     * Vref = 5V = 5000mV, Resolution = 1024 (10-bit)

     * Step Size = 5000 / 1024 = 4.88 mV

     * LM35 Scale = 10mV / 1 C

     * Temp = (ADC * 500) / 1024

     */

    Local_u8Temp = (u8)(((u32)Local_u16ADCRead * 500) / 1024);

    return Local_u8Temp;
}

/******************************************************************/

/* Description : Automatic Fan Control based on Temperature       */

/******************************************************************/

void HOME_voidAutoFanControlTask(void)
{
    static u8 Local_u8LastTemp = 255;
    u8 Local_u8CurrentTemp = HOME_u8GetTemperature();

    /* 1. Emergency High Temperature Override:
     * If temperature reaches or exceeds threshold, force Fan ON and clear Manual Mode
     */
    if(Local_u8CurrentTemp >= FAN_ON_TEMP_THRESHOLD)
    {
        Global_u8FanManualMode = 0; /* Clear manual state so auto control takes over */
        HOME_voidFanOn();
    }
    else
    {
        /* 2. Normal Temp (< 30 C):
         * Only turn OFF if user hasn't manually forced it ON
         */
        if(Global_u8FanManualMode == 0)
        {
            HOME_voidFanOff();
        }
    }

    /* Update screen if temperature changes AND user is currently on the Temp screen */
    if((CurrentMenuState == STATE_TEMP_DISPLAY) && (Local_u8CurrentTemp != Local_u8LastTemp))
    {
        Local_u8LastTemp = Local_u8CurrentTemp;
        HOME_voidDisplayCurrentMenu();
    }
}

/***************************************************************/

/* Description : Initialize Manual Push Buttons                */

/***************************************************************/

void HOME_voidButtonsInit(void)

{

    DIO_SetPinMode(PB_ROOM1_PORT, PB_ROOM1_PIN, DIO_PIN_INPUT);

    DIO_SetPinMode(PB_ROOM2_PORT, PB_ROOM2_PIN, DIO_PIN_INPUT);



    /* Enable Internal Pull-up */

    DIO_SetPinValue(PB_ROOM1_PORT, PB_ROOM1_PIN, DIO_PIN_HIGH);

    DIO_SetPinValue(PB_ROOM2_PORT, PB_ROOM2_PIN, DIO_PIN_HIGH);

}

/***************************************************************/
/* Description : Momentary Manual Room Control                 */
/***************************************************************/
//void HOME_voidButtonsTask(void)
//
//{
//    u8 Local_u8ButtonState;
//
//    /*--------------- Room 1 ----------------*/
//    DIO_GetPinValue(PB_ROOM1_PORT, PB_ROOM1_PIN, &Local_u8ButtonState);
//
//    if(Local_u8ButtonState == DIO_PIN_LOW)
//    {
//        /* Button Pressed */
//        DIO_SetPinValue(ROOM1_PORT, ROOM1_PIN, DIO_PIN_HIGH);
//    }
//    else
//    {
//        /* Button Released */
//        DIO_SetPinValue(ROOM1_PORT, ROOM1_PIN, DIO_PIN_LOW);
//    }
//
//
//    /*--------------- Room 2 ----------------*/
//    DIO_GetPinValue(PB_ROOM2_PORT, PB_ROOM2_PIN, &Local_u8ButtonState);
//
//    if(Local_u8ButtonState == DIO_PIN_LOW)
//    {
//        /* Button Pressed */
//        DIO_SetPinValue(ROOM2_PORT, ROOM2_PIN, DIO_PIN_HIGH);
//    }
//    else
//    {
//        /* Button Released */
//        DIO_SetPinValue(ROOM2_PORT, ROOM2_PIN, DIO_PIN_LOW);
//    }
//}
void HOME_voidButtonsTask(void)
{
    static u8 Local_u8FirstRun = 1;
    static u8 Local_u8LastButton1 = DIO_PIN_HIGH;
    static u8 Local_u8LastButton2 = DIO_PIN_HIGH;

    u8 Local_u8ButtonState;

    /* في أول لفة فقط: تزامن القراءة الحقيقية للأزرار دون تنفيذ أي تغيير */
    if (Local_u8FirstRun == 1)
    {
        DIO_GetPinValue(PB_ROOM1_PORT, PB_ROOM1_PIN, &Local_u8LastButton1);
        DIO_GetPinValue(PB_ROOM2_PORT, PB_ROOM2_PIN, &Local_u8LastButton2);
        Local_u8FirstRun = 0;
        return;
    }

    /*--------------- Room 1 Button ----------------*/
    DIO_GetPinValue(PB_ROOM1_PORT, PB_ROOM1_PIN, &Local_u8ButtonState);

    if(Local_u8ButtonState == DIO_PIN_LOW &&
       Local_u8LastButton1 == DIO_PIN_HIGH)
    {
        if(Room1_State == 0)
        {
            HOME_voidRoomOn(1);
        }
        else
        {
            HOME_voidRoomOff(1);
        }

        _delay_ms(200);
    }

    Local_u8LastButton1 = Local_u8ButtonState;


    /*--------------- Room 2 Button ----------------*/
    DIO_GetPinValue(PB_ROOM2_PORT, PB_ROOM2_PIN, &Local_u8ButtonState);

    if(Local_u8ButtonState == DIO_PIN_LOW &&
       Local_u8LastButton2 == DIO_PIN_HIGH)
    {
        if(Room2_State == 0)
        {
            HOME_voidRoomOn(2);
        }
        else
        {
            HOME_voidRoomOff(2);
        }

        _delay_ms(200);
    }

    Local_u8LastButton2 = Local_u8ButtonState;
}
/*******************************************************************************
 * Brief: Function to Open the Door (Rotates Servo to 90 Degrees)
 * Parameters: Void
 * Return: Void
 *******************************************************************************/
void HOME_voidOpenDoor(void)
{
    /* Set Servo Angle to 90 degrees to unlock/open door */
	SERVOM_enumSetAngle(90);
	HOME_voidLogEvent(LOG_EVT_DOOR_OPEN); /* Data Logging (Feature 2) */
}


/*******************************************************************************
 * Brief: Function to Close the Door (Resets Servo to 0 Degrees)
 * Parameters: Void
 * Return: Void
 *******************************************************************************/
void HOME_voidCloseDoor(void)
{
    /* Set Servo Angle back to 0 degrees to lock/close door */
    SERVOM_enumSetAngle(0);
    HOME_voidLogEvent(LOG_EVT_DOOR_CLOSE); /* Data Logging (Feature 2) */

}
