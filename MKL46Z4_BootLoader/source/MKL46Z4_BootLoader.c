/*
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    MKL46Z4_BootLoader.c
 * @brief   Application entry point.
 */
#include "MKL46Z4.h"
#include "Middleware.h"

volatile uint8_t Data;

volatile FirmwareType FirmBankSelect = 0u;
volatile Boot_ErrorType BootErrorFlag = BOOT_ERR_DEFAULT;
volatile Boot_StateType BootState;
volatile Srec_Flag Srec_LastState = SREC_FLAG_DEFAULT;

volatile uint8_t ByteCount_Flag;
volatile uint8_t Addr_Flag;
volatile uint8_t Data_Flag;
volatile uint8_t Word_Flag;
volatile uint8_t CheckSum_Flag;

volatile uint32_t Store_Addr = 0u;
volatile uint32_t Store_Word = 0u;
volatile uint8_t Byte_Counter = 0u;
volatile uint8_t Store_Flag = 0u;
volatile uint8_t LoadDone_Flag = 0u;
volatile uint8_t CheckLine_Flag = 0u;

SrecType CurLine;

static inline uint8_t ChartoDec(uint8_t Character)
{
	uint8_t retVal = 0u;
	// '0' - '9'
	if ( ('0' <= Character) && (Character <= '9') )
	{
		retVal = Character - '0';
	}
	else if ( ('A' <= Character) && (Character <= 'F') )
	{
		retVal = Character - 'A' + 10u;
	}
	return retVal;
}

static inline void UART_SrecHandler()
{
    switch (Srec_LastState)
    {
        case SREC_FLAG_FIRST:
            CurLine.SrecType = ChartoDec(Data);
            if(1 <= CurLine.SrecType && CurLine.SrecType <= 3)
            {
                CurLine.AddrNum = CurLine.SrecType + 1;
                CurLine.ByteCount = 0;
                Byte_Counter = CurLine.AddrNum;
                Srec_LastState++;
            }
            else
            {
                Srec_LastState = SREC_FLAG_DEFAULT;
                // 7 8 9 mean end of Srec file
                // then change boot state to default because finish loading
                if(CurLine.SrecType >= 7)
                {
                	LoadDone_Flag = 1;
                }
                // else (0 4 5) is just optional so skip
                Srec_LastState = SREC_FLAG_DEFAULT;
            }
            break;
        case SREC_FLAG_TYPE:

            CurLine.ByteCount = (CurLine.ByteCount << 4) + ChartoDec(Data);
            ByteCount_Flag++;

            if(ByteCount_Flag == 2u)
            {
                Srec_LastState++;
                ByteCount_Flag = 0u;
                /*Don't need to clear CurLine.addr to 0 beacause it is handle by check sum*/
                //CurLine.Addr = 0u;
                CurLine.CheckSum = CurLine.ByteCount;
            }
            break;
        case SREC_FLAG_BYTECOUNT:

                CurLine.Addr = (CurLine.Addr << 4) + ChartoDec(Data);
                Addr_Flag++;

                if(Addr_Flag == CurLine.AddrNum * 2)
                {
                    Srec_LastState++;
                    Addr_Flag = 0u;
                    CurLine.Data = 0u;
                    Store_Addr = CurLine.Addr;
                }
                break;
        case SREC_FLAG_ADDRESS:
            CurLine.Data = (CurLine.Data << 4) + ChartoDec(Data);
            Data_Flag++;

            if(Data_Flag == 2u){
                Data_Flag = 0;
                Store_Word = (Store_Word << 8) + CurLine.Data;
                Word_Flag++;
                CurLine.CheckSum += CurLine.Data;
                Byte_Counter++;
            }
            if(Word_Flag == 4u)
            {
                Store_Flag = 1;
                Word_Flag = 0;
            }
            if(Byte_Counter == CurLine.ByteCount - 1u)
            {
                Srec_LastState++;
            }
            break;
        case SREC_FLAG_DATA:
            if(CheckSum_Flag == 0u)
            {
                CurLine.CheckSum += (ChartoDec(Data) << 4);
                CheckSum_Flag++;
            }
            else if(CheckSum_Flag == 1)
            {
                CurLine.CheckSum += (ChartoDec(Data));
                CheckSum_Flag = 0u;
                //raise CheckLine flag
                CheckLine_Flag = 1u;
                Srec_LastState++;
            }
            break;
        case SREC_FLAG_CHECKSUM:
            if (Data == '\n')
            {
                Srec_LastState = SREC_FLAG_DEFAULT;
            }

            break;
        case SREC_FLAG_DEFAULT:
            /*wait for new line */
            if(Data == 'S')
            {
                Srec_LastState = SREC_FLAG_FIRST;
            }
            break;
    }
}
static inline void UART_CMDHandler()
{
    switch (Data)
    {
        case CMD_LOAD_FIRM:
            BootState = BOOT_LOAD_WAIT;
            break;
        case CMD_ERASE_FIRM:
            BootState = BOOT_ERASE_WAIT;
            break;
        case CMD_RUN_FIRM:
            BootState = BOOT_RUN_WAIT;
            break;
        default:
            // this mean user type in an unregconnised command
            if (BootState == BOOT_DEFAULT)
            {
                BootState     = BOOT_ERROR;
                BootErrorFlag = BOOT_ERR_CMD;
            }

            else{
                // check if the firmbank chosen is valid
                if(Data == '1' || Data == '2')
                {

                    FirmBankSelect =  Data - '1';
                    BootState++;
                }
                else
                {
                    BootState      = BOOT_ERROR;
                    BootErrorFlag  = BOOT_ERR_FIRMBANK;
                }
            }
    }
}

static inline uint32_t bigToLittleEndian(uint32_t num)
{
	uint32_t swapped = ((num>>24)&0xff) | ((num<<8)&0xff0000) | ((num>>8)&0xff00) | ((num<<24)&0xff000000);
	return swapped;
}
static inline void delay_ms()
{
	uint32_t timeOut = 200000u;
	while(timeOut--);
}
void BootUpdateMode();


void main(void) {
	Boot_Init();
	// if button presses during power on, enable UART receive and change to update firmware mod
	/* Only use the line below in the first time run, to mark the default app address
	 * Flash_WriteWord(0x00u, BOOT_DEFAULT_FIRM_ADDR);*/
	if(SW2_ReadState() == BUTTON_PRESSED)
	{
		BootUpdateMode();
	}
	// else run default firmware
	Boot_JumpToFirm(BOOT_DEFAULT_FIRM);

}

void BootUpdateMode()
{
	BootSendACK(BootState);
	UART_ReceiveINT();
	while(1)
	{
		switch(BootState)
		{
		case BOOT_LOAD:
			if(Store_Flag)
			{
				Store_Flag =  0u;
				// this mean the address try to write to is not empty
				if((* (uint32_t *)Store_Addr) != 0xFFFFFFFFu)
				{
					BootErrorFlag = BOOT_ERR_FIRMBANK;
					BootState = BOOT_ERROR;
				}
				else
				{
					Store_Word = bigToLittleEndian(Store_Word);
					Flash_WriteWord(Store_Word, Store_Addr);
					Store_Addr += 4u;
				}
			}
			if(CheckLine_Flag)
			{
				CheckLine_Flag = 0u;
				while(CurLine.Addr)
				{
					CurLine.CheckSum += CurLine.Addr & (0xFFu);
					CurLine.Addr >>= 8u;
				}
				// raise error if checksum not equal 0xFF
				if(CurLine.CheckSum != 0xFFu)
				{
					BootState = BOOT_ERROR;
					BootErrorFlag = BOOT_ERR_CHECKSUM;
				}
			}
			if(LoadDone_Flag)
			{
				LoadDone_Flag = 0u;
				UART_DisableRxINT();
				BootSendACK(BootState);
				delay_ms();
				UART_ReceiveINT();
				BootState = BOOT_DEFAULT;
			}
			break;
		case BOOT_ERASE:
			for(uint32_t i = FLASH_BANK_START_SEC(FirmBankSelect); i < FLASH_BANK_END_SEC(FirmBankSelect); i++)
			{
				Flash_EraseSector(i);
			}
			BootSendACK(BootState);
			BootState = BOOT_DEFAULT;
			break;
		case BOOT_RUN:
			BootChangeDefaultFirm(FirmBankSelect);
			BootSendACK(BootState);
			Boot_JumpToFirm(FirmBankSelect);
			break;
		case BOOT_ERROR:
			UART_DisableRxINT();
			BootErrorHanlder(BootErrorFlag);
			delay_ms();
			UART_ReceiveINT();
			BootState = BOOT_DEFAULT;
			BootErrorFlag = BOOT_ERR_DEFAULT;
			break;
		default:
			break;
		}
	}
}

void UART0_IRQHandler()
{

    Data = UART0->D;  // 'S'
    // if load then process SREC file
    if (BootState == BOOT_LOAD)
    {
        UART_SrecHandler();
    }
    else
    {
        UART_CMDHandler();
    }
}










