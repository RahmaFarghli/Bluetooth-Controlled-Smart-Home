#include "../../LIBs/STD_TYPES.h"
#include "../../LIBs/BIT_MATH.h"

/* BUG FIX: removed local "#define F_CPU 8000000UL" - shadowed the project's
 * real 16MHz clock and would have shortened the _delay_ms(10) write-cycle
 * wait below to roughly half the intended time, risking corrupted writes
 * if the next EEPROM access started before the internal write finished. */
#include <util/delay.h>

/* BUG FIX: "TWI_interface.h" (lowercase i) does not match the actual
 * filename "TWI_Interface.h". This works on case-insensitive filesystems
 * (Windows) but fails to compile on Linux/most CI toolchains. */
#include "../../MCALL/TWI/TWI_Interface.h"

#include "EEPROM_Interface.h"
#include "EEPROM_Config.h"
#include "EEPROM_Private.h"

/*********************************************************************************/
/* Function: EEPROM_voidSendDataByte	                       				    **/
/* I/P Parameters: u16 Copy_u16LocationAddress, u8 Copy_u8DataByte   		    **/
/* Returns:it returns No thing                                				    **/
/* Desc:This Function sends byte of data to eeprom								**/
/*********************************************************************************/
/* Copy_u16LocationAddress:the address of the location you want to write on		**/
/* Copy_u8DataByte:Assign the Data you want to send								**/
/*********************************************************************************/
void EEPROM_voidSendDataByte(u16 Copy_u16LocationAddress, u8 Copy_u8DataByte)
{
	u8 Local_u8AddressPacket;

	Local_u8AddressPacket = EEPROM_FIXED_ADDRESS | (A2_CONNECTION <<2) | (u8)(Copy_u16LocationAddress >>8);

	/* BUG FIX: every one of these calls returns a TWI_ErrorStatus_t that was
	 * previously discarded - the function barreled ahead through all 4 I2C
	 * steps even if an earlier one had already failed (no ACK, or a
	 * TWI_TimeoutError), and could leave the bus wedged mid-transaction with
	 * no STOP ever sent, breaking every later I2C call too - including the
	 * new Data Logging feature's writes. Now each step is checked, and STOP
	 * is always sent, even on an early failure, so the bus is released. */

	/*Send start condition*/
	if (TWI_ErrorStatusSendStartConditionWithACK() != NoError) { TWI_voidSendStopCondition(); return; }

	/*Send the address packet*/
	if (TWI_ErrorStatusSendSlaveAddressWithWriteACK(Local_u8AddressPacket) != NoError) { TWI_voidSendStopCondition(); return; }

	/*Send the rest 8bits of the location address*/
	if (TWI_ErrorStatusMasterWriteDataByteWithACK((u8)Copy_u16LocationAddress) != NoError) { TWI_voidSendStopCondition(); return; }

	/*Send the data byte to the memory location*/
	if (TWI_ErrorStatusMasterWriteDataByteWithACK(Copy_u8DataByte) != NoError) { TWI_voidSendStopCondition(); return; }

	/*Send stop condition*/
	TWI_voidSendStopCondition();

	/*Delay until the write cycle is finished*/
	_delay_ms(10);
}

/*********************************************************************************/
/* Function: EEPROM_u8ReadDataByte		                       				    **/
/* I/P Parameters: u16 Copy_u16LocationAddress						  		    **/
/* Returns:it returns u8	                                				    **/
/* Desc:This Function reads a byte of data from eeprom							**/
/*********************************************************************************/
/* Copy_u16LocationAddress:the address of the location you want to read from	**/
/*********************************************************************************/
u8 EEPROM_u8ReadDataByte(u16 Copy_u16LocationAddress)
{
	u8 Local_u8AddressPacket;
	/* BUG FIX: this was declared with NO initial value. On any failed I2C
	 * step, TWI_ErrorStatusMasterReadDataByteWithACK() never touches it (it
	 * only writes the output on success) - so the function returned
	 * whatever uninitialized stack garbage happened to be there. For the
	 * new Data Logging feature that garbage would show up as a random,
	 * meaningless "event" in the log instead of a real one. 0xFF is a safe,
	 * deliberate sentinel (matches LOG_EVT_EMPTY in Home_Config.h). */
	u8 Local_u8Data = 0xFF;

	Local_u8AddressPacket = EEPROM_FIXED_ADDRESS | (A2_CONNECTION <<2) | (u8)(Copy_u16LocationAddress >>8);

	/* Same fix as EEPROM_voidSendDataByte above: check every step, and
	 * always send STOP - including on early failure - to avoid wedging
	 * the bus for every subsequent EEPROM call. */

	/*Send start condition*/
	if (TWI_ErrorStatusSendStartConditionWithACK() != NoError) { TWI_voidSendStopCondition(); return Local_u8Data; }

	/*Send the address packet with write request*/
	if (TWI_ErrorStatusSendSlaveAddressWithWriteACK(Local_u8AddressPacket) != NoError) { TWI_voidSendStopCondition(); return Local_u8Data; }

	/*Send the rest 8bits of the location address*/
	if (TWI_ErrorStatusMasterWriteDataByteWithACK((u8)Copy_u16LocationAddress) != NoError) { TWI_voidSendStopCondition(); return Local_u8Data; }

	/*Send repeated start to change write request into read request*/
	if (TWI_ErrorStatusSendRepeatedStartConditionWithACK() != NoError) { TWI_voidSendStopCondition(); return Local_u8Data; }

	/*Send the address packet with read request*/
	if (TWI_ErrorStatusSendSlaveAddressWithReadACK(Local_u8AddressPacket) != NoError) { TWI_voidSendStopCondition(); return Local_u8Data; }

	/*Get the data from memory*/
	TWI_ErrorStatusMasterReadDataByteWithACK(&Local_u8Data); /* on failure, Local_u8Data keeps its 0xFF sentinel */

	/*send the stop condition*/
	TWI_voidSendStopCondition();

	return Local_u8Data;
}
