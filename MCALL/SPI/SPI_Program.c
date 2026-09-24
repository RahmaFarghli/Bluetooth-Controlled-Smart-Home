#include "../../LIBs/STD_TYPES.h"
#include "../../LIBs/BIT_MATH.h"

#include "SPI_Interface.h"
#include "SPI_Private.h"
#include "SPI_Config.h"

void SPI_voidMasterInit(void)
{
    //Set The Node To Be Master Node
    SET_BIT(SPI_u8_SPCR_REG,SPI_u8_SPCR_MSTR);
    //Set The MSB To Be sent first
    CLEAR_BIT(SPI_u8_SPCR_REG , SPI_u8_SPCR_DORD);
    //Set The Leading Edge To Be The Rising Edge
    CLEAR_BIT(SPI_u8_SPCR_REG , SPI_u8_SPCR_CPOL);
    //Sample At Leading Edge
    CLEAR_BIT(SPI_u8_SPCR_REG , SPI_u8_SPCR_CPHA);

    //Clock Prescaler, Divide by 16
    SET_BIT(SPI_u8_SPCR_REG,SPI_u8_SPCR_SPR0);
    CLEAR_BIT(SPI_u8_SPCR_REG , SPI_u8_SPCR_SPR1);
    CLEAR_BIT(SPI_u8_SPSR_REG , SPI_u8_SPSR_SPI2X);
    //Enable The SPI
    SET_BIT(SPI_u8_SPCR_REG,SPI_u8_SPCR_SPE);
}






void SPI_voidSlaveInit(void)
{
    //Set The Node To Be Slave Node
    CLEAR_BIT(SPI_u8_SPCR_REG,SPI_u8_SPCR_MSTR);
    //Set The MSB To Be sent first
    CLEAR_BIT(SPI_u8_SPCR_REG , SPI_u8_SPCR_DORD);
    //Set The Leading Edge To Be The Rising Edge
    CLEAR_BIT(SPI_u8_SPCR_REG , SPI_u8_SPCR_CPOL);
    //Sample At Leading Edge
    CLEAR_BIT(SPI_u8_SPCR_REG , SPI_u8_SPCR_CPHA);

    //Enable The SPI
    SET_BIT(SPI_u8_SPCR_REG,SPI_u8_SPCR_SPE);
}




u8 SPI_u8Tranceive(u8 Copy_u8Data)
{
    SPI_u8_SPDR_REG = Copy_u8Data;
    while(GET_BIT(SPI_u8_SPSR_REG , SPI_u8_SPSR_SPIF) == 0);
    SET_BIT(SPI_u8_SPSR_REG , SPI_u8_SPSR_SPIF);
    return SPI_u8_SPDR_REG ;
}


