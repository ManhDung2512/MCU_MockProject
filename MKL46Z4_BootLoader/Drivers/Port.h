/*
 * Port.h
 *
 *  Created on: Sep 11, 2023
 *      Author: WIN 10
 */

#ifndef PORT_H_
#define PORT_H_

#include "MKL46Z4.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

#define PORT_PIN_NUM_MIN   ((uint8_t) (0u))
#define PORT_PIN_NUM_MAX   ((uint8_t) (31u))

typedef void (*CallbackType)(void);

typedef enum {
	PORT_MUX_ANALOG		= 0x0U,
	PORT_MUX_GPIO		= 0x1U,
	PORT_MUX_ALT2,
	PORT_MUX_ALT3,
	PORT_MUX_ALT4,
	PORT_MUX_ALT5,
	PORT_MUX_ALT6,
	PORT_MUX_ALT7,
} PortMuxType;

typedef enum {
	PORT_PULL_DISABLE	= 0x0U,
	PORT_PULL_DOWN		= 0x2U,
	PORT_PULL_UP		= 0x3U,
} PortPullType;

typedef enum {
	PORT_INTERRUPT_DISABLE		= 0x0U,
	PORT_INTERRUPT_LOW			= 0x8U,
	PORT_INTERRUPT_RISING		= 0x9U,
	PORT_INTERRUPT_FALLING		= 0xAU,
	PORT_INTERRUPT_EITHER		= 0xBU,
	PORT_INTERRUPT_HIGH			= 0xCU,
} PortInterruptType;

typedef struct {
	PortMuxType 		mux;
	PortPullType 		pull;
	PortInterruptType 	interrupt;
	CallbackType 		callback;
} PortConfigType;


typedef enum
{
	PORT_INVALID_BASE    = 0x00u,
	PORT_VALID_BASE      = 0x01u,
} PortBaseStatusType;

typedef enum
{
	PORT_INIT_SUCCESS         =    0x00u,
	PORT_INVALID_CONFIG       =    0x01u,
	PORT_PIN_OUT_OF_RANGE	  =    0x02u,
	PORT_BASE_ERROR           =    0x03u

} PortInitStatusType;

/**
 * Function to Init a pin of a PORT
 * Input: PORT base address ptr PORTx ; pin number of port; config params described in PortConfigType
 * Output: Init status, can fail or success, if fail specific error is provided in PortInitStatusTyp
 */
PortInitStatusType PORT_Init(PORT_Type * PORTx, uint8_t pinnum, const PortConfigType * Config);

/**
 * Function to disable a pin
 * nput: PORT base address ptr PORTx ; pin number of port
 */
void PORT_DeInit(PORT_Type * PORTx, uint8_t pinnum);
#endif /* PORT_H_ */
