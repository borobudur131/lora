/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: (1) "AS IS" WITH NO WARRANTY; AND 
 * (2)TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, SEMTECH SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 * 
 * Copyright (C) SEMTECH S.A.
 */
/*! 
 * \file       sx1276-Hal.c
 * \brief      SX1276 RF chip low level functions driver
 *
 * \remark     Optional support functions.
 *             These functions are defined only to easy the change of the
 *             parameters.
 *             For a final firmware the radio parameters will be known so
 *             there is no need to support all possible parameters.
 *             Removing these functions will greatly reduce the final firmware
 *             size.
 *
 * \version    2.0.0 
 * \date       May 6 2013
 * \author     Gregory Cristian
 *
 * Last modified by Miguel Luis on Jun 19 2013
 */

#include "platform.h"
#include "radio.h"


#include "sx1276.h"

#include "sx1276-Hal.h"
#include "sx1276-Fsk.h"
#include "sx1276-LoRa.h"

extern SPI_HandleTypeDef hspi2;


void SX1276InitIo( void )
{


}

/*!
 * \brief Set the radio reset pin state
 * 
 * \param state New reset pin state
 */
void  SX1276SetReset(uint8_t State)
{
	switch(State)
	{	
		case ON:    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);  break;
		case OFF:		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);break;
		default:break;
	}

}
void SX1276Nss( uint8_t State )
{
	switch(State)
	{	
		case ON:    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);  break;
		case OFF:		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);break;
		default:break;
	} 
}
/*!
 * \brief Writes the radio register at the specified address
 *
 * \param [IN]: addr Register address
 * \param [IN]: data New register value
 */
void SX1276Write( uint8_t addr, uint8_t data )
{
		osDelay(1);
	SX1276Nss(OFF);
		osDelay(1);
	addr|=0x80;
	HAL_SPI_Transmit_DMA(&hspi2,&addr,1);
	HAL_SPI_Transmit_DMA(&hspi2,&data,1);
	SX1276Nss(ON);


}

/*!
 * \brief Reads the radio register at the specified address
 *
 * \param [IN]: addr Register address
 * \param [OUT]: data Register value
 */
void SX1276Read( uint8_t addr, uint8_t *data )
{
		osDelay(1);
	SX1276Nss(OFF);
		osDelay(1);
	HAL_SPI_Transmit_DMA(&hspi2,&addr,1);
	HAL_SPI_Receive_DMA(&hspi2,data,1);
	SX1276Nss(ON);
}
/*!
 * \brief Writes multiple radio registers starting at address
 *
 * \param [IN] addr   First Radio register address
 * \param [IN] buffer Buffer containing the new register's values
 * \param [IN] size   Number of registers to be written
 */
void SX1276WriteBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
	osDelay(1);
	SX1276Nss(OFF);
	osDelay(1);
	addr|=0x80;
	HAL_SPI_Transmit_DMA(&hspi2,&addr,1);
	HAL_SPI_Transmit_DMA(&hspi2,buffer,size);
	SX1276Nss(ON);


}
/*!
 * \brief Reads multiple radio registers starting at address
 *
 * \param [IN] addr First Radio register address
 * \param [OUT] buffer Buffer where to copy the registers data
 * \param [IN] size Number of registers to be read
 */
void SX1276ReadBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
	osDelay(1);
	SX1276Nss(OFF);
	osDelay(1);
	HAL_SPI_Transmit_DMA(&hspi2,&addr,1);
	HAL_SPI_Receive_DMA(&hspi2,buffer,size);
	SX1276Nss(ON);

}

/*!
 * \brief Writes the buffer contents to the radio FIFO
 *
 * \param [IN] buffer Buffer containing data to be put on the FIFO.
 * \param [IN] size Number of bytes to be written to the FIFO
 */
void SX1276WriteFifo( uint8_t *buffer, uint8_t size )
{
	uint8_t i,addr=0x80;	
	osDelay(1);
	SX1276Nss(OFF);
		osDelay(1);
	for(i=0;i<size;i++)
	{
	HAL_SPI_Transmit_DMA(&hspi2,&addr,1);
	HAL_SPI_Transmit_DMA(&hspi2,buffer+i,1);
	}
	SX1276Nss(ON);

}

/*!
 * \brief Reads the contents of the radio FIFO
 *
 * \param [OUT] buffer Buffer where to copy the FIFO read data.
 * \param [IN] size Number of bytes to be read from the FIFO
 */
void SX1276ReadFifo( uint8_t *buffer, uint8_t size )
{
	uint8_t i,addr=0x00;
	osDelay(1);

	SX1276Nss(OFF);
		osDelay(1);

	for(i=0;i<size;i++)
	{
	HAL_SPI_Transmit_DMA(&hspi2,&addr,1);
	HAL_SPI_Receive_DMA(&hspi2,buffer+i,1);
	}
	SX1276Nss(ON);

}

/*!
 * \brief Gets the SX1276 DIO0 hardware pin status
 *
 * \retval status Current hardware pin status [1, 0]
 */
uint8_t SX1276ReadDio0( void )
{
	
	return HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11);


}
/*!
 * \brief Gets the SX1276 DIO1 hardware pin status
 *
 * \retval status Current hardware pin status [1, 0]
 */
uint8_t SX1276ReadDio1( void )
{

	return HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12);

}

/*!
 * \brief Gets the SX1276 DIO2 hardware pin status
 *
 * \retval status Current hardware pin status [1, 0]
 */
uint8_t SX1276ReadDio2( void )
{

	return 0;

}

/*!
 * \brief Gets the SX1276 DIO3 hardware pin status
 *
 * \retval status Current hardware pin status [1, 0]
 */
uint8_t SX1276ReadDio3( void )
{

		return 0;

}
/*!
 * \brief Gets the SX1276 DIO4 hardware pin status
 *
 * \retval status Current hardware pin status [1, 0]
 */
uint8_t SX1276ReadDio4( void )
{

		return 0;

}

/*!
 * \brief Gets the SX1276 DIO5 hardware pin status
 *
 * \retval status Current hardware pin status [1, 0]
 */
uint8_t SX1276ReadDio5( void )
{

		return 0;

}

/*!
 * \brief Writes the external RxTx pin value
 *
 * \remark see errata note
 *
 * \param [IN] txEnable [1: Tx, 0: Rx]
 */
void SX1276WriteRxTx( uint8_t txEnable )
{


}





