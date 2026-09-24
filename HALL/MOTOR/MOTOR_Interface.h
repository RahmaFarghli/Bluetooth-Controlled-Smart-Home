#ifndef MOTOR_INTERFACE_H_
#define MOTOR_INTERFACE_H_

/* CONFIGURATION TABLE ( DC MOTOR CONTROLLED THROUGH 2 PINS -> e.g H-BRIDGE IN1/IN2 ) */
typedef struct
{
    u8 PortID;
    u8 Pin1ID;
    u8 Pin2ID;
}MOTOR_t;


ErrorCode MOTOR_enumInit        (const MOTOR_t *MotorConfig);
ErrorCode MOTOR_enumRotateRight (const MOTOR_t *MotorConfig);
ErrorCode MOTOR_enumRotateLeft  (const MOTOR_t *MotorConfig);
ErrorCode MOTOR_enumStop        (const MOTOR_t *MotorConfig);


#endif /* MOTOR_INTERFACE_H_ */
