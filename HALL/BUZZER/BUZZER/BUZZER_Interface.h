

#ifndef BUZZER_INTERFACE_H_
#define BUZZER_INTERFACE_H_


/* BUZZER OBJECT INTERFACE */
typedef struct BUZZERx
{
    u8 PortID:3;
    u8 PinID:3;
    u8 Activation:2;
}BUZZER_t;


/* BUZZER ACTIVATION INTERFACE */
#define ACTIVE_LOW       0
#define ACTIVE_HIGH      1


/*================================================ < PROVIDED APIs FOR BUZZER INTERFACING > ================================================*/

ErrorCode BUZZER_enumInit     (const BUZZER_t *BUZZER_CONFIG);
ErrorCode BUZZER_enumPowerON  (const BUZZER_t *BUZZER_CONFIG);
ErrorCode BUZZER_enumPowerOFF (const BUZZER_t *BUZZER_CONFIG);
ErrorCode BUZZER_enumToggle   (const BUZZER_t *BUZZER_CONFIG);


#endif /* BUZZER_INTERFACE_H_ */
