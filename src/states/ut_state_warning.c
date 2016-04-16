/*
 * ut_state_warning.c
 *
 *  Created on: Nov 1, 2015
 *      Author: Fernando
 */

#include "ut_context.h"
#include "ut_state.h"

#include "lcd.h"

#include "FreeRTOS.h"
#include "task.h"
#include "keyboard.h"
#include "plasma.h"

#define WARNING_PAGES	5
#define PAGE_DELAY		1000

/**
 * Warning messages!
 */
static const char* gszWarningMsg[MAX_ROW * WARNING_PAGES] =
{
		/* "12345678901234567890" */
		   "        AVISO!      ",
		   "                    ",
		   "   SOMENTE PESSOAL  ",
		   "    TREINADO PODE   ",
		   "    UTILIZAR ESTE   ",
		   "     EQUIPAMENTO.   ",
		   /* Page 2 */
		   "        AVISO!      ",
		   "                    ",
		   "  LEIA E ENTENDA O  ",
		   "MANUAL DE INSTRU��ES",
		   "  ANTES DE OPERAR.  ",
		   "                    ",
		   /* Page 3 */
		   "        AVISO!      ",
		   "                    ",
		   "   RISCO DE DANOS,  ",
		   " ACIDENTES E MORTES.",
		   "  CUIDADO COM M�OS  ",
		   "      E DEDOS.      ",
		   /* Page 4 */
		   "        AVISO!      ",
		   "                    ",
		   "   AO MOVIMENTAR E  ",
		   "CORTAR, AFASTE-SE DO",
		   "  CABECOTE E TOCHA  ",
		   "      PLASMA.       ",
		   /* Page 5 */
		   "        AVISO!      ",
		   "                    ",
		   "  USE SEMPRE EPI E  ",
		   "   LEIA O MANUAL.   ",
		   "                    ",
		   "                    ",

};

/**
 * Execute warning state.
 * It show a warning message to user.
 *
 * @param pContext Context object.
 * @return next state.
 */
ut_state ut_state_warning(ut_context* pContext)
{
	uint32_t keyEntry = 0;
	uint8_t uiPage = 0, uiMsgRow = 0;

	/* Loop through messages */
	for(uiPage = 0; uiPage < WARNING_PAGES; uiPage++)
	{
		ut_lcd_clear();

		/* Write strings */
		for(uiMsgRow = 0; uiMsgRow < MAX_ROW; uiMsgRow++)
		{
			ut_lcd_drawStr(uiMsgRow, 0, gszWarningMsg[uiPage*MAX_ROW + uiMsgRow], false,u8g_font_6x10);
		}
		/* Output */
		ut_lcd_output_str();

		/* Delay */
		vTaskDelay(PAGE_DELAY / portTICK_PERIOD_MS);
	}
	ut_lcd_output_warning("MODO DE EMERGENCIA\nZERO M�QUINA\nN�O REFERENCIADO\n");
	while(keyEntry != KEY_ESC){
		xQueueReceive( qKeyboard, &keyEntry, portMAX_DELAY );
	}
	pl_emergencia_init();
	return STATE_MAIN_MENU;
}
