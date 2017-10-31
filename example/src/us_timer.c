/*
 * ms_timer.c
 *
 *  Created on: Sep 21, 2015
 *      Author: mortenopprudjakobsen
 */
#include <stdio.h>
#include "board.h"
#include "tone.h"

/* Module Global variables */
static volatile unsigned int usCount;

/**
 * @brief	Handle 1uS irq from 32-bit timer
 * @return	Nothing
 */
void TIMER0_IRQHandler (void)
{
	/* do we have a match Interrupt ?*/
	if (Chip_TIMER_MatchPending (LPC_TIMER0, 1))
	{
		/* Yep, acknowledge and clear periphereal*/
		Chip_TIMER_ClearMatch (LPC_TIMER0, 1);

		/* increment mS counter */
		usCount++;

		//TODO, add callback, so timer does not have to include tone.h
		playNextSample();
	}

}

/**
 * @brief Initializes timer ? to generate interrupts every 1ms
 * @return	Nothing
 */
void us_timer_init (void)
{
	unsigned int timerFreq;

	/* Enable timer 1 clock */
	Chip_TIMER_Init (LPC_TIMER0);

	/* Timer rate is system clock rate */
//	timerFreq = Chip_Clock_GetSystemClockRate ();
	timerFreq = 120000000;

	/* Timer setup for match and interrupt at 1KHz */
	Chip_TIMER_Reset (LPC_TIMER0);
	Chip_TIMER_MatchEnableInt (LPC_TIMER0, 1);
	Chip_TIMER_SetMatch (LPC_TIMER0, 1, (timerFreq / 1000000));
	Chip_TIMER_ResetOnMatchEnable (LPC_TIMER0, 1);
	Chip_TIMER_Enable (LPC_TIMER0);

	/* Enable timer interrupt */
	NVIC_ClearPendingIRQ (TIMER0_IRQn);
	NVIC_EnableIRQ (TIMER0_IRQn);
}

/**
 * @brief starts timer, NOTE does NOT reset the counter value
 * @return	Nothing
 */
void us_timer_start (void)
{
	/* tell HW to start counting*/
	Chip_TIMER_Enable(LPC_TIMER0);
}
