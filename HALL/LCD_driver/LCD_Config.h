#ifndef LCD_CONFIG_H_
#define LCD_CONFIG_H_

/* LCD OPERATING MODE CONFIGURATION */
#define LCD_OPERATION_MODE          LCD_4BIT_MODE


/* LCD DATA PORT CONFIGURATION */
#define LCD_DATA_PortID             DIO_PORTC


/* LCD [ 4 BIT MODE ] DATA PINs CONFIGURATION */
#define LCD_4BIT_PIN0               DIO_PIN4
#define LCD_4BIT_PIN1               DIO_PIN5
#define LCD_4BIT_PIN2               DIO_PIN6
#define LCD_4BIT_PIN3               DIO_PIN7


/* RS [ PORT , PIN ] CONFIGURATION */
#define LCD_RS_PortID               DIO_PORTA
#define LCD_RS_PinID                DIO_PIN3


/* RW [ PORT , PIN ] CONFIGURATION */
#define LCD_RW_PortID               DIO_PORTA
#define LCD_RW_PinID                DIO_PIN4


/* ENABLE [ PORT , PIN ] CONFIGURATION */
#define LCD_ENABLE_PortID           DIO_PORTA
#define LCD_ENABLE_PinID            DIO_PIN5

#endif /* LCD_CONFIG_H_ */
