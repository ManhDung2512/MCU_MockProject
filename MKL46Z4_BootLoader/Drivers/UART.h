/*
 * UART.h
 *
 *  Created on: 16 Oct 2023
 *      Author: DUNG
 */

#ifndef UART_H_
#define UART_H_
#include"Port.h"

void UART0_Init();
void UART0_DeInit();
void UART_ReceiveINT(void);
void UART_DisableRxINT(void);
void UART_SendString(uint8_t * pData, uint8_t Len);
void UART_SendChar(uint8_t character);



#endif /* UART_H_ */
