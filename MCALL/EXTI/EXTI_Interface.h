#ifndef EXIT_INTERFACE_H
#define EXIT_INTERFACE_H

/*



*/

void EXTI_voidEXTIEnable(u8 Copy_u8EXTINo , u8 Copy_u8EXTISense);


void EXTI_voidEXTIDisable(u8 Copy_u8EXTINo);



void EXTI_voidSetCallBack(void(*Copy_ptrToFunc)(void) , u8 Copy_u8EXTIIndex);

#endif


