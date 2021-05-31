#ifndef PROCESSING_GENERATOR_BURST_H_
#define PROCESSING_GENERATOR_BURST_H_

#include "processing/generator.h"

typedef struct {
	uint16_t nCycles;
	uint16_t nCyclesLeft;
	uint16_t nSample;
	uint16_t nSampleLeft;
	float generatorPhaseOffset;
	OSC_Cfg_t * generator;
} BURST_Cfg_t;

void BURST_Init(BURST_Cfg_t *hCfg, OSC_Cfg_t *hGen, uint16_t nCycles, float burstPeriod);
float BURST_GetF(BURST_Cfg_t *hCfg);

#endif /* PROCESSING_GENERATOR_BURST_H_ */
