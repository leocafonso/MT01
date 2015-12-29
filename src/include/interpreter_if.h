/*
 * interpreter_if.h
 *
 *  Created on: 21/12/2015
 *      Author: leocafonso
 */

#ifndef INCLUDE_INTERPRETER_IF_H_
#define INCLUDE_INTERPRETER_IF_H_

#include "tinyg.h"				// #1
#include "config.h"				// #2
#include "controller.h"
#include "json_parser.h"
#include "text_parser.h"
#include "gcode_parser.h"
#include "canonical_machine.h"
#include "xio.h"

typedef void (*iif_func_ptr)(void);

extern iif_func_ptr iif_func_enter;
extern iif_func_ptr iif_func_esc;
extern iif_func_ptr iif_func_down;
extern iif_func_ptr iif_func_up;
extern iif_func_ptr iif_func_left;
extern iif_func_ptr iif_func_right;
extern iif_func_ptr iif_func_released;


extern void iif_bind_filerunning(void);
extern void iif_bind_jog(void);
extern void iif_bind_idle(void);

extern bool zpbutton;

#endif /* INCLUDE_INTERPRETER_IF_H_ */