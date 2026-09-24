#ifndef HOME_INTERFACE_H
#define HOME_INTERFACE_H

/******************************************************************/
/* Description : Initialize all peripherals used in Home System   */
/******************************************************************/
void HOME_voidInit(void);

/******************************************************************/
/* Description : Display welcome message on LCD                   */
/******************************************************************/
void HOME_voidWelcomeScreen(void);

/******************************************************************/
/* Description : Display Main Screen on LCD                       */
/******************************************************************/
void HOME_voidMainScreen(void);


/******************************************************************/
/* Description : Initialize All Room LEDs                         */
/******************************************************************/
void HOME_voidRoomsInit(void);

/******************************************************************/
/* Description : Turn Selected Room LED ON                        */
/* Input       : Room ID (1 ~ 2)                                  */
/******************************************************************/
void HOME_voidRoomOn(u8 Copy_u8RoomID);

/******************************************************************/
/* Description : Turn Selected Room LED OFF                       */
/* Input       : Room ID (1 ~ 2)                                  */
/******************************************************************/
void HOME_voidRoomOff(u8 Copy_u8RoomID);

/******************************************************************/
/* Description : Receive Control Commands                         */
/******************************************************************/
void HOME_voidControlTask(void);

/******************* Fan Functions *******************/
void HOME_voidFanInit(void);
void HOME_voidFanOn(void);
void HOME_voidFanOff(void);

/******************* Buzzer Functions *******************/
void HOME_voidBuzzerInit(void);
void HOME_voidBuzzerOn(void);
void HOME_voidBuzzerOff(void);

/***************************************************************/
/* Feedback Functions                                           */
/***************************************************************/

void HOME_voidFeedbackInit(void);

void HOME_voidSuccessFeedback(void);

void HOME_voidErrorFeedback(void);



/***************************************************************/
/* Door Functions                                               */
/***************************************************************/

u8 HOME_u8CheckPassword(void);

void HOME_voidDoorControl(void);
void HOME_voidCloseDoor(void);
void HOME_voidDoorOpen(void);
void HOME_voidOpenDoor(void);
void HOME_voidOnCorrectPassword(void);
void Private_voidLCDPrintString(u8 *Copy_u8String);
/***************************************************************/
/* Description : Start System and Check Door Password           */
/***************************************************************/
void HOME_voidSystemStart(void);

void HOME_voidDisplayCurrentMenu(void);
void HOME_voidMenuTask(void);

/* Temperature and Fan Automatic Control Functions */
void HOME_voidTempSensorInit(void);
u8   HOME_u8GetTemperature(void);
void HOME_voidAutoFanControlTask(void);

void HOME_voidButtonsInit(void);
void HOME_voidButtonsTask(void);

/***************************************************************/
/* Data Logging Functions (EEPROM)                              */
/***************************************************************/

/* Description : Dumps every stored event in the EEPROM log over UART,   */
/*               oldest first, one line per event.                      */
void HOME_voidPrintEventLog(void);

#endif
