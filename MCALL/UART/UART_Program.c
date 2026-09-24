#include "../../LIBs/STD_TYPES.h"
#include "../../LIBs/BIT_MATH.h"

#include "UART_Config.h"
#include "UART_Interface.h"
#include "UART_Private.h"
#include <avr/io.h>

#define F_CPU         8000000UL

#include <util/delay.h>


void UART_voidInit(void)
{
    /* store the Low byte of the UBBR */
	 UBRRL = (u8) MYUBRR;
    /* store the HIGH byte of the UBBR*/
    UBRRH = (u8) ((MYUBRR >> 8) & 0x7F);
    /*Configration of UCSRnA REG*/
    UCSRnA = ((U2X<<1)  |  (MPCM<<0));
    /*Enable Transmit bit (3) and Recive bit (4) in UCSRnB*/
    UCSRnB = ((RXEN<<4) | (TXEN<<3) | (UCSZ2<<2));
    /*Configration of UCSRnC to make 8-bit data , 1 stop-bit , no parity , Asynch mode*/
    UCSRnC = ((URSEL<<7) | (UMSEL<<6) | (UPM1<<5) |(UPM0<<4) | (USBS<<3) | (UCSZ1<<2) | (UCSZ0<<1));

}




void UART_TransmitData(u8 Copy_u8UART_DataToBeSent)
{
    UDR = Copy_u8UART_DataToBeSent;
    while (GET_BIT(UCSRnA , 5) == 0);
    UDR = Copy_u8UART_DataToBeSent;
    SET_BIT(UCSRnA , 6);
}




u8 UART_ReciveData(void)
{
	if (GET_BIT(UCSRA, RXC) == 1)
	    {
	        return UDR;
	    }

	    return 255;
	}




void UART_voidPrintf(u8 Copy_u8PrintedData[])
{
        u8 iterator = 0;
        for(iterator=0; Copy_u8PrintedData[iterator] !=NULL; iterator++)
        {
            UART_TransmitData(Copy_u8PrintedData[iterator]);
        }
}

/******************************************************************/
/* Description : Check if UART received new data without blocking */
/******************************************************************/

u8 UART_u8IsDataAvailable(void)
{
    if (GET_BIT(UCSRA, 7) == 1)
    {
        return 1;
    }
    return 0;
}


