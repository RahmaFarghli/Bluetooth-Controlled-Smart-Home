#ifndef LCD_INTERFACE_H_
#define LCD_INTERFACE_H_


/* LCD OPERATING MODEs */
#define LCD_4BIT_MODE                 4
#define LCD_8BIT_MODE                 8


/* LCD ROW Positions */
#define LCD_ROW1                      0
#define LCD_ROW2                      1


/* LCD COL Positions */
#define LCD_Col0                      0
#define LCD_Col1                      1
#define LCD_Col2                      2
#define LCD_Col3                      3
#define LCD_Col4                      4
#define LCD_Col5                      5
#define LCD_Col6                      6
#define LCD_Col7                      7
#define LCD_Col8                      8
#define LCD_Col9                      9
#define LCD_Col10                     10
#define LCD_Col11                     11
#define LCD_Col12                     12
#define LCD_Col13                     13
#define LCD_Col14                     14
#define LCD_Col15                     15


/* LCD Function Set According to LCD Mode */
#if (LCD_OPERATION_MODE == LCD_4BIT_MODE)

    #define LCD_FUNCTION_SET  0x28

#elif (LCD_OPERATION_MODE == LCD_8BIT_MODE)

    #define LCD_FUNCTION_SET  0x38

#endif /* LCD_MODE */


/* SPECIAL FUNCTIONs COMMANDS */
#define LCD_DisplayON_CursorOFF    0x0c
#define LCD_DisplayOFF_CursorOFF   0x08
#define LCD_DisplayON_CursorON     0x0e
#define LCD_DisplayCLEAR           0x01
#define LCD_Home                   0x02
#define LCD_EntryMode              0x06
#define LCD_CursorPostion          0x80




/*================================================ < PROVIDED APIs FOR LCD INTERFACING > ================================================*/

void LCD_voidInit             (void                       );
void LCD_voidSendData         (u8 Copy_u8Data             );
void LCD_voidSendCommand      (u8 Copy_u8Command          );

void LCD_voidPrintString      (const u8 Copy_u8Data[]     );
void LCD_voidPrintNumber      (u64 Copy_u64Number         );

void LCD_voidSetCursorPosition(u8 Copy_u8Row,u8 Copy_u8Col);
void LCD_voidDisplayClear     (void                       );


#endif /* LCD_INTERFACE_H_ */
