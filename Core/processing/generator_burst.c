#include "processing/generator_burst.h"

/*
 * @brief Initialization of SWEEP generator instance
 *
 * @param hCfg			Configuration structure for BURS generator
 * @param hCfg			Configuration structure for signal generator
 * @param nCycles		Number of signal period
 * @param burstPeriod	Period of burst [s]
 * @retval None
 */
void BURST_Init(BURST_Cfg_t *hCfg, OSC_Cfg_t *hGen, uint16_t nCycles, float burstPeriod) {
	hCfg->nCycles = nCycles;
	hCfg->nCyclesLeft = hCfg->nCycles;
	hCfg->nSample = (uint16_t) (burstPeriod * PDSP_CODEC_Fs );
	hCfg->nSampleLeft = hCfg->nSample;
	hCfg->generator = hGen;
	hCfg->generatorPhaseOffset = hGen->phase;
}

/*
 * @brief	Calculate next value of BURST generator
 *
 * @param	hCfg		Configuration structure for BURST generator
 * @retval				Value of current sample
 */
float BURST_GetF(BURST_Cfg_t *hCfg) {
	float wave;

	if (hCfg->nCyclesLeft > 0) {
		wave = OSC_GetValueF(hCfg->generator);
		if (hCfg->generator->phase > (PDSP_2PI + hCfg->generatorPhaseOffset)) {
			hCfg->generator->phase -= PDSP_2PI;
			hCfg->nCyclesLeft--;
		}
	} else {
		wave = 0.0f;
	}

	hCfg->nSampleLeft--;
	// Check if reach burst period
	if (hCfg->nSampleLeft == 0) {
		hCfg->nCyclesLeft = hCfg->nCycles;
		hCfg->nSampleLeft = hCfg->nSample;
		hCfg->generator->phase = hCfg->generatorPhaseOffset;
	}

	return wave;
}
