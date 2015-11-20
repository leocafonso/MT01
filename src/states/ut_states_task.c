/*
 * ut_states_task.c
 *
 *  Created on: Nov 1, 2015
 *      Author: Fernando
 */


#include "ut_context.h"
#include "ut_state.h"
#include "lcd.h"

#include <string.h>
#include <stdio.h>

/**
 * Execute state machine.
 */
void states_task(void)
{
	ut_context pContext;
	ut_state currentState = STATE_SPLASH;

	/* Initialize context */
	memset(&pContext, 0, sizeof(ut_context));

	/* Run machine */
	while(currentState < STATE_NUMBER)
	{
		/* Execute state function and get next state */
		currentState = states_table[currentState](&pContext);
	}

	/* Error! */
	while(true)
	{

	}
}
