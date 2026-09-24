#ifndef SERVOM_INTERFACE_H_
#define SERVOM_INTERFACE_H_

#include "../../LIBs/STD_TYPES.h"

ErrorCode SERVOM_enumInit(void);
ErrorCode SERVOM_enumSetAngle(u8 Copy_u8Angle);
ErrorCode SERVOM_enumStop(void);

#endif /* SERVOM_INTERFACE_H_ */
