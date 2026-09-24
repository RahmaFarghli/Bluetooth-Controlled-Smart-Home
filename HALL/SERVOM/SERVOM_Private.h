#ifndef SERVOM_PRIVATE_H_
#define SERVOM_PRIVATE_H_

/*
 * PWM Period = 20ms (50Hz) standard for Servo Motors.
 * At F_CPU 8MHz and Prescaler 8 (Timer1 resolution = 1us),
 * ICR1 Top value = 20000.
 */
#define SERVOM_u16_PERIOD_TOP   20000

/* Min pulse width = 1000us (0 degrees), Max pulse width = 2000us (180 degrees) */
#define SERVOM_u16_MIN_PULSE    1000
#define SERVOM_u16_MAX_PULSE    2000

#endif /* SERVOM_PRIVATE_H_ */
