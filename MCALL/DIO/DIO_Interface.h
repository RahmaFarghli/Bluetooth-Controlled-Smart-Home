#ifndef DIO_INTERFACE_H_
#define DIO_INTERFACE_H_

/* DIO PORTS INTERFACE OPTIONs */
#define DIO_PORTA       0
#define DIO_PORTB       1
#define DIO_PORTC       2
#define DIO_PORTD       3

/* DIO PINS INTERFACE OPTIONs */
#define DIO_PIN0        0
#define DIO_PIN1        1
#define DIO_PIN2        2
#define DIO_PIN3        3
#define DIO_PIN4        4
#define DIO_PIN5        5
#define DIO_PIN6        6
#define DIO_PIN7        7

/* DIO PIN MODES INTERFACE OPTIONs */
#define DIO_PIN_INPUT                  0
#define DIO_PIN_OUTPUT                 1
#define DIO_PIN_INPUT_INTERNALPULLUP   2

/* DIO PORT MODES INTERFACE OPTIONs */
#define DIO_PORT_INPUT                   0x00  //-> 0b00000000
#define DIO_PORT_OUTPUT                  0xFF  //-> 0b11111111
#define DIO_PORT_INPUT_INTERNALPULLUP    2


/* DIO PIN VALUE INTERFACE OPTIONs */
#define DIO_PIN_LOW     0
#define DIO_PIN_HIGH    1

/* DIO PORT VALUE INTERFACE OPTIONs */
#define DIO_PORT_LOW   0x00  //-> 0b00000000
#define DIO_PORT_HIGH  0xFF  //-> 0b11111111


ErrorCode DIO_SetPinMode(u8 Copy_u8PortID,u8 Copy_u8PinID,u8 Copy_u8Mode);
ErrorCode DIO_SetPinValue(u8 Copy_u8PortID,u8 Copy_u8PinID,u8 Copy_u8Value);
ErrorCode DIO_TogglePinValue(u8 Copy_u8PortID,u8 Copy_u8PinID);
ErrorCode DIO_GetPinValue(u8 Copy_u8PortID,u8 Copy_u8PinID,u8 *Ptr_u8InputData);

ErrorCode DIO_SetPortMode(u8 Copy_u8PortID,u8 Copy_u8Mode);
ErrorCode DIO_SetPortValue(u8 Copy_u8PortID,u8 Copy_u8Value);
ErrorCode DIO_TogglePortValue(u8 Copy_u8PortID);
ErrorCode DIO_GetPortValue(u8 Copy_u8PortID,u8 *Ptr_u8InputData);





#define SEG_0    0b00111111
#define SEG_1    0b00000110
#define SEG_2    0b01011011
#define SEG_3    0b01001111
#define SEG_4    0b01100110
#define SEG_5    0b01101101
#define SEG_6    0b01111101
#define SEG_7    0b00000111
#define SEG_8    0b01111111
#define SEG_9    0b01101111







#endif /* DIO_INTERFACE_H_ */
