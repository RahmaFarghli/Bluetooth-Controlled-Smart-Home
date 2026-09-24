#ifndef DIO_PRIVATE_H_
#define DIO_PRIVATE_H_


/* REGISTER DEFINITION FOR DIO PORT A */
#define PORTA  *((volatile u8*) 0x3B)
#define DDRA   *((volatile u8*) 0x3A)
#define PINA   *((volatile u8*) 0x39)

/* REGISTER DEFINITION FOR DIO PORT B */
#define PORTB  *((volatile u8*) 0x38)
#define DDRB   *((volatile u8*) 0x37)
#define PINB   *((volatile u8*) 0x36)

/* REGISTER DEFINITION FOR DIO PORT C */
#define PORTC  *((volatile u8*) 0x35)
#define DDRC   *((volatile u8*) 0x34)
#define PINC   *((volatile u8*) 0x33)

/* REGISTER DEFINITION FOR DIO PORT D */
#define PORTD  *((volatile u8*) 0x32)
#define DDRD   *((volatile u8*) 0x31)
#define PIND   *((volatile u8*) 0x30)

/* SFIOR REGISTER DEFINITION */
#define SFIOR  *((volatile u8*) 0x50)
#define PUD      2



#endif /* DIO_PRIVATE_H_ */

















