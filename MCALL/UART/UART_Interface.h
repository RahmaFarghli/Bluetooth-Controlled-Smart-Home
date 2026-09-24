#ifndef UART_Interface_H
#define UART_Interface_H

void UART_voidInit(void);

void UART_TransmitData(u8 Copy_u8UART_DataToBeSent);

u8 UART_ReciveData(void);

void UART_voidPrintf(u8 Copy_u8PrintedData[]);
/******************************************************************/
/* Description : Check if UART received new data without blocking */
/* Return      : 1 if data is available, 0 if no data             */
/******************************************************************/
u8 UART_u8IsDataAvailable(void);

#endif
