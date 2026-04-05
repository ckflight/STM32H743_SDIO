
#include "CK_SPI.h"
#include "CK_TIME_HAL.h"
#include "CK_GPIO.h"

typedef struct{

    uint32_t timeout;

    uint32_t spi1_timeout;
    int spi1_init;

    uint32_t spi2_timeout;
    int spi2_init;

    uint32_t spi3_timeout;
    int spi3_init;

    uint32_t spi4_timeout;
    int spi4_init;

}SPI_t;

SPI_t spi_variables = {

    .timeout      = 0,

    .spi1_timeout = 0,
    .spi1_init    = 0,

    .spi2_timeout = 0,
    .spi2_init    = 0,

    .spi3_timeout = 0,
    .spi3_init    = 0,

	.spi4_timeout = 0,
	.spi4_init    = 0,

};

#define SPI_TIMEOUT         500

void CK_SPI_Init(SPI_TypeDef* spi_n){

	SPI_TypeDef* SPI_ = spi_n;

	if(SPI_ == SPI1){

		__HAL_RCC_SPI1_CLK_ENABLE();

		// SPI1 GPIO Configuration
		// PA5     ------> SPI1_SCK
		// PA6     ------> SPI1_MISO
		// PD7     ------> SPI1_MOSI

		CK_GPIO_ClockEnable(GPIOA);
		CK_GPIO_ClockEnable(GPIOD);

		CK_GPIO_Init(GPIOA, 5, CK_GPIO_AF_PP, CK_GPIO_AF5, CK_GPIO_VERYHIGH, CK_GPIO_NOPUPD);
		CK_GPIO_Init(GPIOA, 6, CK_GPIO_AF_PP, CK_GPIO_AF5, CK_GPIO_VERYHIGH, CK_GPIO_NOPUPD);
		CK_GPIO_Init(GPIOD, 7, CK_GPIO_AF_PP, CK_GPIO_AF5, CK_GPIO_VERYHIGH, CK_GPIO_NOPUPD);

	}

	/*
	SPI_HandleTypeDef hspi_;

	// SPI parameter configuration
	hspi_.Instance 							= SPI_;
	hspi_.Init.Mode 						= SPI_MODE_MASTER;
	hspi_.Init.Direction 					= SPI_DIRECTION_2LINES;
	hspi_.Init.DataSize 					= SPI_DATASIZE_8BIT;
	hspi_.Init.CLKPolarity 					= SPI_POLARITY_LOW;
	hspi_.Init.CLKPhase 					= SPI_PHASE_1EDGE;
	hspi_.Init.NSS 							= SPI_NSS_SOFT;
	hspi_.Init.BaudRatePrescaler 			= SPI_BAUDRATEPRESCALER_32;
	hspi_.Init.FirstBit 					= SPI_FIRSTBIT_MSB;
	hspi_.Init.TIMode 						= SPI_TIMODE_DISABLE;
	hspi_.Init.CRCCalculation 				= SPI_CRCCALCULATION_DISABLE;
	hspi_.Init.CRCPolynomial 				= 0x0;
	//hspi_.Init.NSSPMode 					= SPI_NSS_PULSE_ENABLE;
	//hspi_.Init.NSSPolarity 				= SPI_NSS_POLARITY_LOW;
	hspi_.Init.FifoThreshold 				= SPI_FIFO_THRESHOLD_01DATA;
	hspi_.Init.TxCRCInitializationPattern 	= SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
	hspi_.Init.RxCRCInitializationPattern 	= SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
	hspi_.Init.MasterSSIdleness 			= SPI_MASTER_SS_IDLENESS_00CYCLE;
	hspi_.Init.MasterInterDataIdleness 		= SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
	hspi_.Init.MasterReceiverAutoSusp 		= SPI_MASTER_RX_AUTOSUSP_DISABLE;
	hspi_.Init.MasterKeepIOState 			= SPI_MASTER_KEEP_IO_STATE_DISABLE;
	hspi_.Init.IOSwap 						= SPI_IO_SWAP_DISABLE;

	if (HAL_SPI_Init(&hspi_) == HAL_OK){

		if(SPI_ == SPI1){

			hspi1 = hspi_;
			spi_variables.spi1_init    = 1;
			spi_variables.spi1_timeout = 0;
		}

	}
	*/

	SPI_->CR1 |= 1u << 12; // SSI bit (might not be needed)

	SPI_->CFG1 |= 1u << 28 | 7u << 0; // SPI clock, 8 bit data frame,

	// AFCNTR, SSM, clock polarity sck 0 at idle, cpha low, master, spi ti, default msb first, default full duplex
	SPI_->CFG2 |= 1u << 31 | 1u << 26 | 0u << 25 | 0u << 24 | 1u << 22 | 0u << 19;


}

void CK_SPI_Enable(SPI_TypeDef* SPI_){

	SPI_->CR1 |= 1u << 0;


}
void CK_SPI_Disable(SPI_TypeDef* SPI_){

	SPI_->CR1 &= ~(1u << 0);

}

void CK_SPI_EnableDMA(SPI_TypeDef* SPI_){


}

void CK_SPI_DisableDMA(SPI_TypeDef* SPI_){

}

uint8_t CK_SPI_WriteRegister(uint8_t reg, uint8_t data, SPI_TypeDef* SPI_, GPIO_TypeDef* GPIOx_CS, uint16_t cs_pin){

	uint8_t rx_data = 0;

	CK_GPIO_ClearPin(GPIOx_CS, cs_pin);

	CK_SPI_Transfer(SPI_, reg);

	rx_data = CK_SPI_Transfer(SPI_, data);

	CK_GPIO_SetPin(GPIOx_CS, cs_pin);

	return rx_data;

}

void CK_SPI_ReadRegister(uint8_t reg, SPI_TypeDef* SPI_, GPIO_TypeDef* GPIOx_CS, uint16_t cs_pin, uint8_t* dataIn, int count){

	CK_GPIO_ClearPin(GPIOx_CS, cs_pin);

	int is_register_sent = 0;
	int c = count + 1;
	uint8_t rx_dummy_data = 0;
	UNUSED(rx_dummy_data);

	SPI_->CR2 |= c;	 		// tx size is
	SPI_->CR1 |= 1u << 0; 	// enable spi
	SPI_->CR1 |= 1u << 9; 	// master tx start

	while(c--){

		// TXP Flag
		spi_variables.timeout = SPI_TIMEOUT;

		while((SPI_->SR & (1u << 1)) == 0){
			if(--spi_variables.timeout == 0x00){
				CK_SPI_TimeOutCounter(SPI_);
			}
		}

		if(is_register_sent == 0){
			*((__IO uint8_t *)&SPI_->TXDR) = reg | 0x80;
		}
		else{
			*((__IO uint8_t *)&SPI_->TXDR) = 0xFF;
		}

		// RXP Flag
		spi_variables.timeout = SPI_TIMEOUT;

		while((SPI_->SR & (1u << 0)) == 0){
			if(--spi_variables.timeout == 0x00){
				CK_SPI_TimeOutCounter(SPI_);
			}
		}

		if(is_register_sent == 0){
			is_register_sent = 1;
			rx_dummy_data = *((__IO uint8_t *)&SPI_->RXDR);
		}
		else{
			*dataIn++ = *((__IO uint8_t *)&SPI_->RXDR);
		}

	}

	while((SPI_->SR & (1u << 3)) == 0);

	SPI_->IFCR |= 1u << 4; // Clear txtf
	SPI_->CR1 &= ~(1u << 0); // disable spi

	CK_GPIO_SetPin(GPIOx_CS, cs_pin);
}


void CK_SPI_ReadRegisterMulti(uint8_t reg, SPI_TypeDef* SPI_, GPIO_TypeDef* GPIOx_CS, uint16_t cs_pin, uint8_t* dataIn, int count){

	CK_GPIO_ClearPin(GPIOx_CS, cs_pin);

	CK_SPI_Transfer(SPI_, reg | 0x80);

	while (count--) {

		*dataIn++ =  CK_SPI_Transfer(SPI_, 0xFF);
	}

	CK_GPIO_SetPin(GPIOx_CS, cs_pin);

}

uint8_t CK_SPI_Transfer(SPI_TypeDef* SPI_, uint8_t data){

	SPI_->CR2 |= 1;		 	// tx size is 1.
	SPI_->CR1 |= 1u << 0; 	// enable spi
	SPI_->CR1 |= 1u << 9; 	// master tx start.

	// TXP Flag
	spi_variables.timeout = SPI_TIMEOUT;
	while((SPI_->SR & (1u << 1)) == 0){
		if(--spi_variables.timeout == 0x00){
			CK_SPI_TimeOutCounter(SPI_);
			return 1;
		}
	}

	*((__IO uint8_t *)&SPI_->TXDR) = data;

	// RXP Flag
	spi_variables.timeout = SPI_TIMEOUT;

	while((SPI_->SR & (1u << 0)) == 0){
	//while(((SPI_->SR & (SPI_FLAG_RXWNE | SPI_FLAG_FRLVL)) == 0UL)){
		if(--spi_variables.timeout == 0x00){
			CK_SPI_TimeOutCounter(SPI_);
			return 1;
		}
	}

	uint8_t rx_data = *((__IO uint8_t *)&SPI_->RXDR);

	// EOT
	while((SPI_->SR & (1u << 3)) == 0);

	SPI_->IFCR |= 1u << 4; // Clear txtf
	SPI_->CR1 &= ~(1u << 0); // disable spi

	return rx_data;

}

uint8_t CK_SPI_WaitTransfer(SPI_TypeDef* SPI_){

	spi_variables.timeout = SPI_TIMEOUT;

	//while(((SPI_)->SR & (CK_SPIx_SR_TXE | CK_SPIx_SR_RXNE)) == 0 || ((SPI_)->SR & CK_SPIx_SR_BSY)){
	while((SPI_->SR & (1u << 1)) == 0){
		if(--spi_variables.timeout == 0x00){
			CK_SPI_TimeOutCounter(SPI_);
			return 1;
		}
	}

	return 0;
}

int CK_SPI_CheckInitialized(SPI_TypeDef* SPI_){

    int res;
    if(SPI_ == SPI1){
        res = spi_variables.spi1_init;
    }
    else{
        res = 2; // Error
    }

	return res;

}

void CK_SPI_TimeOutCounter(SPI_TypeDef* spi){

	if(spi == SPI1){
        spi_variables.spi1_timeout++;
    }
}

uint32_t CK_SPI_GetTimeOut(SPI_TypeDef* spi){

    uint32_t res;

    if(spi == SPI1){
        res = spi_variables.spi1_timeout;
    }

    return res;
}

void CK_SPI_ResetTimeOut(SPI_TypeDef* spi){

    if(spi == SPI1){
        spi_variables.spi1_timeout = 0;
    }

}
