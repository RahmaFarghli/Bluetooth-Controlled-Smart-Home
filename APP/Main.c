/*#include "../LIBs/STD_TYPES.h"
#include "../LIBs/BIT_MATH.h"
#include "HOME_Interface.h"
#include "../MCALL/DIO/DIO_Interface.h"
#include "../MCALL/UART/UART_Interface.h"
#include "../HALL/BUZZER/BUZZER/BUZZER_Interface.h"
#include <util/delay.h>

int main(void)
{
    /* 1. Initialize All System Hardware & Drivers */
   // HOME_voidInit();

    /* 2. Initialize UART Peripheral for Data Logging (9600 BaudRate) */
  // UART_voidInit();

    /* إرسال رسالة ترحيبية عبر السيريال عند إقلاع النظام */
   // UART_voidPrintf((u8*)"--- Smart Home System Initialized ---\r\n");

    /* 3. Start Smart Home System Flow */
    /* الدالة تحتوي على الـ Main Loop الداخلية المصححة الآن */
   // HOME_voidSystemStart();

   // while(1)
   // {
        /* لن نصل إلى هنا لأن الدالة السابقة لا تنتهي،
           ولكن يتم إبقاؤها كإجراء وقائي لبنية الكود */
   /* }
}*/


















#define F_CPU 8000000UL
#include <util/delay.h>

#include "../LIBs/STD_TYPES.h"
#include "../LIBs/BIT_MATH.h"

#include "../MCALL/DIO/DIO_Interface.h"
#include "../HALL/SERVOM/SERVOM_Interface.h"

int main(void)
{
	DIO_SetPortMode(DIO_PORTA,DIO_PORT_OUTPUT);
	DIO_SetPortMode(DIO_PORTB,DIO_PORT_OUTPUT);
	DIO_SetPortMode(DIO_PORTC,DIO_PORT_OUTPUT);
	DIO_SetPortMode(DIO_PORTD,DIO_PORT_OUTPUT);


	DIO_SetPortValue(DIO_PORTA,DIO_PORT_HIGH);
	DIO_SetPortValue(DIO_PORTB,DIO_PORT_HIGH);
	DIO_SetPortValue(DIO_PORTC,DIO_PORT_HIGH);
	DIO_SetPortValue(DIO_PORTD,DIO_PORT_HIGH);
    return 0;
}
