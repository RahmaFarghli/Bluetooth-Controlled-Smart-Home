#ifndef UART_PRIVATE_H
#define UART_PRIVATE_H

#define UCSRnA               *((volatile u8*)(0x2B))          
#define UCSRnB               *((volatile u8*)(0x2A))
#define UCSRnC               *((volatile u8*)(0x40))

#define UDR                 *((volatile u8*)(0x2C))

#define UBRRH               *((volatile u8*)(0x40))
#define UBRRL               *((volatile u8*)(0x29))



#define MYUBRR              (FOCS/(16*BaudRate)-1)




#endif
