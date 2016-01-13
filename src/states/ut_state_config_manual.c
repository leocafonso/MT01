/*
 * ut_state_config_menu.c
 *
 *  Created on: Dec 6, 2015
 *      Author: Fernando
 */

#include "ut_context.h"
#include "ut_state.h"
#include "ut_state_config_var.h"
#include "ut_state_config_manual.h"
#include "interpreter_if.h"

#include "FreeRTOS.h"
#include "task.h"

#include "keyboard.h"
#include "interpreter_if.h"

#include "lcd.h"
#include "lcd_menu.h"

#define DEFAULT_CONFIG_TIMEOUT	30000

const char zera_eixos[]= "\
G21 G90\n\
G28.3 X0 Y0 Z0\n\
m30";

const char home_eixos[]= "\
G21 G90\n\
G00 X0 Y0 Z20\n\
m30";

static void zerar_eixos(void *var);
static void homming_eixos(void *var);
static void idle(void *var);

/* Array with all config variables */
ut_config_var configs_manual[CONFIG_MANUAL_MAX];
static bool initialized = false;
extern ut_config_var* configsVar;

static const ut_state geNextStateManual[5] =
{
	STATE_MANUAL_MODE,
	STATE_CONFIG_VAR,
	STATE_CONFIG_VAR,
	STATE_CONFIG_VAR,
	STATE_MAIN_MENU
};

/* Initial values for each config variable */
static ut_config_type init_types[CONFIG_MANUAL_MAX] =
{
	UT_CONFIG_NULL,
	UT_CONFIG_BOOL,
	UT_CONFIG_BOOL,
	UT_CONFIG_BOOL,
	UT_CONFIG_NULL
};

static uint32_t init_values[CONFIG_MANUAL_MAX] =
{
	0,
	0,
	0,
	0,
	0
};

static char* init_names[CONFIG_MANUAL_MAX] =
{
	" MODO MANUAL",
	" ZERAR EIXOS",
	" DESLOCAR PARA ZERO",
	" JOG R�PIDO E LENTO ",
	" VELOCIDADES DE JOG"
};

static var_func init_func[CONFIG_MANUAL_MAX] =
{
	idle,
	zerar_eixos,
	homming_eixos,
	idle,
	idle
};

static const char* gszConfigMenuTitle = "CONFIG. MANUAL";

/**
 * Initialize config array
 */
static void init()
{
	uint8_t i;

	/* Check if already initialized */
	if(initialized) return;

	/* Zero all values */
	memset(configs_manual, 0, sizeof(configs_manual));

	/* Initialize all variables */
	for(i = 0; i < CONFIG_MANUAL_MAX; i++)
	{
		configs_manual[i].type = init_types[i];
		configs_manual[i].value = init_values[i];
		configs_manual[i].name = init_names[i];
		configs_manual[i].func_var = init_func[i];
	}

	initialized = true;
}

/**
 * Shows a configuration menu for the machine.
 *
 * @param pContext Context object
 * @return Main menu state
 */
//ut_state ut_state_config_manual_menu(ut_context* pContext)
ut_state ut_state_config_manual_menu(ut_context* pContext)
{
	ut_menu config_menu;
	uint8_t i;

	/* Initialize variables */
	init();

	/* Initialize menu */
	ut_menu_init(&config_menu);

	/* Options */
	config_menu.title = gszConfigMenuTitle;
	/* Items */
	for(i = 0; i < CONFIG_MANUAL_MAX; i++)
	{
		config_menu.items[config_menu.numItems++].text = configs_manual[i].name;
	}

	/* Show menu */
	config_menu.selectedItem = 0;
	if(ut_menu_browse(&config_menu, DEFAULT_CONFIG_TIMEOUT) < 0)
	{
		return STATE_MAIN_MENU;
	}

	/* Set selected item */
	if(config_menu.selectedItem == 2)
	{
		pContext->tag = STATE_DESLOCAZERO_MODE;
	}
	else
	{
		pContext->tag = STATE_CONFIG_MANUAL_MODE;
	}
	configsVar = &configs_manual[config_menu.selectedItem];
	return geNextStateManual[config_menu.selectedItem];
}

static void zerar_eixos(void *var)
{
	ut_config_var *lvar = var;

	if(lvar->value)
	{
		tg_set_primary_source(XIO_DEV_COMMAND);
		xio_open(cs.primary_src,zera_eixos,0);
	}
}

static void homming_eixos(void *var)
{
	ut_config_var *lvar = var;

	if(lvar->value)
	{
		tg_set_primary_source(XIO_DEV_COMMAND);
		xio_open(cs.primary_src,home_eixos,0);
	}
}

static void idle(void *var)
{

}