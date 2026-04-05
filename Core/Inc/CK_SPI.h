
#ifndef INC_CK_SPI_H_
#define INC_CK_SPI_H_

#include "stm32h7xx.h"

SPI_HandleTypeDef hspi1;

void CK_SPI_Init(SPI_TypeDef* spi_n);

void CK_SPI_Enable(SPI_TypeDef* SPI_);

void CK_SPI_Disable(SPI_TypeDef* SPI_);

void CK_SPI_EnableDMA(SPI_TypeDef* spi_n);

void CK_SPI_DisableDMA(SPI_TypeDef* spi_n);

void CK_SPI_ReadRegister(uint8_t reg, SPI_TypeDef* SPI_, GPIO_TypeDef* GPIOx_CS, uint16_t cs_pin, uint8_t* dataIn, int count);

uint8_t CK_SPI_WriteRegister(uint8_t reg, uint8_t data, SPI_TypeDef* SPIx, GPIO_TypeDef* GPIOx_CS, uint16_t cs_pin);

void CK_SPI_ReadRegisterMulti(uint8_t reg, SPI_TypeDef* SPIx, GPIO_TypeDef* GPIOx_CS, uint16_t cs_pin, uint8_t* dataIn, int count);

uint8_t CK_SPI_Transfer(SPI_TypeDef* SPIx, uint8_t data);

uint8_t CK_SPI_WaitTransfer(SPI_TypeDef* SPI_);

int CK_SPI_CheckInitialized(SPI_TypeDef* SPIn);

void CK_SPI_TimeOutCounter(SPI_TypeDef* spi);

uint32_t CK_SPI_GetTimeOut(SPI_TypeDef* spi);

void CK_SPI_ResetTimeOut(SPI_TypeDef* spi);

#endif /* INC_CK_SPI_H_ */
