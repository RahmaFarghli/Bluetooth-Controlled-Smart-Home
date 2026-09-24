#ifndef TIMER0_CONFIG_H
#define TIMER0_CONFIG_H

/*****************************************WAVE_GENERATION_OPTIONS ************************
0 NORMAL_MODE
1 PWM_OR_PHASE_CORRECT_MODE
2 CTC_MODE
3 FAST_PWM_MODE
******************************************************************************************/

#define WAVEFORM_GEN_MODE                           NORMAL_MODE
/**************************************************************************************** */


#define OC0_ACTION                                  FAST_PWM_CLEAR_OC0_ON_COMPARE_MATCH_SET_OC0_AT_TOP
/**************************************************************************************** */

#define PRESCALER_TYPES                             CLK_DIV_BY_256

#define PRESCALER_MASK                              0b11111000
/***************************************************************************************** */


//ry the same cocept with timer 1,2



/*
 * Choose Prescaler for Timer1:
 * 1. TIMER1_NO_CLOCK
 * 2. TIMER1_NO_PRESCALING
 * 3. TIMER1_PRESCALER_8
 * 4. TIMER1_PRESCALER_64
 * 5. TIMER1_PRESCALER_256
 * 6. TIMER1_PRESCALER_1024
 */
#define TIMER1_PRESCALER    TIMER1_PRESCALER_8





#endif
