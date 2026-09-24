#ifndef LED_INTERFACE_H_
#define LED_INTERFACE_H_


#define ACTIVE_LOW       0
#define ACTIVE_HIGH      1

/* CONFIGURATION TABLE */
typedef struct
{
    u8 PortID;
    u8 PinID;
    u8 ActiveMode;
}LED_t;


ErrorCode LED_enumInit(const LED_t *LEDConfig);
ErrorCode LED_enumPowerON(const LED_t *LEDConfig);
ErrorCode LED_enumPowerOFF(const LED_t *LEDConfig);
ErrorCode LED_enumToggle(const LED_t *LEDConfig);



#endif /* LED_INTERFACE_H_ */