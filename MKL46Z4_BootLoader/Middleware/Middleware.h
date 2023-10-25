/*
 * Middleware.h
 *
 *  Created on: 12 Oct 2023
 *      Author: DUNG
 */

#ifndef MIDDLEWARE_H_
#define MIDDLEWARE_H_

#include"MKL46Z4.h"
#include"Port.h"
#include"Gpio.h"
#include"CLock.h"
#include"Flash.h"
#include"UART.h"

#define FLASH_FIRM_OFFSET_SEC       (128u)
#define FLASH_BANK_OFFSET           (FLASH_FIRM_OFFSET_SEC * FLASH_SEC_SIZE_BYTE)
#define FLASH_BANK_SIZE_SEC         (50u)
#define FLASH_BANK_START_SEC(x)     (FLASH_FIRM_OFFSET_SEC + (x) * (FLASH_BANK_SIZE_SEC)) /*x can be 0 or 1*/
#define FLASH_BANK_END_SEC(x)       (FLASH_BANK_START_SEC(x) + FLASH_BANK_SIZE_SEC)

#define FLASH_BANK_SIZE             ((uint32_t)(FLASH_SEC_SIZE_BYTE * FLASH_BANK_SIZE_SEC)) /*convert from sector to uint32_t address*/
#define FLASH_FIRM_START_ADDR(x)    ((uint32_t)((x) * FLASH_BANK_SIZE) + FLASH_BANK_OFFSET) /*x can be 0 or 1*/
#define FLASH_FIRM_END_ADDR(x)      ((uint32_t)(FLASH_FIRM_START(x) + FLASH_BANK_SIZE)

#define RESET_HANDLER_OFFSET        (0x04u)
#define USER_MSP_ADDR(x)            (FLASH_FIRM_START_ADDR(x))            /* start addr is where msp value is stored */
#define USER_RESET_ADDR(x)          (FLASH_FIRM_START_ADDR(x) +  RESET_HANDLER_OFFSET)  /* next addr is Reset Handler */
#define USER_MSP_VALUE(x)           (*(volatile uint32_t *)USER_MSP_ADDR(x))
#define USER_RESET_VALUE(x)         (*(volatile uint32_t *)USER_RESET_ADDR(x))


// stored default firmware choice at 0x10000u in FLASH
// if store 0 run firmware1, store 1 run firmware 0
#define BOOT_DEFAULT_FIRM_SECTOR    (255u)
#define BOOT_DEFAULT_FIRM_ADDR      (0x3FFFCu)
#define BOOT_DEFAULT_FIRM         	(*(volatile uint32_t *)BOOT_DEFAULT_FIRM_ADDR)

#define SW2_PORT            PORTC
#define SW2_GPIO            GPIOC
#define SW2_PIN             12u

typedef enum
{
	BUTTON_PRESSED   =   0x00u,
	BUTTON_RELEASED  =   0x01u

}Button_StateType;


typedef enum
{
	FIRMWARE1    = 0u,
	FIRMWARE2    = 1u,
}FirmwareType;

typedef enum
{
	CMD_LOAD_FIRM     =  'L',
	CMD_ERASE_FIRM    =  'E',
	CMD_RUN_FIRM      =  'R',
}UserCommandType;

typedef enum{
    SREC_FLAG_DEFAULT   = 0U,
    SREC_FLAG_FIRST     = 1U,
    SREC_FLAG_TYPE      = 2U,
    SREC_FLAG_BYTECOUNT = 3U,
    SREC_FLAG_ADDRESS   = 4U,
    SREC_FLAG_DATA      = 5U,
    SREC_FLAG_CHECKSUM  = 6U,
} Srec_Flag;

typedef enum{
    BOOT_DEFAULT     =    0u,
    BOOT_LOAD_WAIT   =    1u,
    BOOT_LOAD        =    2u,
    BOOT_ERASE_WAIT  =    3u,
    BOOT_ERASE       =    4u,
    BOOT_RUN_WAIT    =    5u,
    BOOT_RUN         =    6u,
    BOOT_ERROR       =    7u,
} Boot_StateType;

typedef enum{
    BOOT_ERR_DEFAULT     =    0u,
    BOOT_ERR_CMD         =    1u,
    BOOT_ERR_FIRMBANK    =    2u,
	BOOT_ERR_CHECKSUM    =    3u,
} Boot_ErrorType;

typedef struct{
    uint8_t SrecType;
    uint8_t ByteCount;
    uint8_t AddrNum;
    uint32_t Addr;
    uint8_t Data;
    uint8_t CheckSum;
} SrecType;


void BootSendACK(Boot_StateType BootState);
void BootErrorHanlder(Boot_ErrorType BootErrorFlag);
void UART0_Init();
void SW2_Init();
void SW2_DeInit();
void Boot_Init();
void Boot_DeInit();
void BootChangeDefaultFirm(FirmwareType FirmBankSelect);
void Boot_JumpToFirm(FirmwareType userFirmware);
Button_StateType SW2_ReadState();

#endif /* MIDDLEWARE_H_ */
