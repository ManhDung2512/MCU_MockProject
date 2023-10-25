/*
 * Middleware.c
 *
 *  Created on: 12 Oct 2023
 *      Author: DUNG
 */
#include"Middleware.h"




void Boot_JumpToFirm(FirmwareType userFirmware)
{
	// TODO: deint
	//disable  interupt and clear all pending irq
	__disable_irq();
	Boot_DeInit();

	void (*Reset_Handler)(void);


	// Get the value of user application reset handler
	Reset_Handler =  (USER_RESET_VALUE(userFirmware));


	// set the Vector table offset to firmware location
	SCB->VTOR = (FLASH_FIRM_START_ADDR(userFirmware));
	// initialize msp for user firmware
	__set_MSP(USER_MSP_VALUE(userFirmware));
	__set_PSP(USER_MSP_VALUE(userFirmware));
	// jump to user app
	Reset_Handler();
}

void BootChangeDefaultFirm(FirmwareType FirmBankSelect)
{
	//delete the sector that store default firmware select information
	// write new firmwareto it
	Flash_EraseSector(BOOT_DEFAULT_FIRM_SECTOR);
	Flash_WriteWord(FirmBankSelect, BOOT_DEFAULT_FIRM_ADDR);

}
void BootSendACK(Boot_StateType BootState)
{
	char * pMessage = NULL;
	switch(BootState)
	{
		case BOOT_DEFAULT:
			pMessage = "BOOT MODE:";
			break;
		case BOOT_ERASE:
			pMessage = "ERASE DONE";
			break;
		case BOOT_LOAD:
			pMessage = "LOAD DONE ";
			break;
		case BOOT_RUN:
			pMessage = "START APP ";
			break;
	}
	UART_SendString(pMessage, 10u);
}

void Boot_Init()
{
	Clock_Init();
	SW2_Init();
	UART0_Init();
}
void Boot_DeInit()
{
	// reverse of BootInit
	UART0_DeInit();
	SW2_DeInit();
	Clock_DeInit();

}

void SW2_Init()
{
	const PortConfigType SW2_PortConfig = {
			.mux 		=  PORT_MUX_GPIO,
			.pull 		=  PORT_PULL_UP,
			.interrupt 	=  PORT_INTERRUPT_FALLING,
			.callback	=  NULL
	};

	const GPIO_ConfigType SW2_GPIOConfig = {
			.GPIOx      =  SW2_GPIO,
			.pinnum     =  SW2_PIN,
			.direction  =  GPIO_INPUT
	};
	PORT_Init(SW2_PORT, SW2_PIN, &SW2_PortConfig);
	GPIO_Init(&SW2_GPIOConfig);
}
void SW2_DeInit()
{
	GPIO_DeInit(SW2_GPIO, SW2_PIN);
	PORT_DeInit(SW2_PORT, SW2_PIN);
}
void BootErrorHanlder(Boot_ErrorType BootErrorFlag){
	char * pMessage = NULL;
	switch(BootErrorFlag)
	{
		case BOOT_ERR_CHECKSUM:
			pMessage = "CHECKSUM ERR";
			break;
		case BOOT_ERR_CMD:
			pMessage = "CMD ERR     ";
			break;
		case BOOT_ERR_FIRMBANK:
			pMessage = "FIRMBANK ERR";
			break;
	}
	UART_SendString(pMessage, 12u);
}


Button_StateType SW2_ReadState()
{
	if(GPIO_LOW  ==  GPIO_ReadInput(SW2_GPIO, SW2_PIN))
	{
		return BUTTON_PRESSED;
	}
	else
	{
		return BUTTON_RELEASED;
	}
}


