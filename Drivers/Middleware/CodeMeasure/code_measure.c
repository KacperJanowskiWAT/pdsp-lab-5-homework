/*
 * code_measure.c
 *
 *  Created on: Jun 2, 2021
 *      Author: pdabal
 */

#include "main.h"
#include "code_measure.h"

#if MEASURE_N > 0
static uint32_t Start[MEASURE_N];
static int32_t Delta[MEASURE_N];
static uint32_t Offset;

uint32_t CM_Init(void) {
	if (DWT->CTRL != 0) {                        				// Sprawdzenie dostępności
		if (MEASURE_N > 0) {
			uint32_t i;
			for (i = 0; i < MEASURE_N; i++) {					// Wyzerowanie tablic
				Start[i] = 0;
				Delta[i] = 0;
			}
			CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;     // Włączenie modułu CoreDebug TRC
			DWT->CYCCNT = 0;                                    // Wyzerowanie licznika cykli zegarowych
			DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;                // Włączenie licznika cykli zegarowych

			CM_Start(0);
			CM_Delta(0);
			Offset = CM_DeltaGet(0);
			return 0;
		}
	} else {
		return 1;
	}
}

void CM_Start(uint32_t n) {
	if (n < MEASURE_N) {
		Start[n] = DWT->CYCCNT;
	}
}

void CM_Delta(uint32_t n) {
	uint32_t stop = DWT->CYCCNT;
	if (n < MEASURE_N) {
		int32_t delta = (int32_t) (stop - Start[n] - Offset);
		if (delta >= 0)
			Delta[n] = (uint32_t) delta;
	}
}

uint32_t CM_DeltaGet(uint32_t n){
	if (n < MEASURE_N) {
		return Delta[n];
	}else{
		return 0;
	}
}

#else

uint32_t CM_Init(void) {
	return 1;
}

void CM_Start(uint32_t n) {

}

void CM_Delta(uint32_t n){

}

uint32_t CM_DeltaGet(uint32_t n){
	return 0;
}
#endif

void CM_StatInit(CM_Stat_t *hStat, uint32_t count) {
	hStat->avr = 0;
	hStat->min = UINT32_MAX;
	hStat->max = 0;
	hStat->count = count;
	hStat->n = 0;
}

uint32_t CM_StatPut(CM_Stat_t *hStat, uint32_t cycle) {
	if (hStat->n < hStat->count) {
		// Wyznaczanie średniej
		if(hStat->n == 0)
			hStat->avr = cycle;
		else
			hStat->avr = ((hStat->avr + cycle) >> 1);
		// Aktualizacja minimum
		if (cycle < hStat->min)
			hStat->min = cycle;
		// Aktualizacja maksimum
		if (cycle > hStat->max)
			hStat->max = cycle;
		hStat->n++;
		return 0;
	} else {
		hStat->n = 0;
		return 1;
	}
}

void CM_StatReset(CM_Stat_t *hStat){
	hStat->avr = 0;
	hStat->min = UINT32_MAX;
	hStat->max = 0;
	hStat->n = 0;
}

void CM_StatGetTime(CM_Stat_t *hStat, float * avr, float * max, float * min, CM_Unit_t unit){
	float f = SystemCoreClock;
	switch (unit) {
		case CM_ns:
			f /= 1000.0f;
		case CM_us:
			f /= 1000.0f;
		case CM_ms:
			f /= 1000.0f;
		case CM_s:
			break;
	}
	float t = (1.0f / f);
	*avr = hStat->avr * t;
	*max = hStat->max * t;
	*min = hStat->min * t;
}
