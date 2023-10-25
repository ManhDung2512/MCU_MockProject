/*
 * UART.c
 *
 *  Created on: 16 Oct 2023
 *      Author: DUNG
 */

#include "UART.h"
void UART0_Init()
{
	const PortConfigType UART0_Config = {
			.mux = PORT_MUX_ALT2,
			.pull = PORT_PULL_UP
	};
	PORT_Init(PORTA, 1, &UART0_Config);
	PORT_Init(PORTA, 2, &UART0_Config);

//    - Baudrate (Clock - Async module clock / Baud Divider / OSR Divider)
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDL = 41U;

	uint32_t temp = UART0->C4;	// Read
	temp &= ~UART0_C4_OSR_MASK;	// Modify
	temp |= UART0_C4_OSR(9U);
	UART0->C4 = temp;			// Write

//	UART0->C4 = (UART0->C4 & ~UART0_C4_OSR_MASK) | UART0_C4_OSR(9U);

//    - Frame
//        - Data Length (8) C1[M] & C4[M10]
	UART0->C4 &= ~UART0_C4_M10_MASK;
	UART0->C1 &= ~UART0_C1_M_MASK;

//        - Parity (None) C1[PE/PT]
	UART0->C1 &= ~UART0_C1_PE_MASK;

//        - Stop bits (1) BDH[SBNS])
	UART0->BDH &= ~UART0_BDH_SBNS_MASK;
}
void UART0_DeInit()
{
	NVIC_DisableIRQ(UART0_IRQn) ;
	UART_DisableRxINT() ;
	// disable transmitter
	UART0->C2 &= ~UART0_C2_TE_MASK;
	// clear baudrate divider
	UART0->BDH &= ~UART0_BDH_SBR_MASK;

}
void UART_ReceiveINT(void)
{
	// Enable Receive Interrupt
	UART0->C2 |= UART0_C2_RIE(1U);
	NVIC_EnableIRQ(UART0_IRQn);

	// Enable Receiver
	UART0->C2 |= UART0_C2_RE(1U);
}

void UART_DisableRxINT(void)
{
	// Disable Receive Interrupt
	UART0->C2 &= ~UART0_C2_RIE_MASK;

	// Disable Receiver
	UART0->C2 &= ~UART0_C2_RE_MASK;
}

void UART_SendChar(uint8_t character)
{
	// Enable Transmitter
	UART0->C2 |= UART0_C2_TE(1U);

	// 1. Wait TX Buffer is Empty (TDRE = 1)
	while ( (UART0->S1 & UART0_S1_TDRE_MASK) == 0U );

	// 2. Write Data to TX Buffer
	UART0->D = character;

	// 3. Wait Transmit is complete (TC = 1)
	while ( (UART0->S1 & UART0_S1_TC_MASK) == 0U );

	// Disable Transmitter
	UART0->C2 &= ~UART0_C2_TE_MASK;
}


void UART_SendString(uint8_t * pData, uint8_t Len)
{
	uint8_t count;

	// Enable Transmitter
	UART0->C2 |= UART0_C2_TE(1U);

	for (count = 0; count < Len; count++)
	{
		// 1. Wait TX Buffer is Empty (TDRE = 1)
		while ( (UART0->S1 & UART0_S1_TDRE_MASK) == 0U );

		// 2. Write Data to TX Buffer
		UART0->D = pData[count];
	}

	// 3. Wait Transmit is complete (TC = 1)
	while ( (UART0->S1 & UART0_S1_TC_MASK) == 0U );

	// Disable Transmitter
	UART0->C2 &= ~UART0_C2_TE_MASK;
}
