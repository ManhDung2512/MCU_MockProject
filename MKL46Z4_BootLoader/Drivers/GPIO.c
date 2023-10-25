/*
 * GPIO.c
 *
 *  Created on: 13 Sep 2023
 *      Author: DUNG
 */


#include"GPIO.h"


GPIO_Type* s_GPIO_BasePtrs[] = GPIO_BASE_PTRS;

/*this funcion check if the base address for user input GPIOx is valid
 * MKL46Z4 only has GPIOA, B, C, D, E
 * */

static inline GPIO_BaseStatusType GPIO_CheckValidBase(GPIO_Type * GPIOx)
{
	GPIO_BaseStatusType retVal = GPIO_INVALID_BASE;
	for(uint8_t i = 0u; i < 5u; i++)
	{

		if(s_GPIO_BasePtrs[i]  ==  GPIOx)
		{
			retVal = GPIO_VALID_BASE;
			break;
		}

	}
	return retVal;
}


GPIO_DataType GPIO_ReadInput(GPIO_Type* GPIOx, uint8_t pinnum)
{
	return ( (GPIOx->PDIR & (0x01 << pinnum)) >>  pinnum);
}

void GPIO_ClearOutput(GPIO_Type* GPIOx, uint8_t pinnum)
{
	GPIOx->PCOR = (0x01u << pinnum);
}

void GPIO_SetOutput(GPIO_Type* GPIOx, uint8_t pinnum)
{
	GPIOx->PSOR = (0x01u << pinnum);
}

void GPIO_ToggleOutput(GPIO_Type* GPIOx, uint8_t pinnum)
{
	GPIOx->PTOR = (0x01u << pinnum);
}

GPIO_InitStatusType GPIO_Init(const GPIO_ConfigType* GPIO_Config)
{
	GPIO_InitStatusType retVal = GPIO_INIT_SUCCESS;

	/*check for config address and GPIOx base address
	 * return immediately cause invalid in config or base address will cause unexpected behaviours
	 * */
	if(NULL  ==  GPIO_Config)
	{
		return GPIO_INVALID_CONFIG;
	}
	if(GPIO_INVALID_BASE == GPIO_CheckValidBase(GPIO_Config->GPIOx))
	{
		return GPIO_BASE_ERROR;
	}
	/* check for pinnum*/
	if(GPIO_Config->pinnum < GPIO_PIN_NUM_MIN  ||  GPIO_Config->pinnum > GPIO_PIN_NUM_MAX)
	{
		retVal = GPIO_PIN_OUT_OF_RANGE;
	}

	/*Init the pin direction*/
	if(GPIO_INIT_SUCCESS == retVal){
		GPIO_Config->GPIOx->PDDR &= ~(0x01u << GPIO_Config->pinnum);

		if(GPIO_Config->direction == GPIO_OUTPUT)
		{
			GPIO_Config->GPIOx->PDDR |= (0x01u << GPIO_Config->pinnum);
		}
	}
	return retVal;
}
void GPIO_DeInit(GPIO_Type* GPIOx, uint8_t pinnum)
{
	// put gpio state back to input
	GPIOx->PDDR &= ~(0x01u << pinnum);
}
