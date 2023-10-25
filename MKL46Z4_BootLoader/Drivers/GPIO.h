/*
 * GPIO.h
 *
 *  Created on: 13 Sep 2023
 *      Author: DUNG
 */

#ifndef GPIO_H_
#define GPIO_H_

#define GPIO_PIN_NUM_MIN   ((uint8_t) (0u))
#define GPIO_PIN_NUM_MAX   ((uint8_t) (31u))

#ifndef NULL
#define NULL ((void *)0)
#endif

#include "MKL46Z4.h"


typedef enum{
	GPIO_INPUT    =  0x00u,
	GPIO_OUTPUT   =  0x01u
} GPIO_DirectionType;

typedef enum{
	GPIO_LOW      =  0x00u,
	GPIO_HIGH     =  0x01u
}GPIO_DataType;

typedef enum{
	GPIO_VALID_BASE    =  0x00u,
	GPIO_INVALID_BASE  =  0x01u
}GPIO_BaseStatusType;

typedef enum
{
	GPIO_INIT_SUCCESS       =   0x00u,
	GPIO_INVALID_CONFIG     =   0x01u,
	GPIO_PIN_OUT_OF_RANGE   =   0x02u,
	GPIO_BASE_ERROR         =   0x03u,
} GPIO_InitStatusType;

typedef struct{
	GPIO_Type* GPIOx;
	uint8_t pinnum;
	GPIO_DirectionType direction;
}GPIO_ConfigType;



GPIO_InitStatusType GPIO_Init(const GPIO_ConfigType* GPIO_Config);
void GPIO_DeInit(GPIO_Type* GPIOx, uint8_t pinnum);

void GPIO_SetOutput(GPIO_Type* GPIOx, uint8_t pinnum);

void GPIO_ClearOutput(GPIO_Type* GPIOx, uint8_t pinnum);

void GPIO_ToggleOutput(GPIO_Type* GPIOx, uint8_t pinnum);

GPIO_DataType GPIO_ReadInput(GPIO_Type* GPIOx, uint8_t pinnum);

#endif /* GPIO_H_ */
