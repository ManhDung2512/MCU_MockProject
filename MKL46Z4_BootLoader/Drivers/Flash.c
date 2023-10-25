/*
 * Flash.c
 *
 *  Created on: 12 Oct 2023
 *      Author: DUNG
 */

#include "Flash.h"

void Flash_WriteWord(uint32_t Word, uint32_t address)
{
	while( (FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0u);

	if( ((FTFA->FSTAT & FTFA_FSTAT_ACCERR_MASK) != 0u) || ((FTFA->FSTAT & FTFA_FSTAT_FPVIOL_MASK) != 0u))
	{
		FTFA->FSTAT = 0x30u;
	}

	// write command
	FTFA->FCCOB0 = FCMD_WRITE_WORD;
	FTFA->FCCOB1 = (uint8_t)(address >> 16);
	FTFA->FCCOB2 = (uint8_t)(address >> 8);
	FTFA->FCCOB3 = (uint8_t)(address);
	FTFA->FCCOB7 = (uint8_t)(Word);
	FTFA->FCCOB6 = (uint8_t)(Word >> 8);
	FTFA->FCCOB5 = (uint8_t)(Word >> 16);
	FTFA->FCCOB4 = (uint8_t)(Word >> 24);

	FTFA->FSTAT  = 0x80u;
}

void Flash_EraseSector(uint8_t sectorNum)
{
	uint32_t address = FLASH_GET_ADDR(sectorNum);
	while( (FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0u);

	if(((FTFA->FSTAT & FTFA_FSTAT_ACCERR_MASK) != 0u)|| ((FTFA->FSTAT & FTFA_FSTAT_FPVIOL_MASK )!= 0u))
	{
		FTFA->FSTAT = 0x30u;
	}

	// write command
	FTFA->FCCOB0 = FCMD_ERASE_SECTOR;
	FTFA->FCCOB1 = (uint8_t)(address >> 16);
	FTFA->FCCOB2 = (uint8_t)(address >> 8);
	FTFA->FCCOB3 = (uint8_t)(address);

	FTFA->FSTAT  = 0x80u;
}
void Flash_ReadSector(uint8_t sectorNum, uint32_t * SectorBuffer)
{
	uint32_t * wordPointer = (uint32_t*) FLASH_GET_ADDR(sectorNum);
	for(uint32_t i = 0; i < FLASH_SEC_SIZE_WORD; i++)
	{
		SectorBuffer[i] = wordPointer[i];
	}
}
