/*
 * code_measure.h
 *
 *  Created on: Jun 2, 2021
 *      Author: pdabal
 */

#ifndef MIDDLEWARE_CODEMEASURE_CODE_MEASURE_H_
#define MIDDLEWARE_CODEMEASURE_CODE_MEASURE_H_

#define MEASURE_N	3

typedef struct {
	uint32_t avr;
	uint32_t min;
	uint32_t max;
	uint32_t count;
	uint32_t n;
} CM_Stat_t;

typedef enum { CM_ns, CM_us, CM_ms, CM_s } CM_Unit_t;

uint32_t CM_Init(void);
void CM_Start(uint32_t n);
void CM_Delta(uint32_t n);
uint32_t CM_DeltaGet(uint32_t n);

void CM_StatInit(CM_Stat_t *hStat, uint32_t count);
uint32_t CM_StatPut(CM_Stat_t *hStat, uint32_t cycle);
void CM_StatReset(CM_Stat_t *hStat);

void CM_StatGetTime(CM_Stat_t *hStat, float * avr, float * max, float * min, CM_Unit_t unit);

#endif /* MIDDLEWARE_CODEMEASURE_CODE_MEASURE_H_ */
