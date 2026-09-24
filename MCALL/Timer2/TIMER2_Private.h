#ifndef TIMER2_PRIVATE_H
#define TIMER2_PRIVATE_H

// TIMER 2 REGISTERS 
#define TIMO_u8_TCCR2_REG                *((volatile u8 *)(0x45))
#define TIMO_u8_TCCR2_CS20                0
#define TIMO_u8_TCCR2_CS21                1
#define TIMO_u8_TCCR2_CS22                2
#define TIMO_u8_TCCR2_WGM21               3
#define TIMO_u8_TCCR2_COM20               4
#define TIMO_u8_TCCR2_COM21               5
#define TIMO_u8_TCCR2_WGM20               6
#define TIMO_u8_TCCR2_FOC2                7



#define TIMO_u8_TCNT2_REG                *((volatile u8 *)(0x44))






#define TIMEO_u8_OCR2_REG                *((volatile u8 *)(0x43))



#define TIMO_u8_ASSR_REG                *((volatile u8 *)(0x42))
#define TIMO_u8_ASSR_TCR2UB                0
#define TIMO_u8_ASSR_OCR2UB                1
#define TIMO_u8_ASSR_TCN2UB                2
#define TIMO_u8_ASSR_AS2                   3




#define TIMEO_u8_TIMSK_REG                *((volatile u8 *)(0x59))
#define TIMEO_u8_TIMSK_TOIE2              6
#define TIMEO_u8_TIMSK_OCIE2              7




#define TIMEO_u8_TIFR_REG                *((volatile u8 *)(0x58))
#define TIMEO_u8_TIFR_TOV2                6
#define TIMEO_u8_TIFR_OCF2                7









#endif