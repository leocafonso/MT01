/*
 * config_SwTimers.h
 *
 *  Created on: 16/04/2016
 *      Author: leocafonso
 */

#ifndef CONFIG_CONFIG_SWTIMERS_H_
#define CONFIG_CONFIG_SWTIMERS_H_

#include "FreeRTOS.h"
#include "timers.h"

typedef enum{
	ZDOWN_FILERUNNING_TIMER = 0,
	ZUP_FILERUNNING_TIMER,
	DOWN_FILERUNNING_TIMER,
	UP_FILERUNNING_TIMER,
	DOWN_CONFIGVAR_TIMER,
	UP_CONFIGVAR_TIMER,
	AUTO_MENU_TIMER,
	TIMER_SIZE
}sw_timers;

extern TimerHandle_t swTimers[TIMER_SIZE];
#endif /* CONFIG_CONFIG_SWTIMERS_H_ */
