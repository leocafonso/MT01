/*
 * plasma.h
 *
 *  Created on: 14/03/2016
 *      Author: leocafonso
 */

#ifndef CNC_PLASMA_H_
#define CNC_PLASMA_H_

#define ARCO_OK_FAILED  0xFFFFFFFF
#define ARCO_OK_OFF     0xFFFFFFFE

void pl_arcook_init(void);
void pl_arcook_start(void);
void pl_arcook_stop(void);
void pl_arcook_check(void);
void isCuttingSet(bool state);
bool isCuttingGet(void);
void plasma_task(void);

#endif /* CNC_PLASMA_H_ */