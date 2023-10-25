/*
 * Flash.h
 *
 *  Created on: 12 Oct 2023
 *      Author: DUNG
 */

#ifndef FLASH_H_
#define FLASH_H_

#include"MKL46Z4.h"

#define FCMD_WRITE_WORD            (0x06u)
#define FCMD_ERASE_SECTOR          (0x09u)

#define FLASH_SEC_SIZE_BYTE        (1024u)
#define FLASH_SEC_SIZE_WORD        ((uint32_t) (FLASH_SEC_SIZE_BYTE / 4u))
#define FLASH_GET_ADDR(sector)     ((uint32_t) ((sector) * FLASH_SEC_SIZE_BYTE))

void Flash_WriteWord(uint32_t word, uint32_t address);

void Flash_EraseSector(uint8_t sectorNum);

void Flash_ReadSector(uint8_t sectorNum, uint32_t * SectorBuffer);


#endif /* FLASH_H_ */
