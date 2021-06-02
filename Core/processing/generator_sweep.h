/*
 * generator_sweep.h
 *
 *  Created on: Apr 8, 2021
 *      Author: pdabal
 */

#ifndef PROCESSING_GENERATOR_SWEEP_H_
#define PROCESSING_GENERATOR_SWEEP_H_

#include "processing/generator.h"

typedef enum {LINEAR, LOG} SWEEP_MODE_t;

typedef struct {
	float duration;
	SWEEP_MODE_t mode;
	float phase;
	float phaseStep;
	float phaseStepStart;
	float phaseStepStop;
	float phaseInc;
	OSC_Cfg_t * generator;
} SWEEP_Cfg_t;

void SWEEP_Init(SWEEP_Cfg_t *hCfg, OSC_Cfg_t *hGen, float freqStart, float freqStop, float duration, SWEEP_MODE_t mode);
float SWEEP_GetF(SWEEP_Cfg_t *hSweep);
channel_t SWEEP_GetSample(SWEEP_Cfg_t *hCfg);

#endif /* PROCESSING_GENERATOR_SWEEP_H_ */
