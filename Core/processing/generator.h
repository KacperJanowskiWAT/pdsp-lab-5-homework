#ifndef INCLUDE_PROCESSING_GENERATOR_H_
#define INCLUDE_PROCESSING_GENERATOR_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "pdsp/pdsp.h"

typedef enum {
	OSC_Sinusoid, OSC_Triangle, OSC_Square, OSC_Sawtooth
} OSC_WaveType_t;

typedef struct {
	OSC_WaveType_t wavetype;
	float frequency;
	float amplitude;
	float duty;
	float phase;
	float phaseStep;
} OSC_Cfg_t;

typedef struct {
	float ampl;
	float freq;
	float cos_theta;
	float sin_theta;
	float y[3];
} GEN_SinOsc_t;

typedef struct {
	uint16_t ampl;
	uint16_t length;
	uint16_t phaseIndex;
	int16_t *value;

} GEN_SinTab_t;

typedef struct {
	float amplitude;	// wartość rzeczywista w mV w zakresie od 0 do 1650, w przypadku przekroczenia zakresu nasycenie
	float frequency;	// wartość rzeczywista w Hz w zakresie od 0 do Fs/2 (Fs - częstotliwość próbkowania)
	float phaseOffset;
	float duty;
	uint32_t n;
	OSC_WaveType_t type;

} GEN_Cfg_t;

void  GEN_Init(GEN_Cfg_t *hGenCfg, OSC_WaveType_t type, float amplitude, float freq, float phaseOffset, float duty);
channel_t GEN_GetValue(GEN_Cfg_t *hGenCfg);

void OSC_Init(OSC_Cfg_t *hOscCfg, OSC_WaveType_t wavetype, float amplitude, float frequency, float phaseOffset,
		float duty);
float OSC_GetValueF(OSC_Cfg_t *hOscCfg);
channel_t OSC_GetValue(OSC_Cfg_t *hOscCfg);


float OSC_GetValuePeriodF(OSC_Cfg_t *hGen);
float OSC_GetValueF(OSC_Cfg_t *hGen);

void OSC_SetFrequency(OSC_Cfg_t *hGen, float freq);
float OSC_GetFrequency(OSC_Cfg_t *hGen);

void GEN_InitSinOsc(GEN_SinOsc_t *hSinOsc, float freq);
float GEN_GetSinOscF(GEN_SinOsc_t *hSinOsc);
void GEN_SetSinOscFreq(GEN_SinOsc_t *hGen, float freq);

void GEN_InitSinTab(GEN_SinTab_t *hSinTab, int16_t ampl, int16_t length);
int16_t GEN_GetSinTab(GEN_SinTab_t *hSinTab, uint16_t index);
int16_t GEN_GetSinTabNext(GEN_SinTab_t *hSinTab);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_PROCESSING_GENERATOR_H_ */
