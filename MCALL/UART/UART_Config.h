

/* BUG FIX: was hardcoded to a fixed 8000000, completely independent of the
 * project's real F_CPU (the same macro every _delay_ms() call in this
 * project depends on). If the two ever disagree - e.g. this project is
 * actually built for 16MHz - the baud rate comes out wrong while every
 * other timing in the firmware stays correct, which is a confusing,
 * hard-to-spot failure. Deriving FOCS from F_CPU makes it correct
 * automatically no matter which clock the project is actually built for. */
#ifndef FOCS
#define FOCS         F_CPU

#define FOCS         8000000UL


#define BaudRate     9600



#define U2X          LOW
#define MPCM         LOW


#define RXEN         HIGH
#define TXEN         HIGH
#define UCSZ2        LOW


#define URSEL        HIGH
#define UMSEL        LOW


#define UPM1         LOW
#define UPM0         LOW


#define USBS         LOW


#define UCSZ1        HIGH
#define UCSZ0        HIGH



#define TXEN         HIGH
#define UCSZ2        LOW


#define URSEL        HIGH
#define UMSEL        LOW


#define UPM1         LOW
#define UPM0         LOW


#define USBS         LOW


#define UCSZ1        HIGH
#define UCSZ0        HIGH


#endif



