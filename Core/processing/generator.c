#include "processing/generator.h"

/*
 *
 * */
void OSC_Init(OSC_Cfg_t *hOscCfg, OSC_WaveType_t wavetype, float amplitude, float frequency, float phaseOffset,
		float duty) {
	hOscCfg->wavetype = wavetype;
	hOscCfg->amplitude = fmodf(amplitude, (PDSP_CODEC_Vpp * 500.0f));
	hOscCfg->duty = fmodf(duty, 100.0f) / 100.0f;
	hOscCfg->frequency = fmodf(frequency, (PDSP_CODEC_Fs / 2.0f));
	hOscCfg->phase = fmodf(phaseOffset, PDSP_2PI);
	hOscCfg->phaseStep = (hOscCfg->frequency * PDSP_2PI_DIV_FS );
}

/*
 *
 * */
float OSC_GetValueF(OSC_Cfg_t *hOscCfg) {
	float wave;
	float phaseDuty;

	switch (hOscCfg->wavetype) {
	case OSC_Sinusoid:
		wave = pdsp_sinf(hOscCfg->phase);
		break;
	case OSC_Triangle:
		phaseDuty = PDSP_2PI * hOscCfg->duty;
		if (hOscCfg->phase < phaseDuty) {
			wave = (2.0f / (phaseDuty)) * (hOscCfg->phase) - 1.0f;
		} else {
			wave = (2.0f / (phaseDuty - PDSP_2PI )) * (hOscCfg->phase - phaseDuty) + 1.0f;
		}
		break;
	case OSC_Square:
		phaseDuty = PDSP_2PI * hOscCfg->duty;
		wave = (hOscCfg->phase < phaseDuty) ? 1.0f : -1.0f;
		break;
	case OSC_Sawtooth:
		wave = (2.0f / PDSP_2PI) * (hOscCfg->phase) - 1.0f;
		break;
	default:
		wave = 0.0f;
		break;
	}

	hOscCfg->phase += hOscCfg->phaseStep;
	hOscCfg->phase = fmodf(hOscCfg->phase, PDSP_2PI);

	return wave;
}

channel_t OSC_GetValue(OSC_Cfg_t *hOscCfg){
	return (channel_t) ((hOscCfg->amplitude * OSC_GetValueF(hOscCfg)) / PDSP_CODEC_mVres);
}

/*
 *
 * */
void OSC_SetFrequency(OSC_Cfg_t *hOscCfg, float frequency) {
	hOscCfg->frequency = frequency;
	hOscCfg->phaseStep = (frequency * PDSP_2PI_DIV_FS );
}

/*
 *
 * */
float OSC_GetFrequency(OSC_Cfg_t *hOscCfg) {
	return hOscCfg->frequency;
}



float OSC_GetValuePeriodF(OSC_Cfg_t *hOscCfg) {
	float wave;

	wave = OSC_GetValueF(hOscCfg);

	if (hOscCfg->phase > PDSP_2PI)
		hOscCfg->phase -= PDSP_2PI;

	return wave;
}

float OSC_GetSinPeriodF(OSC_Cfg_t *hOscCfg) {
	float wave;

	wave = pdsp_sinf(hOscCfg->phase);

	hOscCfg->phase += hOscCfg->phaseStep;
	if (hOscCfg->phase > PDSP_2PI)
		hOscCfg->phase -= PDSP_2PI;

	return wave;
}

/*---------------------------------------------------------------------------*/

void GEN_InitSinOsc(GEN_SinOsc_t *hSinOsc, float freq) {
	hSinOsc->freq = freq;
	hSinOsc->cos_theta = (pdsp_cosf(PDSP_2PI_DIV_FS * freq) * 2);
	hSinOsc->sin_theta = (pdsp_sinf(PDSP_2PI_DIV_FS * freq));
	hSinOsc->y[0] = 0;
	hSinOsc->y[1] = 1;
	hSinOsc->y[2] = 0;
}

void GEN_SetSinOscFreq(GEN_SinOsc_t *hSinOsc, float freq) {
	hSinOsc->freq = freq;
	hSinOsc->cos_theta = (pdsp_cosf(PDSP_2PI_DIV_FS * freq) * 2);
	hSinOsc->sin_theta = (pdsp_sinf(PDSP_2PI_DIV_FS * freq));
}

float GEN_GetSinOscF(GEN_SinOsc_t *hSinOsc) {
	hSinOsc->y[0] = (hSinOsc->cos_theta * hSinOsc->y[1]) - hSinOsc->y[2];
	hSinOsc->y[2] = hSinOsc->y[1];
	hSinOsc->y[1] = hSinOsc->y[0];
	return (hSinOsc->sin_theta * hSinOsc->y[0]);
}

/*---------------------------------------------------------------------------*/
void GEN_InitSinTab(GEN_SinTab_t *hSinTab, int16_t ampl, int16_t length) {
	hSinTab->ampl = ampl;
	hSinTab->length = length;
	hSinTab->phaseIndex = 0;
	hSinTab->value = (int16_t*) malloc(sizeof(int16_t) * length);

	if (hSinTab->value == NULL)
		return;

	float phaseStep = (PDSP_2PI / length);
	uint16_t i;
	int16_t *pValue = hSinTab->value;
	for (i = 0; i < length; i++) {
		*pValue = ampl * pdsp_sinf(phaseStep * i);
		pValue++;
	}
}

int16_t GEN_GetSinTabNext(GEN_SinTab_t *hSinTab) {
	int16_t y;

	y = hSinTab->value[hSinTab->phaseIndex++];
	if (hSinTab->phaseIndex >= hSinTab->length)
		hSinTab->phaseIndex = 0;

	return y;
}

int16_t GEN_GetSinTab(GEN_SinTab_t *hSinTab, uint16_t index) {
	int16_t y;
	if (hSinTab->length > index)
		y = hSinTab->value[index];
	else
		y = 0;

	return y;
}

/*---------------------------------------------------------------------------*/

