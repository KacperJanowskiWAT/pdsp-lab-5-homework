#include "processing/generator_sweep.h"

/*
 * @brief Initialization of SWEEP generator instance
 *
 * @param hCfg			Configuration structure for generator
 * @param freqStart		Start frequency value [Hz]
 * @param freqStop		Stop frequency value [Hz]
 * @param duration 		Time for change from freqStart to freqStop [s]
 * @param phaseOffset 	Initial phase offset [radiant]
 * @retval None
 */
void SWEEP_Init(SWEEP_Cfg_t *hCfg, OSC_Cfg_t *hGen, float freqStart, float freqStop, float duration, SWEEP_MODE_t mode) {
	hCfg->duration = duration;
	hCfg->mode = mode;
	if (mode == LINEAR) {
		hCfg->phaseStepStart = (freqStart * PDSP_2PI_DIV_FS );	// Phase step on start
		hCfg->phaseStepStop = (freqStop * PDSP_2PI_DIV_FS );	// Phase step on stop
	} else {
		hCfg->phaseStepStart = log10f(freqStart);				// Log10 of start frequency
		hCfg->phaseStepStop = log10f(freqStop);					// Log10 of stop frequency
	}
	hCfg->phaseStep = hCfg->phaseStepStart;						// Current phase step / current log10 of frequency step
	hCfg->phaseInc = ((hCfg->phaseStepStop - hCfg->phaseStepStart) / (duration * PDSP_CODEC_Fs )); // Phase change per sample / log10 of frequency change per sample

	//
	hCfg->generator = hGen;
	hCfg->generator->phaseStep = (freqStart * PDSP_2PI_DIV_FS );

}

/*
 * @brief	Calculate next value of SWEEP generator
 *
 * @param	hCfg		Configuration structure for generator
 * @retval				Value of current sample
 */
float SWEEP_GetF(SWEEP_Cfg_t *hCfg) {
	float wave;
	float phaseStep;

	wave = OSC_GetValueF(hCfg->generator);
	// Increment phase
	hCfg->phaseStep += hCfg->phaseInc;
	// Check if overflow phase step - is over stop frequency
	if (hCfg->phaseStep > hCfg->phaseStepStop)
		hCfg->phaseStep -= (hCfg->phaseStepStop - hCfg->phaseStepStart);

	if (hCfg->mode == LINEAR) {
		phaseStep = hCfg->phaseStep;
	} else {
		phaseStep = (pow10f(hCfg->phaseStep) * PDSP_2PI_DIV_FS );	// Change phase step
	}

	hCfg->generator->phaseStep = phaseStep;
	if (hCfg->generator->phase > PDSP_2PI)
		hCfg->generator->phase -= PDSP_2PI;

	return wave;
}

channel_t SWEEP_GetSample(SWEEP_Cfg_t *hCfg) {
    return (channel_t) ((hCfg->generator->amplitude * SWEEP_GetF(hCfg)) / PDSP_CODEC_mVres);
}
