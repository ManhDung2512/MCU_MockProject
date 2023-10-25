/*
 * Clock.c
 *
 *  Created on: 19 Sep 2023
 *      Author: DUNG
 */

#include "Clock.h"

void Clock_Init(){
	// config clock: internal 1MHz frequency

	// choose fast internal clock
	MCG->C2 &= ~MCG_C2_IRCS_MASK;
	MCG->C2 |= MCG_C2_IRCS(0x01u);
	// no div for fcr
	MCG->SC &= ~MCG_SC_FCRDIV_MASK;

	// config clock divider
	SIM->CLKDIV1 &= ~SIM_CLKDIV1_OUTDIV1_MASK;
	// clear outdiv4
	SIM->CLKDIV1 &= ~SIM_CLKDIV1_OUTDIV4_MASK;

	// choose internal clock source for MCGOUTCLK
	MCG->C1 &= ~MCG_C1_CLKS_MASK;
	MCG->C1 |= MCG_C1_CLKS(0x01u);

	// enable clock for PORT A for UART, PORTC for SW2 IRCLKEN for uart0

	SIM->SCGC4 |= SIM_SCGC4_UART0(1U);
	SIM->SCGC5 |= SIM_SCGC5_PORTA(1U) | SIM_SCGC5_PORTC(0x01u);
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(3U);
	MCG->C1    |= MCG_C1_IRCLKEN(1U);


	SystemCoreClockUpdate();
}
void Clock_DeInit()
{
	// disable clock for peripheral
	SIM->SCGC4 &= ~SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 &= ~(SIM_SCGC5_PORTA(1U) | SIM_SCGC5_PORTC(0x01u));
	MCG->C1    &= ~MCG_C1_IRCLKEN_MASK;
}
