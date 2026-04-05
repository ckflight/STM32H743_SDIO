
#include "main.h"
#include "stdbool.h"
#include "CK_SYSTEM.h"
#include "CK_TIME_HAL.h"
#include "CK_GPIO.h"

SD_HandleTypeDef hsd1;

void SystemClock_Config(void);
void MX_GPIO_Init(void);
uint8_t Wait_SDCARD_Ready(void);
void CK_MICROCARD_AccessCardDetails(void);
uint16_t CK_MICROCARD_GetStartByteOfFile(uint8_t* buffer, uint8_t* filename_to_look);

typedef struct{

    uint16_t BPB_BytsPerSec;	// Byte size of each sector (Offset 11, 2 bytes)
    uint8_t  BPB_SecPerCluster;	// Number of sector of a cluster (Offset 13, 1 byte)
    uint16_t BPB_RsvdSecCnt;	// Number of reserved sectors (Offset 14, 2 bytes)
    uint8_t  BPB_NumFATs;		// Number of FATs (Offset 16, 1 byte)
    uint32_t BPB_TotSec32;		// Total number of sectors (Offset 32, 4 bytes)
    uint32_t BPB_FATSz32;		// Number of sectors used by FAT (Offset 36, 4 bytes)
    uint32_t BPB_RootClus;		// This is set to the cluster number of the first cluster of the root directory, this value should be 2 (Offset 44, 4 bytes)

    uint32_t firstRootDirectorSector;

}microcard_bpb_t;

// Each created file is listed in root directory sector(firstRootDirectorSector)
// with 32 bytes of information for each file
// find the name of the file and decode its 32byte information to fill below variables.
// Later i can create file at desired size as well.

typedef struct{

    uint8_t  DIR_Name[11];		// Name of the DRIVE (Offset 0, 11 bytes)
    uint8_t  DIR_Attr;          // Directory attributes (Offset 11, 1 byte)
    uint16_t DIR_LstAccDate;    // Last access date (Offset 18, 2 bytes)
    uint16_t DIR_FstClusHI;     // High bytes of first cluster. This where the log is started (Offset 20, 2 bytes
    uint16_t DIR_FstClusLo;     // High bytes of first cluster. This where the log is started (Offset 26, 2 bytes
    uint32_t DIR_FileSize;      // File size (Offset 28, 4 bytes)

    uint32_t fileFirstCluster;

    uint32_t firsSectorOfFile;

}microcard_rootdirectory_t;

typedef struct{

	uint8_t init_retry;

	bool is_card_fast;
	bool is_Initialized;
	bool is_dma_ready;
	bool is_log_buffer_full;
	bool is_card_high_capacity;

	uint8_t card_version;
	uint8_t card_speed_clock;

	uint32_t START_SECTOR;
	uint32_t CURRENT_SECTOR;
	uint32_t SECTOR_OFFSET;
	uint32_t INFO_SECTOR;

	bool is_multi_started;
	uint32_t multi_number_of_sector;
	uint32_t max_number_of_sector_write;
	bool is_logging_done;
	bool is_infoSector_write;

	uint32_t TIME_OUT;

}microcard_parameters_t;

microcard_parameters_t card;

microcard_bpb_t boot_sector;

microcard_rootdirectory_t log_file;

microcard_parameters_t card = {
    .init_retry                 	= 50,

    .is_card_fast               	= false,   // Is microcard support fast mode.
    .is_Initialized             	= false,   // Is microcard initialized.
    .is_dma_ready               	= true,    // DMA transfer is ready to start again.
    .is_log_buffer_full         	= false,   // CK_LOG uses this flag to start transfer when its buffer is full.
    .is_card_high_capacity      	= false,   // Is microcard high capacity.

    .card_version                 	= 0,
    .card_speed_clock             	= 0,

    // Start sector to start logging
    .START_SECTOR                	= 0,

    // Current sector the data is logged.
    .CURRENT_SECTOR               	= 0,

    // Each card has an offset. For example to write to sector n, n+offset needed to be sent.
    .SECTOR_OFFSET                	= 0,

    // This sector will be used to write how many sector are written and any later info can be wrtitten.
    // Python code needs to know how much to read.
    .INFO_SECTOR                	= 0,

    .is_multi_started           	= false,   // Is multi dma write started. (CS pin is low during entire logging)

    .multi_number_of_sector     	= 180000,
    .max_number_of_sector_write 	= 0,
    .is_logging_done            	= false,   // When all sectors are written this flag is used.
    .is_infoSector_write        	= false,   // When finally the first sector is filled with logging info this flag is used.

    .TIME_OUT                   	= 100      // some part uses * 10 time_out so 100msec is fine

};

int is_tx_dma_done = 1;
int is_rx_dma_done = 0;

#define NUMBER_OF_SECTORS_TO_WRITE		450

uint8_t writeCache[512 * NUMBER_OF_SECTORS_TO_WRITE] __attribute__ ((aligned (32)));

#define NUMBER_OF_SECTORS_TO_READ	256
uint8_t rx_array[512 * NUMBER_OF_SECTORS_TO_READ] __attribute__ ((aligned (32)));

uint32_t time_buffer[10000];
uint32_t time_buffer_idx = 0;
uint32_t time1, time2, time3, time4, last_time = 0;

uint32_t tx_sector_counter = 0;
uint32_t rx_sector_counter = 0;
uint32_t loop_counter = 0;
uint8_t write_done = 0;
uint8_t read_done = 0;
uint32_t compare_error_count = 0;
uint32_t compare_checked_bytes = 0;

#define LED1_GPIO					GPIOD
#define LED1_GPIO_PIN				12


#define LED2_GPIO					GPIOD
#define LED2_GPIO_PIN				13


typedef enum{

	SEND_DATA,
	WAIT_SOME_TIME,
	READ_SECTORS,
	WAIT_READ_DONE,
	CHECK_SECTORS,
	DONE

}SAMPLING_STATES_E;


SAMPLING_STATES_E sampling_state = SEND_DATA;

uint8_t test_data = 0x01; // data to be written and read

int main(void){

	// Enable I-Cache
	SCB_EnableICache();

	// Enable D-Cache
	SCB_EnableDCache();

	HAL_Init();

	CK_SYSTEM_SetSystemClock(SYSTEM_CLK_480MHz);

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDMMC;
	PeriphClkInitStruct.SdmmcClockSelection = RCC_SDMMCCLKSOURCE_PLL;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

	// Peripheral clock enable */
	__HAL_RCC_SDMMC1_CLK_ENABLE();

	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/**SDMMC1 GPIO Configuration
	PD2     ------> SDMMC1_CMD
	PC11     ------> SDMMC1_D3
	PC10     ------> SDMMC1_D2
	PC12     ------> SDMMC1_CK
	PC9     ------> SDMMC1_D1
	PC8     ------> SDMMC1_D0
	*/

	GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_9
						  |GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_SDIO1;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_SDIO1;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	HAL_NVIC_EnableIRQ(SDMMC1_IRQn);

	hsd1.Instance 					= SDMMC1;
	hsd1.Init.ClockEdge 			= SDMMC_CLOCK_EDGE_RISING;
	hsd1.Init.ClockPowerSave 		= SDMMC_CLOCK_POWER_SAVE_DISABLE;
	hsd1.Init.BusWide 				= SDMMC_BUS_WIDE_4B;
	hsd1.Init.HardwareFlowControl 	= SDMMC_HARDWARE_FLOW_CONTROL_ENABLE;
	hsd1.Init.ClockDiv 				= 0;

	// This function starts hardware, inits card with low speed 1B mode
	// gets card status and reconfigures high speed and 4B mode

	HAL_SD_Init(&hsd1);

	HAL_SD_ConfigSpeedBusOperation(&hsd1, SDMMC_SPEED_MODE_AUTO);

	//CK_MICROCARD_AccessCardDetails();
	card.START_SECTOR = 40000; // I did not put a file on sdcard so an offset is set

	CK_GPIO_ClockEnable(LED1_GPIO);
	CK_GPIO_Init(LED1_GPIO, LED1_GPIO_PIN, CK_GPIO_OUTPUT_PP, CK_GPIO_NOAF, CK_GPIO_VERYHIGH, CK_GPIO_NOPUPD);
	CK_GPIO_SetPin(LED1_GPIO, LED1_GPIO_PIN);

	CK_GPIO_Init(LED2_GPIO, LED2_GPIO_PIN, CK_GPIO_OUTPUT_PP, CK_GPIO_NOAF, CK_GPIO_VERYHIGH, CK_GPIO_NOPUPD);
	CK_GPIO_SetPin(LED2_GPIO, LED2_GPIO_PIN);

	// With single block write 10000 block took 31.4 seconds
	// Multiwrite 512 times x ( 450 multiwrite x 512 byte per sector ) takes 6.44 sec so 18 MB/sec

	// Writing fast is about sending data in big size at once
	// If we write with dma one block at a time then it still need around 3millisec to open close write

	time1 = CK_TIME_GetMilliSec();

	for(int i = 0; i < 512 * NUMBER_OF_SECTORS_TO_WRITE; i++){

		writeCache[i] = test_data;

	}

#define USE_DMA

	while (1){

		switch(sampling_state){

		case SEND_DATA:

			// 512 times x ( 450 multiwrite x 512 byte per sector ) takes 6.44 sec so 18 MB/sec
			if(loop_counter < 512 && !write_done){

				#if defined(USE_DMA)

				if(is_tx_dma_done){

					is_tx_dma_done = 0;

					CK_GPIO_TogglePin(LED1_GPIO, LED1_GPIO_PIN);

					loop_counter++;

					Wait_SDCARD_Ready(); // This is very important!!!

					// I put some different data at some point to see if checking writing really works
					if(loop_counter == 250){
						for(int i = 0; i < 512 * NUMBER_OF_SECTORS_TO_WRITE; i++){

							writeCache[i] = 0xAB;
						}
					}
					// Data has to be corrected again
					if(loop_counter == 251){
						for(int i = 0; i < 512 * NUMBER_OF_SECTORS_TO_WRITE; i++){

							writeCache[i] = test_data;
						}
					}

					// Clean before tx operation when dcache is enabled
					// Buffer is filled by cpu to cache so flush it to sram with cleandcache method for dma to send it to peripheral
					SCB_CleanDCache_by_Addr((uint32_t*)writeCache, (512 * NUMBER_OF_SECTORS_TO_WRITE));

					HAL_SD_WriteBlocks_DMA(&hsd1, writeCache, card.START_SECTOR + tx_sector_counter, NUMBER_OF_SECTORS_TO_WRITE);

					tx_sector_counter += NUMBER_OF_SECTORS_TO_WRITE;

				}

				#else

				CK_GPIO_TogglePin(LED1_GPIO, LED1_GPIO_PIN);

				loop_counter++;

				tx_sector_counter += NUMBER_OF_SECTORS_TO_WRITE;

				Wait_SDCARD_Ready();

				HAL_SD_WriteBlocks(&hsd1, writeCache, card.START_SECTOR + tx_sector_counter, NUMBER_OF_SECTORS_TO_WRITE, 100);

				#endif

			}
			else{
				if(!write_done){
					time2 = CK_TIME_GetMilliSec() - time1;
					write_done = 1;
					CK_GPIO_ClearPin(LED1_GPIO, LED1_GPIO_PIN);
					sampling_state = WAIT_SOME_TIME;
				}
			}
			break;
		case WAIT_SOME_TIME:

			uint32_t timer1 = CK_TIME_GetMilliSec();
			while(CK_TIME_GetMilliSec() - timer1 < 100);

			sampling_state = READ_SECTORS;
			break;

		case READ_SECTORS:

			CK_GPIO_TogglePin(LED2_GPIO, LED2_GPIO_PIN);

			Wait_SDCARD_Ready(); // This is very important!!!

			HAL_SD_ReadBlocks_DMA(&hsd1, rx_array, card.START_SECTOR + rx_sector_counter, NUMBER_OF_SECTORS_TO_READ);

			sampling_state = WAIT_READ_DONE;

			break;

		case WAIT_READ_DONE:

			if(is_rx_dma_done == 1){
				sampling_state = CHECK_SECTORS;
				is_rx_dma_done = 0;
			}
			else{
				sampling_state = WAIT_READ_DONE;
			}
			break;

		case CHECK_SECTORS:

			SCB_InvalidateDCache_by_Addr((uint32_t*)rx_array, (512 * NUMBER_OF_SECTORS_TO_READ));

			// Compare received array with tx array
			for(uint32_t i = 0; i < (512 * NUMBER_OF_SECTORS_TO_READ); i++){
				if(rx_array[i] != test_data){
					compare_error_count++;
				}
				compare_checked_bytes++;
			}


			rx_sector_counter += NUMBER_OF_SECTORS_TO_READ;

			if(rx_sector_counter >= tx_sector_counter){
				read_done = 1;
				CK_GPIO_ClearPin(LED2_GPIO, LED2_GPIO_PIN);
				sampling_state = DONE;
			}
			else{

				sampling_state = READ_SECTORS;
			}

			break;

		case DONE:

			CK_GPIO_TogglePin(LED1_GPIO, LED1_GPIO_PIN);
			CK_GPIO_TogglePin(LED2_GPIO, LED2_GPIO_PIN);
			CK_TIME_DelayMilliSec(100);

			break;
		}
	}
}

void SDMMC1_IRQHandler(void){
	HAL_SD_IRQHandler(&hsd1);
}

void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd){
	is_rx_dma_done = 1;
}

void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd){
	is_tx_dma_done = 1;
}

void HAL_SD_ErrorCallback(SD_HandleTypeDef *hsd){
	Error_Handler();
}

void CK_MICROCARD_AccessCardDetails(void){

    // All details are in Microsoft File Allocation Table Specification.pdf
    // First sector has bpb information and from there,
    // number of the first sector of the root file is calculated.

    // BPB sector 0 (this 0 is actually 0 + offset) so the offset must be found to operate correctly.
    // BPB first byte is 0xEB so read until reaching that byte to determine the offset of the card.
    // One card has 2048, other has 8192 so 10000 read should be fine.

	uint32_t sector_offset = 0; // No need to start searching from 0 since it is 2048 or around 8192

    while(sector_offset < 10000){

    	HAL_SD_ReadBlocks(&hsd1, rx_array, sector_offset, 1, 100);

		if(rx_array[0] != 0xEB && rx_array[2] != 0x90){

			sector_offset++;

		}
		else{

			card.SECTOR_OFFSET = sector_offset;
			break;
		}
    }

    if(card.SECTOR_OFFSET == 0){
    	card.SECTOR_OFFSET = 35760;
    	while(1);
    }
    // Read the boot sector
    HAL_SD_ReadBlocks(&hsd1, rx_array, card.SECTOR_OFFSET, 1, 100);

    /*
     uint16_t BPB_BytsPerSec;     	// Byte size of each sector (Offset 11 2 bytes)
     uint8_t  BPB_SecPerCluster;	// Number of sector of a cluster (Offset 13 1 byte)
     uint16_t BPB_RsvdSecCnt;     	// Number of reserved sectors (Offset 14 2 bytes)
     uint8_t  BPB_NumFATs;        	// Number of FATs (Offset 16 1 byte)
     uint32_t BPB_TotSec32;        	// Total number of sectors (Offset 32 4 bytes)
     uint32_t BPB_FATSz32;        	// Number of sectors used by FAT (Offset 36 4 bytes)
     */

    // Boot sector bytes are in little endian format meaning LSB first
    boot_sector.BPB_BytsPerSec      = (uint16_t)(rx_array[12] << 8 | rx_array[11]);

    boot_sector.BPB_SecPerCluster 	= (uint8_t)rx_array[13];

    boot_sector.BPB_RsvdSecCnt    	= (uint16_t)(rx_array[15] << 8 | rx_array[14]);

    boot_sector.BPB_NumFATs         = (uint8_t)rx_array[16];

    boot_sector.BPB_TotSec32      	= (uint32_t)(rx_array[35] << 24 | rx_array[34] << 16 | rx_array[33] << 8 | rx_array[32]);

    boot_sector.BPB_FATSz32			= (uint32_t)(rx_array[39] << 24 | rx_array[38] << 16 | rx_array[37] << 8 | rx_array[36]);

    boot_sector.BPB_RootClus      	= (uint32_t)(rx_array[47] << 24 | rx_array[46] << 16 | rx_array[45] << 8 | rx_array[44]);

    boot_sector.firstRootDirectorSector = boot_sector.BPB_RsvdSecCnt + (boot_sector.BPB_FATSz32 * boot_sector.BPB_NumFATs);

    // Read the first root directory sector where info of files are stored.
    HAL_SD_ReadBlocks(&hsd1, rx_array, card.SECTOR_OFFSET + boot_sector.firstRootDirectorSector, 1, 100);

    /*
     uint8_t  DIR_Name[11];		// Name of the DRIVE (Offset 0, 11 bytes)
     uint8_t  DIR_Attr;			// Directory attributes (Offset 11, 1 byte)
     uint16_t DIR_LstAccDate;   // Last access date (Offset 18, 2 bytes)
     uint16_t DIR_FstClusHI;    // High bytes of first cluster. This where the log is started (Offset 20, 2 bytes
     uint16_t DIR_FstClusLo;    // High bytes of first cluster. This where the log is started (Offset 26, 2 bytes
     uint32_t DIR_FileSize;     // File size (Offset 28, 4 bytes)
     */

    // Even if the name of file is flight_log.txt this is how it is stored
    uint8_t filename_to_look[11] = {'F','L','I','G','H','T','~','1','T','X','T'};
    uint16_t start_byte_of_file = CK_MICROCARD_GetStartByteOfFile(rx_array, filename_to_look);

    for(int i = 0; i < 11; i++){
        log_file.DIR_Name[i] = rx_array[start_byte_of_file + i];
    }

    log_file.DIR_Attr = rx_array[start_byte_of_file + 11];

    log_file.DIR_LstAccDate = (uint16_t)(rx_array[start_byte_of_file + 19] << 8 | rx_array[start_byte_of_file + 18]);

    log_file.DIR_FstClusHI = (uint16_t)(rx_array[start_byte_of_file + 21] << 8 | rx_array[start_byte_of_file + 20]);

    log_file.DIR_FstClusLo = (uint16_t)(rx_array[start_byte_of_file + 27] << 8 | rx_array[start_byte_of_file + 26]);

    log_file.DIR_FileSize = (uint32_t)(rx_array[start_byte_of_file + 31] << 24 | rx_array[start_byte_of_file + 30] << 16 | rx_array[start_byte_of_file + 29] << 8 | rx_array[start_byte_of_file + 28]);

    log_file.fileFirstCluster = (uint32_t)(log_file.DIR_FstClusHI << 16 | log_file.DIR_FstClusLo);


    log_file.firsSectorOfFile = ((log_file.fileFirstCluster - boot_sector.BPB_RootClus) * boot_sector.BPB_SecPerCluster) + boot_sector.firstRootDirectorSector;

    // This part is for precaution
    // If for some reason above info are corrupted
    // set these parameters to some safe values.
    if(log_file.firsSectorOfFile == 0){
        // I am setting to this value rather than 32832 to understand that this loop sets it.
        log_file.firsSectorOfFile = 32900;
    }
    if(card.SECTOR_OFFSET == 0){
        card.SECTOR_OFFSET = 8192;
    }

    card.START_SECTOR = log_file.firsSectorOfFile + card.SECTOR_OFFSET;

    // First sector will be used to store data needed to be known by python log-analyzer code.
    // I moved it to second sector.
    card.INFO_SECTOR = card.START_SECTOR;

    // Next sector will be the start of logging.
    card.START_SECTOR++;

    // Based on the file size calculate maximum number of sector can be written.
    if(boot_sector.BPB_BytsPerSec)card.max_number_of_sector_write = log_file.DIR_FileSize / boot_sector.BPB_BytsPerSec;

}

uint16_t CK_MICROCARD_GetStartByteOfFile(uint8_t* buffer, uint8_t* filename_to_look){

    int states = 0;
    int start_byte_of_file = 0;

    for(int index = 0; index < 512; index++){

        uint8_t current_byte = buffer[index];

        switch(states){
            case 0:
                if(current_byte == filename_to_look[0]){
                    states++;
                    start_byte_of_file = index;
                    break;
                }
                start_byte_of_file = 0;
                states = 0;
                break;

            case 1:
                if(current_byte == filename_to_look[1]){
                    states++;
                    break;
                }
                start_byte_of_file = 0;
                states = 0;
                break;

            case 2:
                if(current_byte == filename_to_look[2]){
                    states++;
                    break;
                }
                start_byte_of_file = 0;
                states = 0;
                break;

            case 3:
                if(current_byte == filename_to_look[3]){
                    states++;
                    break;
                }
                start_byte_of_file = 0;
                states = 0;
                break;

            case 4:
                if(current_byte == filename_to_look[4]){
                    states++;
                    break;
                }
                start_byte_of_file = 0;
                states = 0;
                break;

            case 5:
                if(current_byte == filename_to_look[5]){
                    states++;
                    break;
                }
                start_byte_of_file = 0;
                states = 0;
                break;

            case 6:
                if(current_byte == filename_to_look[6]){
                    states++;
                    break;
                }
                start_byte_of_file = 0;
                states = 0;
                break;

            case 7:
                if(current_byte == filename_to_look[7]){
                    states++;
                    break;
                }
                start_byte_of_file = 0;
                states = 0;
                break;

            case 8:
                if(current_byte == filename_to_look[8]){
                    states++;
                    break;
                }
                start_byte_of_file = 0;
                states = 0;
                break;

            case 9:
                if(current_byte == filename_to_look[9]){
                    states++;
                    break;
                }
                start_byte_of_file = 0;
                states = 0;
                break;

            case 10:
                if(current_byte == filename_to_look[10]){
                    states = 0;
                    return start_byte_of_file;
                    break;
                }
                start_byte_of_file = 0;
                states = 0;
                break;

        }
    }

    return 0;

}

#define SD_TIMEOUT             ((uint32_t)0x00100000U)
uint8_t Wait_SDCARD_Ready(void){

  uint32_t loop = SD_TIMEOUT;

  /* Wait for the Erasing process is completed */
  /* Verify that SD card is ready to use after the Erase */
  while(loop > 0)
  {
    loop--;
    if(HAL_SD_GetCardState(&hsd1) == HAL_SD_CARD_TRANSFER)
    {
        return HAL_OK;
    }
  }
  return HAL_ERROR;
}

void Error_Handler(void){

	__disable_irq();
	while (1){


	}
}
