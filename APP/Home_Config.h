#ifndef HOME_CONFIG_H
#define HOME_CONFIG_H

/******************* FAN Configuration **********************/
#define HOME_FAN_PORT       DIO_PORTA
#define HOME_FAN_PIN        DIO_PIN1

/******************* Buzzer Configuration *******************/
#define HOME_BUZZER_PORT    DIO_PORTA
#define HOME_BUZZER_PIN     DIO_PIN2

/***************************************************************/
/* Feedback LEDs Configuration                                 */
/***************************************************************/

#define GREEN_LED_PORT     DIO_PORTD
#define GREEN_LED_PIN      DIO_PIN4


#define RED_LED_PORT       DIO_PORTD
#define RED_LED_PIN        DIO_PIN6

/***************************************************************/
/* Door Password Configuration                                  */
/***************************************************************/

#define PASSWORD_SIZE      4

/* Manual Hardware Push Buttons Configuration */

#define PB_ROOM1_PORT    DIO_PORTA
#define PB_ROOM1_PIN     DIO_PIN7

#define PB_ROOM2_PORT    DIO_PORTD
#define PB_ROOM2_PIN     DIO_PIN7

/******************* Room LEDs Configuration *******************/

#define ROOM1_PORT      DIO_PORTD
#define ROOM1_PIN       DIO_PIN2

#define ROOM2_PORT      DIO_PORTD
#define ROOM2_PIN       DIO_PIN3

/* ADC Channel for Temperature Sensor (LM35) */
#define HOME_LM35_ADC_CHANNEL    ADC_CHANNEL_0    /* PA0 */

/* Temperature Thresholds in Celsius */
#define FAN_ON_TEMP_THRESHOLD    30

/* Servo Motor Signal Pin (OC1A on ATmega32 is PORTD Pin 5) */
#define SERVOM_u8_PORT          DIO_PORTD
#define SERVOM_u8_PIN           DIO_PIN5

/***************************************************************/
/* EEPROM Data Logging Configuration                            */
/***************************************************************/

/* Address 0 stores the current write index (0 .. LOG_CAPACITY-1) so the
 * log survives a reset/power cycle. Events are stored one byte each,
 * starting right after the index byte. 24C02B has 256 bytes total. */
#define LOG_INDEX_ADDR          0
#define LOG_BASE_ADDR           1
#define LOG_CAPACITY            50   /* addresses 1..50 - well inside the 256-byte EEPROM */

/* Event codes written to the log (one byte per event) */
#define LOG_EVT_ROOM1_OFF       0x10
#define LOG_EVT_ROOM1_ON        0x11
#define LOG_EVT_ROOM2_OFF       0x20
#define LOG_EVT_ROOM2_ON        0x21
#define LOG_EVT_FAN_OFF         0x30
#define LOG_EVT_FAN_ON          0x31
#define LOG_EVT_DOOR_CLOSE      0x40
#define LOG_EVT_DOOR_OPEN       0x41
#define LOG_EVT_EMPTY           0xFF /* blank/erased EEPROM sentinel */

#endif
