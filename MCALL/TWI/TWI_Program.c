#include "../../LIBs/STD_TYPES.h"
#include "../../LIBs/BIT_MATH.h"

#include "TWI_Interface.h"
#include "TWI_Private.h"
#include "TWI_Config.h"

/* BUG FIX: TWBR was hardcoded to 2, which only gives ~100kHz SCL at an
 * assumed 8MHz F_CPU (SCL = F_CPU / (16 + 2*TWBR)). At this project's actual
 * 16MHz clock that hardcoded value would double the SCL frequency to
 * ~400kHz, well outside the M24C02 EEPROM's 100kHz Standard-mode spec, and
 * would generally break I2C reliability on ANY module clocked at other
 * than 8MHz. Deriving TWBR from F_CPU keeps this correct for any clock. */
#ifndef TWI_SCL_FREQ_HZ
#define TWI_SCL_FREQ_HZ   100000UL
#endif
#define TWI_TWBR_VALUE    ((u8)(((F_CPU / TWI_SCL_FREQ_HZ) - 16) / 2))

/* ROBUSTNESS FIX: every TWI step below used to wait "while (TWINT==0) {}"
 * with no bound. If the bus ever glitches or stalls (a marginal wire, a
 * simulation timing edge case) so TWINT never sets, that call - and
 * therefore anything that called it, including every EEPROM read the
 * passcode check does - hangs the whole system forever. This gives every
 * wait a ~20ms ceiling (plenty for a normal 100kHz I2C step) and reports
 * TWI_TimeoutError instead of hanging. */
#define TWI_TWINT_TIMEOUT_LOOPS   2000UL
static u8 TWI_u8WaitForTWINT(void)
{
	u32 Local_u32Timeout = TWI_TWINT_TIMEOUT_LOOPS;
	while ((GET_BIT(TWI_u8_TWCR_REG, TWI_u8_TWCR_TWINT) == 0) && (Local_u32Timeout > 0))
	{
		Local_u32Timeout--;
	}
	return (Local_u32Timeout > 0) ? 1 : 0;
}

/*********************************************************************************/
/* Function: TWI_voidInitMaster			                       				    **/
/* I/P Parameters: Copy_u8Address							          		    **/
/* Returns:it returns No thing                                				    **/
/* Desc:This Function initialize Two wire interface (I2C) as Master Node		**/
/*********************************************************************************/
/* Copy_u8Address:Assign the address of the Master Node							**/
/*********************************************************************************/
void TWI_voidInitMaster(u8 Copy_u8Address)
{
	/*Enable Acknowledge Bit*/
	SET_BIT(TWI_u8_TWCR_REG, TWI_u8_TWCR_TWEA);

	/*Set SCL frequency to TWI_SCL_FREQ_HZ (100KHz default), computed for the project's actual F_CPU*/
	TWI_u8_TWBR_REG = TWI_TWBR_VALUE;
	/*2- Clear The Prescaler bit (TWPS0 - TWPS1)*/
	CLEAR_BIT(TWI_u8_TWSR_REG, TWI_u8_TWSR_TWPS0);
	CLEAR_BIT(TWI_u8_TWSR_REG, TWI_u8_TWSR_TWPS1);

	/*Check if the master node will be addressed or not*/
	if(Copy_u8Address == 0)
	{
		/*Do Nothing*/
	}
	else
	{
		/*Set The Required Address to The Master*/
		TWI_u8_TWAR_REG = (Copy_u8Address << 1);
	}

	/*Enable TWI*/
	SET_BIT(TWI_u8_TWCR_REG, TWI_u8_TWCR_TWEN);
}

/*********************************************************************************/
/* Function: TWI_voidInitSlave			                       				    **/
/* I/P Parameters: Copy_u8Address							          		    **/
/* Returns:it returns No thing                                				    **/
/* Desc:This Function initialize Two wire interface (I2C) as Slave Node			**/
/*********************************************************************************/
/* Copy_u8Address:Assign the address of the Slave Node							**/
/*********************************************************************************/
void TWI_voidInitSlave(u8 Copy_u8Address)
{
	/*Set The Required Address to The Slave*/
	TWI_u8_TWAR_REG = Copy_u8Address << 1;

	/*Enable Acknowledge Bit*/
	SET_BIT(TWI_u8_TWCR_REG, TWI_u8_TWCR_TWEA);

	/*Enable TWI*/
	SET_BIT(TWI_u8_TWCR_REG, TWI_u8_TWCR_TWEN);
}

/*********************************************************************************/
/* Function: TWI_ErrorStatusSendStartConditionWithACK	    				    **/
/* I/P Parameters: Nothing									          		    **/
/* Returns:it returns TWI_ErrorStatus_t                        				    **/
/* Desc:This Function Send Start Condition With ACK								**/
/*********************************************************************************/
TWI_ErrorStatus_t TWI_ErrorStatusSendStartConditionWithACK(void)
{
	TWI_ErrorStatus_t Local_ErrorStatus = NoError;
//	TWI_u8_TWCR |= (1 << TWI_u8_TWCR_TWINT) | (1 << TWI_u8_TWCR_TWSTA) | (1 << TWI_u8_TWCR_TWEN);
	/*Send Start Condition Bit*/
		SET_BIT(TWI_u8_TWCR_REG, TWI_u8_TWCR_TWSTA);

	/*Clear The TWINT Flag*/
	SET_BIT(TWI_u8_TWCR_REG, TWI_u8_TWCR_TWINT);

	/*wait until the operation finishes and the flag is raised again*/
	if (!TWI_u8WaitForTWINT()) { return TWI_TimeoutError; }

	if((TWI_u8_TWSR_REG & STATUS_BIT_MASK) != START_ACK)
	{
		Local_ErrorStatus = StartConditionError;
	}
	else
	{
		/*Do Nothing*/
	}
	return Local_ErrorStatus;
}

/*********************************************************************************/
/* Function: TWI_ErrorStatusSendRepeatedStartConditionWithACK				    **/
/* I/P Parameters: Nothing									          		    **/
/* Returns:it returns TWI_ErrorStatus_t                        				    **/
/* Desc:This Function Send repeated Start Condition With ACK					**/
/*********************************************************************************/
TWI_ErrorStatus_t TWI_ErrorStatusSendRepeatedStartConditionWithACK(void)
{
	TWI_ErrorStatus_t Local_ErrorStatus = NoError;
	TWI_u8_TWCR_REG |= (1 << TWI_u8_TWCR_TWINT) | (1 << TWI_u8_TWCR_TWSTA) | (1 << TWI_u8_TWCR_TWEN);
	/*Send Start Condition Bit*/
	//	SET_BIT(TWI_u8_TWCR, TWI_u8_TWCR_TWSTA);

	/*Clear The TWINT Flag*/
	//	SET_BIT(TWI_u8_TWCR, TWI_u8_TWCR_TWINT);

	/*wait until the operation finishes and the flag is raised again*/
	if (!TWI_u8WaitForTWINT()) { return TWI_TimeoutError; }

	/*Check For The Condition Status Code*/
	if((TWI_u8_TWSR_REG & STATUS_BIT_MASK) != REP_START_ACK)
	{
		Local_ErrorStatus = RepeatedStartError;
	}
	return Local_ErrorStatus;
}

/*********************************************************************************/
/* Function: TWI_ErrorStatusSendSlaveAddressWithWriteACK					    **/
/* I/P Parameters: Nothing									          		    **/
/* Returns:it returns TWI_ErrorStatus_t                        				    **/
/* Desc:This Function Send Slave Address With Write ACK							**/
/*********************************************************************************/
TWI_ErrorStatus_t TWI_ErrorStatusSendSlaveAddressWithWriteACK(u8 Copy_u8SlaveAddress)
{
	TWI_ErrorStatus_t Local_ErrorStatus = NoError;

	/*Set 7 bits slave address to the bus*/
	TWI_u8_TWDR_REG = (Copy_u8SlaveAddress << 1);

	/*Set The Write Request in the LSB in the data Register*/
	CLEAR_BIT(TWI_u8_TWDR_REG, TWI_u8_TWDR_TWD0);

	/*Clear The Start Condition Bit*/
	CLEAR_BIT(TWI_u8_TWCR_REG, TWI_u8_TWCR_TWSTA);

	/*Clear The TWINT Flag*/
	SET_BIT(TWI_u8_TWCR_REG, TWI_u8_TWCR_TWINT);

	/*wait until the operation finishes and the flag is raised again*/
	if (!TWI_u8WaitForTWINT()) { return TWI_TimeoutError; }

	/*Check For The Condition Status Code*/
	if((TWI_u8_TWSR_REG & STATUS_BIT_MASK) != SLAVE_ADD_AND_WR_ACK)
	{
		Local_ErrorStatus = SlaveAddressWithWriteError;
	}
	return Local_ErrorStatus;
}


/*********************************************************************************/
/* Function: TWI_ErrorStatusSendSlaveAddressWithReadACK						    **/
/* I/P Parameters: Nothing									          		    **/
/* Returns:it returns TWI_ErrorStatus_t                        				    **/
/* Desc:This Function Send Slave Address With Read ACK							**/
/*********************************************************************************/
TWI_ErrorStatus_t TWI_ErrorStatusSendSlaveAddressWithReadACK(u8 Copy_u8SlaveAddress)
{
	TWI_ErrorStatus_t Local_ErrorStatus = NoError;

	/*Set 7 bits slave address to the bus*/
	TWI_u8_TWDR_REG = Copy_u8SlaveAddress << 1;

	/*Set The Read Request in the LSB in the data Register*/
	SET_BIT(TWI_u8_TWDR_REG, TWI_u8_TWDR_TWD0);

	/*Clear The Start Condition Bit*/
	CLEAR_BIT(TWI_u8_TWCR_REG, TWI_u8_TWCR_TWSTA);

	/*Clear The TWINT Flag*/
	SET_BIT(TWI_u8_TWCR_REG, TWI_u8_TWCR_TWINT);

	/*wait until the operation finishes and the flag is raised again*/
	if (!TWI_u8WaitForTWINT()) { return TWI_TimeoutError; }

	/*Check For The Condition Status Code*/
	if((TWI_u8_TWSR_REG & STATUS_BIT_MASK) != SLAVE_ADD_AND_RD_ACK)
	{
		Local_ErrorStatus = SlaveAddressWithReadError;
	}
	return Local_ErrorStatus;
}



/*********************************************************************************/
/* Function: TWI_ErrorStatusMasterWriteDataByteWithACK						    **/
/* I/P Parameters: Copy_u8DataByte							          		    **/
/* Returns:it returns TWI_ErrorStatus_t                        				    **/
/* Desc:This Function allows master to write byte of data With ACK				**/
/*********************************************************************************/
/* Copy_u8DataByte options: Byte of data to be sent			          		    **/
/*********************************************************************************/
TWI_ErrorStatus_t TWI_ErrorStatusMasterWriteDataByteWithACK(u8 Copy_u8DataByte)
{

	TWI_ErrorStatus_t Local_ErrorStatus = NoError;


	/*Set The Data To The TWI_u8_TWDR Register*/
	TWI_u8_TWDR_REG = Copy_u8DataByte;



	/*Clear The TWINT Flag*/
	SET_BIT(TWI_u8_TWCR_REG, TWI_u8_TWCR_TWINT);


	/*wait until the operation finishes and the flag is raised again*/
	if (!TWI_u8WaitForTWINT()) { return TWI_TimeoutError; }

	/*Check For The Condition Status Code*/
	if((TWI_u8_TWSR_REG & STATUS_BIT_MASK) != MSTR_WR_BYTE_ACK)
	{
		Local_ErrorStatus = MasterWriteByteWithACKError;
	}
	return Local_ErrorStatus;
}

/*********************************************************************************/
/* Function: TWI_ErrorStatusMasterReadDataByteWithACK						    **/
/* I/P Parameters: *Copy_pu8ReceivedByte					          		    **/
/* Returns:it returns TWI_ErrorStatus_t                        				    **/
/* Desc:This Function allows master to recieve byte of data With ACK			**/
/*********************************************************************************/
/* *Copy_pu8ReceivedByte options: Address of variable to store the recieved byte**/
/*********************************************************************************/
TWI_ErrorStatus_t TWI_ErrorStatusMasterReadDataByteWithACK(u8 * Copy_pu8ReceivedByte)
{
	TWI_ErrorStatus_t Local_ErrorStatus = NoError;

	/*Clear The TWINT Flag, To Make The Slave To Send its Data*/
	SET_BIT(TWI_u8_TWCR_REG, TWI_u8_TWCR_TWINT);

	/*wait until the operation finishes and the flag is raised again*/
	if (!TWI_u8WaitForTWINT()) { return TWI_TimeoutError; }

	/*Check For The Condition Status Code*/
	if((TWI_u8_TWSR_REG & STATUS_BIT_MASK) != MSTR_RD_BYTE_WITH_ACK)
	{
		Local_ErrorStatus = MasterReadByteWithACKError;
	}
	else
	{
		/*Read The Received Data*/
		*Copy_pu8ReceivedByte = TWI_u8_TWDR_REG;
	}

	return Local_ErrorStatus;

}

/*********************************************************************************/
/* Function: TWI_voidSendStopCondition	                       				    **/
/* I/P Parameters: Nothing									          		    **/
/* Returns:it returns No thing                                				    **/
/* Desc:This Function Sends a stop condition to the bus							**/
/*********************************************************************************/
void TWI_voidSendStopCondition(void)
{

	/*Send a stop condition on the bus*/
	SET_BIT(TWI_u8_TWCR_REG, TWI_u8_TWCR_TWSTO);

	/*Clear The TWINT Flag*/
	SET_BIT(TWI_u8_TWCR_REG, TWI_u8_TWCR_TWINT);
}

















