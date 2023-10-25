/*
 * Port.c
 *
 *  Created on: Sep 11, 2023
 *      Author: WIN 10
 */

/* Params:
 * Port ? Pin ?
 * MUX (Analog, GPIO, Alt2->7)
 * Pull (Pullup / Pulldown / Pull-Disable)
 * Interrupt (Disable / Falling / Rasing / Both / High / Low)
 * */
#include "Port.h"

#define NO_INTERRUPT  ((uint8_t) (32u))

/* array of pointers to all port base addresses*/
static PORT_Type * const s_PORTBases[] = PORT_BASE_PTRS;

/*callback functions for port A and Port CD EXT interrupt*/
static CallbackType s_gCallback_PortCD = NULL;
static CallbackType s_gCallback_PortA = NULL;



/*check which pin of PORTx cause the interrupt, and return pin number
 * Input: PORTx base address
 * Output: pinnum
 * */
static inline uint8_t PORT_ReadInterruptFlag(PORT_Type * PORTx);

/*this function clear the ISF flag on the pin
 * Input: pinnum and port base
 * */
static inline void PORT_ClearInterruptFlag(PORT_Type * PORTx, uint8_t pinnum);

//check if the input PORTx is valid for this mcu, this mcu only have port A B C D E
static inline PortBaseStatusType PORT_CheckValidBase(PORT_Type * PORTx);



PortInitStatusType PORT_Init(PORT_Type * PORTx, uint8_t pinnum, const PortConfigType * Config)
{

	PortInitStatusType retVal = PORT_INIT_SUCCESS;

	/* check if PORTx value is valid and return immediately
	 * Not returning will cause unexpected behaviours because the user input "PORTx" maybe address of another peripheral register
	 * */
	if(PORT_INVALID_BASE == PORT_CheckValidBase(PORTx))
	{
		return PORT_BASE_ERROR;
	}
	// check if config field is empty
	if(NULL == Config)
	{
		retVal = PORT_INVALID_CONFIG;
	}
	// check for valid pin num
	if(pinnum > PORT_PIN_NUM_MAX  ||   pinnum < PORT_PIN_NUM_MIN)
	{
		retVal = PORT_PIN_OUT_OF_RANGE;
	}
	// check for valid port

	if(PORT_INIT_SUCCESS == retVal)
	{
		/* Configure Multiplexing */
		PORTx->PCR[pinnum] &= ~PORT_PCR_MUX_MASK;
		PORTx->PCR[pinnum] |= PORT_PCR_MUX(Config->mux);

		/* Configure Pull Control */
		PORTx->PCR[pinnum] &= ~PORT_PCR_PE_MASK;
		PORTx->PCR[pinnum] &= ~PORT_PCR_PS_MASK;
		PORTx->PCR[pinnum] |= (Config->pull << 0U);

		/* Configure Interrupt Control */
		PORTx->PCR[pinnum] &= ~PORT_PCR_IRQC_MASK;
		PORTx->PCR[pinnum] |= PORT_PCR_IRQC(Config->interrupt);

		if (PORT_INTERRUPT_DISABLE != Config->interrupt)
		{
			// 2. NVIC - Enable Interrupt.
			if (PORTx == PORTA)
			{
				NVIC_EnableIRQ(PORTA_IRQn);
				s_gCallback_PortA = Config->callback;
			}
			if (PORTx == PORTC || PORTx == PORTD)
			{
				NVIC_EnableIRQ(PORTC_PORTD_IRQn);
				s_gCallback_PortCD = Config->callback;
			}
		}
	}
	return retVal;
}

void PORT_DeInit(PORT_Type * PORTx, uint8_t pinnum){
	// change to disable
	PORTx->PCR[pinnum] |= ~PORT_PCR_MUX_MASK;
	// disable pull up pull down
	PORTx->PCR[pinnum] &= ~PORT_PCR_PE_MASK;
	PORTx->PCR[pinnum] &= ~PORT_PCR_PS_MASK;
	//disable interrupt flag
	PORTx->PCR[pinnum] &= ~PORT_PCR_IRQC_MASK;
}


static inline PortBaseStatusType PORT_CheckValidBase(PORT_Type * PORTx)
{
	PortBaseStatusType retVal = PORT_INVALID_BASE;
	for(uint8_t i = 0u; i < 5u; i++)
	{

		if(s_PORTBases[i]  ==  PORTx)
		{
			retVal = PORT_VALID_BASE;
			break;
		}

}
	return retVal;
}


static inline void PORT_ClearInterruptFlag(PORT_Type * PORTx, uint8_t pinnum)
{
	PORTx->ISFR |= (0x01u << pinnum);
}



static inline uint8_t PORT_ReadInterruptFlag(PORT_Type * PORTx)
{
	uint8_t retVal = NO_INTERRUPT;
	for(uint8_t i = PORT_PIN_NUM_MIN; i <= PORT_PIN_NUM_MAX; i++)
	{
		if ( PORTx->ISFR |= (0x01u << i) )
		{
			retVal = i;
			break;
		}
	}
	return retVal;
}

void PORTC_PORTD_IRQHandler(void)
{
	// Call function - address? - function pointer
	uint8_t pinnum;

	// check if interrupt cause by port c
	pinnum = PORT_ReadInterruptFlag(PORTC);

	// if cause by port c then clear flag
	if (pinnum != NO_INTERRUPT)
	{
		// clear flag
		PORT_ClearInterruptFlag(PORTC, pinnum);

	}
	// interrupt cause by PORT D, similar to PORT C case
	else
	{
		pinnum = PORT_ReadInterruptFlag(PORTD);

		PORT_ClearInterruptFlag(PORTD, pinnum);

	}
	// function call back for PORTCD if user configuration has a callback
	if(NULL != s_gCallback_PortCD)
	{
		(*s_gCallback_PortCD)();
	}

}

void PORTA_IRQHandler(void)
{
	uint8_t pinnum;

	// check which pin cause interrupt and clear
	pinnum = PORT_ReadInterruptFlag(PORTA);
	PORT_ClearInterruptFlag(PORTC, pinnum);

	// function call back for PORTA if user configuration has a callback
	if(NULL != s_gCallback_PortA)
	{
		(*s_gCallback_PortA)();
	}
}





